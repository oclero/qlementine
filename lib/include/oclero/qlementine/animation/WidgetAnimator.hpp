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

#include <oclero/qlementine/animation/WidgetAnimation.hpp>
#include <oclero/qlementine/style/Theme.hpp>

#include <QObject>
#include <QEvent>
#include <QWidget>
#include <QColor>

namespace oclero::qlementine {
// The WidgetAnimator class can't be templated as it is used in a map.
// Also, we won't use a map for properties, for performance reasons.
// So we declare all available animations with a macro, to avoid boilerplate code.
// The animations themselves will be ceated lazily, on demand, when needed to paint.
#define DECLARE_ANIMATION(NAME, TYPE) \
private: \
  mutable std::unique_ptr<WidgetAnimation<TYPE>> _##NAME; \
  WidgetAnimation<TYPE>& get##NAME##Animation() const { \
    if (!_##NAME) { \
      _##NAME = std::make_unique<WidgetAnimation<TYPE>>(_parentWidget); \
    } \
    return *_##NAME; \
  } \
\
public: \
  TYPE get##NAME() const { \
    return get##NAME##Animation().value(); \
  } \
  void set##NAME(const TYPE& value) { \
    get##NAME##Animation().restartIfNeeded(value); \
  } \
  void set##NAME##Duration(const int duration) { \
    get##NAME##Animation().setDuration(duration); \
  } \
  void set##NAME##Easing(const QEasingCurve& easing) { \
    get##NAME##Animation().setEasing(easing); \
  } \
  void stop##NAME() { \
    get##NAME##Animation().stop(); \
  } \
  void set##NAME##LoopEnabled(const bool loop) { \
    get##NAME##Animation().setLoopEnabled(loop); \
  }

class WidgetAnimator : public QObject {
public:
  explicit WidgetAnimator(QWidget* parent)
    : QObject(parent)
    , _parentWidget(parent) {}

  virtual ~WidgetAnimator() = default;

  // All the animatable properties should be here.
  DECLARE_ANIMATION(BackgroundColor, QColor)
  DECLARE_ANIMATION(ForegroundColor, QColor)

  DECLARE_ANIMATION(BackgroundColor2, QColor)
  DECLARE_ANIMATION(ForegroundColor2, QColor)

  DECLARE_ANIMATION(BorderColor, QColor)
  DECLARE_ANIMATION(BorderWidth, qreal)
  DECLARE_ANIMATION(FocusBorderProgress, qreal)
  DECLARE_ANIMATION(Progress, qreal)
  DECLARE_ANIMATION(Progress2, qreal)
  DECLARE_ANIMATION(Progress3, qreal)

private:
  QWidget* _parentWidget;
};
} // namespace oclero::qlementine
