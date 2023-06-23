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

#include <oclero/qlementine/csd/WindowsTitleBarButton.hpp>

#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionButton>

namespace oclero::qlementine {
// This size has been measured on Windows 10 system button icons (10*10px).
constexpr auto systemIconSize = 10; // px
constexpr auto systemButtonWidth = 45; // px
constexpr auto systemButtonWidthSmall = 30; // px
constexpr auto systemButtonHeight = 30; // px

WindowsTitleBarButton::WindowsTitleBarButton(const ButtonType type, QWidget* parent)
  : WindowsTitleBarButton(parent) {
  setButtonType(type);
  setToolTip(getTooltip());
  _theme = getTheme(type, _systemTheme);
}

WindowsTitleBarButton::WindowsTitleBarButton(QWidget* parent)
  : QAbstractButton(parent) {
  setSizePolicy({ QSizePolicy::Fixed, QSizePolicy::Fixed });
  setFixedSize(sizeHint());
  setFocusPolicy(Qt::NoFocus);
  setContextMenuPolicy(Qt::NoContextMenu);
  setIconSize(QSize(systemIconSize, systemIconSize));

  _backgroundAnimation.setStartValue(getBackgroundColor(_state));
  QObject::connect(&_backgroundAnimation, &QVariantAnimation::valueChanged, this, [this]() {
    this->update();
  });

  const auto window = this->window();
  window->installEventFilter(this);
}

WindowsTitleBarButton::ButtonType WindowsTitleBarButton::buttonType() const {
  return _type;
}

void WindowsTitleBarButton::setButtonType(const ButtonType type) {
  if (type != _type) {
    _type = type;
    _theme = getTheme(_type, _systemTheme);
    setToolTip(getTooltip());
    update();
    emit buttonTypeChanged();
  }
}

SystemWindowType WindowsTitleBarButton::systemWindowType() const {
  return _windowType;
}

void WindowsTitleBarButton::setSystemWindowType(const SystemWindowType type) {
  if (type != _windowType) {
    _windowType = type;
    setFixedSize(sizeHint());
    updateGeometry();
    emit systemWindowTypeChanged();
  }
}

QSize WindowsTitleBarButton::sizeHint() const {
  return _windowType == SystemWindowType::Default ? QSize(systemButtonWidth, systemButtonHeight)
                                                  : QSize(systemButtonWidthSmall, systemButtonHeight);
}

const QPixmap& WindowsTitleBarButton::getIcon() const {
  // Special case.
  const auto useAlternative = _type == ButtonType::Maximize && window()->isMaximized();

  switch (_state) {
    case State::Pressed:
    case State::Hovered:
      return useAlternative ? _theme.alternativeIconHover : _theme.iconHover;
    default:
      return useAlternative ? _theme.alternativeIconNormal : _theme.iconNormal;
  }
}

void WindowsTitleBarButton::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QStyleOptionButton option;
  option.initFrom(this);

  // Fix bug.
  if (isDown()) {
    option.state.setFlag(QStyle::StateFlag::State_Sunken);
  }

  const auto newState = getState(option);
  setState(newState);

  // Background
  const auto rect = event->rect();
  painter.fillRect(rect, getBackgroundColor());

  // Foreground
  const auto& icon = getIcon();
  const auto iconX = (width() - systemIconSize) / 2;
  const auto iconY = (height() - systemIconSize) / 2;
  if (_state == State::Deactivated || _state == State::Disabled) {
    painter.setOpacity(0.5);
  }
  painter.drawPixmap(iconX, iconY, icon);
}

bool WindowsTitleBarButton::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::Hide:
      _backgroundAnimation.stop();
      update();
      break;
    case QEvent::Enter:
    case QEvent::Leave:
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
      update();
      break;
    default:
      break;
  }

  return QAbstractButton::event(event);
}

bool WindowsTitleBarButton::eventFilter(QObject* obj, QEvent* event) {
  const auto type = event->type();
  if (type == QEvent::WindowStateChange) {
    update();
    setToolTip(getTooltip());
  }

  return QAbstractButton::eventFilter(obj, event);
}

WindowsTitleBarButton::State WindowsTitleBarButton::getState(const QStyleOptionButton& option) const {
  State state;
  if (!option.state.testFlag(QStyle::StateFlag::State_Enabled)) {
    state = State::Disabled;
  } else if (option.state.testFlag(QStyle::StateFlag::State_Sunken)
             && option.state.testFlag(QStyle::StateFlag::State_MouseOver)) {
    state = State::Pressed;
  } else if (option.state.testFlag(QStyle::StateFlag::State_MouseOver)) {
    state = State::Hovered;
  } else if (window()->isActiveWindow()) {
    state = State::Normal;
  } else {
    state = State::Deactivated;
  }
  return state;
}

void WindowsTitleBarButton::setState(const State newState) {
  if (newState != _state) {
    const auto currentBackgroundColor = getBackgroundColor();
    const auto previousState = _state;
    _state = newState;
    const auto wasActive = previousState != State::Deactivated && previousState != State::Disabled;
    const auto animationDuration = newState == State::Normal && wasActive && isVisible() ? _animationDuration : 0;

    _backgroundAnimation.stop();
    _backgroundAnimation.setDuration(animationDuration);
    _backgroundAnimation.setStartValue(QVariant::fromValue<QColor>(currentBackgroundColor));
    _backgroundAnimation.setEndValue(QVariant::fromValue<QColor>(getBackgroundColor(_state)));
    _backgroundAnimation.start();
  }
}

