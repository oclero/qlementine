# Theme

## Introduction

Qlementine supports a theming system. This theming system is **basic compared to CSS**, but please keep in mind that this is specifically made **on purpose**. The goal of Qlementine is not to be a fully customizable theming system, but instead an **opinionated** look n'feel for your Qt application.

## Theme specs

Qlementine themes are JSON files.

- Every key is **optional**. If a key-value pair is absent, the theme will use its default value.
- The key names are mostly based on the `Theme` struct members, defined in `Theme.hpp`.
- Values must be either `string`, `integer` or `double`.
- Colors are defined as `string` with their **hexadecimal RGB or RGBA representation**, beginning by a `#`, following this syntax `#rrggbb` or `#rrggbbaa` (Example: `"#ff000077"` for semi-transparent red). They will be parsed and turned into `QColor`.
- Key-Value pairs are defined at the root of the JSON object file, except for the metadata.

Not all the `Theme` struct members are customizable, because some are used as cache to avoid computing the same colors again and again (e.g. the colors with the _-Transparent_ suffix) during animations.

!!! info

    info: A theme editor widget, named `ThemeEditor` is available in Qlementine, `/tools` folder.

## Keys

### Metadata

Metadata must be a JSON object for the key `meta`.

!!! info

    It is mandatory if you want to use `ThemeManager`, because the theme name is used as the identifier that should be unique for every theme.

Example:

```json
{
  "meta": {
    "author": "John Doe",
    "name": "My Awesome Theme",
    "version": "1.2.3"
  }
}
```

| Key       |  Type  | Role                                                 |
| :-------- | :----: | :--------------------------------------------------- |
| `name`    | string | Name of the theme.</br>Example: `"My Awesome Theme"` |
| `version` | string | Version of the theme.</br>Example: `"1.2.3"`         |
| `author`  | string | Author of the theme.</br>Example: `"John Doe"`       |

### Colors

Example:

```json
{
  "backgroundColorMain1": "#ffffff"
}
```

