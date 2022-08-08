#include "EventFilters.hpp"

#include <oclero/qlementine/utils/StateUtils.hpp>
#include <oclero/qlementine/utils/ImageUtils.hpp>
#include <oclero/qlementine/utils/PrimitiveUtils.hpp>

#include <QMouseEvent>
#include <QWidget>
#include <QLineEdit>
#include <QObject>
#include <QMenu>
#include <QMenuBar>

namespace oclero::qlementine {
LineEditButtonEventFilter::LineEditButtonEventFilter(QlementineStyle& style, WidgetAnimationManager& animManager, QToolButton* button)
	: QObject(button)
	, _style(style)
	, _animManager(animManager)
	, _button(button) {
	// Qt doesn't emit this signal so we emit it by ourselves.
	if (auto* parent = button->parentWidget()) {
		if (auto* lineEdit = qobject_cast<QLineEdit*>(parent)) {
			QObject::connect(_button, &QAbstractButton::clicked, lineEdit, &QLineEdit::returnPressed);
		}
	}
}

bool LineEditButtonEventFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
	const auto type = evt->type();
	if (type == QEvent::Resize) {
		// Prevent resizing from qlineedit_p.cpp:540
		evt->ignore();
		return true;
	} else if (type == QEvent::Move) {
		// Prevent moving from qlineedit_p.cpp:540
		evt->ignore();
		// Instead, place the button by ourselves.
		const auto* parentLineEdit = _button->parentWidget();
		const auto parentRect = parentLineEdit->rect();
		const auto& theme = _style.theme();
		const auto buttonH = theme.controlHeightMedium;
		const auto buttonW = buttonH;
		const auto spacing = theme.spacing / 2;
		const auto buttonX = parentRect.x() + parentRect.width() - buttonW - spacing;
		const auto buttonY = parentRect.y() + (parentRect.height() - buttonH) / 2;
		_button->setGeometry(buttonX, buttonY, buttonW, buttonH);
		return true;
	} else if (type == QEvent::Paint) {
		// Draw the button by ourselves to bypass QLineEditIconButton::paintEvent in qlineedit_p.cpp:353
		const auto enabled = _button->isEnabled();
		if (!enabled) {
			evt->accept();
			return true;
		}

		const auto hovered = _button->underMouse();
		const auto pressed = _button->isDown();
		const auto mouse = getMouseState(pressed, hovered, enabled);
		const auto& theme = _style.theme();
		const auto rect = _button->rect();
		const auto& bgColor = theme.toolButtonBackgroundColor(mouse, ColorRole::Neutral);
		const auto circleH = theme.controlHeightMedium;
		const auto circleW = circleH;
		const auto circleX = rect.x() + (rect.width() - circleW) / 2;
		const auto circleY = rect.y() + (rect.height() - circleH) / 2;
		const auto circleRect = QRect(QPoint{ circleX, circleY }, QSize{ circleW, circleH });
		// Get opacity animated in qlinedit_p.cpp:436
		const auto opacity = _button->property(QByteArrayLiteral("opacity")).toDouble();
		const auto pixelRatio = getPixelRatio(_button);
		const auto pixmap = getPixmap(_button->icon(), theme.iconSize, pixelRatio, mouse, CheckState::NotChecked);
		const auto pixmapX = circleRect.x() + (circleRect.width() - theme.iconSize.width()) / 2;
		const auto pixmapY = circleRect.y() + (circleRect.height() - theme.iconSize.height()) / 2;
		const auto pixmapRect = QRect{ { pixmapX, pixmapY }, theme.iconSize };
		const auto& currentBgColor = _animManager.animateBackgroundColor(_button, bgColor, theme.animationDuration);

		QPainter p(_button);
		p.setOpacity(opacity);
		p.setPen(Qt::NoPen);
		p.setBrush(currentBgColor);
		p.setRenderHint(QPainter::Antialiasing, true);
		p.drawEllipse(circleRect);
		p.drawPixmap(pixmapRect, pixmap);

		evt->accept();
		return true;
	}

