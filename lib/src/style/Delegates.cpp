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

#include <oclero/qlementine/style/Delegates.hpp>

#include <oclero/qlementine/utils/StateUtils.hpp>
#include <oclero/qlementine/utils/ImageUtils.hpp>
#include <oclero/qlementine/utils/PrimitiveUtils.hpp>
#include <oclero/qlementine/utils/FontUtils.hpp>
#include <oclero/qlementine/utils/ColorUtils.hpp>

#include <QPainter>

namespace oclero::qlementine {
ComboBoxDelegate::ComboBoxDelegate(QWidget* widget)
  : QItemDelegate(widget)
  , _widget(widget) {}

void ComboBoxDelegate::paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const {
  const auto* style = _widget->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  const auto& theme = qlementineStyle ? qlementineStyle->theme() : Theme{};

  const auto isSeparator = idx.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("separator");
  if (isSeparator) {
    const auto& rect = opt.rect;
    const auto& color =
      qlementineStyle ? qlementineStyle->toolBarSeparatorColor() : Theme().neutralAlternativeColorDisabled;
    const auto lineW = theme.borderWidth;
    constexpr auto padding = 0; //_impl->theme.spacing / 2;
    const auto x = rect.x() + (rect.width() - lineW) / 2.;
    const auto y1 = static_cast<double>(rect.y() + padding);
    const auto y2 = static_cast<double>(rect.y() + rect.height() - padding);
    p->setBrush(Qt::NoBrush);
    p->setPen(QPen(color, lineW, Qt::SolidLine, Qt::FlatCap));
    p->drawLine(QPointF{ x, y1 }, QPointF{ x, y2 });
  } else {
    const auto mouse = getMenuItemMouseState(opt.state);

    // Background.
    const auto hPadding = theme.spacing;
    const auto& bgRect = opt.rect;
    const auto& bgColor =
      qlementineStyle ? qlementineStyle->menuItemBackgroundColor(mouse) : Theme().primaryColorTransparent;
    constexpr auto radius = 0;
    p->setRenderHint(QPainter::Antialiasing, true);
    p->setPen(Qt::NoPen);
    p->setBrush(bgColor);
    p->drawRoundedRect(bgRect, radius, radius);

    // Foreground.
    const auto fgRect = bgRect.marginsRemoved(QMargins{ hPadding, 0, hPadding, 0 });
    if (fgRect.isEmpty()) {
      return;
    }

    const auto selected = getSelectionState(opt.state);
    const auto active = getActiveState(opt.state);
    const auto focus = selected == SelectionState::Selected ? FocusState::Focused : FocusState::NotFocused;
    auto availableW = fgRect.width();
    auto availableX = fgRect.x();
    const auto& fgColor = qlementineStyle ? qlementineStyle->menuItemForegroundColor(mouse) : Theme().neutralColor;

    // Icon.
    const auto iconVariant = idx.data(Qt::DecorationRole);
    const auto& icon =
      iconVariant.isValid() && iconVariant.type() == QVariant::Type::Icon ? iconVariant.value<QIcon>() : QIcon{};
    if (availableW > 0 && !icon.isNull()) {
      const auto spacing = theme.spacing;
      const auto& iconSize = opt.decorationSize; // Get icon size.
      const auto pixmap = getPixmap(icon, iconSize, mouse, CheckState::NotChecked);
      const auto* qlementineStyle = qobject_cast<QlementineStyle*>(_widget->style());
      const auto colorize = qlementineStyle && qlementineStyle->isAutoIconColorEnabled(_widget);
      const auto pixmapPixelRatio = pixmap.devicePixelRatio();
      const auto pixmapW = pixmapPixelRatio != 0 ? static_cast<int>((qreal) pixmap.width() / pixmapPixelRatio) : 0;
      const auto pixmapH = pixmapPixelRatio != 0 ? static_cast<int>((qreal) pixmap.height() / pixmapPixelRatio) : 0;
      const auto pixmapX = availableX + (iconSize.width() - pixmapW) / 2; // Center the icon in the rect.
      const auto pixmapY = fgRect.y() + (fgRect.height() - pixmapH) / 2;
      const auto pixmapRect = QRect{ pixmapX, pixmapY, pixmapW, pixmapH };
      availableW -= iconSize.width() + spacing;
      availableX += iconSize.width() + spacing;

      if (mouse == MouseState::Disabled && !colorize) {
        // Change only the icon's tint and opacity, so it looks disabled.
        const auto& bgColor = qlementineStyle
                                ? qlementineStyle->listItemBackgroundColor(MouseState::Normal, selected, focus, active)
                                : Theme().adaptativeColorTransparent;
        const auto premultipiedColor = getColorSourceOver(bgColor, fgColor);
        const auto& tintedPixmap = getTintedPixmap(pixmap, premultipiedColor);
        const auto opacity = selected == SelectionState::Selected ? 1. : 0.25;
        const auto backupOpacity = p->opacity();
        p->setOpacity(opacity * backupOpacity);
        p->drawPixmap(pixmapRect, tintedPixmap);
        p->setOpacity(backupOpacity);
      } else {
        // Actually color the whole icon if needed.
        const auto& colorizedPixmap = colorize ? getColorizedPixmap(pixmap, fgColor) : pixmap;
        p->drawPixmap(pixmapRect, colorizedPixmap);
      }
    }

    // Text.
    const auto textVariant = idx.data(Qt::DisplayRole);
    const auto& text =
      textVariant.isValid() && textVariant.type() == QVariant::Type::String ? textVariant.value<QString>() : QString{};
    if (availableW > 0 && !text.isEmpty()) {
      const auto& fm = opt.fontMetrics;
      const auto elidedText = fm.elidedText(text, Qt::ElideRight, availableW);
      const auto textX = availableX;
      const auto textRect = QRect{ textX, fgRect.y(), availableW, fgRect.height() };
      const auto textFlags = Qt::AlignVCenter | Qt::AlignBaseline | Qt::TextSingleLine | Qt::AlignLeft;
      p->setBrush(Qt::NoBrush);
      p->setPen(fgColor);
      p->drawText(textRect, textFlags, elidedText, nullptr);
    }
  }
}

QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& idx) const {
  const auto* style = _widget->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);
  const auto& theme = qlementineStyle ? qlementineStyle->theme() : Theme{};

  const auto isSeparator = idx.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("separator");
  if (isSeparator) {
    const auto h = theme.spacing + theme.borderWidth;
    return QSize{ h, h };
  } else {
    const auto hPadding = theme.spacing;
    const auto vPadding = theme.spacing / 2;
    const auto iconSize = theme.iconSize;
    const auto spacing = theme.spacing;
    const auto& fm = opt.fontMetrics;

    const auto textVariant = idx.data(Qt::DisplayRole);
    const auto& text =
      textVariant.isValid() && textVariant.type() == QVariant::Type::String ? textVariant.value<QString>() : QString{};
    const auto iconVariant = idx.data(Qt::DecorationRole);
    const auto& icon =
      iconVariant.isValid() && iconVariant.type() == QVariant::Type::Icon ? iconVariant.value<QIcon>() : QIcon{};
    const auto textW = qlementine::textWidth(fm, text);
    const auto iconW = !icon.isNull() ? iconSize.width() + spacing : 0;
    const auto w = std::max(0, hPadding + iconW + textW + hPadding);
    const auto h = std::max(theme.controlHeightMedium, std::max(iconSize.height(), vPadding) + vPadding);
    return QSize{ w, h };
  }
}
} // namespace oclero::qlementine
