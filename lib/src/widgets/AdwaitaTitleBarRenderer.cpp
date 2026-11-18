// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include "TitleBarRenderer.hpp"

#include <oclero/qlementine/style/Theme.hpp>

#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>

namespace oclero::qlementine {
namespace {
// Metrics follow libadwaita (24px circular window controls, bold title), with a header bar more compact than the 47px
// of libadwaita.
// Colors come from the Qlementine theme so the title bar matches the rest of the application.
// The corner radius and the circle opacities are approximations.
constexpr auto barHeightDp = 38;
constexpr auto buttonDiameter = 24;
constexpr auto buttonSpacing = 6;
constexpr auto barMargin = 6;
constexpr auto topCornerRadius = 12.;
constexpr auto titlePointSize = 11.;

QColor foregroundColor(const Theme& theme, bool windowActive) {
  auto color = theme.titleBarForegroundColor;
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

  QSize captionButtonSize(CaptionButton, const Theme&) const override {
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
    QPainter& painter, const QRect& rect, const Theme& theme, bool windowActive, bool maximized) const override {
    const auto& background = windowActive ? theme.titleBarBackgroundColor : theme.titleBarBackgroundColorInactive;

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
    painter.fillRect(QRect(rect.left(), rect.bottom(), rect.width(), 1), theme.titleBarBorderColor);
    painter.restore();
  }

  void paintTitle(
    QPainter& painter, const QRect& rect, const QString& title, const Theme& theme, bool windowActive) const override {
    painter.save();
    auto font = painter.font();
    font.setWeight(QFont::Bold);
    font.setPointSizeF(titlePointSize);
    painter.setFont(font);
    painter.setPen(foregroundColor(theme, windowActive));
    painter.drawText(
      rect, Qt::AlignCenter, QFontMetrics(font).elidedText(title, Qt::ElideRight, rect.width()));
    painter.restore();
  }

  void paintCaptionButton(QPainter& painter, const QRect& rect, CaptionButton button, const CaptionButtonState& state,
    const Theme& theme) const override {
    const auto foreground = foregroundColor(theme, state.windowActive);
    const auto center = QRectF(rect).center();

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Circle background.
    if (state.pressed || state.hovered) {
      auto circle = theme.titleBarForegroundColor;
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
