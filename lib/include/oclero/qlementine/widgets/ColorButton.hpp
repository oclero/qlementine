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

#include <QAbstractButton>
#include <QColor>

#include <oclero/qlementine/Common.hpp>

namespace oclero::qlementine {
class ColorButton : public QAbstractButton {
  Q_OBJECT

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(ColorMode colorMode READ colorMode WRITE setColorMode NOTIFY colorModeChanged)

public:
  ColorButton(QWidget* parent = nullptr);
  ColorButton(const QColor& color, QWidget* parent = nullptr);
  ColorButton(const QColor& color, ColorMode mode, QWidget* parent = nullptr);

  const QColor& color() const;
  void setColor(const QColor& color);

  ColorMode colorMode() const;
  void setColorMode(ColorMode mode);

  QSize sizeHint() const override;

signals:
  void colorChanged();
  void colorModeChanged();

protected:
  void paintEvent(QPaintEvent*) override;

private:
  void setup();
  QColor adaptColorToMode(const QColor& color) const;

  QColor _color;
  ColorMode _colorMode{ ColorMode::RGBA };
};
} // namespace oclero::qlementine
