// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <oclero/qlementine/widgets/RoundedFocusFrame.hpp>

#include <QAbstractButton>
#include <QVariantAnimation>

namespace oclero::qlementine {
class QStyleOptionFocusRoundedRect;

/// A switch toggle button like on iOS and Android.
class Switch : public QAbstractButton {
  Q_OBJECT

public:
  explicit Switch(QWidget* parent = nullptr);

  QSize sizeHint() const override;

  void initStyleOptionFocus(QStyleOptionFocusRoundedRect& opt) const;

  void setTristate(bool);
  bool isTristate() const;

  Qt::CheckState checkState() const;
  void setCheckState(Qt::CheckState state);

Q_SIGNALS:
  void checkStateChanged(int);

protected:
  void paintEvent(QPaintEvent* e) override;
  void enterEvent(QEnterEvent* e) override;
  void leaveEvent(QEvent* e) override;
  void changeEvent(QEvent* e) override;
  void focusInEvent(QFocusEvent* e) override;
  void focusOutEvent(QFocusEvent* e) override;
  void checkStateSet() override;
  void nextCheckState() override;

private:
  void setupAnimation();
  void startAnimation();
  QRect getSwitchRect() const;

  const QColor& getBgColor() const;
  const QColor& getBorderColor() const;
  const QColor& getFgColor() const;
  const QColor& getTextColor() const;

private:
  int _handlePadding{ 2 };
  bool _isMouseOver{ false };
  bool _tristate{ false };
  bool _intermediate{ false };
  bool _blockRefresh{ false };
  Qt::CheckState _publishedState{ Qt::CheckState::Unchecked };
  QVariantAnimation _handleAnimation;
  QVariantAnimation _bgAnimation;
  QVariantAnimation _borderAnimation;
  QVariantAnimation _fgAnimation;
  RoundedFocusFrame* _focusFrame{ nullptr };
};
} // namespace oclero::qlementine
