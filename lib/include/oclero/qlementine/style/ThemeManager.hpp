#pragma once

#include <QObject>
#include <QVector>
#include <QPointer>

#include <oclero/qlementine/style/Theme.hpp>
#include <oclero/qlementine/style/QlementineStyle.hpp>

namespace oclero::qlementine {
class ThemeManager : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString currentTheme READ currentTheme WRITE setCurrentTheme NOTIFY currentThemeChanged)
  Q_PROPERTY(int themeCount READ themeCount NOTIFY themeCountChanged)

public:
  ThemeManager(QObject* parent = nullptr);
  ThemeManager(QlementineStyle* style, QObject* parent = nullptr);
  ~ThemeManager() = default;

  QlementineStyle* style() const;
  void setStyle(QlementineStyle* style);

  const QVector<Theme>& themes() const;
  void addTheme(const Theme& theme);

  QString currentTheme() const;
  void setCurrentTheme(const QString& key);
  Q_SIGNAL void currentThemeChanged();

  int themeCount() const;
  Q_SIGNAL void themeCountChanged();

  Q_SLOT void setNextTheme();
  Q_SLOT void setPreviousTheme();

  int themeIndex(const QString& key) const;

  int currentThemeIndex() const;
  void setCurrentThemeIndex(int index);

  QString getLocalizedThemeName(const QString& baseThemeName) const;

private:
  void synchronizeThemeOnStyle();

private:
  QVector<Theme> _themes;
  int _currentIndex{ -1 };
  QPointer<QlementineStyle> _style{ nullptr };
};
} // namespace oclero::qlementine
