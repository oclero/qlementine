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

#pragma once

#include <oclero/qlementine/animation/WidgetAnimator.hpp>

#include <unordered_map>
#include <optional>

namespace oclero::qlementine {
#define DECLARE_ANIMATE(NAME, TYPE, easing) \
  TYPE animate##NAME(const QWidget* w, const TYPE& target, int duration, bool loop = false) { \
    if (_animationsEnabled && w != nullptr) { \
      auto* animator = getOrCreateAnimator(w); \
      animator->set##NAME##Duration(w->isEnabled() ? duration : 0); \
      animator->set##NAME##Easing(easing); \
      animator->set##NAME##LoopEnabled(loop); \
      animator->set##NAME(target); \
      return animator->get##NAME(); \
    } else { \
      return target; \
    } \
  } \
\
  std::optional<TYPE> getAnimated##NAME(const QWidget* w) const { \
    const WidgetAnimator* animator = getAnimator(w); \
    return animator ? animator->get##NAME() : std::optional<TYPE>{}; \
  }

// Handles animations for all widgets.
class WidgetAnimationManager {
public:
  WidgetAnimationManager();

  bool enabled() const;
  void setEnabled(bool enabled);

  const WidgetAnimator* getAnimator(const QWidget* w) const;
  WidgetAnimator* getOrCreateAnimator(const QWidget* w);
  void stopAll();

  DECLARE_ANIMATE(BackgroundColor, QColor, _defaultEasingCurve)
  DECLARE_ANIMATE(BackgroundColor2, QColor, _defaultEasingCurve)
  DECLARE_ANIMATE(ForegroundColor, QColor, _defaultEasingCurve)
  DECLARE_ANIMATE(ForegroundColor2, QColor, _defaultEasingCurve)

  DECLARE_ANIMATE(BorderColor, QColor, _defaultEasingCurve)
  DECLARE_ANIMATE(BorderWidth, qreal, _defaultEasingCurve)
  DECLARE_ANIMATE(FocusBorderProgress, qreal, _focusEasingCurve)
  DECLARE_ANIMATE(Progress, qreal, _defaultEasingCurve)
  DECLARE_ANIMATE(Progress2, qreal, _defaultEasingCurve)
  DECLARE_ANIMATE(Progress3, qreal, _linearEasingCurve)

  const QEasingCurve& focusEasingCurve() const;
  const QEasingCurve& defaultEasingCurve() const;

private:
  void addWidget(const QWidget* widget, WidgetAnimator* widgetAnimator);
  void removeWidget(const QWidget* widget);
  WidgetAnimator* findWidget(const QWidget* widget) const;
  void forEarch(const std::function<void(const QWidget* w, WidgetAnimator* a)>& cb);

  void initializeEasingCurves();

private:
  bool _animationsEnabled{ true };
  QEasingCurve _focusEasingCurve;
  QEasingCurve _defaultEasingCurve;
  QEasingCurve _linearEasingCurve;
  std::unordered_map<const QWidget*, WidgetAnimator*> _map;
};
} // namespace oclero::qlementine
