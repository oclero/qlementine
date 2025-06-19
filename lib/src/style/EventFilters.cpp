// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include "EventFilters.hpp"

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/utils/StateUtils.hpp>
#include <oclero/qlementine/utils/ImageUtils.hpp>
#include <oclero/qlementine/utils/PrimitiveUtils.hpp>
#include <oclero/qlementine/utils/MenuUtils.hpp>
#include <oclero/qlementine/utils/WidgetUtils.hpp>

#include <QMouseEvent>
#include <QWidget>
#include <QLineEdit>
#include <QObject>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>
#include <QStylePainter>
#include <QFocusFrame>
#include <QPointer>
#include <QSpinBox>
#include <QComboBox>
#include <QListView>
#include <QLayout>
#include <QPlainTextEdit>

namespace oclero::qlementine {
LineEditButtonEventFilter::LineEditButtonEventFilter(
  QlementineStyle* style, WidgetAnimationManager& animManager, QToolButton* button)
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
    const auto& theme = _style ? _style->theme() : Theme{};
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
    const auto& theme = _style ? _style->theme() : Theme{};
    const auto rect = _button->rect();
    const auto palette = _button->style()->standardPalette();

    const auto& bgColor = _style ? _style->toolButtonBackgroundColor(mouse, ColorRole::Secondary)
                                 : palette.color(getPaletteColorGroup(mouse), QPalette::ColorRole::ButtonText);
    const auto& fgColor = _style ? _style->toolButtonForegroundColor(mouse, ColorRole::Secondary)
                                 : palette.color(getPaletteColorGroup(mouse), QPalette::ColorRole::Button);
    const auto animationDuration = _style ? _style->theme().animationDuration : 0;
    const auto& currentBgColor = _animManager.animateBackgroundColor(_button, bgColor, animationDuration);
    const auto& currentFgColor = _animManager.animateForegroundColor(_button, fgColor, animationDuration);

    // Get opacity animated in qlinedit_p.cpp:436
    const auto opacity = _button->property(QByteArrayLiteral("opacity")).toDouble();

    const auto circleH = theme.controlHeightMedium;
    const auto circleW = circleH;
    const auto circleX = rect.x() + (rect.width() - circleW) / 2;
    const auto circleY = rect.y() + (rect.height() - circleH) / 2;
    const auto circleRect = QRect(QPoint{ circleX, circleY }, QSize{ circleW, circleH });

    const auto pixmap = getPixmap(_button->icon(), theme.iconSize, mouse, CheckState::NotChecked, _button);
    const auto autoIconColor = _style ? _style->autoIconColor(_button) : AutoIconColor::None;
    const auto& colorizedPixmap = _style->getColorizedPixmap(pixmap, autoIconColor, currentFgColor, currentFgColor);
    const auto pixmapX = circleRect.x() + (circleRect.width() - theme.iconSize.width()) / 2;
    const auto pixmapY = circleRect.y() + (circleRect.height() - theme.iconSize.height()) / 2;
    const auto pixmapRect = QRect{ { pixmapX, pixmapY }, theme.iconSize };

    QPainter p(_button);
    p.setOpacity(opacity);
    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Background.
    p.setBrush(currentBgColor);
    p.drawEllipse(circleRect);

    // Foreground.
    p.drawPixmap(pixmapRect, colorizedPixmap);

    evt->accept();
    return true;
  }

  return QObject::eventFilter(watchedObject, evt);
}

CommandLinkButtonPaintEventFilter::CommandLinkButtonPaintEventFilter(
  QlementineStyle& style, WidgetAnimationManager& animManager, QCommandLinkButton* button)
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
    const auto& bgColor = _style.toolButtonBackgroundColor(mouse, ColorRole::Secondary);
    const auto& currentBgColor = _animManager.animateBackgroundColor(_button, bgColor, theme.animationDuration);
    const auto radius = theme.borderRadius;

    const auto iconSize = theme.iconSize;
    const auto pixmap = getPixmap(_button->icon(), iconSize, mouse, CheckState::NotChecked, _button);
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
  bool eventFilter(QObject* watchedObject, QEvent* evt) override {
    const auto type = evt->type();
    if (type == QEvent::Leave || type == QEvent::Enter) {
      if (_tabBar) {
        _tabBar->update();
      }
    }

    return QObject::eventFilter(watchedObject, evt);
  }