| Key                                 | Type  | Role                                                                              |
| :---------------------------------- | :---: | :-------------------------------------------------------------------------------- |
| **`backgroundColorMain1`**          | color | Textfields, checkboxes, radiobuttons background.                                  |
| `backgroundColorMain2`              | color | Window background.                                                                |
| `backgroundColorMain3`              | color | Container background, more contrast.                                              |
| `backgroundColorMain4`              | color | Same as above, more contrast.                                                     |
| **`neutralColor`**                  | color | Neutral interactive elements, such as buttons.                                    |
| `neutralColorHovered`               | color | Same as above, in hovered state.                                                  |
| `neutralColorPressed`               | color | Same as above, in pressed state.                                                  |
| `neutralColorDisabled`              | color | Same as above, in disabled state.                                                 |
| **`focusColor`**                    | color | Border around the widget that has keyboard focus with `QFocusFrame`.              |
| **`backgroundColorWorkspace`**      | color | Window workspace backround.                                                       |
| **`backgroundColorTabBar`**         | color | `QTabBar` backround.                                                              |
| **`primaryColor`**                  | color | Highlighted elements (default, checked or selected).                              |
| `primaryColorHovered`               | color | Same as above, in hovered state.                                                  |
| `primaryColorPressed`               | color | Same as above, in pressed state.                                                  |
| `primaryColorDisabled`              | color | Same as above, in disabled state.                                                 |
| **`primaryColorForeground`**        | color | Text written over highlighted elements.                                           |
| `primaryColorForegroundHovered`     | color | Same as above, in hovered state.                                                  |
| `primaryColorForegroundPressed`     | color | Same as above, in pressed state.                                                  |
| `primaryColorForegroundDisabled`    | color | Same as above, in disabled state.                                                 |
| **`primaryAlternativeColor`**       | color | A darker/lighter tint for highlighted elements over already highlighted elements. |
| `primaryAlternativeColorHovered`    | color | Same as above, in hovered state.                                                  |
| `primaryAlternativeColorPressed`    | color | Same as above, in pressed state.                                                  |
| `primaryAlternativeColorDisabled`   | color | Same as above, in disabled state.                                                 |
| **`secondaryColor`**                | color | Text.                                                                             |
| `secondaryColorHovered`             | color | Same as above, in hovered state.                                                  |
| `secondaryColorPressed`             | color | Same as above, in pressed state.                                                  |
| `secondaryColorDisabled`            | color | Same as above, in disabled state.                                                 |
| **`secondaryColorForeground`**      | color | Text written over elements that already have text color.                          |
| `secondaryColorForegroundHovered`   | color | Same as above, in hovered state.                                                  |
| `secondaryColorForegroundPressed`   | color | Same as above, in pressed state.                                                  |
| `secondaryColorForegroundDisabled`  | color | Same as above, in disabled state.                                                 |
| **`secondaryAlternativeColor`**     | color | Less important text.                                                              |
| `secondaryAlternativeColorHovered`  | color | Same as above, in hovered state.                                                  |
| `secondaryAlternativeColorPressed`  | color | Same as above, in pressed state.                                                  |
| `secondaryAlternativeColorDisabled` | color | Same as above, in disabled state.                                                 |
| **`statusColorSuccess`**            | color | Feedback for success/validity.                                                    |
| `statusColorSuccessHovered`         | color | Same as above, in hovered state.                                                  |
| `statusColorSuccessPressed`         | color | Same as above, in pressed state.                                                  |
| `statusColorSuccessDisabled`        | color | Same as above, in disabled state.                                                 |
| **`statusColorInfo`**               | color | Feedback for information.                                                         |
| `statusColorInfoHovered`            | color | Same as above, in hovered state.                                                  |
| `statusColorInfoPressed`            | color | Same as above, in pressed state.                                                  |
| `statusColorInfoDisabled`           | color | Same as above, in disabled state.                                                 |
| **`statusColorWarning`**            | color | Feedback for warning.                                                             |
| `statusColorWarningHovered`         | color | Same as above, in hovered state.                                                  |
| `statusColorWarningPressed`         | color | Same as above, in pressed state.                                                  |
| `statusColorWarningDisabled`        | color | Same as above, in disabled state.                                                 |
| **`statusColorError`**              | color | Feedback for error.                                                               |
| `statusColorErrorHovered`           | color | Same as above, in hovered state.                                                  |
| `statusColorErrorPressed`           | color | Same as above, in pressed state.                                                  |
| `statusColorErrorDisabled`          | color | Same as above, in disabled state.                                                 |
| **`statusColorForeground`**         | color | Text over status colors.                                                          |
| `statusColorForegroundHovered`      | color | Same as above, in hovered state.                                                  |
| `statusColorForegroundPressed`      | color | Same as above, in pressed state.                                                  |
| `statusColorForegroundDisabled`     | color | Same as above, in disabled state.                                                 |
| **`shadowColor1`**                  | color | Shadow for elevated elements.                                                     |
| `shadowColor2`                      | color | Same as above, more contrast.                                                     |
| `shadowColor3`                      | color | Same as above, more contrast.                                                     |
| **`borderColor`**                   | color | Borders of textfields, checkboxes, radiobuttons, switches and other elements.     |
| `borderColorHovered`                | color | Same as above, in hovered state.                                                  |
| `borderColorPressed`                | color | Same as above, in pressed state.                                                  |
| `borderColorDisabled`               | color | Same as above, in disabled state.                                                 |
| **`semiTransparentColor1`**         | color | To be used over another color to lighten/darken it.                               |
| `semiTransparentColor2`             | color | Same as above but more contrast.                                                  |
| `semiTransparentColor3`             | color | Same as above but more contrast.                                                  |
| `semiTransparentColor4`             | color | Same as above but more contrast.                                                  |

### Numeric Values

Example:

```json
{
  "fontSize": 12
}
```

