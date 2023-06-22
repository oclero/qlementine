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

#include <oclero/qlementine/widgets/Popover.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/utils/PrimitiveUtils.hpp>
#include <oclero/qlementine/utils/ImageUtils.hpp>

#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>
#include <QScreen>
#include <QApplication>

#include <array>

namespace oclero::qlementine {
class PopoverFrame : public QWidget {
public:
  QVBoxLayout* _layout;

  PopoverFrame(QWidget* parent)
    : QWidget(parent) {
    // Transparent background.
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);
    setBackgroundRole(QPalette::NoRole);

    // Layout where the content will be.
    _layout = new QVBoxLayout(this);
    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);
    setLayout(_layout);

    // Mask to have rounded corners on content.
    // NB: There won't be any antialiasing because this is a limitation of Qt.
    updateMask();
  }

  void resizeEvent(QResizeEvent* e) override {
    QWidget::resizeEvent(e);
    updateMask();
  }

  QPixmap getMask() const {
    const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(this->style());
    const auto radius = qlementineStyle ? qlementineStyle->theme().borderRadius : 0;
    const auto borderWidth = qlementineStyle ? qlementineStyle->theme().borderWidth : 1;
    const auto totalRect = rect();
    const auto innerRect = totalRect.marginsRemoved({ borderWidth, borderWidth, borderWidth, borderWidth });
    QPixmap mask(size());
    mask.fill(Qt::white);
    QPainter p(&mask);
    p.setRenderHint(QPainter::Antialiasing, true);
    drawRoundedRect(&p, innerRect, Qt::black, radius);
    return mask;
  }

  void updateMask() {
    const auto mask = getMask();
    setMask(QBitmap(mask));
  }
};

Popover::Popover(QWidget* parent)
  : QWidget(parent) {
  setWindowFlag(Qt::WindowType::Tool, true);
  setWindowFlag(Qt::WindowType::FramelessWindowHint, true);
  setWindowFlag(Qt::WindowType::NoDropShadowWindowHint, true);
  setBackgroundRole(QPalette::NoRole);
  setAutoFillBackground(false);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setAttribute(Qt::WA_OpaquePaintEvent, false);
  setAttribute(Qt::WA_NoSystemBackground, true);
  setWindowModality(Qt::WindowModality::NonModal);

  const auto* style = this->style();

  // Popover layout, used to place the content and keep space for the drop shadow.
  const auto dropShadowYOffset = style->pixelMetric(QStyle::PM_LayoutVerticalSpacing) / 2;
  const auto dropShadowBlurRadius = style->pixelMetric(QStyle::PM_LayoutVerticalSpacing);

  auto* popoverLayout = new QVBoxLayout(this);
  popoverLayout->setSpacing(0);
  popoverLayout->setContentsMargins(dropShadowBlurRadius * 2, dropShadowBlurRadius * 2, dropShadowBlurRadius * 2,
    dropShadowYOffset + dropShadowBlurRadius * 2);

  // Layout.
  auto* frame = new PopoverFrame(this);
  _frame = frame;
  _frameLayout = frame->_layout;
  popoverLayout->addWidget(frame);

  // Opacity animation.
  _opacityAnimation.setDuration(style->styleHint(QStyle::SH_Widget_Animation_Duration));
  constexpr auto startOpacity = 0.;
  const auto startOpacityVar = QVariant::fromValue<double>(startOpacity);
  _opacityAnimation.setStartValue(startOpacityVar);
  _opacityAnimation.setEndValue(startOpacityVar);
  setWindowOpacity(startOpacity);
  QObject::connect(&_opacityAnimation, &QVariantAnimation::valueChanged, this, [this]() {
    const auto currentOpacity = _opacityAnimation.currentValue().toDouble();
    setWindowOpacity(currentOpacity);
  });
  QObject::connect(&_opacityAnimation, &QVariantAnimation::finished, this, [this]() {
    if (_opened) {
      emit opened();
    } else {
      _mousePressWasOnAnchor = false;
      emit closed();
      hide();
    }
  });
}

Popover::Position Popover::preferredPosition() const {
  return _preferredPosition;
}

void Popover::setPreferredPosition(Position position) {
  if (position != _preferredPosition) {
    _preferredPosition = position;
    if (isVisible()) {
      updatePopoverGeometry();
    }
    emit preferredPositionChanged();
  }
}


