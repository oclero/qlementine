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

#pragma once

#include <oclero/qlementine/Common.hpp>

#include <QString>
#include <QRgb>
#include <QJsonDocument>
#include <QColor>
#include <QFont>
#include <QSize>
#include <QPalette>

namespace oclero::qlementine {
/// Metadata about a Qlementine Theme.
struct ThemeMeta {
  QString name;
  QString version;
  QString author;

  inline bool operator==(const ThemeMeta& other) const {
    return name == other.name && version == other.version && author == other.author;
  }
  inline bool operator!=(const ThemeMeta& other) const {
    return !(*this == other);
  }
};

/// Color and sizes configuration for a Qlementine Theme.
class Theme {
public: // Ctor.
  Theme();
  Theme(Theme const& other) = default;
  Theme(Theme&& other) = default;
  explicit Theme(QJsonDocument const& jsonDoc);
  Theme(QString const& jsonPath);
  virtual ~Theme() = default;

public: // Operators.
  Theme& operator=(Theme const& other) = default;
  Theme& operator=(Theme&& other) = default;
  bool operator==(const Theme& other) const;
  bool operator!=(const Theme& other) const;

public: // Values.
  ThemeMeta meta;

  QColor backgroundColorMain1{ 0xFFFFFF };
  QColor backgroundColorMain2{ 0xF3F3F3 };
  QColor backgroundColorMain3{ 0xE3E3E3 };
  QColor backgroundColorMain4{ 0xDCDCDC };
  QColor backgroundColorMainTransparent{ QRgba64::fromArgb32(0x00FAFAFA) };

  QColor neutralColor{ 0xE1E1E1 };
  QColor neutralColorHovered{ 0xDADADA };
  QColor neutralColorPressed{ 0xD2D2D2 };
  QColor neutralColorDisabled{ 0xEEEEEE };
  QColor neutralColorTransparent{ QRgba64::fromArgb32(0x00E1E1E1) };

  QColor focusColor{ QRgba64::fromArgb32(0xa9ff6640) };

  QColor primaryColor{ 0x1890FF };
  QColor primaryColorHovered{ 0x2C9DFF };
  QColor primaryColorPressed{ 0x40A9FF };
  QColor primaryColorDisabled{ 0xD1E9FF };
  QColor primaryColorTransparent{ QRgba64::fromArgb32(0x001890FF) };

  QColor primaryColorForeground{ 0xFFFFFF };
  QColor primaryColorForegroundHovered{ 0xFFFFFF };
  QColor primaryColorForegroundPressed{ 0xFFFFFF };
  QColor primaryColorForegroundDisabled{ 0xECF6FF };
  QColor primaryColorForegroundTransparent{ QRgba64::fromArgb32(0x00FFFFFF) };

  QColor primaryAlternativeColor{ 0x106EF9 };
  QColor primaryAlternativeColorHovered{ 0x0F7BFD };
  QColor primaryAlternativeColorPressed{ 0x0F8BFD };
  QColor primaryAlternativeColorDisabled{ 0xa9d6ff };
  QColor primaryAlternativeColorTransparent{ QRgba64::fromArgb32(0x001875ff) };

  QColor secondaryColor{ 0x404040 };
  QColor secondaryColorHovered{ 0x333333 };
  QColor secondaryColorPressed{ 0x262626 };
  QColor secondaryColorDisabled{ 0xD4D4D4 };
  QColor secondaryColorTransparent{ QRgba64::fromArgb32(0x00404040) };

  QColor secondaryColorForeground{ 0xFFFFFF };
  QColor secondaryColorForegroundHovered{ 0xFFFFFF };
  QColor secondaryColorForegroundPressed{ 0xFFFFFF };
  QColor secondaryColorForegroundDisabled{ 0xEDEDED };
  QColor secondaryColorForegroundTransparent{ QRgba64::fromArgb32(0x00FFFFFF) };

  QColor secondaryAlternativeColor{ 0x909090 };
  QColor secondaryAlternativeColorHovered{ 0x747474 };
  QColor secondaryAlternativeColorPressed{ 0x828282 };
  QColor secondaryAlternativeColorDisabled{ 0xC3C3C3 };
  QColor secondaryAlternativeColorTransparent{ QRgba64::fromArgb32(0x00909090) };