| Key                       |  Type  | Role                                                                                                                                                      |
| :------------------------ | :----: | :-------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `fontSize`                |  int   | Font size for normal text (`QLabel`, `QPushButton`, `QCheckBox`, etc.).                                                                                   |
| `fontSizeMonospace`       |  int   | Font size for monospace text.                                                                                                                             |
| `fontSizeH1`              |  int   | Font size for level 1 headers.                                                                                                                            |
| `fontSizeH2`              |  int   | Font size for level 2 headers.                                                                                                                            |
| `fontSizeH3`              |  int   | Font size for level 3 headers.                                                                                                                            |
| `fontSizeH4`              |  int   | Font size for level 4 headers.                                                                                                                            |
| `fontSizeH5`              |  int   | Font size for level 4 headers.                                                                                                                            |
| `fontSizeS1`              |  int   | Font size for level 1 captions.                                                                                                                           |
| `spacing`                 |  int   | Spacing between elements. Multiples of this value will be used across the various widgets, including default `QLayout` spacings and margins.              |
| `iconExtent`              |  int   | Size for icons. Multiple of this value will be used across the various widgets, according to their size for `QIcon`.                                      |
| `animationDuration`       |  int   | Duration (in milliseconds) for a UI animation, such as a color change.                                                                                    |
| `focusAnimationDuration`  |  int   | Duration (in milliseconds) for the focus border animation.</br>Note: can be longer to allow the user to see the focus .                                   |
| `sliderAnimationDuration` |  int   | Duration (in milliseconds) for the slider animation when its value changes.</br>Note: must be quick to feel responsive.                                   |
| `borderRadius`            | double | Corner radius for most widgets.                                                                                                                           |
| `checkBoxBorderRadius`    | double | Corner radius for checkboxes.</br>Note: smaller than `borderRadius` because checkboxes are smaller.                                                       |
| `menuItemBorderRadius`    | double | Corner radius for menu items.</br>Note: Even smaller than `borderRadius` because menu already have a corner radius and padding.                           |
| `menuBarItemBorderRadius` | double | Corner radius for menu bar items.                                                                                                                         |
| `borderWidth`             | double | Border thickness for widgets that have borders.                                                                                                           |
| `focusBorderWidth`        |  int   | Border thickness for the focus indicator.                                                                                                                 |
| `controlHeightLarge`      |  int   | Height for most basics `QWidget`s, such as `QPushButton` or `QCheckBox`.                                                                                  |
| `controlHeightMedium`     |  int   | Height for a smaller `QWidget`, such as `QSlider`.                                                                                                        |
| `controlHeightSmall`      |  int   | Height for an even smaller `QWidget`, such as the scroll buttons of a `QMenu`.                                                                            |
| `controlDefaultWidth`     |  int   | Width used as the default width in `sizeHint()` method for widgets that are extensible, such as `QSlider` or `QProgressBar`.                              |
| `dialMarkLength`          |  int   | For a `QDial`, the length of the knob needle that indicates its value.                                                                                    |
| `dialMarkThickness`       |  int   | For a `QDial`, the thickness of a tick, if visible.                                                                                                       |
| `dialTickLength`          |  int   | For a `QDial`, the length of a tick, if visible.                                                                                                          |
| `dialTickSpacing`         |  int   | For a `QDial`, the spacing between knob and ticks, if visible.                                                                                            |
| `dialGrooveThickness`     |  int   | For a `QDial`, the thickness of the knob highlighted zone.                                                                                                |
| `sliderTickSize`          |  int   | For a `QSlider`, the length of a tick, if visible.                                                                                                        |
| `sliderTickSpacing`       |  int   | For a `QSlider`, the spacing between slider and ticks, if visible.                                                                                        |
| `sliderTickThickness`     |  int   | For a `QSlider`, the thickness of a tick, if visible.                                                                                                     |
| `sliderGrooveHeight`      |  int   | For a `QSlider`, the thickness of the groove (i.e. the highlited zone).                                                                                   |
| `progressBarGrooveHeight` |  int   | For a `QProgressBar`, the thickness of the groove (i.e. the highlited zone).                                                                              |
| `scrollBarThicknessFull`  |  int   | For a `QScrollBar`, the thickness when the mouse is over.                                                                                                 |
| `scrollBarThicknessSmall` |  int   | For a `QScrollBar`, the thickness when the mouse is not hover.                                                                                            |
| `scrollBarMargin`         |  int   | For a `QScrollBar`, the margin between the scrollbar and its parent.                                                                                      |
| `tabBarPaddingTop`        |  int   | For a `QTabBar`, the space between the top of the bar and the top of the tabs.                                                                            |
| `tabBarTabMaxWidth`       |  int   | For a `QTabBar`, the maximum width a tab can have.</br>Note: any value below or equal to `0` will be ignored and treated as if there is no maximum width. |
| `tabBarTabMinWidth`       |  int   | For a `QTabBar`, the minimum width a tab can have.</br>Note: any value below or equal to `0` will be ignored and treated as if there is no minimum width. |

## Full example

Here is a full Qlementine theme in all its glory. Please note that every value is optional, and some might be redundant with the default values. Remove/add the key-value pairs you don't need to customize.

