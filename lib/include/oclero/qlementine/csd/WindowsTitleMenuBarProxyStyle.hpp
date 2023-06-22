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

#include <QProxyStyle>

namespace oclero::qlementine {
/**
 * @brief Used to customize the QMenuBar color and height.
 */
class WindowsTitleMenuBarProxyStyle : public QProxyStyle {
public:
  explicit WindowsTitleMenuBarProxyStyle(QStyle* parent = nullptr);

  virtual QSize sizeFromContents(
    QStyle::ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

  virtual void drawControl(QStyle::ControlElement element, const QStyleOption* option, QPainter* painter,
    const QWidget* widget = nullptr) const override;

  virtual void drawPrimitive(
    PrimitiveElement pe, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

  virtual int pixelMetric(PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* w = nullptr) const override;
};
} // namespace oclero::qlementine