private:
  QTabBar* _tabBar{ nullptr };
};

TabBarEventFilter::TabBarEventFilter(QlementineStyle& style, QTabBar* tabBar)
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
    style.setAutoIconColor(_leftButton, AutoIconColor::None);
    _leftButton->installEventFilter(buttonEvtFilter);

    _rightButton = toolButtons.at(1);
    _rightButton->setFocusPolicy(Qt::NoFocus);
    _rightButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _rightButton->setFixedSize(_rightButton->sizeHint());
    style.setAutoIconColor(_rightButton, AutoIconColor::None);
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
        Q_EMIT _tabBar->tabCloseRequested(tabIndex);
        return true;
      }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
      // Tab context menu.
      const auto tabIndex = _tabBar->tabAt(mouseEvent->pos());
      if (tabIndex != -1 && _tabBar->isTabVisible(tabIndex)) {
        evt->accept();
        Q_EMIT _tabBar->customContextMenuRequested(mouseEvent->pos());
        return true;
      }
    }
  } else if (type == QEvent::Wheel) {
    const auto* wheelEvent = static_cast<QWheelEvent*>(evt);

    // Block non-horizontal scroll.
    const bool wheelVertical = qAbs(wheelEvent->angleDelta().y()) > qAbs(wheelEvent->angleDelta().x());
    if (wheelVertical) {
      evt->ignore();
      return true;
    }

    auto delta = wheelEvent->pixelDelta().x();

    // If delta is null, it might be because we are on MacOS, using a trackpad.
    // So let's use angleDelta instead.
    if (delta == 0) {
      delta = wheelEvent->angleDelta().x();
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

bool MenuEventFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
  const auto type = evt->type();

  switch (type) {
    case QEvent::Type::Show: {
      _mousePressed = false;

      // Place the QMenu correctly by making up for the drop shadow margins.
      // It'll be reset before every show, so we can safely move it every time.
      // Submenus should already be placed correctly, so there's no need to translate their geometry.
      // Also, make up for the menu item padding so the texts are aligned.
      const auto isMenuBarMenu = qobject_cast<QMenuBar*>(_menu->parentWidget()) != nullptr;
      const auto isSubMenu = qobject_cast<QMenu*>(_menu->parentWidget()) != nullptr;
      const auto alignForMenuBar = isMenuBarMenu && !isSubMenu;
      const auto* qlementineStyle = qobject_cast<QlementineStyle*>(_menu->style());
      const auto menuItemHPadding = qlementineStyle ? qlementineStyle->theme().spacing : 0;
      const auto menuDropShadowWidth = qlementineStyle ? qlementineStyle->theme().spacing : 0;
      const auto menuOriginalPos = _menu->pos();
      const auto menuBarTranslation = alignForMenuBar ? QPoint(-menuItemHPadding, 0) : QPoint(0, 0);
      const auto shadowTranslation = QPoint(-menuDropShadowWidth, -menuDropShadowWidth);
      const auto menuNewPos = menuOriginalPos + menuBarTranslation + shadowTranslation;

      // Menus have weird sizing bugs when moving them from this event.
      // We have to wait for the event loop to be processed before setting the final position.
      const auto menuSize = _menu->size();
      if (menuSize != QSize(0, 0)) {
        _menu->resize(0, 0); // Hide the menu for now until we can set the position.
        QTimer::singleShot(0, _menu, [this, menuNewPos, menuSize]() {
          _menu->move(menuNewPos);
          _menu->resize(menuSize);
        });
      }
    } break;
    case QEvent::Type::MouseMove: {
      if (static_cast<QMouseEvent*>(evt)->buttons()) {
        _mousePressed = true;
      }
    } break;
    case QEvent::Type::MouseButtonPress: {
      _mousePressed = true;
      const auto* mouseEvt = static_cast<QMouseEvent*>(evt);
      const auto mousePos = mouseEvt->pos();
      if (const auto* action = _menu->actionAt(mousePos)) {
        if (action->isSeparator() || !action->isEnabled() || action->property("qlementine_flashing").toBool()) {
          return true;
        }
      } else if (_menu->rect().contains(mousePos)) {
        return true;
      }
    } break;
    case QEvent::Type::MouseButtonRelease: {
      if (!_mousePressed) {
        if (evt == _mouseEventToNotFilter) {
          // Do not delete the event, it will be deleted for us.
          _mouseEventToNotFilter = nullptr;
          // Let it go to the widget.
          return false;
        }
        return true; // ignore
      }
      _mousePressed = false;
      const auto* mouseEvt = static_cast<QMouseEvent*>(evt);
      const auto mousePos = mouseEvt->pos();
      if (auto* action = _menu->actionAt(mousePos)) {
        if (action->isSeparator() || !action->isEnabled() || action->property("qlementine_flashing").toBool())
          return true;

        if (action->menu() == nullptr) {
          flashAction(action, _menu, [this, action]() {
            // We send a manually-built mouse event to click on the menu item.
            const auto menuItemCenter = _menu->actionGeometry(action).center();
            _mouseEventToNotFilter = new QMouseEvent(QEvent::MouseButtonRelease, menuItemCenter,
              _menu->mapToGlobal(menuItemCenter), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
            QCoreApplication::postEvent(_menu, _mouseEventToNotFilter);
          });
          return true;
        }
      } else if (_menu->rect().contains(mousePos)) {
        return true;
      }
    } break;
    default:
      break;
  }

  return QObject::eventFilter(watchedObject, evt);
}

ComboboxItemViewFilter::ComboboxItemViewFilter(QComboBox* comboBox, QListView* view)
  : QObject(view)
  , _comboBox(comboBox)
  , _view(view)
  , _initialMaxHeight(view->maximumHeight()) {
  _view->installEventFilter(this);

  auto* comboBoxPopup = _view->parentWidget();
  comboBoxPopup->installEventFilter(this);

  /*
  const auto childWidgets = comboBoxPopup->findChildren<QWidget*>();
  for (auto* child : childWidgets) {
    if (child->inherits("QComboBoxPrivateScroller")) {
      child->setFixedHeight(0);
      child->setVisible(false);
    }
  }
*/

  _comboBox->installEventFilter(this);
}

bool ComboboxItemViewFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
  const auto type = evt->type();
  switch (type) {
    case QEvent::Type::Show:
      fixViewGeometry();
      break;
    case QEvent::Type::Resize:
      if (watchedObject == _comboBox) {
        fixViewGeometry();
      }
      break;
    default:
      break;
  }
  return QObject::eventFilter(watchedObject, evt);
}

void ComboboxItemViewFilter::fixViewGeometry() {
  const auto* comboBox = findFirstParentOfType<QComboBox>(_view);
  if (const auto* qlementineStyle = qobject_cast<QlementineStyle*>(comboBox->style())) {
    const auto hMargin = qlementineStyle->pixelMetric(QStyle::PM_MenuHMargin);
    const auto shadowWidth = qlementineStyle->theme().spacing;
    const auto borderWidth = qlementineStyle->theme().borderWidth;
    const auto width =
      std::max(comboBox->width(), _view->sizeHintForColumn(0) + shadowWidth * 2) + hMargin * 2 + borderWidth * 2;
    const auto height = std::min(800, viewMinimumSizeHint().height());
    _view->setFixedWidth(width);
    _view->setFixedHeight(height);
    _view->parentWidget()->adjustSize();
  }
}

QSize ComboboxItemViewFilter::viewMinimumSizeHint() const {
  // QListView::minimumSizeHint() doesn't give the correct minimumHeight,
  // so we have to compute it.
  const auto rowCount = _view->model()->rowCount();
  auto height = 0;
  for (auto i = 0; i < rowCount && height <= _initialMaxHeight; ++i) {
    const auto rowSizeHint = _view->sizeHintForRow(i);
    height = std::min(_initialMaxHeight, height + rowSizeHint);
  }
  // It looks like it is OK for the width, though.
  const auto width = _view->sizeHintForColumn(0);
  return { width, height };
}

TextEditEventFilter::TextEditEventFilter(QAbstractScrollArea* textEdit)
  : QObject(textEdit)
  , _textEdit(textEdit) {}

bool TextEditEventFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
  const auto type = evt->type();
  switch (type) {
    case QEvent::Enter:
    case QEvent::Leave:
      _textEdit->update();
      break;
    case QEvent::Paint: {
      const auto* qlementineStyle = qobject_cast<QlementineStyle*>(_textEdit->style());
      if (!qlementineStyle)
        break;

      const auto frameShape = _textEdit->frameShape();
      switch (frameShape) {
        case QFrame::Shape::StyledPanel: {
          QStyleOptionFrame opt;
          opt.initFrom(_textEdit);
          opt.rect = _textEdit->rect();
          QPainter p(_textEdit);
          qlementineStyle->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, &p, _textEdit);
        } break;
        case QFrame::Shape::Panel: {
          const auto mouse = getMouseState(_textEdit->hasFocus(), _textEdit->underMouse(), _textEdit->isEnabled());
          const auto& bgColor = qlementineStyle->textFieldBackgroundColor(mouse, Status::Default);
          const auto rect = _textEdit->rect();
          QPainter p(_textEdit);
          p.fillRect(rect, bgColor);
        } break;
        default:
          break;
      }
    } break;
    default:
      break;
  }

  return QObject::eventFilter(watchedObject, evt);
}

