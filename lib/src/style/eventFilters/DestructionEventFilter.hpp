// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/Delegates.hpp>

#include <QEvent>
#include <QObject>

namespace oclero::qlementine {
class DestructionEventFilter : public QObject {
  Q_OBJECT

public:
  using DestructionCallback = std::function<void(QWidget*)>;

  DestructionEventFilter(QWidget* widget, QObject* parent, DestructionCallback callback)
    : QObject(parent)  // Parent to QlementineStyle, not the widget!
    , _widget(widget)
    , _callback(std::move(callback)) {}
  virtual ~DestructionEventFilter() {}

protected:
  bool eventFilter(QObject* watched, QEvent* event) override {
    if (event->type() == QEvent::Destroy && watched == _widget) {
      // Call the cleanup callback before the widget is destroyed
      if (_callback) {
        _callback(_widget);
      }
      // The filter will be deleted automatically when the widget is destroyed
      // since it's parented to the widget
    }
    return QObject::eventFilter(watched, event);
  }

private:
  QWidget* _widget;
  DestructionCallback _callback;
};
} // namespace oclero::qlementine