Popover::Alignment Popover::preferredAlignment() const {
  return _preferredAlignment;
}

void Popover::setPreferredAlignment(Alignment alignment) {
  if (alignment != _preferredAlignment) {
    _preferredAlignment = alignment;
    if (isVisible()) {
      updatePopoverGeometry();
    }
    emit preferredAlignmentChanged();
  }
}

QWidget* Popover::contentWidget() const {
  return _content;
}

void Popover::setContentWidget(QWidget* widget) {
  if (widget != _content) {
    if (_content) {
      _content = nullptr;

      // Empty layout.
      if (auto* layoutItem = _frameLayout->takeAt(0)) {
        if (auto* widget = layoutItem->widget()) {
          delete widget;
        }
        delete layoutItem;
      }
    }

    _content = widget;

    if (_content) {
      _content->setParent(this);
      _content->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
      _frameLayout->addWidget(_content);
    }

    if (isVisible()) {
      updatePopoverGeometry();
    }
    emit contentWidgetChanged();
  }
}

bool Popover::isOpened() const {
  return _opened;
}

void Popover::setOpened(bool opened) {
  if (opened != _opened) {
    _opened = opened;
    if (_opened) {
      emit aboutToOpen();
      updatePopoverGeometry();
      show();
      startAnimation();
    } else {
      emit aboutToClose();
      startAnimation();
    }

    emit openedChanged();
  }
}

QMargins Popover::padding() const {
  return _frameLayout->contentsMargins();
}

void Popover::setPadding(const QMargins& padding) {
  if (padding != _frameLayout->contentsMargins()) {
    _frameLayout->setContentsMargins(padding);
    emit paddingChanged();
  }
}

QMargins Popover::screenPadding() const {
  return _screenPadding;
}

void Popover::setScreenPadding(const QMargins& padding) {
  if (padding != _screenPadding) {
    if (isVisible()) {
      updatePopoverGeometry();
    }
  }
}

QWidget* Popover::anchorWidget() const {
  return _anchor;
}

void Popover::setAnchorWidget(QWidget* widget) {
  if (widget != _anchor) {
    if (_anchor) {
      _anchor->removeEventFilter(this);
    }

    _anchor = widget;

    if (_anchor) {
      // Move the popup when the anchor moves.
      _anchor->installEventFilter(this);

      // Move the popup when the anchor's window moves.
      if (auto* anchorWindow = _anchor->window(); anchorWindow != _anchor) {
        anchorWindow->installEventFilter(this);
      }
    }

    if (isVisible()) {
      updatePopoverGeometry();
    }
    emit anchorWidgetChanged();
  }
}

int Popover::verticalSpacing() const {
  return _verticalSpacing;
}

void Popover::setVerticalSpacing(int spacing) {
  if (spacing != _verticalSpacing) {
    _verticalSpacing = spacing;
    if (isVisible()) {
      updatePopoverGeometry();
    }
    emit verticalSpacingChanged();
  }
}

int Popover::horizontalSpacing() const {
  return _horizontalSpacing;
}

void Popover::setHorizontalSpacing(int spacing) {
  if (spacing != _horizontalSpacing) {
    _horizontalSpacing = spacing;
    if (isVisible()) {
      updatePopoverGeometry();
    }
    emit horizontalSpacingChanged();
  }
}

void Popover::openPopover() {
  setOpened(true);
}

void Popover::closePopover() {
  setOpened(false);
}

