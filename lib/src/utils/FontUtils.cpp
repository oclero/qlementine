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

#include <oclero/qlementine/utils/FontUtils.hpp>

namespace oclero::qlementine {
static constexpr auto STANDARD_DPI = 72.;

double pointSizeToPixelSize(double pointSize, double dpi) {
  return pointSize / STANDARD_DPI * dpi;
}

double pixelSizeToPointSize(double pixelSize, double dpi) {
  return pixelSize * STANDARD_DPI / dpi;
}

int textWidth(const QFontMetrics& fm, const QString& text) {
  // After some tests, it looks like QFontMetrics::boundingRect() with these parameters
  // gives the correct results, and ensure the text isn't truncated.
  // Qt's documentation recommends QFontMetrics::horizontalAdvance() but this one gives
  // incorrect results (i.e. a value not big enough) most of the time.
  return fm.boundingRect({}, Qt::AlignCenter, text, 0, nullptr).width();
}
} // namespace oclero::qlementine
