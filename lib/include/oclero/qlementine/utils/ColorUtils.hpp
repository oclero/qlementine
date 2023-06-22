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

#include <QVariant>
#include <QColor>
#include <optional>

namespace oclero::qlementine {

/// Computes the contrast ratio between the two colors, according to w3.org spec.
double getContrastRatio(const QColor& c1, const QColor& c2);

/// Gets the color with the new alpha channel (from 0.0 to 1.0).
QColor colorWithAlphaF(QColor const& color, qreal alpha);

/// Gets the color the new alpha chanel (from 0 to 255).
QColor colorWithAlpha(QColor const& color, int alpha);

/// Gets the resulting color by applying the foreground color over the background color with 'SourceOver' composition mode.
QColor getColorSourceOver(const QColor& bg, const QColor& fg);

/// Attempts to parse the QVariant array to get a QColor from the array's values.
/// The order is R, G, B, A.
std::optional<QColor> tryGetColorFromVariantList(QVariantList const& variantList);

///  Attempts to parse the QString to get a QColor from the string's values.
/// The string must follow this rule: "R,G,B,A" (spaces are ignored).
std::optional<QColor> tryGetColorFromRGBAString(QString const& str);

/// Attempts to parse the QString to get a QColor from the string's values.
/// The string must follow this rule: "#RRGGBBAA" (AA is facultative, spaces are ignored).
std::optional<QColor> tryGetColorFromHexaString(QString const& str);

/// Attempts to parse the QVariant to get a QColor.
std::optional<QColor> tryGetColorFromVariant(QVariant const& variant);

/// Gives the color's hexadecimal RGBA string.
QString toHexRGBA(const QColor& color);
} // namespace oclero::qlementine