void Popover::paintEvent(QPaintEvent*) {
  const auto* style = this->style();
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(style);

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);

  // Drop shadow.
  {
    // Update cache if necessary.
    if (_dropShadowCacheSize != size()) {
      const auto dropShadowBlurRadius = style->pixelMetric(QStyle::PM_LayoutVerticalSpacing);
      const auto dropShadowColor = qlementineStyle ? qlementineStyle->theme().shadowColor1 : QColor(0, 0, 0, 64);
      const auto framePixmap = getFrameShape();
      _dropShadowCache = qlementine::getDropShadowPixmap(framePixmap, dropShadowBlurRadius, dropShadowColor);
      _dropShadowCacheSize = size();
    }
    const auto dropShadowYOffset = style->pixelMetric(QStyle::PM_LayoutVerticalSpacing) / 2;
    const auto dropShadowX = _frame->x() + (_frame->width() - _dropShadowCache.width()) / 2;
    const auto dropShadowY = _frame->y() + (_frame->height() - _dropShadowCache.height()) / 2 + dropShadowYOffset;

    const auto compModeBackup = p.compositionMode();
    p.setCompositionMode(QPainter::CompositionMode::CompositionMode_Multiply);
    p.drawPixmap(dropShadowX, dropShadowY, _dropShadowCache);
    p.setCompositionMode(compModeBackup);
  }

  // Frame.
  {
    const auto radius = qlementineStyle ? qlementineStyle->theme().borderRadius : 0;
    const auto bgColor = palette().color(QPalette::ColorGroup::Normal, QPalette::ColorRole::Window);
    const auto borderColor = qlementineStyle ? qlementineStyle->theme().borderColor2
                                             : palette().color(QPalette::ColorGroup::Normal, QPalette::ColorRole::Mid);
    const auto borderWidth = qlementineStyle ? qlementineStyle->theme().borderWidth : 1;
    const auto bgRect = _frame->rect().translated(_frame->mapTo(this, QPoint(0, 0)));

    QPainter p(this);
    drawRoundedRect(&p, bgRect, bgColor, radius);
    drawRoundedRectBorder(&p, bgRect, borderColor, borderWidth, radius);
  }
}

void Popover::mousePressEvent(QMouseEvent* e) {
  const auto clickPos = mapToGlobal(e->pos());
  _mousePressWasOnAnchor = _anchor ? _anchor->rect().contains(_anchor->mapFromGlobal(clickPos)) : false;
  if (!rect().contains(e->pos())) {
    setOpened(false);
  }
}

void Popover::mouseReleaseEvent(QMouseEvent* e) {
  QWidget::mousePressEvent(e);
  const auto& frameRect = _frame->geometry();
  if (!frameRect.contains(e->pos())) {
    setOpened(false);
  }
}

void Popover::hideEvent(QHideEvent* e) {
  QWidget::hideEvent(e);
  setOpened(false);
}

void Popover::showEvent(QShowEvent* e) {
  QWidget::showEvent(e);
  setOpened(true);
  setFocus(Qt::FocusReason::ActiveWindowFocusReason);
}

bool Popover::event(QEvent* e) {
  const auto type = e->type();

  // Close popup with animation when Escape is pressed.
  if (type == QEvent::KeyPress || type == QEvent::KeyRelease) {
    const auto* keyEvent = static_cast<QKeyEvent*>(e);
    const auto key = keyEvent->key();
    if (key == Qt::Key_Escape) {
      setOpened(false);
      e->accept();
      return true;
    }
  } else if (e->spontaneous() && type == QEvent::ShortcutOverride) {
    const auto* keyEvent = static_cast<QKeyEvent*>(e);
    const auto key = keyEvent->key();
    if (key == Qt::Key_Escape) {
      setOpened(false);
      e->accept();
      return true;
    }
  } else if (e->spontaneous() && type == QEvent::Shortcut) {
    const auto* shortcutEvent = static_cast<QShortcutEvent*>(e);
    const auto& key = shortcutEvent->key();
    if (key == Qt::Key_Escape) {
      setOpened(false);
      e->accept();
      return true;
    }
  }

  return QWidget::event(e);
}

bool Popover::eventFilter(QObject*, QEvent* e) {
  const auto type = e->type();
  if (type == QEvent::Type::MouseButtonRelease) {
    const auto* mouseEvent = static_cast<QMouseEvent*>(e);
    const auto mouseReleaseIsOnAnchor = _anchor ? _anchor->rect().contains(mouseEvent->pos()) : false;
    const auto tmp = _mousePressWasOnAnchor;
    _mousePressWasOnAnchor = false;
    if (tmp && mouseReleaseIsOnAnchor) {
      e->accept();
      return true;
    }
  } else if (type == QEvent::Type::Resize || type == QEvent::Type::Move) {
    if (isVisible()) {
      updatePopoverGeometry();
    }
  }
  return false;
}

void Popover::focusOutEvent(QFocusEvent* e) {
  QWidget::focusOutEvent(e);
  if (auto* focusWidget = QApplication::focusWidget(); !isAncestorOf(focusWidget)) {
    setOpened(false);
  }
}

