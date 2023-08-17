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

#include <oclero/qlementine/widgets/ColorButton.hpp>

#include <QStyle>
#include <QPainter>
#include <QColorDialog>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/utils/PrimitiveUtils.hpp>

namespace oclero::qlementine {

ColorButton::ColorButton(QWidget* parent)
  : QAbstractButton(parent) {
  setup();
}

ColorButton::ColorButton(const QColor& color, QWidget* parent)
  : QAbstractButton(parent)
  , _color(color) {
  setup();
}

ColorButton::ColorButton(const QColor& color, ColorMode mode, QWidget* parent)
  : QAbstractButton(parent)
  , _color(color)
  , _colorMode(mode) {
  setup();
}

void ColorButton::setup() {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  QObject::connect(this, &QAbstractButton::clicked, this, [this]() {
    const auto newColor = QColorDialog::getColor(_color, this, {}, QColorDialog::ShowAlphaChannel);
    if (newColor.isValid()) {
      setColor(newColor);
    }
  });
}

QColor ColorButton::adaptColorToMode(const QColor& color) const {
  switch (_colorMode) {
    case ColorMode::RGB: {
      auto newColor = QColor(color);
      newColor.setAlphaF(1.);
      return newColor;
    } break;
    default:
      return color;
  }
}

const QColor& ColorButton::color() const {
  return _color;
}

void ColorButton::setColor(const QColor& color) {
  const auto newColor = adaptColorToMode(color);
  if (newColor != _color) {
    _color = newColor;
    update();
    emit colorChanged();
  }
}

ColorMode ColorButton::colorMode() const {
  return _colorMode;
}

void ColorButton::setColorMode(ColorMode mode) {
  if (mode != _colorMode) {
    _colorMode = mode;
    emit colorModeChanged();
    setColor(adaptColorToMode(_color));
  }
}

QSize ColorButton::sizeHint() const {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  const auto extent = qlementineStyle ? qlementineStyle->theme().controlHeightMedium
                                      : style->pixelMetric(QStyle::PM_DialogButtonsButtonHeight);
  return { extent, extent };
}

void ColorButton::paintEvent(QPaintEvent*) {
  QPainter p(this);
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);

  const auto opacity = isEnabled() ? 1.0 : 0.2;
  p.setOpacity(opacity);
  const auto hasFocus = this->hasFocus();

  // Background
  const auto borderWidth = qlementineStyle ? qlementineStyle->theme().borderWidth : 1;
  const auto borderColor = qlementineStyle ? qlementineStyle->theme().semiTransparentColor4 : Qt::black;
  qlementine::drawColorMark(&p, rect(), _color, hasFocus ? Qt::transparent : borderColor, borderWidth);

  // To improve readability when the button has focus, draw a stroke with the focus color.
  if (hasFocus) {
    const auto focusColor = qlementineStyle ? qlementineStyle->theme().primaryColor : Qt::white;
    qlementine::drawColorMarkBorder(&p, rect(), focusColor, borderWidth);
  }
}
} // namespace oclero::qlementine
