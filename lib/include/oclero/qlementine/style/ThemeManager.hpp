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