WidgetWithFocusFrameEventFilter::WidgetWithFocusFrameEventFilter(QWidget* widget)
  : QObject(widget)
  , _widget(widget) {
  _focusFrame = new QFocusFrame(_widget);
}

bool WidgetWithFocusFrameEventFilter::eventFilter(QObject* watchedObject, QEvent* evt) {
  if (!_added && watchedObject == _widget) {
    const auto type = evt->type();
    // Create the focus frame as late as possible to give
    // more chances to any parent (e.g. scrollarea) to already exist.
    // QEvent::Show isn't sufficient. We need to delay even more, so
    // waiting for the first QEvent::Paint is our only solution.
    if (type == QEvent::Paint) {
      QTimer::singleShot(0, this, [this]() {
        if (!_added) {
          _added = true;
          _focusFrame->setWidget(_widget);
        }
      });
    }
  }

  return QObject::eventFilter(watchedObject, evt);
}

class LineEditMenuIconsBehavior : public QObject {
  QPointer<QMenu> _menu{ nullptr };
  bool _menuCustomized{ false };

  enum class IconListMode {
    None,
    LineEdit, // Use of QLineEdit and QPlainTextEdit.
    ReadOnlyLineEdit,
    SpinBox,
  };

  static std::vector<QIcon> iconList(IconListMode mode) {
    const auto* qlem = oclero::qlementine::appStyle();
    if (!qlem)
      return {};

    // The order follows the one defined QLineEdit.cpp and QSpinBox.cpp (Qt6).
    switch (mode) {
      case IconListMode::LineEdit:
        return {
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-undo"),
          qlem->makeThemedIconFromName("edit-redo"),
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-cut"),
          qlem->makeThemedIconFromName("edit-copy"),
          qlem->makeThemedIconFromName("edit-paste"),
          qlem->makeThemedIconFromName("edit-delete"),
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-select-all"),
        };
      case IconListMode::ReadOnlyLineEdit:
        return {
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-copy"),
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-select-all"),
        };
      case IconListMode::SpinBox:
        return {
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-undo"),
          qlem->makeThemedIconFromName("edit-redo"),
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-cut"),
          qlem->makeThemedIconFromName("edit-copy"),
          qlem->makeThemedIconFromName("edit-paste"),
          qlem->makeThemedIconFromName("edit-delete"),
          QIcon(), // Separator
          QIcon(), // Separator
          qlem->makeThemedIconFromName("edit-select-all"),
          QIcon(), // Separator
          qlem->makeThemedIconFromName("go-up"),
          qlem->makeThemedIconFromName("go-down"),
        };
      default:
        return {};
    }
  }

