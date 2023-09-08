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

Metadata must be a JSON object for the key `meta`. It is not mandatory, but might be useful if a GUI to switch themes is implemented someday.

Example:

```json
{
  "meta": {
    "author": "Olivier Cléro",
    "name": "Light",
    "version": "1.2.3"
  }
}
```

| Key | Type | Role |
|:----|:----:|:-----|
| `name` | string | Name of the theme.</br>Example: `"Light Theme"` |
| `version` | string | Version of the theme.</br>Example: `"1.0.0"` |
| `author` | string | Author of the theme.</br>Example: `"John Doe"` |

### Colors

Example:

```json
{
  "backgroundColorMain1": "#ffffff"
}
```

| Key | Type | Role | Default |
|:----|:----:|:-----|:--------|
| **`backgroundColorMain1`** | color | Textfields, checkboxes, radiobuttons background. | `#ffffff` |
| `backgroundColorMain2` | color | Window background. | `#f3f3f3` |
| `backgroundColorMain3` | color | Container background, more contrast. | `#e3e3e3` |
| `backgroundColorMain4` | color | Same as above, more contrast. | `#dcdcdc` |
| **`neutralColor`** | color | Neutral interactive elements, such as buttons. | `#e1e1e1` |
| `neutralColorHovered` | color | Same as above, in hovered state. | `#dadada` |
| `neutralColorPressed` | color | Same as above, in pressed state. | `#d2d2d2` |
| `neutralColorDisabled` | color | Same as above, in disabled state. | `#eeeeee` |
| **`focusColor`** | color | Border around the widget that has keyboard focus. | `#40a9ff66` |
| **`primaryColor`** | color | Highlighted elements (default, checked or selected). | `#1890ff` |
| `primaryColorHovered` | color | Same as above, in hovered state. | `#2c9dff` |
| `primaryColorPressed` | color | Same as above, in pressed state. | `#40a9ff` |
| `primaryColorDisabled` | color | Same as above, in disabled state. | `#d1e9ff` |
| **`primaryColorForeground`** | color | Text written over highlighted elements. | `#ffffff` |
| `primaryColorForegroundHovered` | color | Same as above, in hovered state. | `#ffffff` |
| `primaryColorForegroundPressed` | color | Same as above, in pressed state. | `#ffffff` |
| `primaryColorForegroundDisabled` | color | Same as above, in disabled state. | `#ecf6ff` |
| **`primaryAlternativeColor`** | color | A darker/lighter tint for highlighted elements over already highlighted elements. | `#106ef9` |
| `primaryAlternativeColorHovered` | color | Same as above, in hovered state. | `#0f7bfd` |
| `primaryAlternativeColorPressed` | color | Same as above, in pressed state. | `#0f88fd` |
| `primaryAlternativeColorDisabled` | color | Same as above, in disabled state. | `#a9d6ff` |
| **`secondaryColor`** | color | Text. | `#404040` |
| `secondaryColorHovered` | color | Same as above, in hovered state. | `#333333` |
| `secondaryColorPressed` | color | Same as above, in pressed state. | `#262626` |
| `secondaryColorDisabled` | color | Same as above, in disabled state. | `#d4d4d4` |
| **`secondaryColorForeground`** | color | | `#ffffff` |
| `secondaryColorForegroundHovered` | color | Same as above, in hovered state. | `#ffffff` |
| `secondaryColorForegroundPressed` | color | Same as above, in pressed state. | `#ffffff` |
| `secondaryColorForegroundDisabled` | color | Same as above, in disabled state. | `#ededed` |
| **`secondaryAlternativeColor`** | color | Less important text. | `#909090` |
| `secondaryAlternativeColorHovered` | color | Same as above, in hovered state. | `#747474` |
| `secondaryAlternativeColorPressed` | color | Same as above, in pressed state. | `#828282` |
| `secondaryAlternativeColorDisabled` | color | Same as above, in disabled state. | `#c3c3c3` |
| **`statusColorSuccess`** | color | Feedback for success/validity. | `#2bb5a0` |
| `statusColorSuccessHovered` | color | Same as above, in hovered state. | `#3cbfab` |
| `statusColorSuccessPressed` | color | Same as above, in pressed state. | `#4ecdb9` |
| `statusColorSuccessDisabled` | color | Same as above, in disabled state. | `#d5f0ec` |
| **`statusColorInfo`** | color | Feedback for information. | `#1ba8d5` |
| `statusColorInfoHovered` | color | Same as above, in hovered state. | `#1eb5e5` |
| `statusColorInfoPressed` | color | Same as above, in pressed state. | `#29c0f0` |
| `statusColorInfoDisabled` | color | Same as above, in disabled state. | `#c7eaf5` |
| **`statusColorWarning`** | color | Feedback for warning. | `#fbc064` |
| `statusColorWarningHovered` | color | Same as above, in hovered state. | `#ffcf6c` |
| `statusColorWarningPressed` | color | Same as above, in pressed state. | `#ffd880` |
| `statusColorWarningDisabled` | color | Same as above, in disabled state. | `#feefd8` |
| **`statusColorError`** | color | Feedback for error. | `#e96b72` |
| `statusColorErrorHovered` | color | Same as above, in hovered state. | `#f47c83` |
| `statusColorErrorPressed` | color | Same as above, in pressed state. | `#ff9197` |
| `statusColorErrorDisabled` | color | Same as above, in disabled state. | `#f9dadc` |
| **`statusColorForeground`** | color | Text over status colors. | `#ffffff` |
| `statusColorForegroundHovered` | color | Same as above, in hovered state. | `#ffffff` |
| `statusColorForegroundPressed` | color | Same as above, in pressed state. | `#ffffff` |
| `statusColorForegroundDisabled` | color | Same as above, in disabled state. | `#ffffff99` |
| **`shadowColor1`** | color | Shadow for elevated elements. | `#00000020` |
| `shadowColor2` | color | Same as above, more contrast. | `#00000040` |
| `shadowColor3` | color | Same as above, more contrast. | `#00000060` |
| **`borderColor`** | color | Borders of textfields, checkboxes, radiobuttons, switches and other elements. | `#d3d3d3` |
| `borderColorHovered` | color | Same as above, in hovered state. | `#b3b3b3` |
| `borderColorPressed` | color | Same as above, in pressed state. | `#a3a3a3` |
| `borderColorDisabled` | color | Same as above, in disabled state. | `#b3b3b3` |
| **`semiTransparentColor1`** | color | To be used over another color to lighten/darken it. | `#0000000a` |
| `semiTransparentColor2` | color | Same as above but more contrast. | `#00000019` |
| `semiTransparentColor3` | color | Same as above but more contrast. | `#00000021` |
| `semiTransparentColor4` | color | Same as above but more contrast. | `#00000028` |