  QColor statusColorSuccess{ 0x21AF73 };
  QColor statusColorSuccessHovered{ 0x1A8D5D };
  QColor statusColorSuccessPressed{ 0x1E9E68 };
  QColor statusColorSuccessDisabled{ 0xC4E7D8 };
  QColor statusColorInfo{ 0x1BA8D5 };
  QColor statusColorInfoHovered{ 0x1686AA };
  QColor statusColorInfoPressed{ 0x1897C0 };
  QColor statusColorInfoDisabled{ 0xcde9f2 };
  QColor statusColorWarning{ 0xF1C861 };
  QColor statusColorWarningHovered{ 0xD9B457 };
  QColor statusColorWarningPressed{ 0xE5BE5C };
  QColor statusColorWarningDisabled{ 0xF7EBCC };
  QColor statusColorError{ 0xDF535B };
  QColor statusColorErrorHovered{ 0xB24249 };
  QColor statusColorErrorPressed{ 0xC94B52 };
  QColor statusColorErrorDisabled{ 0xF4D8DA };
  QColor statusColorForeground{ 0xFFFFFF };
  QColor statusColorForegroundHovered{ 0xFFFFFF };
  QColor statusColorForegroundPressed{ 0xFFFFFF };
  QColor statusColorForegroundDisabled{ QRgba64::fromArgb32(0x99FFFFFF) };

  QColor shadowColor1{ QRgba64::fromArgb32(0x20000000) };
  QColor shadowColor2{ QRgba64::fromArgb32(0x40000000) };
  QColor shadowColor3{ QRgba64::fromArgb32(0x60000000) };
  QColor shadowColorTransparent{ QRgba64::fromArgb32(0x00000000) };

  QColor borderColor{ 0xD3D3D3 };
  QColor borderColorHovered{ 0xB3B3B3 };
  QColor borderColorPressed{ 0XA3A3A3 };
  QColor borderColorDisabled{ 0xE9E9E9 };
  QColor borderColorTransparent{ QRgba64::fromArgb32(0x00D3D3D3) };

  QColor semiTransparentColor1{ QRgba64::fromArgb32(0x0A000000) };
  QColor semiTransparentColor2{ QRgba64::fromArgb32(0x19000000) };
  QColor semiTransparentColor3{ QRgba64::fromArgb32(0x21000000) };
  QColor semiTransparentColor4{ QRgba64::fromArgb32(0x28000000) };
  QColor semiTransparentColorTransparent{ QRgba64::fromArgb32(0x00000000) };

  int fontSize{ 12 };
  int fontSizeMonospace{ 13 };
  int fontSizeH1{ 34 };
  int fontSizeH2{ 26 };
  int fontSizeH3{ 22 };
  int fontSizeH4{ 18 };
  int fontSizeH5{ 14 };
  int fontSizeS1{ 10 };
  int animationDuration{ 192 };
  int focusAnimationDuration{ 384 };
  int sliderAnimationDuration{ 96 };
  double borderRadius{ 6. };
  double checkBoxBorderRadius{ 4. };
  double menuItemBorderRadius{ 4. };
  double menuBarItemBorderRadius{ 2. };
  int borderWidth{ 1 };
  int controlHeightLarge{ 28 };
  int controlHeightMedium{ 24 };
  int controlHeightSmall{ 16 };
  int controlDefaultWidth{ 96 };
  int dialMarkLength{ 8 };
  int dialMarkThickness{ 2 };
  int dialTickLength{ 4 };
  int dialTickSpacing{ 4 };
  int dialGrooveThickness{ 4 };
  int focusBorderWidth{ 2 };
  QSize iconSize{ 16, 16 };
  QSize iconSizeMedium{ 24, 24 };
  QSize iconSizeLarge{ 24, 24 };
  QSize iconSizeExtraSmall{ 12, 12 };
  int sliderTickSize{ 3 };
  int sliderTickSpacing{ 2 };
  int sliderTickThickness{ 1 };
  int sliderGrooveHeight{ 4 };
  int progressBarGrooveHeight{ 6 };
  int spacing{ 8 };
  int scrollBarThicknessFull{ 12 };
  int scrollBarThicknessSmall{ 6 };
  int scrollBarMargin{ 0 };
  int tabBarPaddingTop{ 4 };
  int tabBarTabMaxWidth{ 0 };
  int tabBarTabMinWidth{ 0 };

  QFont fontRegular;
  QFont fontBold;

  QFont fontH1;
  QFont fontH2;
  QFont fontH3;
  QFont fontH4;
  QFont fontH5;
  QFont fontCaption;
  QFont fontMonospace;

  QPalette palette;

public:
  QJsonDocument toJson() const;

private:
  void initializeFonts();
  void initializePalette();
  void initializeFromJson(QJsonDocument const& jsonDoc);
};
} // namespace oclero::qlementine
