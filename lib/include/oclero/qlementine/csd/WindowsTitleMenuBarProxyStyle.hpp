// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

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