### Numeric Values

Example:

```json
{
  "fontSize": 12
}
```

| Key | Type | Role | Default |
|:----|:----:|:-----|:-------:|
| `fontSize` | int | Font size for normal text. | `12` |
| `fontSizeMonospace` | int | Font size for monospace text. | `13` |
| `fontSizeH1` | int | Font size for level 1 headers. | `34` |
| `fontSizeH2` | int | Font size for level 2 headers. | `26` |
| `fontSizeH3` | int | Font size for level 3 headers. | `22` |
| `fontSizeH4` | int | Font size for level 4 headers. | `18` |
| `fontSizeH5` | int | Font size for level 4 headers. | `14` |
| `fontSizeS1` | int | Font size for level 1 captions. | `10` |
| `spacing` | int | Spacing between elements. Multiples of this value will be used across the various widgets.| `8` |
| `iconExtent` | int | Size for icons. Multiple of this value will be used across the various widgets, according to their size. | `16` |
| `animationDuration` | int | Duration (in milliseconds) for a UI animation, such as a color change. | `192` |
| `focusAnimationDuration` | int | Duration (in milliseconds) for the focus border animation.</br>Note: can be longer to allow the user to see the focus . | `384` |
| `sliderAnimationDuration` | int | Duration (in milliseconds) for the slider animation when its value changes.</br>Note: must be quick to feel responsive. | `96` |
| `borderRadius` | double | Corner radius for most widgets. | `6.0` |
| `checkBoxBorderRadius` | double | Corner radius for checkboxes.</br>Note: smaller than `borderRadius` because checkboxes are smaller. | `4.0` |
| `menuItemBorderRadius` | double | Corner radius for menu items.</br>Note: Even smaller than `borderRadius` because menu already have a corner radius and padding. | `4.0` |
| `menuBarItemBorderRadius` | double | Corner radius for menu bar items. | `2.0` |
| `borderWidth` | double | Border thickness for widgets that have borders. | `1` |
| `focusBorderWidth` | int | Border thickness for the focus indicator. | `2` |
| `controlHeightLarge` | int | Height for most basics `QWidget`s, such as `QPushButton` or `QCheckBox`. | `28` |
| `controlHeightMedium` | int | Height for a smaller `QWidget`, such as `QSlider`. | `24` |
| `controlHeightSmall` | int | Height for an even smaller `QWidget`, such as the scroll buttons of a `QMenu`. | `16` |
| `controlDefaultWidth` | int | Width used as the default width in `sizeHint()` method for widgets that are extensible, such as  `QSlider` or `QProgressBar`. | `96` |
| `dialMarkLength` | int | For a `QDial`, the length of the knob needle that indicates its value. | `8` |
| `dialMarkThickness` | int | For a `QDial`, the thickness of a tick, if visible. | `2` |
| `dialTickLength` | int | For a `QDial`, the length of a tick, if visible. | `4` |
| `dialTickSpacing` | int | For a `QDial`, the spacing between knob and ticks, if visible. | `4` |
| `dialGrooveThickness` | int | For a `QDial`, the thickness of the knob highlighted zone. | `4` |
| `sliderTickSize` | int | For a `QSlider`, the length of a tick, if visible. | `3` |
| `sliderTickSpacing` | int | For a `QSlider`, the spacing between slider and ticks, if visible. | `2` |
| `sliderTickThickness` | int | For a `QSlider`, the thickness of a tick, if visible.| `1` |
| `sliderGrooveHeight` | int | For a `QSlider`, the thickness of the groove (i.e. the highlited zone). | `4` |
| `progressBarGrooveHeight` | int | For a `QProgressBar`, the thickness of the groove (i.e. the highlited zone). | `6` |
| `scrollBarThicknessFull` | int | For a `QScrollBar`, the thickness when the mouse is over. | `12` |
| `scrollBarThicknessSmall` | int | For a `QScrollBar`, the thickness when the mouse is not hover. | `6` |
| `scrollBarMargin` | int | For a `QScrollBar`, the margin between the scrollbar and its parent. | `0` |
| `tabBarPaddingTop` | int | For a `QTabBar`, the space between the top of the bar and the top of the tabs. | `4` |
| `tabBarTabMaxWidth` | int | For a `QTabBar`, the maximum width a tab can have.</br>Note: any value below or equal to `0` will be ignored and treated as if there is no maximum width. | `0` |
| `tabBarTabMinWidth` | int | For a `QTabBar`, the minimum width a tab can have.</br>Note: any value below or equal to `0` will be ignored and treated as if there is no minimum width. | `0` |

