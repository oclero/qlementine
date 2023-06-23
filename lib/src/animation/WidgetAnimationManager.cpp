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

#include <oclero/qlementine/animation/WidgetAnimationManager.hpp>

namespace oclero::qlementine {
WidgetAnimationManager::WidgetAnimationManager() {
  initializeEasingCurves();
}

bool WidgetAnimationManager::enabled() const {
  return _animationsEnabled;
}

void WidgetAnimationManager::setEnabled(bool enabled) {
  _animationsEnabled = enabled;
  if (!enabled) {
    stopAll();
  }
}

const WidgetAnimator* WidgetAnimationManager::getAnimator(const QWidget* w) const {
  const auto* res = findWidget(w);
  return res;
}

WidgetAnimator* WidgetAnimationManager::getOrCreateAnimator(const QWidget* w) {
  auto* animator = findWidget(w);
  if (!animator) {
    animator = new WidgetAnimator(const_cast<QWidget*>(w));
    addWidget(w, animator);
  }
  return animator;
}

void WidgetAnimationManager::addWidget(const QWidget* widget, WidgetAnimator* widgetAnimator) {
  if (!findWidget(widget)) {
    _map.insert_or_assign(widget, widgetAnimator);

    QObject::connect(widget, &QObject::destroyed, widget, [this, widget]() {
      removeWidget(widget);
    });
  }
}

void WidgetAnimationManager::removeWidget(const QWidget* widget) {
  auto it = _map.find(widget);
  if (it != _map.end()) {
    const auto widgetAnimator = it->second;
    _map.erase(it);
    widgetAnimator->deleteLater();
  }
}

WidgetAnimator* WidgetAnimationManager::findWidget(const QWidget* widget) const {
  auto it = _map.find(widget);
  if (it != _map.end()) {
    return it->second;
  }
  return nullptr;
}

void WidgetAnimationManager::forEarch(const std::function<void(const QWidget* w, WidgetAnimator* a)>& cb) {
  if (!cb)
    return;
  std::for_each(_map.begin(), _map.end(), [&cb](const auto& kvp) {
    cb(kvp.first, kvp.second);
  });
}

void WidgetAnimationManager::stopAll() {
  forEarch([this](const QWidget* w, WidgetAnimator*) {
    // Stop all animations happening on the widget.
    removeWidget(w);
  });
}

void WidgetAnimationManager::initializeEasingCurves() {
  _focusEasingCurve.setOvershoot(5.);
  _focusEasingCurve.setType(QEasingCurve::Type::OutBack);
  _defaultEasingCurve.setType(QEasingCurve::Type::OutCubic);
  _linearEasingCurve.setType(QEasingCurve::Type::Linear);
}

const QEasingCurve& WidgetAnimationManager::focusEasingCurve() const {
  return _focusEasingCurve;
}

const QEasingCurve& WidgetAnimationManager::defaultEasingCurve() const {
  return _defaultEasingCurve;
}
} // namespace oclero::qlementine
