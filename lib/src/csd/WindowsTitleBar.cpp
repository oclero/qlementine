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

#include <oclero/qlementine/csd/WindowsTitleBar.hpp>

#include <oclero/qlementine/csd/WindowsTitleBarButton.hpp>
#include <oclero/qlementine/csd/WindowsTitleMenuBarProxyStyle.hpp>
#include <oclero/qlementine/style/QlementineStyle.hpp>

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyle>

#include <functional>

namespace oclero::qlementine {
// Measured on Windows 10 system windows.
constexpr auto titleBarHeight = 30; // px
constexpr auto windowIconButtonWidth = 22; // px
constexpr auto windowIconSize = 16; // px
constexpr auto titleBarLeftMargin = 4; // px
constexpr auto systemTitleBarFontSize = 9; // points
constexpr auto iconAndTitleSpacing = 5; // px

class ElidedLabel : public QLabel {
public:
  using QLabel::QLabel;

protected:
  virtual void paintEvent(QPaintEvent*) override {
    QPainter p(this);
    p.setFont(font());
    p.setPen(palette().text().color());
    const auto elidedText =
      fontMetrics().elidedText(text(), Qt::TextElideMode::ElideRight, width(), Qt::TextSingleLine);
    p.drawText(rect(), alignment(), elidedText);
  }
};

/**
 * @brief Very basic widget that reacts on MousePress and draws the window icon.
 */
class SystemMenuButton : public QWidget {
public:
  SystemMenuButton(QWidget* parent)
    : QWidget(parent) {
    setSizePolicy({ QSizePolicy::Fixed, QSizePolicy::Fixed });
    setFixedHeight(titleBarHeight);
    setFocusPolicy(Qt::NoFocus);
    setContextMenuPolicy(Qt::NoContextMenu);
    qApp->installEventFilter(this);
  }

  std::function<void()> onMousePress;

  QSize sizeHint() const override {
    const auto icon = tryGetIcon();
    if (icon.isNull()) {
      return QSize{ 0, titleBarHeight };
    } else {
      return QSize{ windowIconButtonWidth, titleBarHeight };
    }
  }

protected:
  void paintEvent(QPaintEvent*) override {
    constexpr auto iconSize = windowIconSize;
    const auto icon = tryGetIcon();
    if (icon.isNull())
      return;

    const auto pixmap = icon.pixmap({ iconSize, iconSize });
    const auto x = (width() - iconSize) / 2;
    const auto y = (height() - iconSize) / 2;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPixmap(x, y, iconSize, iconSize, pixmap);
  }

  void mousePressEvent(QMouseEvent* event) override {
    if (event->button() == Qt::LeftButton) {
      if (onMousePress) {
        onMousePress();
      }
    }
  }

  bool event(QEvent* e) override {
    if (e->type() == QEvent::WindowIconChange) {
      update();
      updateGeometry();
    }
    return QWidget::event(e);
  }

