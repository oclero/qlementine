# Widgets

Qlementine provides a collection of `QWidgets` to extend Qt's standard ones.

More information about them coming soon. You can check them in the Sandbox application, at the moment.

## AboutDialog

A standard "About" window that you can use to quickly display basic information about your app.

![AboutDialog](assets/images/widgets/aboutdialog.png)

## AbstractItemListWidget

Base class for [`NavigationBar`](#navigationbar), [`NavigationSidebar`](#navigationsidebar) and [`SegmentedControl`](#segmentedcontrol).

## Action

Improves `QAction` by adding predicates (lambda functions) to update the state.

## ActionButton

Links a `QPushButton` to a `QAction`.

## ColorButton

Allows to pick and display a `QColor`.

## ColorEditor

Displays a `ColorButton` and the color value in an editable `QLineEdit` besides.

![ColorEditor](assets/images/widgets/coloreditor.png)

## CommandLinkButton

Improves `QCommandLinkButton` by supporting the `icon` property of `QAbstractButton`.

![CommandLinkButton](assets/images/widgets/commandlinkbutton.png)

## Expander

Container that allows to hide/show its content with a nice height animation.

## FlowLayout

A layout that arranges its items from left to right and wraps them to the next row when the available width is exceeded, such as a gallery of thumbnails. Its height depends on its width, so use it in a widget placed in a `QScrollArea` with `setWidgetResizable(true)`. The horizontal and vertical spacing use the style's spacing by default and can be overridden with `setHorizontalSpacing()` and `setVerticalSpacing()`.

## IconWidget

Widget that just shows an icon.

## Label

Improves `QLabel`` by allowing text roles as with HTML (headings h1, etc.).

![Label](assets/images/widgets/label.png)

## LineEdit

Improves `QLineEdit` by allowing statuses to display feedback for user (valid, error, warning, etc.).

![LineEdit](assets/images/widgets/lineedit.png)

## LoadingRing

A modern progress ring that you can use to show that an asynchronous operation is currently running (indeterminate), or to show a progress value (determinate).

![LoadingRing](assets/images/widgets/loadingring.png)

## LoadingSpinner

A small widget that you can use to show that an asynchronous operation is currently running.

![LoadingSpinner](assets/images/widgets/loadingspinner.png)

## Menu

Improves `QMenu` by adding predicates (lambda functions) to update state.

## NavigationBar

A widget that allows to switch between a range of elements, such as seen on Android.

![NavigationBar](assets/images/widgets/navigationbar.png)

## NavigationSidebar

A vertical sidebar that allows to switch between a range of elements, such as seen in settings windows. It extends [`NavigationBar`](#navigationbar).

- Top items (`addItem`) and bottom items (`addBottomItem`) that stay pinned below the scrollable top items.
- Nested items (`addChildItem`), expanded and collapsed with an animation by clicking the parent, with `setItemExpanded`, or with the Left and Right arrow keys.
- Non-selectable sections (`addSection`) and separators (`addSeparator`, `addBottomSeparator`).
- Icons and badges on every item.
- A scroll bar, displayed when the top items don't fit.
- A header widget and a footer widget (`setHeaderWidget`, `setFooterWidget`) that can contain anything, such as a search field.
- A compact (icon-only) mode (`setCompact`, `toggleCompact`), animated, with tooltips. The header and footer widgets are hidden in compact mode: give the user another way to expand the sidebar again. Nested items are hidden too: clicking their parent opens a flyout menu that contains them, and the parent shows the selection when one of its children is selected.
- `setExpandedWidth` to change the width when not compact.

![NavigationSidebar](assets/images/widgets/navigationsidebar.png)

## NavigationView

A [`NavigationSidebar`](#navigationsidebar) combined with a `QStackedWidget`: selecting an item in the sidebar shows its page. The view takes ownership of the pages.

Use `addPage`, `addBottomPage`, `addChildPage` and `removePage` to manage the pages. Use `sidebar()` to add sections and separators, set the header and footer, or toggle the compact mode. `setAutoCompactWidth` makes the sidebar compact when the view is narrower than the given width.

A complete example is available in the Showcase application: click the "NavigationView" button in the toolbar.

## NotificationBadge

A small badge to display a notification (with or without text) on another widget.

```cpp
// The parent set to the constructor is not important.
auto* badge = new NotificationBadge(someParent);

// The badge's parent will change to theWidget's parent.
badge->setWidget(theWidget);
```

![NotificationBadge](assets/images/widgets/notificationbadge.png)

## PageDots

A row of dots that shows the current page among a number of pages, such as in a carousel. Set the number of pages with `setCount()` and the current one with `setCurrentIndex()`. Clicking a dot selects its page and emits `currentIndexChanged()`, so it can be connected directly to a `QStackedWidget`.

![PageDots](assets/images/widgets/pagedots.png)

## Popover

A popup-like widget that opens in a separate system window, with a drop shadow and corner radiuses (hence transparency) and an opacity animation when toggling visibility.

![Popover](assets/images/widgets/popover.png)

## PopoverButton

A button that opens a `Popover` when clicked.

## RoundedFocusFrame

Improves `QFocusFrame` by adding a corner radius property.

## SegmentedControl

A widget that allows to switch between a range of elements, such as seen on iOS or macOS.

![SegmentedControl](assets/images/widgets/segmentedcontrol.png)

## StatusBadgeWidget

Widget to display a status icon: info, warning, error, success. Available in two standard sizes.

![StatusBadgeWidget](assets/images/widgets/badges.png)

## Switch

The famous Switch to toggle between states.

![Switch](assets/images/widgets/switch-checked.png)

## TitleBar

A themed title bar that is part of the window content. It draws the caption buttons (minimize, maximize, close, menu) when the platform doesn't provide them, hosts an optional content widget (menu bar, search field...) and tells the window which parts of it can be used to drag the window. The content container, plain `QWidget`/`QFrame` containers and display-only widgets (`QLabel`, `QProgressBar`, `QLCDNumber`, `IconWidget`, `StatusBadgeWidget`, `NotificationBadge`) are draggable by default, any other widget placed in the title bar is interactive, unless overridden with `setHitTestVisible()`. By default the content widget starts after the leading area of the window (such as a sidebar); `setContentAfterLeading(false)` places it right after the window controls on the left instead (the traffic lights on macOS), over the leading area, which is useful for a toolbar. A `UnifiedWindow` sets it to `false` on its title bar. `QTabBar`, `QMenuBar`, `QToolBar`, `NavigationBar` and `SegmentedControl` are draggable only in their empty space, not over their tabs, items or child buttons.

## UnifiedWindow

A top-level window whose content extends into the title bar area. It owns a `TitleBar`, a central widget and an optional `QStatusBar` (`setStatusBar()`) displayed at the bottom of the window across its whole width, under both the central widget and the leading widget. The central widget can be inset in the window with `setContentCornerRadius()` (0 by default, which keeps it square and without border): with a radius, its top and left sides have a border and its top-left corner is rounded. The title bar border is then hidden, since the inset central widget has its own border. `setContentInsetEdges()` chooses the inset sides, a side that is not inset blends into the window without border, and a corner is rounded when both of its sides are inset. The bottom side is inset when a status bar is set. When using it with a `NavigationSidebar` as leading widget, hide the sidebar border with `setBorderVisible(false)` to avoid a double line. macOS keeps the traffic lights at the top left, Windows keeps the caption buttons at the top right and supports snap layouts, and Linux draws client-side decorations following the user's button layout. A leading widget, such as a sidebar, can extend up to the top of the window: the title bar then starts after it and the caption buttons on the left are drawn over it.

![UnifiedWindow](assets/images/widgets/unifiedwindow.png)
