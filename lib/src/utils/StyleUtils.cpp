#include <oclero/qlementine/utils/StyleUtils.hpp>

#include <QAbstractButton>
#include <QComboBox>
#include <QMenuBar>
#include <QScrollBar>
#include <QSplitterHandle>
#include <QTabBar>
#include <QAbstractSlider>
#include <QLineEdit>
#include <QAbstractSpinBox>
#include <qabstractitemview.h>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QToolButton>

namespace oclero::qlementine {
bool shouldHaveHoverEvents(const QWidget* w) {
	return qobject_cast<const QAbstractButton*>(w) || qobject_cast<const QComboBox*>(w) || qobject_cast<const QMenuBar*>(w) || qobject_cast<const QScrollBar*>(w)
				 || qobject_cast<const QSplitterHandle*>(w) || qobject_cast<const QTabBar*>(w) || qobject_cast<const QAbstractSlider*>(w) || qobject_cast<const QLineEdit*>(w)
				 || qobject_cast<const QAbstractSpinBox*>(w) || qobject_cast<const QAbstractItemView*>(w) || qobject_cast<const QGroupBox*>(w) || w->inherits("QDockSeparator")
				 || w->inherits("QDockWidgetSeparator");
}

bool shouldHaveMouseTracking(const QWidget* w) {
	return qobject_cast<const QAbstractItemView*>(w);
}

bool shouldHaveBoldFont(const QWidget* w) {
	return qobject_cast<const QPushButton*>(w) || qobject_cast<const QToolButton*>(w);
}

bool shouldHaveExternalFocusFrame(const QWidget* w) {
	return (w && (qobject_cast<const QCheckBox*>(w) || qobject_cast<const QPushButton*>(w) || qobject_cast<const QRadioButton*>(w) || qobject_cast<const QToolButton*>(w))
					 && !qobject_cast<const QTabBar*>(w->parentWidget()))
				 || qobject_cast<const QComboBox*>(w) || qobject_cast<const QLineEdit*>(w) || (!qobject_cast<const QScrollBar*>(w) && qobject_cast<const QAbstractSlider*>(w))
				 || qobject_cast<const QGroupBox*>(w);
}

bool shouldHaveTabFocus(const QWidget* w) {
	return w && (w->focusPolicy() == Qt::StrongFocus || w->focusPolicy() == Qt::ClickFocus) && (qobject_cast<const QAbstractButton*>(w) || qobject_cast<const QGroupBox*>(w));
}

bool shouldNotBeVerticallyCompressed(const QWidget* w) {
	return qobject_cast<const QAbstractButton*>(w) || qobject_cast<const QComboBox*>(w) || qobject_cast<const QLineEdit*>(w) || qobject_cast<const QAbstractSpinBox*>(w);
}

std::tuple<int, int> getHPaddings(const bool hasIcon, const bool hasText, const bool hasIndicator, const int padding) {
	if (hasText) {
		if (!hasIcon && hasIndicator)
			return { padding * 2, padding };
		else if (hasIcon && !hasIndicator)
			return { padding, padding * 2 };
		else if (hasIcon && hasIndicator)
			return { padding, padding };
		else
			return { padding * 2, padding * 2 };
	}
	return { padding, padding };
}

bool shouldNotHaveWheelEvents(const QWidget* w) {
	return (!qobject_cast<const QScrollBar*>(w) && qobject_cast<const QAbstractSlider*>(w)) || qobject_cast<const QAbstractSpinBox*>(w);
}
} // namespace oclero::qlementine
