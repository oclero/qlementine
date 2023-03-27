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
