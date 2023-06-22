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

#include <oclero/qlementine/widgets/Expander.hpp>

#include <QStyle>
#include <QEvent>

namespace oclero::qlementine {
constexpr auto animationDurationFactor = 1;

Expander::Expander(QWidget* parent)
  : QWidget(parent) {
  const auto animationDuration = style()->styleHint(QStyle::SH_Widget_Animation_Duration);
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  setFocusPolicy(Qt::NoFocus);
  _animation.setStartValue(QVariant::fromValue<int>(0));
  _animation.setEndValue(QVariant::fromValue<int>(0));
  _animation.setDuration(animationDuration * animationDurationFactor);
  _animation.setEasingCurve(QEasingCurve::Type::OutCubic);
  QObject::connect(&_animation, &QVariantAnimation::valueChanged, this, [this]() {
    updateGeometry();
  });
}

QSize Expander::sizeHint() const {
  const auto contentSizeHint = _content ? _content->sizeHint() : QSize{ 0, 0 };
  const auto w = contentSizeHint.width();
  const auto h = _animation.state() == QVariantAnimation::Running ? _animation.currentValue().toInt()
                                                                  : (_expanded ? contentSizeHint.height() : 0);
  return { w, h };
}

bool Expander::event(QEvent* e) {
  const auto type = e->type();
  if (type == QEvent::Type::LayoutRequest) {
    updateGeometry();
    updateContentGeometry();
  }
  return QWidget::event(e);
}

void Expander::resizeEvent(QResizeEvent* e) {
  QWidget::resizeEvent(e);
  updateContentGeometry();
}

bool Expander::eventFilter(QObject* o, QEvent* e) {
  const auto type = e->type();
  if (type == QEvent::Type::Resize) {
    updateGeometry();
  }
  return QWidget::eventFilter(o, e);
}

void Expander::updateContentGeometry() {
  if (_content) {
    _content->ensurePolished();
    const auto& availableSize = size();
    const auto contentSizeHint = _content->sizeHint();
    const auto w = availableSize.width();
    const auto h = contentSizeHint.height();
    _content->setVisible(w > 0);
    _content->setGeometry(0, 0, w, h);
  }
}

bool Expander::expanded() const {
  return _expanded;
}

void Expander::setExpanded(bool expanded) {
  if (expanded != _expanded) {
    if (_content) {
      _content->setVisible(true);
    }
    _expanded = expanded;

    const auto current = height();
    const auto target = _content && _expanded ? _content->sizeHint().height() : 0;
    const auto animationDuration = isVisible() ? style()->styleHint(QStyle::SH_Widget_Animation_Duration) : 0;
    _animation.stop();
    _animation.setDuration(animationDuration * animationDurationFactor);
    _animation.setStartValue(QVariant::fromValue<int>(current));
    _animation.setEndValue(QVariant::fromValue<int>(target));
    _animation.start();

    emit expandedChanged();
  }
}

QWidget* Expander::content() const {
  return _content;
}

void Expander::setContent(QWidget* content) {
  if (content != _content) {
    if (_content) {
      _content->removeEventFilter(this);
      delete _content;
    }

    _content = content;
    if (_content) {
      _content->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);
      _content->setParent(this);
      _content->installEventFilter(this);
      _content->setVisible(_expanded);
    }
    updateGeometry();
    emit contentChanged();
  }
}
} // namespace oclero::qlementine
