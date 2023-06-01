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

#include <oclero/qlementine/utils/RadiusesF.hpp>

#include <QStyleOption>

namespace oclero::qlementine {
/// Allows to customize the radius of the focus border.
class QStyleOptionFocusRoundedRect : public QStyleOptionFocusRect {
public:
  RadiusesF radiuses;
  int hMargin{ 0 };
  int vMargin{ 0 };
  QColor borderColor;

  QStyleOptionFocusRoundedRect() = default;

  static QStyleOptionFocusRoundedRect fromBase(QStyleOption const& opt, QRect const& rect, RadiusesF const& radiuses) {
    QStyleOptionFocusRoundedRect newOpt;
    newOpt.QStyleOption::operator=(opt);
    newOpt.radiuses = radiuses;
    newOpt.rect = rect;
    return newOpt;
  }

  QStyleOptionFocusRoundedRect(const QStyleOptionFocusRoundedRect& other)
    : QStyleOptionFocusRect(other) {
    *this = other;
  }

  QStyleOptionFocusRoundedRect& operator=(const QStyleOptionFocusRoundedRect&) = default;
};

/// Adds the ability to transition from one visual position to another.
class QStyleOptionSliderF : public QStyleOptionSlider {
public:
  static constexpr auto INITIALIZED = 2;
  qreal sliderPositionF{ 0. };
  int status{ 0 }; // Needed to track that it was created by us.

  QStyleOptionSliderF() = default;

  QStyleOptionSliderF(const QStyleOptionSliderF& other)
    : QStyleOptionSlider(other) {
    *this = other;
  }

  QStyleOptionSliderF& operator=(const QStyleOptionSliderF&) = default;
};

/// Adds the ability to have a second line of text in the button.
class QStyleOptionCommandLinkButton : public QStyleOptionButton {
public:
  QString description;

  using QStyleOptionButton::QStyleOptionButton;
};
} // namespace oclero::qlementine
