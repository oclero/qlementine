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

#include <QObject>
#include <QEvent>
#include <QWidget>
#include <QVariant>
#include <QVariantAnimation>

namespace oclero::qlementine {
template<typename T>
// This is just a wrapper around QVariantAnimation to get typed animated values.
class WidgetAnimation : public QObject {
public:
  explicit WidgetAnimation(QWidget* parentWidget = nullptr)
    : QObject(parentWidget) {
    assert(parentWidget);
    if (parentWidget) {
      _qVariantAnimation.setEasingCurve(QEasingCurve::Type::OutCubic);
      QObject::connect(
        &_qVariantAnimation, &QVariantAnimation::valueChanged, parentWidget,
        [parentWidget](const QVariant&) {
          // Force widget repaint.
          parentWidget->update();
        },
        Qt::ConnectionType::QueuedConnection);
      parentWidget->installEventFilter(this);
    }
  }

  ~WidgetAnimation() {
    stop();
  }

  void start() {
    _qVariantAnimation.setLoopCount(_loopEnabled ? -1 : 1);
    _qVariantAnimation.start();
  }

  void stop() {
    _qVariantAnimation.stop();
    _qVariantAnimation.setLoopCount(1);
    if (hasFinalValue()) {
      setStartValue(_finalValue);
    }
  }

  void setLoopEnabled(bool enabled) {
    _loopEnabled = enabled;
  }

  bool loopEnabled() const {
    return _loopEnabled;
  }

  void restart(T const& value) {
    stop();

    // Ensure it has a start value.
    if (!hasStartValue()) {
      if (loopEnabled()) {
        setStartValue(T());
      } else {
        setStartValue(value);
      }
    } else {
      setStartValue(this->value());
    }

    setFinalValue(value);
    start();
  }

  void restartIfNeeded(T const& value) {
    if (value != finalValue() || !hasFinalValue()) {
      restart(value);
    }
  }

  bool isRunning() const {
    return _qVariantAnimation.state() == QVariantAnimation::Running;
  }

  void setDuration(int const milliseconds) {
    if (milliseconds != _qVariantAnimation.duration()) {
      stop();
      _qVariantAnimation.setDuration(milliseconds);
    }
  }

  int duration() const {
    return _qVariantAnimation.duration();
  }

  T const& finalValue() const {
    return _finalValue;
  }

  void setFinalValue(T const& value) {
    if (value != _finalValue || !hasFinalValue()) {
      // Ensure it has a start value.
      if (!hasStartValue()) {
        setStartValue(value);
      }

      _finalValue = value;
      _qVariantAnimation.setEndValue(QVariant::fromValue<T>(value));
      _finalValueInitialized = true;
    }
  }

  T const& startValue() const {
    return _startValue;
  }

  void setStartValue(T const& value) {
    _startValue = value;
    _qVariantAnimation.setStartValue(QVariant::fromValue<T>(value));
    _startValueInitialized = true;
  }

  T value() const {
    if (isRunning()) {
      const auto variant = _qVariantAnimation.currentValue();
      return variant.template canConvert<T>() ? variant.template value<T>() : _finalValue;
    } else {
      return _finalValue;
    }
  }

  void setEasing(const QEasingCurve& easing) {
    _qVariantAnimation.setEasingCurve(easing);
  }

protected:
  bool eventFilter(QObject* obj, QEvent* evt) override {
    if (evt->type() == QEvent::Hide) {
      stop();
    }
    return QObject::eventFilter(obj, evt);
  }

  bool hasStartValue() const {
    return _startValueInitialized;
  }

  bool hasFinalValue() const {
    return _finalValueInitialized;
  }

private:
  bool _startValueInitialized{ false };
  bool _finalValueInitialized{ false };
  bool _loopEnabled{ false };
  QVariantAnimation _qVariantAnimation{};
  T _startValue{};
  T _finalValue{};
};
} // namespace oclero::qlementine
