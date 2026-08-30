// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

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

  QStyleOptionFocusRoundedRect(const QStyleOptionFocusRoundedRect& other) = default;

  QStyleOptionFocusRoundedRect& operator=(const QStyleOptionFocusRoundedRect&) = default;
};

/// Allows to customize the radius of a button.
class QStyleOptionRoundedButton : public QStyleOptionButton {
public:
  enum StyleOptionType { Type = SO_CustomBase + 1 };

  RadiusesF radiuses{ 0. };

  QStyleOptionRoundedButton() {
    type = Type;
  }

  QStyleOptionRoundedButton(const QStyleOptionRoundedButton& other)
    : QStyleOptionButton(other)
    , radiuses(other.radiuses) {
    type = Type;
  }

  QStyleOptionRoundedButton& operator=(const QStyleOptionRoundedButton&) = default;
};

/// Adds the ability to have a second line of text in the button.
class QStyleOptionCommandLinkButton : public QStyleOptionButton {
public:
  QString description;

  using QStyleOptionButton::QStyleOptionButton;
};
} // namespace oclero::qlementine