QColor WindowsTitleBarButton::getBackgroundColor() const {
  if (_backgroundAnimation.state() == QAbstractAnimation::Running) {
    return _backgroundAnimation.currentValue().value<QColor>();
  } else {
    return getBackgroundColor(_state);
  }
}

QString WindowsTitleBarButton::getTooltip() const {
  switch (_type) {
    case ButtonType::Close:
      return tr("Close");
    case ButtonType::Minimize:
      return tr("Minimize");
    case ButtonType::Maximize:
      return window()->isMaximized() ? tr("Restore Down") : tr("Maximize");
    default:
      return QLatin1String("");
  }
}

const QColor& WindowsTitleBarButton::getBackgroundColor(const State state) const {
  switch (state) {
    case State::Pressed:
      return _theme.backgroundColorPressed;
    case State::Hovered:
      return _theme.backgroundColorHover;
    case State::Disabled:
      return _theme.backgroundColorDisabled;
    case State::Deactivated:
      return _theme.backgroundColorDeactivated;
    case State::Normal:
    default:
      return _theme.backgroundColorNormal;
  }
}

WindowsTitleBarButton::Theme WindowsTitleBarButton::getTheme(const ButtonType type, const SystemTheme systemeTheme) {
  // These colors have been taken from Windows 10 system windows.
  // The icons have been made manually to look like the actual icons.
  const auto light = systemeTheme == SystemTheme::Light;
  Theme t;
  switch (type) {
    case ButtonType::Minimize:
      t.iconNormal = light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/minimize.svg"))
                           : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/minimize.svg"));
      t.iconHover = light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/minimize.svg"))
                          : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/minimize.svg"));
      t.backgroundColorNormal = light ? QColor{ 0, 0, 0, 0 } : QColor{ 255, 255, 255, 0 };
      t.backgroundColorHover = light ? QColor{ 0, 0, 0, 24 } : QColor{ 255, 255, 255, 24 };
      t.backgroundColorPressed = light ? QColor{ 0, 0, 0, 48 } : QColor{ 255, 255, 255, 48 };
      t.backgroundColorDisabled = light ? QColor{ 0, 0, 0, 0 } : QColor{ 255, 255, 255, 0 };
      t.backgroundColorDeactivated = light ? QColor{ 0, 0, 0, 0 } : QColor{ 255, 255, 255, 0 };
      break;
    case ButtonType::Maximize:
      t.iconNormal = light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/maximize.svg"))
                           : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/maximize.svg"));
      t.iconHover = light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/maximize.svg"))
                          : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/maximize.svg"));
      t.alternativeIconNormal =
        light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/maximize-alternative.svg"))
              : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/maximize-alternative.svg"));
      t.alternativeIconHover =
        light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/maximize-alternative.svg"))
              : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/maximize-alternative.svg"));
      t.backgroundColorNormal = light ? QColor{ 0, 0, 0, 0 } : QColor{ 255, 255, 255, 0 };
      t.backgroundColorHover = light ? QColor{ 0, 0, 0, 24 } : QColor{ 255, 255, 255, 24 };
      t.backgroundColorPressed = light ? QColor{ 0, 0, 0, 48 } : QColor{ 255, 255, 255, 48 };
      t.backgroundColorDisabled = light ? QColor{ 0, 0, 0, 0 } : QColor{ 255, 255, 255, 0 };
      t.backgroundColorDeactivated = light ? QColor{ 0, 0, 0, 0 } : QColor{ 255, 255, 255, 0 };
      break;
    case ButtonType::Close:
      t.iconNormal = light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/close.svg"))
                           : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/close.svg"));
      t.iconHover = light ? QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/light/close-hover.svg"))
                          : QPixmap(QStringLiteral(":/qlementine/resources/csd/windows/dark/close-hover.svg"));
      t.backgroundColorNormal = light ? QColor{ 232, 17, 35, 0 } : QColor{ 232, 17, 35, 0 };
      t.backgroundColorHover = light ? QColor{ 232, 17, 35, 255 } : QColor{ 232, 17, 35, 255 };
      t.backgroundColorPressed = light ? QColor{ 232, 17, 35, 152 } : QColor{ 232, 17, 35, 154 };
      t.backgroundColorDisabled = light ? QColor{ 232, 17, 35, 0 } : QColor{ 232, 17, 35, 0 };
      t.backgroundColorDeactivated = light ? QColor{ 232, 17, 35, 0 } : QColor{ 232, 17, 35, 0 };
      break;
  }
  return t;
}

SystemTheme WindowsTitleBarButton::systemTheme() const {
  return _systemTheme;
}

void WindowsTitleBarButton::setSystemTheme(const SystemTheme systemTheme) {
  if (systemTheme != _systemTheme) {
    _systemTheme = systemTheme;
    _theme = getTheme(_type, _systemTheme);
    update();
    emit systemThemeChanged();
  }
}
} // namespace oclero::qlementine
