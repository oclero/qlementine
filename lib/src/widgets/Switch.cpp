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

#include <oclero/qlementine/widgets/Switch.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/QlementineStyleOption.hpp>
#include <oclero/qlementine/style/Theme.hpp>
#include <oclero/qlementine/style/QlementineStyleOption.hpp>

#include <oclero/qlementine/utils/StateUtils.hpp>
#include <oclero/qlementine/utils/ImageUtils.hpp>
#include <oclero/qlementine/utils/FontUtils.hpp>

#include <QPainter>
#include <QEvent>

namespace oclero::qlementine {
Switch::Switch(QWidget* parent)
  : QAbstractButton(parent) {
  setCheckable(true);
  setChecked(false);
  setAutoRepeat(false);
  setupAnimation();
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  _handlePadding = qlementineStyle ? qlementineStyle->theme().borderWidth * 2 : 2;

  // Focus frame.
  _focusFrame = new RoundedFocusFrame(this);
  _focusFrame->setRadiuses(qlementineStyle ? qlementineStyle->theme().borderRadius : 0);
  _focusFrame->setWidget(this);
}

QSize Switch::sizeHint() const {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);

  const auto fm = fontMetrics();
  const auto textW = qlementine::textWidth(fm, text());
  const auto textH = fm.height();
  const auto spacing = style->pixelMetric(QStyle::PM_LayoutHorizontalSpacing);
  const auto switchRect = getSwitchRect();
  const auto w = (textW > 0 ? textW + spacing : 0) + switchRect.width();
  const auto defaultH = qlementineStyle ? qlementineStyle->theme().controlHeightMedium : 0;
  const auto h = std::max({ textH, switchRect.height(), defaultH });
  return { w, h };
}

void Switch::paintEvent(QPaintEvent*) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);

  const auto* style = this->style();
  const auto spacing = style->pixelMetric(QStyle::PM_LayoutHorizontalSpacing);
  const auto contentRect = rect();
  const auto iconSize = this->iconSize();
  const auto hasIcon = !icon().isNull() || iconSize.isEmpty();
  const auto hasText = !text().isEmpty();

  // Draw switch button.
  const auto switchRect = getSwitchRect();
  const auto switchRadius = switchRect.height() / 2.;
  const auto& bgColor = _bgAnimation.currentValue().value<QColor>();
  const auto& fgColor = _fgAnimation.currentValue().value<QColor>();
  const auto textColor = getTextColor();
  p.setPen(Qt::NoPen);
  p.setBrush(bgColor);
  p.drawRoundedRect(switchRect, switchRadius, switchRadius);


  // Draw handle.
  const auto handleXRatio = _handleAnimation.currentValue().toDouble();
  const auto handleDiameter = static_cast<double>(switchRect.height() - _handlePadding * 2);
  const auto handleGrooveWidth = switchRect.width() - _handlePadding * 2 - handleDiameter;
  const auto handleX = switchRect.x() + _handlePadding + handleGrooveWidth * handleXRatio;
  const auto handleY = static_cast<double>(switchRect.y() + _handlePadding);
  const auto handleRect = QRectF{ handleX, handleY, handleDiameter, handleDiameter };
  p.setBrush(fgColor);
  p.drawEllipse(handleRect);
  auto availableX = switchRect.x() + switchRect.width() + spacing;
  auto availableW = contentRect.width() - switchRect.width() - spacing;

  // Draw icon.
  const auto extent = iconSize.height();
  if (hasIcon && availableW >= extent) {
    const auto pixmap = icon().pixmap(extent, QIcon::Mode::Normal, QIcon::State::On);
    const auto coloredPixmap = colorizePixmap(pixmap, textColor);
    const auto iconX = availableX;
    const auto iconY = contentRect.y() + (contentRect.height() - extent) / 2;
    const auto iconRect = QRect{ iconX, iconY, extent, extent };
    p.drawPixmap(iconRect, coloredPixmap);

    availableX += extent + spacing;
    availableW -= extent + spacing;
  }

  // Draw text.
  if (hasText && availableW > 0) {
    const auto textX = availableX;
    const auto textY = contentRect.y();
    const auto textH = contentRect.height();
    const auto textW = availableW;
    const auto textRect = QRect{ textX, textY, textW, textH };
    const auto fm = fontMetrics();
    const auto elidedText = fm.elidedText(text(), Qt::TextElideMode::ElideRight, textRect.width(), Qt::TextSingleLine);
    p.setBrush(Qt::NoBrush);
    p.setPen(textColor);
    p.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, elidedText);
  }
}

void Switch::enterEvent(QEvent* e) {
  QAbstractButton::enterEvent(e);
  _isMouseOver = true;
  startAnimation();
}

void Switch::changeEvent(QEvent* e) {
  QAbstractButton::changeEvent(e);
  if (e->type() == QEvent::Type::EnabledChange) {
    startAnimation();
  }
}

void Switch::checkStateSet() {
  QAbstractButton::checkStateSet();
  startAnimation();
}

