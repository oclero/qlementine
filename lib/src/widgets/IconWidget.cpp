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

#include <oclero/qlementine/widgets/IconWidget.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/utils/ImageUtils.hpp>

#include <QPainter>

namespace oclero::qlementine {
IconWidget::IconWidget(QWidget* parent)
  : QWidget(parent) {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::NoFocus);
  const auto* style = this->style();
  const auto extent = style ? style->pixelMetric(QStyle::PM_ButtonIconSize, nullptr, this) : 16;
  _iconSize = QSize{ extent, extent };
}

IconWidget::IconWidget(const QIcon& icon, QWidget* parent)
  : QWidget(parent)
  , _icon{ icon } {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::NoFocus);
  const auto* style = this->style();
  const auto extent = style ? style->pixelMetric(QStyle::PM_ButtonIconSize, nullptr, this) : 16;
  _iconSize = QSize{ extent, extent };
}

IconWidget::IconWidget(const QIcon& icon, const QSize& size, QWidget* parent)
  : QWidget(parent)
  , _iconSize{ size }
  , _icon{ icon } {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::NoFocus);
}

const QIcon& IconWidget::icon() const {
  return _icon;
}

void IconWidget::setIcon(const QIcon& icon) {
  _icon = icon;
  update();
}

const QSize& IconWidget::iconSize() const {
  return _iconSize;
}

void IconWidget::setIconSize(const QSize& iconSize) {
  if (iconSize != _iconSize) {
    _iconSize = iconSize;
    emit iconSizeChanged();
    updateGeometry();
    update();
  }
}

QSize IconWidget::sizeHint() const {
  const auto padding = contentsMargins();
  const auto w = _iconSize.width() + padding.left() + padding.right();
  const auto h = _iconSize.height() + padding.top() + padding.bottom();
  return { w, h };
}

void IconWidget::paintEvent(QPaintEvent*) {
  const auto qlementineStyle = qobject_cast<QlementineStyle*>(style());
  const auto autoColorize = qlementineStyle && qlementineStyle->isAutoIconColorEnabled(this);
  const auto iconMode = isEnabled() || autoColorize ? QIcon::Mode::Normal : QIcon::Mode::Disabled;
  const auto pixmap = _icon.pixmap(_iconSize.height(), iconMode, QIcon::State::Off);
  if (pixmap.isNull())
    return;

  const auto& colorizedPixmap = autoColorize ? colorizePixmap(pixmap,
                                  palette().color(isEnabled() ? QPalette::Normal : QPalette::Disabled, QPalette::Text))
                                             : pixmap;

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);
  const auto x = (width() - colorizedPixmap.width()) / 2;
  const auto y = (height() - colorizedPixmap.height()) / 2;
  p.drawPixmap(x, y, colorizedPixmap);
}
} // namespace oclero::qlementine
