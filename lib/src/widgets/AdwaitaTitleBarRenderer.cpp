// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include "TitleBarRenderer.hpp"

#include <QFontMetrics>
#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QStyleHints>

namespace oclero::qlementine {
namespace {
// Metrics follow libadwaita: 47px header bar, 24px circular window controls, bold title.
// Colors come from libadwaita's CSS variables documentation (headerbar-bg/fg/backdrop/shade-color).
// The corner radius, the circle opacities and the backdrop title opacity are approximations.
constexpr auto barHeightDp = 47;
constexpr auto buttonDiameter = 24;
constexpr auto buttonSpacing = 6;
constexpr auto barMargin = 6;
constexpr auto topCornerRadius = 12.;
constexpr auto titlePointSize = 11.;

bool isDarkScheme() {
  const auto scheme = QGuiApplication::styleHints()->colorScheme();
  if (scheme != Qt::ColorScheme::Unknown) {
    return scheme == Qt::ColorScheme::Dark;
  }
  return QGuiApplication::palette().color(QPalette::Window).lightness() < 128;
}

QColor foregroundColor(bool dark, bool windowActive) {
  auto color = dark ? QColor(255, 255, 255) : QColor(0, 0, 6, 204);
  if (!windowActive) {
    color.setAlpha(color.alpha() / 2);
  }
  return color;
}

class AdwaitaTitleBarRenderer final : public TitleBarRenderer {
public:
  int barHeight(const Theme&) const override {
    return barHeightDp;
  }

  QSize captionButtonSize(CaptionButton) const override {
    return { buttonDiameter, buttonDiameter };
  }

  int captionButtonSpacing() const override {
    return buttonSpacing;
  }

  QMargins captionButtonMargins() const override {
    const auto vertical = (barHeightDp - buttonDiameter) / 2;
    return { barMargin, vertical, barMargin, barHeightDp - buttonDiameter - vertical };
  }

  void paintBackground(
    QPainter& painter, const QRect& rect, const Theme&, bool windowActive, bool maximized) const override {
    const auto dark = isDarkScheme();
    const auto background = windowActive ? (dark ? QColor(0x2e, 0x2e, 0x32) : QColor(0xff, 0xff, 0xff))
                                         : (dark ? QColor(0x22, 0x22, 0x26) : QColor(0xfa, 0xfa, 0xfb));
    const auto shade = dark ? QColor(0, 0, 6, 92) : QColor(0, 0, 6, 31);

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Rounded top corners, square bottom corners.
    const auto radius = maximized ? 0. : topCornerRadius;
    auto shape = QPainterPath{};
    shape.addRoundedRect(QRectF(rect).adjusted(0, 0, 0, radius), radius, radius);
    auto clip = QPainterPath{};
    clip.addRect(rect);
    painter.fillPath(shape.intersected(clip), background);

    // 1px bottom border.
    painter.fillRect(QRect(rect.left(), rect.bottom(), rect.width(), 1), shade);
    painter.restore();
  }

  void paintTitle(
    QPainter& painter, const QRect& rect, const QString& title, const Theme&, bool windowActive) const override {
    painter.save();
    auto font = painter.font();
    font.setWeight(QFont::Bold);
    font.setPointSizeF(titlePointSize);
    painter.setFont(font);
    painter.setPen(foregroundColor(isDarkScheme(), windowActive));
    painter.drawText(
      rect, Qt::AlignCenter, QFontMetrics(font).elidedText(title, Qt::ElideRight, rect.width()));
    painter.restore();
  }

  void paintCaptionButton(QPainter& painter, const QRect& rect, CaptionButton button, const CaptionButtonState& state,
    const Theme&) const override {
    const auto dark = isDarkScheme();
    const auto foreground = foregroundColor(dark, state.windowActive);
    const auto center = QRectF(rect).center();

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Circle background.
    if (state.pressed || state.hovered) {
      auto circle = dark ? QColor(255, 255, 255) : QColor(0, 0, 6);
      circle.setAlphaF(state.pressed ? .15 : .1);
      painter.setPen(Qt::NoPen);
      painter.setBrush(circle);
      const auto diameter = qMin(rect.width(), rect.height());
      painter.drawEllipse(center, diameter / 2., diameter / 2.);
    }

    // Glyph.
    painter.setPen(QPen(foreground, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);
    switch (button) {
      case CaptionButton::Close:
        painter.drawLine(center + QPointF(-3.5, -3.5), center + QPointF(3.5, 3.5));
        painter.drawLine(center + QPointF(-3.5, 3.5), center + QPointF(3.5, -3.5));
        break;
      case CaptionButton::Minimize:
        painter.drawLine(center + QPointF(-4, 3), center + QPointF(4, 3));
        break;
      case CaptionButton::Maximize:
        if (state.maximized) {
          painter.drawRoundedRect(QRectF(center.x() - 4, center.y() - 2, 6, 6), 1, 1);
          auto back = QPainterPath{};
          back.moveTo(center + QPointF(-2, -2));
          back.lineTo(center + QPointF(-2, -4));
          back.lineTo(center + QPointF(4, -4));
          back.lineTo(center + QPointF(4, 2));
          back.lineTo(center + QPointF(2, 2));
          painter.drawPath(back);
        } else {
          painter.drawRoundedRect(QRectF(center.x() - 4, center.y() - 4, 8, 8), 1, 1);
        }
        break;
      case CaptionButton::Menu:
        painter.drawLine(center + QPointF(-4.5, -4), center + QPointF(4.5, -4));
        painter.drawLine(center + QPointF(-4.5, 0), center + QPointF(4.5, 0));
        painter.drawLine(center + QPointF(-4.5, 4), center + QPointF(4.5, 4));
        break;
    }
    painter.restore();
  }
};
} // namespace

std::unique_ptr<TitleBarRenderer> createAdwaitaTitleBarRenderer() {
  return std::make_unique<AdwaitaTitleBarRenderer>();
}
} // namespace oclero::qlementine
