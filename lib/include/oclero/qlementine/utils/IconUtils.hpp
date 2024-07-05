// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QIcon>
#include <QColor>

#include <oclero/qlementine/icons/Icons12.hpp>
#include <oclero/qlementine/icons/Icons16.hpp>
#include <oclero/qlementine/icons/Icons32.hpp>

namespace oclero::qlementine {
struct IconTheme {
  QColor normal;
  QColor disabled;
  QColor checkedNormal;
  QColor checkedDisabled;

  IconTheme(const QColor& normal);
  IconTheme(const QColor& normal, const QColor& disabled);
  IconTheme(const QColor& normal, const QColor& disabled, const QColor& checkedNormal, QColor checkedDisabled);

  const QColor& color(QIcon::Mode mode, QIcon::State state) const;
};

/// Makes an icon from the file located at the path in parameter. Fixes the standard Qt behavior.
[[maybe_unused]] QIcon makeIconFromSvg(const QString& svgPath, const QSize& size);

/// Makes an icon from the file located at the path in parameter and colorizes the QPixmaps. Fixes the standard Qt behavior.
[[maybe_unused]] QIcon makeIconFromSvg(
  const QString& svgPath, const IconTheme& iconTheme, const QSize& size = QSize(16, 16));

[[maybe_unused]] QIcon makeIconFromSvg(icons::Icons16 id, const QSize& size = QSize(16, 16));

/// Makes an icon from the icon ID in parameter and colorizes the QPixmaps. Fixes the standard Qt behavior.
[[maybe_unused]] QIcon makeIconFromSvg(
  icons::Icons16 id, const IconTheme& iconTheme, const QSize& size = QSize(16, 16));
} // namespace oclero::qlementine