  static IconListMode getMode(const QMenu* menu) {
    if (const auto* menuParent = menu->parent()) {
      if (qobject_cast<const QAbstractSpinBox*>(menuParent->parent())) {
        return IconListMode::SpinBox;
      } else if (const auto* line_edit = qobject_cast<const QLineEdit*>(menuParent)) {
        return line_edit->isReadOnly() ? IconListMode::ReadOnlyLineEdit : IconListMode::LineEdit;
      } else if (const auto* menuParentParent = menuParent ? menuParent->parent() : nullptr) {
        if (const auto* plainTextEdit = qobject_cast<const QPlainTextEdit*>(menuParentParent)) {
          return plainTextEdit->isReadOnly() ? IconListMode::ReadOnlyLineEdit : IconListMode::LineEdit;
        }
      }
    }
    return IconListMode::None;
  }

  void customizeMenu() {
    const auto actions = _menu->findChildren<QAction*>();
    if (!actions.empty()) {
      const auto icons = iconList(getMode(_menu));
      if (!icons.empty()) {
        for (auto i = 0; i < static_cast<int>(icons.size()) && i < static_cast<int>(actions.size()); ++i) {
          if (auto* action = actions.at(i)) {
            action->setIcon(icons.at(i));
          }
        }
      }
    }
    _menu->adjustSize();
  }

public:
  LineEditMenuIconsBehavior(QMenu* menu)
    : QObject(menu)
    , _menu(menu) {
    // Hack pour modifier les icones du menu contextuel des line edit.
    QObject::connect(_menu, &QMenu::aboutToShow, this, [this]() {
      if (!_menuCustomized) {
        customizeMenu();
        _menuCustomized = true;
      }
    });
  }
};

