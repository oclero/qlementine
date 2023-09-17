## Widgets

Qlementine provides a collection of `QWidgets` to extend Qt's standard ones.

More information about them coming soon. You can check them in the Sandbox application, at the moment.

## AbstractItemListWidget

Base class for [`NavigationBar`](#navigation-bar) and [`SegmentedControl`](#segmented-control).

## Action

Improves `QAction` by adding predicates (lambda functions) to update the state.

## ActionButton

Links a `QPushButton` to a `QAction`.

## ColorButton

Allows to pick and display a `QColor`.

## ColorEditor

Displays a `ColorButton` and the color value in an editable `QLineEdit` besides.

![Color Editor](assets/images/widgets/coloreditor.png)

<img width="156" alt="ColorEditor" src="./assets/images/widgets/coloreditor.png">

## CommandLinkButton

Improves `QCommandLinkButton` by supporting the `icon` property of `QAbstractButton`.

<img width="462" alt="CommandLinkButton" src="/assets/images/widgets/commandlinkbutton.png">

## Expander

Container that allows to hide/show its content with a nice height animation.

## IconWidget

Widget that just shows an icon.

## Label

Improves `QLabel`` by allowing text roles as with HTML (headings h1, etc.).

<img width="194" alt="Label" src="/assets/images/widgets/label.png">

## LineEdit

Improves `QLineEdit` by allowing statuses to display feedback for user (valid, error, warning, etc.).

<img width="228" alt="LineEdit" src="/assets/images/widgets/lineedit.png">

## Menu

Improves `QMenu` by adding predicates (lambda functions) to update state.

## NavigationBar

A widget that allows to switch between a range of elements, such as seen on Android.

<img width="391" alt="NavigationBar" src="/assets/images/widgets/navigationbar.png">

## Popover

A popup-like widget that opens in a separate system window, with a drop shadow and corner radiuses (hence transparency) and an opacity animation when toggling visibility.

<img width="203" alt="Popover" src="/assets/images/widgets/popover.png">

## PopoverButton

A button that opens a `Popover` when clicked.

## RoundedFocusFrame

Improves `QFocusFrame` by adding a corner radius property.

## SegmentedControl

A widget that allows to switch between a range of elements, such as seen on iOS or macOS.

<img width="405" alt="SegmentedControl" src="/assets/images/widgets/segmentedcontrol.png">

## StatusBadgeWidget

Widget to display a status icon: info, warning, error, success. Available in two standard sizes.

<img width="234" alt="StatusBadgeWidget" src="/assets/images/widgets/badges.png">

## Switch

The famous Switch to toggle between states.

<img width="193" alt="Switch" src="/assets/images/widgets/switch-checked.png">
