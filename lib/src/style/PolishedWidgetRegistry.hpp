// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QList>
#include <QMap>

class QObject;
class QWidget;

namespace oclero::qlementine {
class PolishedWidgetRegistry {
public:
  struct Entry {
    QList<QObject*> eventFilters;
  };

  Entry& ensure(QWidget* widget);
  bool hasEventFilters(QWidget* widget) const;

  void addEventFilter(QWidget* widget, Entry& entry, QObject* eventFilter);
  void rememberEventFilter(Entry& entry, QObject* eventFilter);
  void removeAndDeleteEventFilters(QWidget* widget);
  void forgetDestroyedWidget(QWidget* widget, QObject* destructionEventFilter);

private:
  QMap<QWidget*, Entry> _widgets;
};
} // namespace oclero::qlementine