LineEditMenuEventFilter::LineEditMenuEventFilter(QWidget* parent)
  : QObject(parent) {
  assert(parent);
  // Might be a menu's submenu.
  if (auto* menu = qobject_cast<QMenu*>(parent)) {
    new LineEditMenuIconsBehavior(menu);
  } else {
    // The QLineEdit.
    parent->installEventFilter(this);
  }
}

bool LineEditMenuEventFilter::eventFilter(QObject*, QEvent* evt) {
  const auto type = evt->type();
  if (type == QEvent::ChildPolished) {
    constexpr auto propertyName = "qlementine_tweak_menu_icons";
    auto* child = static_cast<QChildEvent*>(evt)->child();

    const auto tweaked = child->property(propertyName).toBool();
    if (!tweaked) {
      child->setProperty(propertyName, true);

      // QLineEdit child of QSpinBox.
      if (auto* lineEdit = qobject_cast<QLineEdit*>(child)) {
        lineEdit->installEventFilter(this);

      }
      // Qmenu that needs tweaking.
      else if (auto* menu = qobject_cast<QMenu*>(child)) {
        new LineEditMenuIconsBehavior(menu);

        // Forward auto icon color mode from parent to the menu.
        if (const auto* menuParent = menu->parentWidget()) {
          if (const auto* style = qobject_cast<oclero::qlementine::QlementineStyle*>(menuParent->style())) {
            const auto autoIconColor = style->autoIconColor(menuParent);
            QlementineStyle::setAutoIconColor(menu, autoIconColor);
          }
        }
      }
      // Case of a QPlainTextEdit (inherits QAbstractScrollArea).
      else if (child->objectName() == "qt_scrollarea_viewport") {
        if (auto* childWidget = qobject_cast<QWidget*>(child)) {
          childWidget->installEventFilter(this);
        }
      }
    }
  }

  return false;
}
} // namespace oclero::qlementine