void Popover::updatePopoverGeometry() {
  // First of all, ensure the popup fits its content.
  adjustSize();

  // Check if the preferred position fits entirely on screen, or try another position until it works.
  const auto& priority = positionPriority(_preferredPosition);
  const auto screenGeometry = screen()->availableGeometry();
  for (auto i = 0; i < static_cast<int>(priority.size()); ++i) {
    const auto position = priority.at(i);
    auto geometry = getGeometryForPosition(position, _preferredAlignment);

    // Success: the popup fits on screen.
    const auto aboveAnchor =
      _anchor ? _anchor->rect().translated(_anchor->mapToGlobal(QPoint{ 0, 0 })).intersects(geometry) : false;
    if (screenGeometry.contains(geometry) && !aboveAnchor) {
      // Now, translate it to include the drop shadow.
      const auto dropShadowMargins = layout()->contentsMargins();
      geometry.translate(-dropShadowMargins.left(), -dropShadowMargins.top());
      setGeometry(geometry);
      return;
    }
  }

  // If it is impossible to respect any position, show it centered on screen.
  const auto geometry = getFallbackGeometry();
  setGeometry(geometry);
}

const std::array<Popover::Position, 4>& Popover::positionPriority(Position const position) {
  static const auto leftPriority =
    std::array<Position, 4>{ Position::Left, Position::Right, Position::Top, Position::Bottom };
  static const auto rightPriority =
    std::array<Position, 4>{ Position::Right, Position::Left, Position::Top, Position::Bottom };
  static const auto topPriority =
    std::array<Position, 4>{ Position::Top, Position::Bottom, Position::Left, Position::Right };
  static const auto bottomPriority =
    std::array<Position, 4>{ Position::Bottom, Position::Top, Position::Left, Position::Right };
  switch (position) {
    case Position::Left:
      return leftPriority;
    case Position::Right:
      return rightPriority;
    case Position::Top:
      return topPriority;
    case Position::Bottom:
    default:
      return bottomPriority;
  }
}