```json
{
  "meta": {
    "author": "Olivier Cléro",
    "name": "Light",
    "version": "1.5.0"
  },

  "backgroundColorMain1": "#ffffff",
  "backgroundColorMain2": "#f3f3f3",
  "backgroundColorMain3": "#e3e3e3",
  "backgroundColorMain4": "#dfdfdf",

  "backgroundColorWorkspace": "#b7b7b7",
  "backgroundColorTabBar": "#dfdfdf",

  "borderColor": "#d3d3d3",
  "borderColorDisabled": "#e9e9e9",
  "borderColorHovered": "#b3b3b3",
  "borderColorPressed": "#a3a3a3",

  "focusColor": "#40a9ff66",

  "neutralColor": "#d1d1d1",
  "neutralColorHovered": "#d3d3d3",
  "neutralColorPressed": "#d5d5d5",
  "neutralColorDisabled": "#eeeeee",

  "primaryColor": "#1890ff",
  "primaryColorHovered": "#2c9dff",
  "primaryColorPressed": "#40a9ff",
  "primaryColorDisabled": "#d1e9ff",

  "primaryAlternativeColor": "#106ef9",
  "primaryAlternativeColorDisabled": "#a9d6ff",
  "primaryAlternativeColorHovered": "#107bfd",
  "primaryAlternativeColorPressed": "#108bfd",

  "primaryColorForeground": "#ffffff",
  "primaryColorForegroundDisabled": "#ecf6ff",
  "primaryColorForegroundHovered": "#ffffff",
  "primaryColorForegroundPressed": "#ffffff",

  "secondaryColor": "#404040",
  "secondaryColorHovered": "#333333",
  "secondaryColorPressed": "#262626",
  "secondaryColorDisabled": "#d4d4d4",

  "secondaryAlternativeColor": "#909090",
  "secondaryAlternativeColorDisabled": "#c3c3c3",
  "secondaryAlternativeColorHovered": "#747474",
  "secondaryAlternativeColorPressed": "#828282",

  "secondaryColorForeground": "#ffffff",
  "secondaryColorForegroundDisabled": "#ededed",
  "secondaryColorForegroundHovered": "#ffffff",
  "secondaryColorForegroundPressed": "#ffffff",

  "semiTransparentColor1": "#0000000a",
  "semiTransparentColor2": "#00000019",
  "semiTransparentColor3": "#00000021",
  "semiTransparentColor4": "#00000028",

  "shadowColor1": "#00000020",
  "shadowColor2": "#00000040",
  "shadowColor3": "#00000060",

  "statusColorError": "#e96b72",
  "statusColorErrorHovered": "#f47c83",
  "statusColorErrorPressed": "#ff9197",
  "statusColorErrorDisabled": "#f9dadc",

  "statusColorForeground": "#ffffff",
  "statusColorForegroundHovered": "#ffffff",
  "statusColorForegroundPressed": "#ffffff",
  "statusColorForegroundDisabled": "#ffffff99",

  "statusColorInfo": "#1ba8d5",
  "statusColorInfoHovered": "#1eb5e5",
  "statusColorInfoPressed": "#29c0f0",
  "statusColorInfoDisabled": "#c7eaf5",

  "statusColorSuccess": "#2bb5a0",
  "statusColorSuccessHovered": "#3cbfab",
  "statusColorSuccessPressed": "#4ecdb9",
  "statusColorSuccessDisabled": "#d5f0ec",

  "statusColorWarning": "#fbc064",
  "statusColorWarningHovered": "#ffcf6c",
  "statusColorWarningPressed": "#ffd880",
  "statusColorWarningDisabled": "#feefd8",

  "fontSize": 12,
  "fontSizeMonospace": 13,
  "fontSizeH1": 34,
  "fontSizeH2": 26,
  "fontSizeH3": 22,
  "fontSizeH4": 18,
  "fontSizeH5": 14,
  "fontSizeS1": 10,

  "spacing": 8,
  "iconExtent": 16,

  "animationDuration": 192,
  "focusAnimationDuration": 384,
  "sliderAnimationDuration": 96,

  "borderRadius": 6.0,
  "checkBoxBorderRadius": 4.0,
  "menuItemBorderRadius": 4.0,
  "menuBarItemBorderRadius": 2.0,

  "borderWidth": 1,
  "focusBorderWidth": 2,

  "controlHeightLarge": 28,
  "controlHeightMedium": 24,
  "controlHeightSmall": 16,
  "controlDefaultWidth": 96,

  "dialMarkLength": 4,
  "dialMarkThickness": 2,
  "dialTickLength": 4,
  "dialTickSpacing": 4,
  "dialGrooveThickness": 4,

  "sliderTickSize": 3,
  "sliderTickSpacing": 2,
  "sliderTickThickness": 1,
  "sliderGrooveHeight": 4,

  "progressBarGrooveHeight": 6,

  "scrollBarThicknessFull": 12,
  "scrollBarThicknessSmall": 6,
  "scrollBarMargin": 0,

  "tabBarPaddingTop": 4,
  "tabBarTabMaxWidth": 0,
  "tabBarTabMinWidth": 0
}
```
