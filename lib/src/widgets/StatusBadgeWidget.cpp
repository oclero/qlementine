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

#include <oclero/qlementine/widgets/StatusBadgeWidget.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>

namespace oclero::qlementine {
StatusBadgeWidget::StatusBadgeWidget(QWidget* parent)
  : StatusBadgeWidget(StatusBadge::Info, StatusBadgeSize::Medium, parent) {}

StatusBadgeWidget::StatusBadgeWidget(StatusBadge badge, QWidget* parent)
  : StatusBadgeWidget(badge, StatusBadgeSize::Medium, parent) {}

StatusBadgeWidget::StatusBadgeWidget(StatusBadge badge, StatusBadgeSize badgeSize, QWidget* parent)
  : QWidget(parent)
  , _badgeSize(badgeSize)
  , _badge(badge) {
  setFocusPolicy(Qt::NoFocus);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

StatusBadge StatusBadgeWidget::badge() const {
  return _badge;
}
void StatusBadgeWidget::setBadge(StatusBadge badge) {
  if (badge != _badge) {
    _badge = badge;
    update();
    emit badgeChanged();
  }
}

StatusBadgeSize StatusBadgeWidget::badgeSize() const {
  return _badgeSize;
}

void StatusBadgeWidget::setBadgeSize(StatusBadgeSize size) {
  if (size != _badgeSize) {
    _badgeSize = size;
    updateGeometry();
    update();
    emit badgeSizeChanged();
  }
}

QSize StatusBadgeWidget::sizeHint() const {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const qlementine::QlementineStyle*>(style);

  auto extent =
    qlementineStyle ? qlementineStyle->theme().iconSizeMedium.height() : style->pixelMetric(QStyle::PM_LargeIconSize);

  switch (_badgeSize) {
    case StatusBadgeSize::Small:
      extent =
        qlementineStyle ? qlementineStyle->theme().iconSize.height() : style->pixelMetric(QStyle::PM_ButtonIconSize);
      break;
    case StatusBadgeSize::Medium:
      extent = qlementineStyle ? qlementineStyle->theme().iconSizeMedium.height()
                               : style->pixelMetric(QStyle::PM_LargeIconSize);
      break;
    default:
      break;
  }

  return QSize(extent, extent);
}

void StatusBadgeWidget::paintEvent(QPaintEvent* /*e*/) {
  QPainter p(this);
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const qlementine::QlementineStyle*>(style);
  const auto& theme = qlementineStyle ? qlementineStyle->theme() : qlementine::Theme{};
  const auto opacity = isEnabled() ? 1.0 : 0.35;
  p.setOpacity(opacity);
  qlementine::drawStatusBadge(&p, rect(), _badge, _badgeSize, theme);
}
} // namespace oclero::qlementine
