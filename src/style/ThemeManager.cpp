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
	const auto it = std::find_if(_themes.begin(), _themes.end(), [&key](auto& theme) { return theme.meta.name == key; });
	if (it != _themes.end())
		return std::distance(_themes.begin(), it);
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
