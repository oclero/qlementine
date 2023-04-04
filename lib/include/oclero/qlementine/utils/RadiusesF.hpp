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

namespace oclero::qlementine {
// Handles radiuses for the 4 angles (topLeft, topRight, bottomRight, bottomLeft).
struct RadiusesF {
  double topLeft{ 0. };
  double topRight{ 0. };
  double bottomRight{ 0. };
  double bottomLeft{ 0. };

  RadiusesF() = default;
  RadiusesF(int radius);
  RadiusesF(int left, int right);
  RadiusesF(int topLeft, int topRight, int bottomRight, int bottomLeft);
  RadiusesF(double radius);
  RadiusesF(double left, double right);
  RadiusesF(double topLeft, double topRight, double bottomRight, double bottomLeft);

  bool hasSameRadius() const;
  bool hasDifferentRadius() const;

  RadiusesF& operator=(const double rhs);
  RadiusesF operator+(const double rhs) const;
  RadiusesF operator-(const double rhs) const;
  bool operator<(const double rhs) const;
  bool operator<=(const double rhs) const;
  bool operator>(const double rhs) const;
  bool operator>=(const double rhs) const;
  bool operator==(const double rhs) const;
  bool operator!=(const double rhs) const;
  bool operator==(const RadiusesF& rhs) const;
  bool operator!=(const RadiusesF& rhs) const;
};
} // namespace oclero::qlementine