  bool eventFilter(QObject* o, QEvent* e) override {
    if (e->type() == QEvent::ApplicationWindowIconChange) {
      update();
      updateGeometry();
    }
    return QWidget::eventFilter(o, e);
  }

private:
  QIcon tryGetIcon() const {
    auto icon = windowIcon();
    if (icon.isNull())
      icon = QApplication::windowIcon();
    return icon;
  }
};

WindowsTitleBar::WindowsTitleBar(QWidget* parent)
  : QWidget(parent) {
  setupUi();
  setFixedHeight(titleBarHeight);
  setSizePolicy({ QSizePolicy::Preferred, QSizePolicy::Fixed });
  setFocusPolicy(Qt::NoFocus);

  // Set window icon.
  auto* const parentWindow = window();
  auto windowIcon = parentWindow->windowIcon();
  if (windowIcon.isNull()) {
    windowIcon = QApplication::windowIcon();
  }
  setWindowIcon(windowIcon);

  // Set title.
  updateWindowTitle();
  QObject::connect(parentWindow, &QWidget::windowTitleChanged, this, &WindowsTitleBar::updateWindowTitle);
}

void WindowsTitleBar::setupUi() {
  _rootLayout = new QHBoxLayout(this);
  _rootLayout->setSpacing(0);
  _rootLayout->setContentsMargins(titleBarLeftMargin, 0, 0, 0);
  setLayout(_rootLayout);

  // Icone
  _iconButton = new SystemMenuButton(this);
  _rootLayout->addWidget(_iconButton);
  _iconButton->onMousePress = [this]() {
    if (systemWindowType() == SystemWindowType::Default) {
      emit systemMenuTriggered();
    }
  };

  // MenuBar
  _menuBar = new QMenuBar(this);
  _menuBar->setFocusPolicy(Qt::NoFocus);
  _menuBar->setSizePolicy({ QSizePolicy::Preferred, QSizePolicy::Fixed });
  _menuBar->setNativeMenuBar(false);
  _rootLayout->addWidget(_menuBar, 0, Qt::AlignLeft);
  _menuBar->setStyle(new WindowsTitleMenuBarProxyStyle(_menuBar->style()));

  // Spacer
  auto* horizontalSpacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  _rootLayout->addItem(horizontalSpacer1);

  // Window buttons.
  _minimizeButton = new WindowsTitleBarButton(WindowsTitleBarButton::ButtonType::Minimize, this);
  _rootLayout->addWidget(_minimizeButton, 0, Qt::AlignRight);

  _maximizeButton = new WindowsTitleBarButton(WindowsTitleBarButton::ButtonType::Maximize, this);
  _rootLayout->addWidget(_maximizeButton, 0, Qt::AlignRight);

  _closeButton = new WindowsTitleBarButton(WindowsTitleBarButton::ButtonType::Close, this);
  _rootLayout->addWidget(_closeButton, 0, Qt::AlignRight);

  QObject::connect(_minimizeButton, &QAbstractButton::clicked, this, &WindowsTitleBar::minimizeWindow);
  QObject::connect(_maximizeButton, &QAbstractButton::clicked, this, &WindowsTitleBar::maximizeWindow);
  QObject::connect(_closeButton, &QAbstractButton::clicked, this, &WindowsTitleBar::closeWindow);

  // Title (not in layout).
  _titleLabel = new ElidedLabel(this);
  _titleLabel->setFocusPolicy(Qt::NoFocus);
  _titleLabel->setAttribute(Qt::WidgetAttribute::WA_TransparentForMouseEvents);
  if (qobject_cast<QlementineStyle*>(style()) == nullptr) {
    // Use system font if not using our QStyle.
    const QFont systemFont(QStringLiteral("Segoe UI"), systemTitleBarFontSize);
    _titleLabel->setFont(systemFont);
  }
  _titleLabel->setAlignment(Qt::AlignCenter);
  _titleLabel->setSizePolicy({ QSizePolicy::Preferred, QSizePolicy::Preferred });
  _titleLabel->setEnabled(false);
}

QWidget* WindowsTitleBar::iconButton() const {
  return _iconButton;
}

QAbstractButton* WindowsTitleBar::minimizeButton() const {
  return _minimizeButton;
}

QAbstractButton* WindowsTitleBar::maximizeButton() const {
  return _maximizeButton;
}

QAbstractButton* WindowsTitleBar::closeButton() const {
  return _closeButton;
}

QMenuBar* WindowsTitleBar::menuBar() const {
  return _menuBar;
}

const QColor& WindowsTitleBar::backgroundColor() const {
  return _backgroundColor;
}

void WindowsTitleBar::setBackgroundColor(const QColor& color) {
  if (color != _backgroundColor) {
    _backgroundColor = color;
    update();
  }
}

SystemTheme WindowsTitleBar::buttonsSystemTheme() const {
  return _minimizeButton->systemTheme();
}

void WindowsTitleBar::setButtonsSystemTheme(const SystemTheme systemTheme) {
  if (buttonsSystemTheme() != systemTheme) {
    _minimizeButton->setSystemTheme(systemTheme);
    _maximizeButton->setSystemTheme(systemTheme);
    _closeButton->setSystemTheme(systemTheme);
    emit buttonsSystemThemeChanged();
  }
}

SystemWindowType WindowsTitleBar::systemWindowType() const {
  return _closeButton->systemWindowType();
}

void WindowsTitleBar::setSystemWindowType(const SystemWindowType type) {
  if (systemWindowType() != type) {
    _menuBar->setVisible(type == SystemWindowType::Default);
    _minimizeButton->setVisible(type == SystemWindowType::Default);
    _maximizeButton->setVisible(type == SystemWindowType::Default);
    _closeButton->setSystemWindowType(type);
    updateTitleGeometry();
    emit systemWindowTypeChanged();
  }
}

void WindowsTitleBar::updateWindowTitle() {
  auto* const parentWindow = window();
  const auto title = parentWindow->windowTitle();
  _titleLabel->setText(title);
  updateTitleGeometry();
}

void WindowsTitleBar::minimizeWindow() {
  if (auto window = this->window()) {
    window->showMinimized();
  }
}

void WindowsTitleBar::maximizeWindow() {
  if (auto window = this->window()) {
    if (window->windowState() & Qt::WindowMaximized) {
      window->showNormal();
    } else {
      window->showMaximized();
    }
  }
}

void WindowsTitleBar::closeWindow() {
  if (auto* window = this->window()) {
    window->close();
  }
}

void WindowsTitleBar::paintEvent(QPaintEvent* event) {
  if (_backgroundColor.isValid() && _backgroundColor.alpha() > 0) {
    QPainter painter(this);
    painter.fillRect(event->rect(), _backgroundColor);
  }
}

void WindowsTitleBar::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  updateTitleGeometry();
}

