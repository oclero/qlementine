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

#include <oclero/qlementine/widgets/FramelessWindow.hpp>

#include <oclero/qlementine/csd/FramelessWindowBehavior.hpp>
#include <oclero/qlementine/csd/WindowsTitleBar.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>

#include <QBoxLayout>
#include <QMenuBar>
#include <QPainter>
#include <QPointer>
#include <QAbstractButton>
#include <QWindowStateChangeEvent>

namespace oclero::qlementine {
struct FramelessWindow::Impl {
  FramelessWindow& owner;
  QVBoxLayout* rootLayout{ nullptr };
  QMenuBar* menuBar{ nullptr };
  QWidget* contentWidget{ nullptr };
#ifdef WIN32
  WindowsTitleBar* titleBar{ nullptr };
  QPointer<FramelessWindowBehavior> behavior{ nullptr };
#endif

  Impl(FramelessWindow& o)
    : owner(o) {}

  void setupUi() {
    rootLayout = new QVBoxLayout(&owner);
    rootLayout->setSpacing(0);
    rootLayout->setContentsMargins(0, 0, 0, 0);

#ifdef WIN32
    // Title bar: stuck the top.
    titleBar = new WindowsTitleBar(&owner);
    rootLayout->addWidget(titleBar, 0, Qt::AlignTop);
    menuBar = titleBar->menuBar();

    // Behavior to handle native events.
    createBehavior();
    updateTheme();

    QObject::connect(titleBar, &WindowsTitleBar::systemMenuTriggered, titleBar, [this]() {
      auto* button = titleBar->iconButton();
      const auto leftMargin = titleBar->layout()->contentsMargins().left();
      const auto pos = owner.mapFromGlobal(button->mapToGlobal(QPoint{ -leftMargin, button->height() }));
      if (behavior) {
        behavior->showSystemMenu(pos);
      }
    });
#else
    menuBar = new QMenuBar(&owner);
    menuBar->setNativeMenuBar(true);
    rootLayout->setMenuBar(menuBar);
#endif
  }
#ifdef WIN32
  void createBehavior() {
    if (behavior)
      return;

    behavior = new FramelessWindowBehavior(&owner);
    behavior->addInteractiveWidget(titleBar->iconButton());
    behavior->addInteractiveWidget(titleBar->minimizeButton());
    behavior->addInteractiveWidget(titleBar->maximizeButton());
    behavior->addInteractiveWidget(titleBar->closeButton());
    behavior->addInteractiveWidget(titleBar->menuBar());
  }

  void updateTheme() {
    // Check if background color is light or dark and adapt the buttons.
    const auto& bgColor = owner.palette().color(QPalette::ColorRole::Window);
    const auto isLight = bgColor.lightnessF() >= 0.5;
    const auto systemTheme = isLight ? qlementine::SystemTheme::Light : qlementine::SystemTheme::Dark;
    titleBar->setButtonsSystemTheme(systemTheme);
  }
#endif
};

FramelessWindow::FramelessWindow(QWidget* parent)
  : QWidget(parent)
  , _impl{ new Impl(*this) } {
  // Get rid of system's native window frame.
  setWindowFlag(Qt::WindowType::Window, true);
  setWindowFlag(Qt::WindowType::WindowContextHelpButtonHint, false);
#ifdef WIN32
  setWindowFlag(Qt::FramelessWindowHint, true);
#endif
  setFocusPolicy(Qt::NoFocus);
  _impl->setupUi();
}

FramelessWindow::~FramelessWindow() {}

QMenuBar* FramelessWindow::menuBar() const {
  return _impl->menuBar;
}

QWidget* FramelessWindow::contentWidget() const {
  return _impl->contentWidget;
}

void FramelessWindow::setContentWidget(QWidget* content) {
  if (content != _impl->contentWidget) {
#ifdef WIN32
    const auto index = 1;
#else
    const auto index = 0;
#endif
    // Remove current content. Keep QMenuBar on Windows.
    if (_impl->contentWidget) {
      if (auto* layoutItem = _impl->rootLayout->takeAt(index)) {
        if (auto* widget = layoutItem->widget()) {
          delete widget;
        }
        delete layoutItem;
      }
    }

    _impl->contentWidget = content;

    // Add new content.
    if (_impl->contentWidget) {
      _impl->contentWidget->setParent(this);
      _impl->contentWidget->setSizePolicy({ QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding });
      _impl->contentWidget->setFocusPolicy(Qt::NoFocus);
      _impl->rootLayout->addWidget(_impl->contentWidget, 1);
    }
  }
}

void FramelessWindow::paintEvent(QPaintEvent* e) {
  QPainter painter(this);
  const auto& bgColor = palette().color(QPalette::ColorRole::Window);
  painter.fillRect(rect(), bgColor);

  QWidget::paintEvent(e);
}

bool FramelessWindow::event(QEvent* e) {
#ifdef WIN32
  const auto type = e->type();
  switch (type) {
    case QEvent::Type::PaletteChange:
    case QEvent::Type::ApplicationPaletteChange:
      _impl->updateTheme();
      break;
    case QEvent::Type::Show: {
      // Ensure behavior is created and installed.
      _impl->createBehavior();

      // Usually called before showing on screen.
      const auto systemWindowType =
        windowFlags().testFlag(Qt::WindowType::Dialog) ? SystemWindowType::Dialog : SystemWindowType::Default;
      _impl->titleBar->setSystemWindowType(systemWindowType);
    } break;
    case QEvent::Type::WindowStateChange:
      if (static_cast<QWindowStateChangeEvent*>(e)) {
        if (windowFlags().testFlag(Qt::WindowType::Dialog)) {
          const auto newState = windowState();
          if (newState != Qt::WindowState::WindowNoState) {
            setWindowState(Qt::WindowState::WindowNoState);
          }
        }
      }
      break;
    case QEvent::Type::Close:
      // Prevents a crash.
      if (_impl->behavior) {
        delete _impl->behavior;
      }
      break;
    default:
      break;
  }
#endif
  return QWidget::event(e);
}
} // namespace oclero::qlementine
