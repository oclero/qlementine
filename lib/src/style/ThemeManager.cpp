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

#include <oclero/qlementine/style/ThemeManager.hpp>

namespace oclero::qlementine {
ThemeManager::ThemeManager(QObject* parent)
  : ThemeManager(nullptr, parent) {}

ThemeManager::ThemeManager(QlementineStyle* style, QObject* parent)
  : QObject(parent) {
  setStyle(style);
}

QlementineStyle* ThemeManager::style() const {
  return nullptr;
}

void ThemeManager::setStyle(QlementineStyle* style) {
  if (style != _style) {
    _style = style;
    synchronizeThemeOnStyle();
    emit currentThemeChanged();
    emit themeCountChanged();
  }
}

const QVector<Theme>& ThemeManager::themes() const {
  return _themes;
}

void ThemeManager::addTheme(const Theme& theme) {
  _themes.append(theme);
  emit themeCountChanged();
  if (_currentIndex == -1) {
    setCurrentThemeIndex(0);
  }
}

QString ThemeManager::currentTheme() const {
  if (_currentIndex > -1 && _currentIndex < _themes.size()) {
    return _themes.at(_currentIndex).meta.name;
  }
  return {};
}

void ThemeManager::setCurrentTheme(const QString& key) {
  const auto index = themeIndex(key);
  setCurrentThemeIndex(index);
}

int ThemeManager::currentThemeIndex() const {
  return _currentIndex;
}

void ThemeManager::setCurrentThemeIndex(int index) {
  index = std::max(-1, std::min(_themes.size() - 1, index));
  if (index != _currentIndex) {
    _currentIndex = index;
    synchronizeThemeOnStyle();
    emit currentThemeChanged();
  }
}

int ThemeManager::themeCount() const {
  return _themes.size();
}

void ThemeManager::setNextTheme() {
  if (_themes.size() > 1) {
    // Wrap.
    const auto next = (_currentIndex + 1) % _themes.size();
    setCurrentThemeIndex(next);
  }
}

void ThemeManager::setPreviousTheme() {
  if (_themes.size() > 1) {
    // Wrap.
    auto previous = _currentIndex - 1;
    if (previous < 0)
      previous = _themes.size() - 1;
    setCurrentThemeIndex(previous);
  }
}

int ThemeManager::themeIndex(const QString& key) const {
  const auto it = std::find_if(_themes.begin(), _themes.end(), [&key](auto& theme) {
    return theme.meta.name == key;
  });
  if (it != _themes.end())
    return static_cast<int>(std::distance(_themes.begin(), it));
  return -1;
}

QString ThemeManager::getLocalizedThemeName(const QString& baseThemeName) const {
  if (baseThemeName.toLower() == QStringLiteral("light")) {
    return tr("Light Theme");
  } else if (baseThemeName.toLower() == QStringLiteral("dark")) {
    return tr("Dark Theme");
  }
  return baseThemeName;
}

void ThemeManager::synchronizeThemeOnStyle() {
  if (_style && _currentIndex != -1 && _themes.size() > 0 && _currentIndex < _themes.size()) {
    _style->setTheme(_themes.at(_currentIndex));
  }
}
} // namespace oclero::qlementine
