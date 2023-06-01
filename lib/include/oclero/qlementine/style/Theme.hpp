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

  QColor backgroundColorMainTransparent{ QRgba64::fromArgb32(0x00FFFFFF) };
  QColor backgroundColorMain1{ 0xFFFFFF };
  QColor backgroundColorMain2{ 0xF6F7F8 };
  QColor backgroundColorMain3{ 0xEEF0F2 };

  QColor adaptativeColorTransparent{ QRgba64::fromArgb32(0x00000000) };
  QColor adaptativeColor1{ QRgba64::fromArgb32(0x0A000000) };
  QColor adaptativeColor2{ QRgba64::fromArgb32(0x12000000) };
  QColor adaptativeColor3{ QRgba64::fromArgb32(0x1A000000) };
  QColor adaptativeColor4{ QRgba64::fromArgb32(0x21000000) };
  QColor adaptativeColor5{ QRgba64::fromArgb32(0x29000000) };

  QColor focusColor{ QRgba64::fromArgb32(0x6600C4A1) };

  QColor primaryColor{ 0x0CA277 };
  QColor primaryColorHovered{ 0x3DB592 };
  QColor primaryColorPressed{ 0x6DC7AD };
  QColor primaryColorDisabled{ QRgba64::fromArgb32(0x360CA277) };
  QColor primaryColorTransparent{ QRgba64::fromArgb32(0x000CA277) };

  QColor primaryColorForeground{ 0xFFFFFF };
  QColor primaryColorForegroundHovered{ 0xFFFFFF };
  QColor primaryColorForegroundPressed{ 0xFFFFFF };
  QColor primaryColorForegroundDisabled{ QRgba64::fromArgb32(0x7FFFFFFF) };
  QColor primaryColorForegroundTransparent{ QRgba64::fromArgb32(0x00FFFFFF) };

  QColor neutralColor{ 0x414141 };
  QColor neutralColorHovered{ 0x545454 };
  QColor neutralColorPressed{ 0x676767 };
  QColor neutralColorDisabled{ QRgba64::fromArgb32(0x33414141) };
  QColor neutralColorTransparent{ QRgba64::fromArgb32(0x00414141) };

  QColor neutralAlternativeColor{ 0x919191 };
  QColor neutralAlternativeColorHovered{ 0x9B9B9B };
  QColor neutralAlternativeColorPressed{ 0xA6A6A6c };
  QColor neutralAlternativeColorDisabled{ QRgba64::fromArgb32(0x40909090) };
  QColor neutralAlternativeColorTransparent{ QRgba64::fromArgb32(0x00919191) };

  QColor neutralColorForeground{ 0xFFFFFF };
  QColor neutralColorForegroundHovered{ 0xFFFFFF };
  QColor neutralColorForegroundPressed{ 0xFFFFFF };
  QColor neutralColorForegroundDisabled{ 0xF6F6F6 };
  QColor neutralColorForegroundTransparent{ QRgba64::fromArgb32(0x00F6F6F6) };

  QColor statusColorSuccess{ 0x2BA764 };
  QColor statusColorSuccessHovered{ 0x40B073 };
  QColor statusColorSuccessPressed{ 0x55B983 };
  QColor statusColorSuccessDisabled{ QRgba64::fromArgb32(0x192BA764) };
  QColor statusColorInfo{ 0x1790C4 };
  QColor statusColorInfoHovered{ 0x2E9BCA };
  QColor statusColorInfoPressed{ 0x45A6D0 };
  QColor statusColorInfoDisabled{ QRgba64::fromArgb32(0x191790C4) };
  QColor statusColorWarning{ 0xF3BF07 };
  QColor statusColorWarningHovered{ 0xF7DD92 };
  QColor statusColorWarningPressed{ 0xF8E2A1 };
  QColor statusColorWarningDisabled{ QRgba64::fromArgb32(0x19F3BF07) };
  QColor statusColorError{ 0xE23D3D };
  QColor statusColorErrorHovered{ 0xE55050 };
  QColor statusColorErrorPressed{ 0xE86464 };
  QColor statusColorErrorDisabled{ QRgba64::fromArgb32(0x19E23D3D) };
  QColor statusColorForeground{ 0xFFFFFF };
  QColor statusColorForegroundHovered{ 0xFFFFFF };
  QColor statusColorForegroundPressed{ 0xFFFFFF };
  QColor statusColorForegroundDisabled{ QRgba64::fromArgb32(0x19FFFFFF) };

  QColor shadowColor1{ QRgba64::fromArgb32(0x0e000000) };
  QColor shadowColor2{ QRgba64::fromArgb32(0x0c000000) };
  QColor shadowColor3{ QRgba64::fromArgb32(0x21000000) };
  QColor shadowColorTransparent{ Qt::transparent };

  QColor borderColor1{ QRgba64::fromArgb32(0x08000000) };
  QColor borderColor2{ QRgba64::fromArgb32(0x0F000000) };
  QColor borderColor3{ QRgba64::fromArgb32(0x1F000000) };
  QColor borderColorTransparent{ Qt::transparent };

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

private:
  virtual void initializePalette();
  void initializeFromJson(QJsonDocument const& jsonDoc);
};
} // namespace oclero::qlementine