## Full example

Here is a full Qlementine theme in all its glory. Please note that every value is optional, and some might be redundant with the default values. Remove/add the key-value pairs you don't need to customize.

```json
{
  "meta": {
    "author": "Olivier Cléro",
    "name": "Light",
    "version": "1.4.0"
  },

  "backgroundColorMain1": "#ffffff",
  "backgroundColorMain2": "#f3f3f3",
  "backgroundColorMain3": "#e3e3e3",
  "backgroundColorMain4": "#dcdcdc",

  "borderColor": "#d3d3d3",
  "borderColorHovered": "#b3b3b3",
  "borderColorPressed": "#a3a3a3",
  "borderColorDisabled": "#e9e9e9",

  "focusColor": "#40a9ff66",

  "neutralColor": "#e1e1e1",
  "neutralColorHovered": "#d9d9d9",
  "neutralColorPressed": "#d2d2d2",
  "neutralColorDisabled": "#eeeeee",

  "primaryAlternativeColor": "#106ef9",
  "primaryAlternativeColorHovered": "#107bfd",
  "primaryAlternativeColorPressed": "#108bfd",
  "primaryAlternativeColorDisabled": "#a9d6ff",

  "primaryColor": "#1890ff",
  "primaryColorHovered": "#2c9dff",
  "primaryColorDisabled": "#d1e9ff",
  "primaryColorPressed": "#40a9ff",

  "primaryColorForeground": "#ffffff",
  "primaryColorForegroundHovered": "#ffffff",
  "primaryColorForegroundPressed": "#ffffff",
  "primaryColorForegroundDisabled": "#ecf6ff",

  "secondaryAlternativeColor": "#909090",
  "secondaryAlternativeColorHovered": "#747474",
  "secondaryAlternativeColorPressed": "#828282",
  "secondaryAlternativeColorDisabled": "#c3c3c3",

  "secondaryColor": "#404040",
  "secondaryColorHovered": "#333333",
  "secondaryColorPressed": "#262626",
  "secondaryColorDisabled": "#d4d4d4",

  "secondaryColorForeground": "#ffffff",
  "secondaryColorForegroundHovered": "#ffffff",
  "secondaryColorForegroundPressed": "#ffffff",
  "secondaryColorForegroundDisabled": "#ededed",

  "semiTransparentColor1": "#0000000a",
  "semiTransparentColor2": "#00000019",
  "semiTransparentColor3": "#00000021",
  "semiTransparentColor4": "#00000028",

  "shadowColor1": "#00000020",
  "shadowColor2": "#00000040",
  "shadowColor3": "#00000060",

  "statusColorForeground": "#ffffff",
  "statusColorForegroundDisabled": "#ffffff99",
  "statusColorForegroundHovered": "#ffffff",
  "statusColorForegroundPressed": "#ffffff",

  "statusColorError": "#e96b72",
  "statusColorErrorHovered": "#f47c83",
  "statusColorErrorPressed": "#ff9197",
  "statusColorErrorDisabled": "#f9dadc",

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
