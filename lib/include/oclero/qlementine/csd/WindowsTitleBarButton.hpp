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

#pragma once

#include <QPropertyAnimation>
#include <QPushButton>

#include <oclero/qlementine/csd/WindowsTitleBarCommon.hpp>

namespace oclero::qlementine {
/**
 * @brief Button that mimicks the look of Windows 10 window buttons.
 */
class WindowsTitleBarButton : public QAbstractButton {
  Q_OBJECT

  Q_PROPERTY(WindowsTitleBarButton::ButtonType buttonType READ buttonType WRITE setButtonType NOTIFY buttonTypeChanged)
  Q_PROPERTY(
    oclero::qlementine::SystemTheme systemTheme READ systemTheme WRITE setSystemTheme NOTIFY systemThemeChanged)
  Q_PROPERTY(oclero::qlementine::SystemWindowType systemWindowType READ systemWindowType WRITE setSystemWindowType
      NOTIFY systemWindowTypeChanged)

public:
  /**
   * @brief
   */
  struct Theme {
    QColor backgroundColorNormal{ 0, 0, 0, 0 };
    QColor backgroundColorHover{ 0, 0, 0, 24 };
    QColor backgroundColorPressed{ 0, 0, 0, 48 };
    QColor backgroundColorDisabled{ 0, 0, 0, 0 };
    QColor backgroundColorDeactivated{ 0, 0, 0, 0 };
    QPixmap iconNormal;
    QPixmap iconHover;
    QPixmap alternativeIconNormal;
    QPixmap alternativeIconHover;
  };

  enum class ButtonType {
    Minimize,
    Maximize,
    Close,
  };
  Q_ENUM(ButtonType)

public:
  explicit WindowsTitleBarButton(QWidget* parent = nullptr);
  explicit WindowsTitleBarButton(const ButtonType type, QWidget* parent = nullptr);

  SystemTheme systemTheme() const;
  void setSystemTheme(const SystemTheme systemTheme);
  Q_SIGNAL void systemThemeChanged();

  ButtonType buttonType() const;
  void setButtonType(const ButtonType type);
  Q_SIGNAL void buttonTypeChanged();

  SystemWindowType systemWindowType() const;
  void setSystemWindowType(const SystemWindowType type);
  Q_SIGNAL void systemWindowTypeChanged();

  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent* event) override;
  bool event(QEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  enum class State {
    Normal,
    Hovered,
    Pressed,
    Disabled,
    Deactivated,
  };

  State getState(const QStyleOptionButton& option) const;
  void setState(const State state);
  QColor getBackgroundColor() const;
  QString getTooltip() const;
  const QPixmap& getIcon() const;
  const QColor& getBackgroundColor(const State state) const;
  static Theme getTheme(const ButtonType type, const SystemTheme systemeTheme);

  ButtonType _type{ ButtonType::Minimize };
  SystemWindowType _windowType{ SystemWindowType::Default };
  State _state{ State::Normal };
  Theme _theme;
  SystemTheme _systemTheme{ SystemTheme::Light };
  int _animationDuration{ 100 };
  QVariantAnimation _backgroundAnimation;
};
} // namespace oclero::qlementine