QRect Popover::getGeometryForPosition(Position const position, Alignment const alignment) const {
  // Take the content size as a basis, rather than the whole size that includes drop shadow.
  const auto popoverSize = _frame->sizeHint();
  const auto screenGeometry = screen()->availableGeometry().marginsRemoved(_screenPadding);
  const auto popoverFittedSize = QSize{
    std::min(screenGeometry.width(), popoverSize.width()),
    std::min(screenGeometry.height(), popoverSize.height()),
  };
  QRect result;

  if (_anchor) {
    QPoint topLeft;
    switch (position) {
      case Position::Left: {
        switch (alignment) {
          case Alignment::Begin: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ 0, 0 });
            const auto x = anchorPoint.x() - _horizontalSpacing - popoverFittedSize.width();
            const auto y = anchorPoint.y() + _verticalSpacing;
            topLeft = { x, y };
          } break;
          case Alignment::Center: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ 0, _anchor->height() / 2 });
            const auto x = anchorPoint.x() - _horizontalSpacing - popoverFittedSize.width();
            const auto y = anchorPoint.y() - popoverSize.height() / 2;
            topLeft = { x, y };
          } break;
          case Alignment::End: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ 0, _anchor->height() });
            const auto x = anchorPoint.x() - _horizontalSpacing - popoverFittedSize.width();
            const auto y = anchorPoint.y() - popoverSize.height() + _verticalSpacing;
            topLeft = { x, y };
          } break;
          default:
            break;
        }
      } break;
      case Position::Right: {
        switch (alignment) {
          case Alignment::Begin: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ _anchor->width(), 0 });
            const auto x = anchorPoint.x() + _horizontalSpacing;
            const auto y = anchorPoint.y() + _verticalSpacing;
            topLeft = { x, y };
          } break;
          case Alignment::Center: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ _anchor->width(), _anchor->height() / 2 });
            const auto x = anchorPoint.x() + _horizontalSpacing;
            const auto y = anchorPoint.y() - popoverSize.height() / 2;
            topLeft = { x, y };
          } break;
          case Alignment::End: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ _anchor->width(), _anchor->height() });
            const auto x = anchorPoint.x() + _horizontalSpacing;
            const auto y = anchorPoint.y() - popoverSize.height() + _verticalSpacing;
            topLeft = { x, y };
          } break;
          default:
            break;
        }
      } break;
      case Position::Top: {
        switch (alignment) {
          case Alignment::Begin: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ 0, 0 });
            const auto x = anchorPoint.x() + _horizontalSpacing;
            const auto y = anchorPoint.y() - _verticalSpacing - popoverFittedSize.height();
            topLeft = { x, y };
          } break;
          case Alignment::Center: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ _anchor->width() / 2, 0 });
            const auto x = anchorPoint.x() - popoverFittedSize.width() / 2;
            const auto y = anchorPoint.y() - _verticalSpacing - popoverFittedSize.height();
            topLeft = { x, y };
          } break;
          case Alignment::End: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ _anchor->width(), 0 });
            const auto x = anchorPoint.x() + _horizontalSpacing;
            const auto y = anchorPoint.y() - _verticalSpacing - popoverFittedSize.height();
            topLeft = { x, y };
          } break;
          default:
            break;
        }
      } break;
      case Position::Bottom: {
        switch (alignment) {
          case Alignment::Begin: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ 0, _anchor->height() });
            const auto x = anchorPoint.x() + _horizontalSpacing;
            const auto y = anchorPoint.y() + _verticalSpacing;
            topLeft = { x, y };
          } break;
          case Alignment::Center: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ _anchor->width() / 2, _anchor->height() });
            const auto x = anchorPoint.x() - popoverFittedSize.width() / 2;
            const auto y = anchorPoint.y() + _verticalSpacing;
            topLeft = { x, y };
          } break;
          case Alignment::End: {
            const auto anchorPoint = _anchor->mapToGlobal(QPoint{ _anchor->width(), _anchor->height() });
            const auto x = anchorPoint.x() + _horizontalSpacing;
            const auto y = anchorPoint.y() + _verticalSpacing;
            topLeft = { x, y };
          } break;
          default:
            break;
        }
      } break;
      default:
        break;
    }
    result = QRect(topLeft, popoverFittedSize);
  } else {
    result = { QPoint{ _screenPadding.left(), _screenPadding.top() }, popoverFittedSize };
  }

  // Make sure the bounds fit the screen.
  const auto yDiff = (result.y() + result.height()) - (screenGeometry.y() + screenGeometry.height());
  if (yDiff > 0) {
    result.translate(0, -yDiff);
  } else if (result.y() < _screenPadding.top()) {
    result.translate(0, -result.y() + _screenPadding.top());
  }

  const auto xDiff = (result.x() + result.width()) - (screenGeometry.x() + screenGeometry.width());
  if (xDiff > 0) {
    result.translate(-xDiff, 0);
  } else if (result.x() < _screenPadding.left()) {
    result.translate(-result.x() + _screenPadding.left(), 0);
  }

  return result;
}

QRect Popover::getFallbackGeometry() const {
  const auto popoverSize = _frame->sizeHint();
  const auto screenGeometry = screen()->availableGeometry().marginsRemoved(_screenPadding);
  const auto popoverFittedSize = QSize{
    std::min(screenGeometry.width(), popoverSize.width()),
    std::min(screenGeometry.height(), popoverSize.height()),
  };

  const auto dropShadowMargins = layout()->contentsMargins();
  const auto x =
    screenGeometry.x() + (screenGeometry.width() - popoverFittedSize.width()) / 2 - dropShadowMargins.left();
  const auto y =
    screenGeometry.y() + (screenGeometry.height() - popoverFittedSize.height()) / 2 - dropShadowMargins.top();
  return { QPoint{ x, y }, popoverFittedSize };
}

void Popover::startAnimation() {
  const auto currentOpacity = _opacityAnimation.currentValue();
  _opacityAnimation.stop();
  _opacityAnimation.setStartValue(currentOpacity);
  _opacityAnimation.setEndValue(QVariant::fromValue<double>(_opened ? 1. : 0.));
  _opacityAnimation.start();
}

QPixmap Popover::getFrameShape() const {
  const auto* qlementineStyle = qobject_cast<const QlementineStyle*>(this->style());
  const auto radius = qlementineStyle ? qlementineStyle->theme().borderRadius : 0;
  QPixmap result(_frame->size());
  result.fill(Qt::transparent);
  QPainter p(&result);
  drawRoundedRect(&p, QRect{ QPoint(0, 0), _frame->size() }, Qt::black, radius);
  return result;
}
} // namespace oclero::qlementine
