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

  QColor backgroundColorMainTransparent{ QRgba64::fromArgb32(0x00FAFAFA) };
  QColor backgroundColorMain1{ 0xFAFAFA };
  QColor backgroundColorMain2{ 0xF3F3F5 };
  QColor backgroundColorMain3{ 0xE3E5E7 };

  QColor adaptativeColorTransparent{ QRgba64::fromArgb32(0x00000000) };
  QColor adaptativeColor1{ QRgba64::fromArgb32(0x0A000000) };
  QColor adaptativeColor2{ QRgba64::fromArgb32(0x11000000) };
  QColor adaptativeColor3{ QRgba64::fromArgb32(0x19000000) };
  QColor adaptativeColor4{ QRgba64::fromArgb32(0x21000000) };
  QColor adaptativeColor5{ QRgba64::fromArgb32(0x28000000) };

  QColor focusColor{ QRgba64::fromArgb32(0x66349886) };

  QColor primaryColor{ 0x349886 };
  QColor primaryColorHovered{ 0X2A7A6B };
  QColor primaryColorPressed{ 0X2F8979 };
  QColor primaryColorDisabled{ QRgba64::fromArgb32(0x3F349886) };
  QColor primaryColorTransparent{ QRgba64::fromArgb32(0x00349886) };

  QColor primaryColorForeground{ 0xFFFFFF };
  QColor primaryColorForegroundHovered{ 0xFFFFFF };
  QColor primaryColorForegroundPressed{ 0xFFFFFF };
  QColor primaryColorForegroundDisabled{ QRgba64::fromArgb32(0x99FFFFFF) };
  QColor primaryColorForegroundTransparent{ QRgba64::fromArgb32(0x00FFFFFF) };

  QColor neutralColor{ 0x404040 };
  QColor neutralColorHovered{ 0x333333 };
  QColor neutralColorPressed{ 0x262626 };
  QColor neutralColorDisabled{ QRgba64::fromArgb32(0x33404040) };
  QColor neutralColorTransparent{ QRgba64::fromArgb32(0x00404040) };

  QColor neutralAlternativeColor{ 0x909090 };
  QColor neutralAlternativeColorHovered{ 0x747474 };
  QColor neutralAlternativeColorPressed{ 0x828282 };
  QColor neutralAlternativeColorDisabled{ QRgba64::fromArgb32(0x3F909090) };
  QColor neutralAlternativeColorTransparent{ QRgba64::fromArgb32(0x00909090) };

  QColor neutralColorForeground{ 0xFFFFFF };
  QColor neutralColorForegroundHovered{ 0xFFFFFF };
  QColor neutralColorForegroundPressed{ 0xFFFFFF };
  QColor neutralColorForegroundDisabled{ 0x99FFFFFF };
  QColor neutralColorForegroundTransparent{ QRgba64::fromArgb32(0x00FFFFFF) };

  QColor statusColorSuccess{ 0x21AF73 };
  QColor statusColorSuccessHovered{ 0x1A8D5D };
  QColor statusColorSuccessPressed{ 0x1E9E68 };
  QColor statusColorSuccessDisabled{ QRgba64::fromArgb32(0x3F21b074) };
  QColor statusColorInfo{ 0x1BA8D5 };
  QColor statusColorInfoHovered{ 0x1686AA };
  QColor statusColorInfoPressed{ 0X1897C0 };
  QColor statusColorInfoDisabled{ QRgba64::fromArgb32(0x331BA8D5) };
  QColor statusColorWarning{ 0xF1C861 };
  QColor statusColorWarningHovered{ 0xD9B457 };
  QColor statusColorWarningPressed{ 0xE5BE5C };
  QColor statusColorWarningDisabled{ QRgba64::fromArgb32(0x4CF1C861) };
  QColor statusColorError{ 0xDF535B };
  QColor statusColorErrorHovered{ 0xB24249 };
  QColor statusColorErrorPressed{ 0xC94B52 };
  QColor statusColorErrorDisabled{ QRgba64::fromArgb32(0x33DF535B) };
  QColor statusColorForeground{ 0xFFFFFF };
  QColor statusColorForegroundHovered{ 0xFFFFFF };
  QColor statusColorForegroundPressed{ 0xFFFFFF };
  QColor statusColorForegroundDisabled{ QRgba64::fromArgb32(0x99FFFFFF) };

  QColor shadowColor1{ QRgba64::fromArgb32(0X20000000) };
  QColor shadowColor2{ QRgba64::fromArgb32(0X40000000) };
  QColor shadowColor3{ QRgba64::fromArgb32(0X60000000) };
  QColor shadowColorTransparent{ QRgba64::fromArgb32(0x00000000) };

  QColor borderColor1{ QRgba64::fromArgb32(0x07000000) };
  QColor borderColor2{ QRgba64::fromArgb32(0x0F000000) };
  QColor borderColor3{ QRgba64::fromArgb32(0x1E000000) };
  QColor borderColorTransparent{ QRgba64::fromArgb32(0x00000000) };

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
  int scrollBarThicknessFull{ 10 };
  int scrollBarThicknessSmall{ 4 };
  int scrollBarPadding{ 2 };
  int tabBarPaddingTop{ 4 };

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
  virtual void initializePalette();
  void initializeFromJson(QJsonDocument const& jsonDoc);
};
} // namespace oclero::qlementine
