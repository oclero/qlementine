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

#include <oclero/qlementine/utils/RadiusesF.hpp>

namespace oclero::qlementine {
RadiusesF::RadiusesF(int radius)
  : RadiusesF(static_cast<double>(radius)) {}

RadiusesF::RadiusesF(int left, int right)
  : RadiusesF(static_cast<double>(left), static_cast<double>(right)) {}

RadiusesF::RadiusesF(int topLeft, int topRight, int bottomRight, int bottomLeft)
  : RadiusesF(static_cast<double>(topLeft), static_cast<double>(topRight), static_cast<double>(bottomRight),
    static_cast<double>(bottomLeft)) {}

RadiusesF::RadiusesF(double radius) {
  *this = radius;
}

RadiusesF::RadiusesF(double left, double right)
  : topLeft(left)
  , topRight(right)
  , bottomRight(right)
  , bottomLeft(left) {}


RadiusesF::RadiusesF(double topLeft, double topRight, double bottomRight, double bottomLeft)
  : topLeft(topLeft)
  , topRight(topRight)
  , bottomRight(bottomRight)
  , bottomLeft(bottomLeft) {}

bool RadiusesF::hasSameRadius() const {
  return topLeft == topRight && topLeft == bottomRight && topLeft == bottomLeft;
}

bool RadiusesF::hasDifferentRadius() const {
  return !hasSameRadius();
}

RadiusesF& RadiusesF::operator=(const double rhs) {
  topLeft = rhs;
  topRight = rhs;
  bottomRight = rhs;
  bottomLeft = rhs;
  return *this;
}

RadiusesF RadiusesF::operator+(const double rhs) const {
  return { topLeft + rhs, topRight + rhs, bottomRight + rhs, bottomLeft + rhs };
}

RadiusesF RadiusesF::operator-(const double rhs) const {
  return { topLeft - rhs, topRight - rhs, bottomRight - rhs, bottomLeft - rhs };
}

bool RadiusesF::operator<(const double rhs) const {
  return topLeft < rhs && topRight < rhs && bottomRight < rhs && bottomLeft < rhs;
}

bool RadiusesF::operator<=(const double rhs) const {
  return *this == rhs || *this < rhs;
}

bool RadiusesF::operator>(const double rhs) const {
  return topLeft > rhs && topRight > rhs && bottomRight > rhs && bottomLeft > rhs;
}

bool RadiusesF::operator>=(const double rhs) const {
  return *this == rhs || *this > rhs;
}

bool RadiusesF::operator==(const double rhs) const {
  return topLeft == rhs && topRight == rhs && bottomRight == rhs && bottomLeft == rhs;
}

bool RadiusesF::operator!=(const double rhs) const {
  return topLeft != rhs && topRight != rhs && bottomRight != rhs && bottomLeft != rhs;
}

bool RadiusesF::operator==(const RadiusesF& rhs) const {
  return topLeft == rhs.topLeft && topRight == rhs.topRight && bottomRight == rhs.bottomRight
         && bottomLeft == rhs.bottomLeft;
}

bool RadiusesF::operator!=(const RadiusesF& rhs) const {
  return !(*this == rhs);
}
} // namespace oclero::qlementine
