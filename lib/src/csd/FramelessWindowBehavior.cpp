// MIT License
//
// Copyright (c) 2023 Olivier Clero
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <oclero/qlementine/csd/FramelessWindowBehavior.hpp>

#include <QApplication>
#include <QEvent>
#include <QOperatingSystemVersion>
#include <QScreen>
#include <QWindow>

#ifdef WIN32
#  include <QtWin>
#  include <windows.h>
#  include <windowsx.h>
#  include <winuser.h>
#endif // WIN32

#include <array>
#include <algorithm>

namespace oclero::qlementine {
FramelessWindowBehavior::FramelessWindowBehavior(QWidget* parent)
  : QObject(parent) {
  _parentWindowWidget = parent ? parent->window() : nullptr;
  Q_CHECK_PTR(_parentWindowWidget);

  QObject::connect(qApp, &QApplication::aboutToQuit, this, &FramelessWindowBehavior::uninstall);

  install();
}

FramelessWindowBehavior::~FramelessWindowBehavior() {
  uninstall();
}

const QMargins& FramelessWindowBehavior::resizeMargins() const {
  return _resizeMargins;
}

void FramelessWindowBehavior::setResizeMargins(const QMargins& margins) {
  if (margins != _resizeMargins) {
    _resizeMargins = margins;
    emit resizeMarginsChanged();
  }
}

const QMargins& FramelessWindowBehavior::maximizedMargins() const {
  return _maximizedMargins;
}

void FramelessWindowBehavior::setMaximizedMargins(const QMargins& margins) {
  if (margins != _maximizedMargins) {
    _maximizedMargins = margins;
    emit maximizedMarginsChanged();
  }
}

int FramelessWindowBehavior::titleBarHeight() const {
  return _titleBarHeight;
}

void FramelessWindowBehavior::setTitleBarHeight(const int height) {
  if (height != _titleBarHeight) {
    _titleBarHeight = height;
    emit titleBarHeightChanged();
  }
}

void FramelessWindowBehavior::addInteractiveWidget(QWidget* widget) {
  if (!widget)
    return;

  _interactiveWidgets.insert(widget);
}

void FramelessWindowBehavior::removeInteractiveWidget(QWidget* widget) {
  if (!widget)
    return;

  _interactiveWidgets.remove(widget);
}

void FramelessWindowBehavior::addNonInteractiveWidget(QWidget* widget) {
  if (!widget)
    return;

  _nonInteractiveWidgets.insert(widget);
}

void FramelessWindowBehavior::removeNonInteractiveWidget(QWidget* widget) {
  if (!widget)
    return;

  _nonInteractiveWidgets.remove(widget);
}

int FramelessWindowBehavior::systemMenuAreaWidth() const {
  return _systemMenuAreaWidth;
}

void FramelessWindowBehavior::setSystemMenuAreaWidth(int width) {
  width = std::max(0, width);
  if (width != _systemMenuAreaWidth) {
    _systemMenuAreaWidth = width;
    emit systemMenuAreaWidthChanged();
  }
}

void FramelessWindowBehavior::showSystemMenu(const QPoint& position) {
#ifdef WIN32
  const auto hWnd = reinterpret_cast<HWND>(_parentWindowHandle->winId());

  const auto menu = ::GetSystemMenu(hWnd, FALSE);
  if (menu) {
    const auto x = _parentWindowHandle->x() + position.x();
    const auto y = _parentWindowHandle->y() + position.y();
    const auto flags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD;
    const int cmd = ::TrackPopupMenu(menu, flags, x, y, NULL, hWnd, 0);
    if (cmd > 0) {
      SendMessage(hWnd, WM_SYSCOMMAND, cmd, 0);
    }
  }
#else
  Q_UNUSED(position);
#endif // WIN32
}

bool FramelessWindowBehavior::eventFilter(QObject* obj, QEvent* evt) {
  const auto type = evt->type();

  if (qApp->closingDown())
    return false;

  if (type == QEvent::WinIdChange || type == QEvent::Show || type == QEvent::WindowStateChange) {
    updateNativeWindowProperties();
  } else if (type == QEvent::Destroy) {
    uninstall();
  }

  return QObject::eventFilter(obj, evt);
}

bool FramelessWindowBehavior::nativeEventFilter(const QByteArray& eventType, void* message, long* result) {
#ifdef WIN32
  if (eventType != QByteArrayLiteral("windows_generic_MSG"))
    return false;

  if (!_parentWindowHandle)
    return false;

  if (qApp->closingDown())
    return false;

  const auto lpMsg = reinterpret_cast<LPMSG>(message);
  const auto winId = reinterpret_cast<WId>(lpMsg->hwnd);
  if (winId != _parentWindowHandle->winId())
    return false;

  const WPARAM wParam = lpMsg->wParam;
  const LPARAM lParam = lpMsg->lParam;

  switch (lpMsg->message) {
    case WM_NCHITTEST: {
      *result = hitTest({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
      return true;
    } break;
    case WM_NCACTIVATE: {
      if (!QtWin::isCompositionEnabled()) {
        if (result) {
          *result = 1;
        }
        return true;
      }
    } break;
    case WM_NCCALCSIZE: {
      if (wParam == TRUE) {
        if (FramelessWindowBehavior::isMaximized(_parentWindowHandle)) {
          auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

          const auto g = availableGeometry();
          const auto m = _maximizedMargins * _scaleFactor;

          params.rgrc[0].top = g.top() - m.top();
          params.rgrc[0].left = g.left() - m.left();
          params.rgrc[0].right = g.right() + m.right() + 1;
          params.rgrc[0].bottom = g.bottom() + m.bottom() + 1;
        }

        if (result) {
          *result = 0;
        }

        return true;
      }
    } break;
    case WM_GETMINMAXINFO: {
      auto lpMinMaxInfo = reinterpret_cast<LPMINMAXINFO>(lParam);

      const auto g = availableGeometry();
      const auto m = _scaleFactor * _maximizedMargins;

      lpMinMaxInfo->ptMaxPosition.x = -m.left();
      lpMinMaxInfo->ptMaxPosition.y = -m.top();
      lpMinMaxInfo->ptMaxSize.x = g.right() - g.left() + 1 + m.left() + m.right();
      lpMinMaxInfo->ptMaxSize.y = g.bottom() - g.top() + 1 + m.top() + m.bottom();

      lpMinMaxInfo->ptMinTrackSize.x = _parentWindowHandle->minimumWidth();
      lpMinMaxInfo->ptMinTrackSize.y = _parentWindowHandle->minimumHeight();
      lpMinMaxInfo->ptMaxTrackSize.x = _parentWindowHandle->maximumWidth();
      lpMinMaxInfo->ptMaxTrackSize.y = _parentWindowHandle->maximumHeight();

      if (result) {
        *result = 0;
      }
      return true;
    } break;
    case WM_NCLBUTTONDBLCLK: {
      const auto isDialog = _parentWindowWidget->windowFlags().testFlag(Qt::WindowType::Dialog);
      if (isDialog) {
        // No maximize.
        return true;
      }
      const auto mousePos = QCursor::pos();
      const auto localPos = _parentWindowHandle->mapFromGlobal(mousePos);
      const auto systemMenuArea = this->systemMenuArea();
      if (systemMenuArea.contains(localPos)) {
        // Just eat the event.
        _systemMenuWasVisible = false;
        return true;
      } else {
        const auto minimumSize = _parentWindowHandle->minimumSize();
        const auto maximumSize = _parentWindowHandle->maximumSize();
        if ((minimumSize.width() >= maximumSize.width()) || (minimumSize.height() >= maximumSize.height())) {
          if (result) {
            *result = 0;
          }
          return true;
        }
      }
    } break;
    case WM_DPICHANGED: {
      const auto previousScaleFactor = _scaleFactor;
      const auto newDPI = HIWORD(wParam);
      if (newDPI < 144) {
        _scaleFactor = 1.0;
      } else {
        _scaleFactor = 2.0;
      }

      if (!qFuzzyCompare(previousScaleFactor, _scaleFactor)) {
        emit scaleFactorChanged();
      }

      const auto hWnd = reinterpret_cast<HWND>(_parentWindowHandle->winId());
      const auto rect = reinterpret_cast<LPRECT>(lParam);
      ::SetWindowPos(hWnd, NULL, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
        SWP_NOZORDER | SWP_NOACTIVATE);
    } break;
    case WM_NCRBUTTONUP: {
      const auto mousePos = QCursor::pos();
      const auto localPos = _parentWindowHandle->mapFromGlobal(mousePos);
      const auto isDialog = _parentWindowWidget->windowFlags().testFlag(Qt::WindowType::Dialog);
      if (!isDialog) {
        showSystemMenu(localPos);
      }
      return true;
    } break;
    case WM_NCRBUTTONDOWN: {
      const auto mousePos = QCursor::pos();
      const auto localPos = _parentWindowHandle->mapFromGlobal(mousePos);
      const auto systemMenuArea = this->systemMenuArea();
      const auto isDialog = _parentWindowWidget->windowFlags().testFlag(Qt::WindowType::Dialog);
      if (systemMenuArea.contains(localPos) && !isDialog) {
        if (!_systemMenuWasVisible) {
          _systemMenuWasVisible = true;
          showSystemMenu({ 0, _titleBarHeight });
          if (result) {
            *result = 0;
          }
          return true;
        } else {
          // Just eat the event.
          _systemMenuWasVisible = false;
        }
      }
    } break;
    case WM_SYSCOMMAND: {
      // Prevent dialogs to be minimized or maximized.
      if (_parentWindowWidget->windowFlags().testFlag(Qt::WindowType::Dialog)) {
        if (lpMsg->wParam == SC_MAXIMIZE || lpMsg->wParam == SC_MINIMIZE) {
          return true;
        }
      }
    } break;
    default:
      break;
  }
#else
  Q_UNUSED(eventType);
  Q_UNUSED(message);
  Q_UNUSED(result);
#endif

  return false;
}

void FramelessWindowBehavior::install() {
  if (_parentWindowWidget) {
    // Check if containing window (QWidget, not QWindow) has the frameless flag.
    if (_parentWindowWidget->windowFlags().testFlag(Qt::WindowType::FramelessWindowHint)) {
      // Get notified when a QWindow (WinId) is attributed.
      _parentWindowWidget->installEventFilter(this);
      // First initialisation.
      updateNativeWindowProperties();
    }
  }
}

void FramelessWindowBehavior::uninstall() {
  if (_installed) {
    qApp->removeNativeEventFilter(this);

    _parentWindowHandle = nullptr;

    if (_parentWindowWidget) {
      _parentWindowWidget->removeEventFilter(this);
      _parentWindowWidget = nullptr;
    }

    _installed = false;
  }
}

void FramelessWindowBehavior::updateNativeWindowProperties() {
  // Get QWindow, which means a WinId has been attributed (it might be null until now).
  _parentWindowHandle = _parentWindowWidget->windowHandle();

  if (_parentWindowHandle) {
    // Modify the native window appearance (buttons, margins, etc).
    FramelessWindowBehavior::updateNativeWindowProperties(_parentWindowHandle);
  }

  if (_parentWindowHandle && !_installed) {
    // Get notified for native window events (resize, minimize, etc).
    qApp->installNativeEventFilter(this);

    _installed = true;
  }
}

int FramelessWindowBehavior::hitTest(const QPoint& mousePos) const {
#ifdef WIN32
  enum RegionMask {
    Client = 0x0000,
    Top = 0x0001,
    Left = 0x0010,
    Right = 0x0100,
    Bottom = 0x1000,
  };

  const auto x = mousePos.x() / _parentWindowHandle->devicePixelRatio();
  const auto y = mousePos.y() / _parentWindowHandle->devicePixelRatio();

  const auto wfg = _parentWindowHandle->frameGeometry();
  const auto draggableMargins = _resizeMargins * _scaleFactor;

  auto top = draggableMargins.top();
  auto left = draggableMargins.left();
  auto right = draggableMargins.right();
  auto bottom = draggableMargins.bottom();

  if (top <= 0)
    top = GetSystemMetrics(SM_CYFRAME);
  if (left <= 0)
    left = GetSystemMetrics(SM_CXFRAME);
  if (right <= 0)
    right = GetSystemMetrics(SM_CXFRAME);
  if (bottom <= 0)
    bottom = GetSystemMetrics(SM_CYFRAME);

  const auto result = (Top * (y < (wfg.top() + top))) | (Left * (x < (wfg.left() + left)))
                      | (Right * (x > (wfg.right() - right))) | (Bottom * (y > (wfg.bottom() - bottom)));

  const auto fixedDialog = _parentWindowWidget->windowFlags().testFlag(Qt::WindowType::MSWindowsFixedSizeDialogHint);
  const auto sizePolicy = _parentWindowWidget->sizePolicy();
  const auto wResizable = !fixedDialog && _parentWindowHandle->minimumWidth() < _parentWindowHandle->maximumWidth()
                          && sizePolicy.horizontalPolicy() != QSizePolicy::Fixed;
  const auto hResizable = !fixedDialog && _parentWindowHandle->minimumHeight() < _parentWindowHandle->maximumHeight()
                          && sizePolicy.verticalPolicy() != QSizePolicy::Fixed;

  switch (result) {
    case Top | Left:
      return wResizable && hResizable ? HTTOPLEFT : HTCLIENT;
    case Top:
      return hResizable ? HTTOP : HTCLIENT;
    case Top | Right:
      return wResizable && hResizable ? HTTOPRIGHT : HTCLIENT;
    case Right:
      return wResizable ? HTRIGHT : HTCLIENT;
    case Bottom | Right:
      return wResizable && hResizable ? HTBOTTOMRIGHT : HTCLIENT;
    case Bottom:
      return hResizable ? HTBOTTOM : HTCLIENT;
    case Bottom | Left:
      return wResizable && hResizable ? HTBOTTOMLEFT : HTCLIENT;
    case Left:
      return wResizable ? HTLEFT : HTCLIENT;
  }

  const auto localPos = _parentWindowHandle->mapFromGlobal(QPoint(x, y));
  return hitTestNativeTitleBar(localPos) ? HTCAPTION : HTCLIENT;
#else
  Q_UNUSED(mousePos);
#endif // WIN32

  return false;
}

bool FramelessWindowBehavior::hitTestNativeTitleBar(const QPoint& mousePos) const {
#ifdef WIN32
  const int scaledTitleBarHeight = _titleBarHeight * _scaleFactor;

  if (!_parentWindowWidget)
    return false;
  else if (scaledTitleBarHeight == 0)
    return false;
  else if ((scaledTitleBarHeight > 0) && (mousePos.y() >= scaledTitleBarHeight))
    return false;

  int currentIndex = -1;
  std::array<QWidget*, 32> items = { nullptr };
  auto child = _parentWindowWidget;
  QPoint p = mousePos;

  while (child && (currentIndex < 31)) {
    items[++currentIndex] = child;
    const auto grandchild = child->childAt(p);

    if (!grandchild) {
      break;
    }

    p = grandchild->mapFrom(child, p);
    child = grandchild;
  }

  while (currentIndex > 0) {
    child = items[currentIndex];
    --currentIndex;

    if (_nonInteractiveWidgets.contains(child)) {
      break;
    } else if (_interactiveWidgets.contains(child)) {
      return false;
    } else if (_parentWindowWidget == child) {
      break;
    }
  }
#else
  Q_UNUSED(mousePos);
#endif // WIN32
  return true;
}

QRect FramelessWindowBehavior::availableGeometry() const {
#ifdef WIN32
  MONITORINFO monitorInfo{ 0, RECT(), RECT(), 0 };
  monitorInfo.cbSize = sizeof(MONITORINFO);

  const auto hWnd = reinterpret_cast<HWND>(_parentWindowHandle->winId());
  const auto hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
  if (!hMonitor || !::GetMonitorInfoW(hMonitor, &monitorInfo)) {
    Q_ASSERT(NULL != hMonitor);
    return _parentWindowHandle->screen()->availableGeometry();
  }

  return QRect(monitorInfo.rcWork.left, monitorInfo.rcWork.top, monitorInfo.rcWork.right - monitorInfo.rcWork.left,
    monitorInfo.rcWork.bottom - monitorInfo.rcWork.top);
#endif // WIN32

  return QRect();
}

QRect FramelessWindowBehavior::systemMenuArea() const {
#ifdef WIN32
  QRect rect{ 0, 0, _systemMenuAreaWidth, _titleBarHeight };

  if (isMaximized(_parentWindowHandle)) {
    rect.adjust(_maximizedMargins.left(), _maximizedMargins.top(), _maximizedMargins.bottom(), 0);
  } else {
    rect.adjust(_resizeMargins.left(), _resizeMargins.top(), 0, 0);
  }

  return rect;
#endif // WIN32

  return QRect();
}

void FramelessWindowBehavior::updateNativeWindowProperties(QWindow* const window) {
#ifdef WIN32
  if (!window)
    return;

  const HWND hWnd = reinterpret_cast<HWND>(window->winId());
  if (NULL == hWnd)
    return;

  const auto currentOSVersion = QOperatingSystemVersion::current();
  if (currentOSVersion < QOperatingSystemVersion::Windows8)
    return;

  const LONG oldStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

  LONG newStyle = WS_POPUP | WS_THICKFRAME;

  if (QtWin::isCompositionEnabled()) {
    newStyle |= WS_CAPTION;
  }

  const auto flags = window->flags();

  if (flags.testFlag(Qt::CustomizeWindowHint)) {
    if (flags.testFlag(Qt::WindowSystemMenuHint))
      newStyle |= WS_SYSMENU;
    if (flags.testFlag(Qt::WindowMinimizeButtonHint))
      newStyle |= WS_MINIMIZEBOX;
    if (flags.testFlag(Qt::WindowMaximizeButtonHint))
      newStyle |= WS_MAXIMIZEBOX;
  } else {
    newStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
  }

  const LONG currentStyle = GetWindowLong(hWnd, GWL_STYLE);
  SetWindowLong(hWnd, GWL_STYLE, (currentStyle & ~oldStyle) | newStyle);

  ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

  if (QtWin::isCompositionEnabled()) {
    QtWin::extendFrameIntoClientArea(window, 1, 1, 1, 1);
  }
#else
  Q_UNUSED(window);
#endif // WIN32
}

bool FramelessWindowBehavior::isMaximized(const QWindow* const window) {
#ifdef WIN32
  if (!window)
    return false;

  const auto hWnd = reinterpret_cast<HWND>(window->winId());
  if (NULL == hWnd)
    return false;

  WINDOWPLACEMENT windowPlacement{ WINDOWPLACEMENT{} };
  if (!::GetWindowPlacement(hWnd, &windowPlacement))
    return false;

  return (SW_MAXIMIZE == windowPlacement.showCmd);
#else
  Q_UNUSED(window);
  return false;
#endif // WIN32
}
} // namespace oclero::qlementine