	return QObject::eventFilter(watchedObject, evt);
}

CommandLinkButtonPaintEventFilter::CommandLinkButtonPaintEventFilter(QlementineStyle& style, WidgetAnimationManager& animManager, QCommandLinkButton* button)
	: QObject(button)
	, _style(style)
	, _animManager(animManager)
	, _button(button) {}

bool CommandLinkButtonPaintEventFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
	const auto type = evt->type();
	if (type == QEvent::Paint) {
		// Draw the button by ourselves to bypass QLineEditIconButton::paintEvent in qlineedit_p.cpp:353
		const auto enabled = _button->isEnabled();
		const auto hovered = _button->underMouse();
		const auto pressed = _button->isDown();
		const auto mouse = getMouseState(pressed, hovered, enabled);
		const auto& theme = _style.theme();
		const auto rect = _button->rect();
		const auto spacing = theme.spacing;
		const auto hPadding = spacing * 2;
		const auto fgRect = rect.marginsRemoved(QMargins(hPadding, 0, hPadding, 0));
		const auto& bgColor = theme.toolButtonBackgroundColor(mouse, ColorRole::Neutral);
		const auto& currentBgColor = _animManager.animateBackgroundColor(_button, bgColor, theme.animationDuration);
		const auto radius = theme.borderRadius;

		const auto iconSize = theme.iconSize;
		const auto pxRatio = getPixelRatio(_button);
		const auto pixmap = getPixmap(_button->icon(), iconSize, pxRatio, mouse, CheckState::NotChecked);
		const auto pixmapX = fgRect.x();
		const auto pixmapY = fgRect.y() + (fgRect.height() - iconSize.height()) / 2;
		const auto pixmapRect = QRect{ { pixmapX, pixmapY }, iconSize };

		QPainter p(_button);
		p.setPen(Qt::NoPen);
		p.setBrush(currentBgColor);
		p.setRenderHint(QPainter::Antialiasing, true);
		p.drawRoundedRect(rect, radius, radius);
		p.drawPixmap(pixmapRect, pixmap);

		evt->accept();
		return true;
	}

	return QObject::eventFilter(watchedObject, evt);
}

MouseWheelBlockerEventFilter::MouseWheelBlockerEventFilter(QWidget* widget)
	: QObject(widget)
	, _widget(widget) {}

bool MouseWheelBlockerEventFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
	const auto type = evt->type();
	if (type == QEvent::Wheel) {
		if (!_widget->hasFocus()) {
			evt->ignore();
			return true;
		}
	}

	return QObject::eventFilter(watchedObject, evt);
}

class TabBarButtonEventFilter : public QObject {
public:
	TabBarButtonEventFilter(QTabBar* tabBar)
		: QObject(tabBar)
		, _tabBar(tabBar) {}

protected:
	bool eventFilter(QObject*, QEvent* evt) override {
		const auto type = evt->type();
		if (type == QEvent::Leave || type == QEvent::Enter) {
			if (_tabBar) {
				_tabBar->update();
			}
		}

		return false;
	}

private:
	QTabBar* _tabBar{ nullptr };
};

TabBarEventFilter::TabBarEventFilter(QTabBar* tabBar)
	: QObject(tabBar)
	, _tabBar(tabBar) {
	// Tweak left/right buttons.
	const auto toolButtons = tabBar->findChildren<QToolButton*>();
	if (toolButtons.size() == 2) {
		auto* buttonEvtFilter = new TabBarButtonEventFilter(_tabBar);

		_leftButton = toolButtons.at(0);
		_leftButton->setFocusPolicy(Qt::NoFocus);
		_leftButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		_leftButton->setFixedSize(_leftButton->sizeHint());
		QlementineStyle::setAutoIconColorEnabled(_leftButton, false);
		_leftButton->installEventFilter(buttonEvtFilter);

		_rightButton = toolButtons.at(1);
		_rightButton->setFocusPolicy(Qt::NoFocus);
		_rightButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		_rightButton->setFixedSize(_rightButton->sizeHint());
		QlementineStyle::setAutoIconColorEnabled(_rightButton, false);
		_rightButton->installEventFilter(buttonEvtFilter);
	}
}