void WindowsTitleBar::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  updateTitleGeometry();
}

bool WindowsTitleBar::event(QEvent* event) {
  // Disable title when not activated.
  const auto type = event->type();
  if (type == QEvent::WindowActivate) {
    _titleLabel->setEnabled(true);
  } else if (type == QEvent::WindowDeactivate) {
    _titleLabel->setEnabled(false);
  } else if (type == QEvent::Enter || type == QEvent::Leave) {
    _menuBar->update();
  } else if (type == QEvent::LayoutRequest) {
    updateTitleGeometry();
  }

  return QWidget::event(event);
}

void WindowsTitleBar::updateTitleGeometry() {
  if (systemWindowType() == SystemWindowType::Dialog
      || (systemWindowType() == SystemWindowType::Default && _menuBar->width() == 0)) {
    // Align left.
    const auto sh = _titleLabel->sizeHint();
    _titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    const auto x = _rootLayout->contentsMargins().left() + _iconButton->sizeHint().width() + iconAndTitleSpacing;
    const auto y = (height() - sh.height()) / 2;
    const auto availableWidth = width() - (_menuBar->isVisibleTo(this) ? _menuBar->width() : 0)
                                - (_minimizeButton->isVisibleTo(this) ? _minimizeButton->width() : 0)
                                - (_maximizeButton->isVisibleTo(this) ? _maximizeButton->width() : 0)
                                - _closeButton->width() - x;
    const auto w = std::min(sh.width(), availableWidth);
    _titleLabel->setGeometry(QRect(QPoint(x, y), QSize(w, sh.height())));
    _titleLabel->setVisible(true);
  } else {
    // Align center.
    _titleLabel->setAlignment(Qt::AlignCenter);
    const auto sh = _titleLabel->sizeHint();
    const auto x = (width() - sh.width()) / 2;
    const auto y = (height() - sh.height()) / 2;
    _titleLabel->setGeometry(QRect(QPoint(x, y), sh));
    const auto titleGeometry = _titleLabel->geometry().marginsAdded({ 16, 0, 4, 0 });
    const auto overlapsMenuBar =
      _menuBar->isVisibleTo(this) && _menuBar->width() > 0 && titleGeometry.intersects(_menuBar->geometry());
    const auto overlapsMinimize =
      _minimizeButton->isVisibleTo(this) && titleGeometry.intersects(_minimizeButton->geometry());
    const auto overlapsMaximize =
      _maximizeButton->isVisibleTo(this) && titleGeometry.intersects(_maximizeButton->geometry());
    const auto overlapsClose = _closeButton->isVisibleTo(this) && titleGeometry.intersects(_closeButton->geometry());

    const auto visible = !overlapsMenuBar && !overlapsMinimize && !overlapsMaximize && !overlapsClose;
    _titleLabel->setVisible(visible);
  }
}
} // namespace oclero::qlementine
