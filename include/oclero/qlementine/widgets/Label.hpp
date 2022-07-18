#pragma once

#include <oclero/qlementine/style/Theme.hpp>

#include <QLabel>

namespace oclero::qlementine {
/// A QLabel that handles automatic styling for different text roles (titles, normal text, ect.)
/// like h1,h2,p in HTML.
class Label : public QLabel {
	Q_OBJECT

	Q_PROPERTY(oclero::qlementine::Theme::TextRole role READ role WRITE setRole NOTIFY roleChanged)

public:
	explicit Label(QWidget* parent = nullptr);
	explicit Label(const QString& text, QWidget* parent = nullptr);
	~Label();

	Theme::TextRole role() const;
	Q_SLOT void setRole(Theme::TextRole role);
	Q_SIGNAL void roleChanged();

protected:
	bool event(QEvent* e) override;
	bool eventFilter(QObject* o, QEvent* e) override;

private:
	void updatePaletteFromTheme();

	Theme::TextRole _role{ Theme::TextRole::Default };
	bool _changingPaletteFlag{ false };
};
} // namespace oclero::qlementine
