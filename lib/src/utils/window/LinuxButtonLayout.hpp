// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <oclero/qlementine/widgets/TitleBar.hpp>

#include <QFileSystemWatcher>
#include <QObject>
#include <QString>
#include <QTimer>

class QDBusVariant;

namespace oclero::qlementine {
/// Resolves the caption button layout preferred by the user and watches it for changes.
/// GNOME and other desktops use the settings portal (org.gnome.desktop.wm.preferences/button-layout),
/// KDE uses ButtonsOnLeft and ButtonsOnRight from kwinrc. The default is `appmenu:close`.
class LinuxButtonLayout : public QObject {
  Q_OBJECT

public:
  explicit LinuxButtonLayout(QObject* parent = nullptr);

  CaptionButtonLayout layout() const;
  bool isGnome() const;

Q_SIGNALS:
  void changed();

private:
  Q_SLOT void onSettingChanged(const QString& settingNamespace, const QString& key, const QDBusVariant& value);
  void refresh();

  bool _isGnome{ false };
  bool _isKde{ false };
  QString _kwinrcPath;
  CaptionButtonLayout _layout;
  QFileSystemWatcher _watcher;
  QTimer _debounceTimer;
};
} // namespace oclero::qlementine
