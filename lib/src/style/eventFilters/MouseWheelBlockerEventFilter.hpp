// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QEvent>
#include <QObject>
#include <QWidget>

namespace oclero::qlementine {
class MouseWheelBlockerEventFilter : public QObject {
public:
  explicit MouseWheelBlockerEventFilter(QWidget* widget)
    : QObject(widget)
    , _widget(widget) {}

  bool eventFilter(QObject*, QEvent* evt) override {
    const auto type = evt->type();
    if (type == QEvent::Wheel) {
      if (!_widget->hasFocus()) {
        evt->ignore();
        return true;
      }
    }

    return false;
  }

private:
  QWidget* _widget{ nullptr };
};
} // namespace oclero::qlementine