void Switch::startAnimation() {
  const auto animationDuration = isVisible() ? style()->styleHint(QStyle::SH_Widget_Animation_Duration) : 0;

  const auto currentBg = _bgAnimation.currentValue();
  _bgAnimation.stop();
  _bgAnimation.setDuration(animationDuration);
  _bgAnimation.setStartValue(currentBg);
  _bgAnimation.setEndValue(getBgColor());
  _bgAnimation.start();

  const auto currentFg = _fgAnimation.currentValue();
  _fgAnimation.stop();
  _fgAnimation.setDuration(animationDuration);
  _fgAnimation.setStartValue(currentFg);
  _fgAnimation.setEndValue(getFgColor());
  _fgAnimation.start();

  const auto currentXRatio = _handleAnimation.currentValue();
  _handleAnimation.stop();
  _handleAnimation.setDuration(animationDuration);
  _handleAnimation.setStartValue(currentXRatio);
  _handleAnimation.setEndValue(isChecked() ? 1. : 0.);
  _handleAnimation.start();
}

void Switch::setupAnimation() {
  constexpr auto animationDuration = 0; // Don't animate before it is shown.
  _bgAnimation.setDuration(animationDuration);
  _bgAnimation.setEasingCurve(QEasingCurve::Type::OutCubic);
  _bgAnimation.setStartValue(getBgColor());
  _bgAnimation.setEndValue(getBgColor());
  QObject::connect(&_bgAnimation, &QVariantAnimation::valueChanged, this, [this]() {
    update();
  });

  _fgAnimation.setDuration(animationDuration);
  _fgAnimation.setEasingCurve(QEasingCurve::Type::OutCubic);
  _fgAnimation.setStartValue(getFgColor());
  _fgAnimation.setEndValue(getFgColor());
  QObject::connect(&_fgAnimation, &QVariantAnimation::valueChanged, this, [this]() {
    update();
  });

  QObject::connect(this, &QAbstractButton::pressed, this, [this]() {
    startAnimation();
  });
  QObject::connect(this, &QAbstractButton::released, this, [this]() {
    startAnimation();
  });
  QObject::connect(this, &QAbstractButton::toggled, this, [this]() {
    startAnimation();
  });

  _handleAnimation.setDuration(animationDuration);
  _handleAnimation.setEasingCurve(QEasingCurve::Type::OutCubic);
  _handleAnimation.setStartValue(0.);
  _handleAnimation.setEndValue(0.);
  QObject::connect(&_handleAnimation, &QVariantAnimation::valueChanged, this, [this]() {
    update();
  });
}

const QColor& Switch::getBgColor() const {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  const auto& bgColor = qlementineStyle ? qlementineStyle->switchGrooveColor(
                          getMouseState(isDown(), _isMouseOver, isEnabled()), getCheckState(isChecked()))
                                        : style->standardPalette().color(
                                          isEnabled() ? QPalette::ColorGroup::Normal : QPalette::ColorGroup::Disabled,
                                          QPalette::ColorRole::Button);
  return bgColor;
}

const QColor& Switch::getFgColor() const {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  const auto& fgColor = qlementineStyle ? qlementineStyle->switchHandleColor(
                          getMouseState(isDown(), _isMouseOver, isEnabled()), getCheckState(isChecked()))
                                        : style->standardPalette().color(
                                          isEnabled() ? QPalette::ColorGroup::Normal : QPalette::ColorGroup::Disabled,
                                          QPalette::ColorRole::ButtonText);
  return fgColor;
}

const QColor& Switch::getTextColor() const {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  const auto& textColor =
    qlementineStyle
      ? qlementineStyle->labelForegroundColor(getMouseState(isDown(), _isMouseOver, isEnabled()))
      : style->standardPalette().color(
        isEnabled() ? QPalette::ColorGroup::Normal : QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text);
  return textColor;
}

QRect Switch::getSwitchRect() const {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  const auto contentRect = rect();
  const auto switchW = qlementineStyle ? qlementineStyle->theme().controlHeightLarge : 28;
  const auto switchH = qlementineStyle ? qlementineStyle->theme().controlHeightSmall : 16;
  const auto switchX = contentRect.x();
  const auto switchY = contentRect.y() + (contentRect.height() - switchH) / 2;
  const auto switchRect = QRect{ switchX, switchY, switchW, switchH };
  return switchRect;
}

void Switch::leaveEvent(QEvent* e) {
  QAbstractButton::leaveEvent(e);
  _isMouseOver = false;
  startAnimation();
}

void Switch::initStyleOptionFocus(QStyleOptionFocusRoundedRect& opt) const {
  const auto* style = this->style();
  const auto deltaX = style->pixelMetric(QStyle::PM_FocusFrameHMargin, &opt, this);
  const auto deltaY = style->pixelMetric(QStyle::PM_FocusFrameVMargin, &opt, this);
  const auto switchRect = getSwitchRect();
  opt.rect = switchRect.marginsAdded({ deltaX / 2, deltaY / 2, deltaX / 2, deltaY / 2 }).translated(deltaX, deltaY);
  opt.radiuses = switchRect.height() / 2.;
}
} // namespace oclero::qlementine