bool TabBarEventFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
	const auto type = evt->type();

	if (type == QEvent::MouseButtonRelease) {
		const auto* mouseEvent = static_cast<QMouseEvent*>(evt);
		if (mouseEvent->button() == Qt::MouseButton::MiddleButton) {
			// Close tab.
			const auto tabIndex = _tabBar->tabAt(mouseEvent->pos());
			if (tabIndex != -1 && _tabBar->isTabVisible(tabIndex)) {
				evt->accept();
				emit _tabBar->tabCloseRequested(tabIndex);
				return true;
			}
		} else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
			// Tab context menu.
			const auto tabIndex = _tabBar->tabAt(mouseEvent->pos());
			if (tabIndex != -1 && _tabBar->isTabVisible(tabIndex)) {
				evt->accept();
				emit _tabBar->customContextMenuRequested(mouseEvent->pos());
				return true;
			}
		}
	} else if (type == QEvent::Wheel) {
		const auto* wheelEvent = static_cast<QWheelEvent*>(evt);
		auto delta = wheelEvent->pixelDelta().x();

		// If delta is null, it might be because we are on MacOS, using a trackpad.
		// So let's use angleDelta instead.
		if (delta == 0) {
			delta = wheelEvent->angleDelta().y();
		}

		// Invert the value if necessary.
		if (wheelEvent->inverted()) {
			delta = -delta;
		}

		if (delta > 0 && _rightButton) {
			// delta > 0 : scroll to the right.
			_rightButton->click();
			evt->accept();
			return true;
		} else if (delta < 0 && _leftButton) {
			// delta < 0 : scroll to the left.
			_leftButton->click();
			evt->accept();
			return true;
		} else {
			evt->ignore();
			return true;
		}
	} else if (type == QEvent::HoverMove) {
		const auto* mouseEvent = static_cast<QMouseEvent*>(evt);
		const auto beginX = _leftButton->x();
		if (mouseEvent->pos().x() > beginX) {
			_tabBar->update();
		}
	}

	return QObject::eventFilter(watchedObject, evt);
}
MenuEventFilter::MenuEventFilter(QMenu* menu)
	: QObject(menu)
	, _menu(menu) {
	menu->installEventFilter(this);
}

bool MenuEventFilter::eventFilter(QObject*, QEvent* evt) {
	const auto type = evt->type();
	if (type == QEvent::Type::Show) {
		// Place the QMenu correctly by making up for the drop shadow margins.
		// It'll be reset before every show, so we can safely move it every time.
		// Submenus should already be placed correctly, so there's no need to translate their geometry.
		// Also, make up for the menu item padding so the texts are aligned.
		const auto isMenuBarMenu = qobject_cast<QMenuBar*>(_menu->parentWidget()) != nullptr;
		const auto isSubMenu = qobject_cast<QMenu*>(_menu->parentWidget()) != nullptr;
		if (isMenuBarMenu && !isSubMenu) {
			if (const auto* qlementineStyle = qobject_cast<QlementineStyle*>(_menu->style())) {
				const auto menuItemHPadding = qlementineStyle->theme().spacing;
				const auto menuDropShadowWidth = qlementineStyle->theme().spacing;
				const auto menuRect = _menu->geometry().translated(-menuDropShadowWidth - menuItemHPadding, -menuDropShadowWidth);
				_menu->setGeometry(menuRect);
			}
		}
	}

	return false;
}

ComboboxItemViewFilter::ComboboxItemViewFilter(QAbstractItemView* view)
	: QObject(view)
	, _view(view) {
	view->installEventFilter(this);
}

bool ComboboxItemViewFilter::eventFilter(QObject* /*watchedObject*/, QEvent* evt) {
	const auto type = evt->type();
	if (type == QEvent::Type::Show) {
		// Fix Qt bug.
		const auto width = _view->sizeHintForColumn(0);
		_view->setMinimumWidth(width);
	}
	return false;
}
} // namespace oclero::qlementine
