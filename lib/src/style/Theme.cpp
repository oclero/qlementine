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

#include <oclero/qlementine/style/Theme.hpp>
#include <oclero/qlementine/utils/PrimitiveUtils.hpp>
#include <oclero/qlementine/utils/FontUtils.hpp>
#include <oclero/qlementine/utils/ColorUtils.hpp>

#include <QGuiApplication>
#include <QJsonObject>
#include <QFile>
#include <QScreen>
#include <QColor>

#include <optional>

namespace oclero::qlementine {
namespace {
#define TRY_SET_COLOR_ATTRIBUTE(JSON_OBJ, NAME) trySetColor(JSON_OBJ, QStringLiteral(#NAME), NAME)
#define TRY_SET_INT_ATTRIBUTE(JSON_OBJ, NAME) trySetInt(JSON_OBJ, QStringLiteral(#NAME), NAME)
#define TRY_SET_DOUBLE_ATTRIBUTE(JSON_OBJ, NAME) trySetDouble(JSON_OBJ, QStringLiteral(#NAME), NAME)

std::optional<QColor> tryGetColor(QJsonObject const& jsonObj, QString const& key, int maxRecursiveCalls = 1) {
  if (jsonObj.contains(key)) {
    const auto variant = jsonObj.value(key).toVariant();

    // Try parse a QColor.
    const auto color = tryGetColorFromVariant(variant);
    if (color.has_value()) {
      return color.value();
    }

    // Check if the value is a reference to another value.
    if (variant.type() == QVariant::String && maxRecursiveCalls > 0) {
      const auto variantString = variant.toString();
      if (variantString != key) {
        return tryGetColor(jsonObj, variantString, --maxRecursiveCalls);
      }
    }
  }

  return {};
}

void trySetColor(QJsonObject const& jsonObj, QString const& key, QColor& target) {
  if (const auto opt = tryGetColor(jsonObj, key)) {
    target = opt.value();
  }
}

QString tryGetString(QJsonObject const& jsonObj, QString const& key, QString const& defaultValue) {
  if (jsonObj.contains(key)) {
    const auto variant = jsonObj.value(key).toVariant();
    if (variant.isValid() && variant.canConvert<QString>()) {
      return variant.toString();
    }
  }
  return defaultValue;
}

std::optional<int> tryGetInt(QJsonObject const& jsonObj, QString const& key, int maxRecursiveCalls = 1) {
  if (jsonObj.contains(key)) {
    const auto variant = jsonObj.value(key).toVariant();

    // Check if the value is a reference to another value.
    if (variant.type() == QVariant::String && maxRecursiveCalls > 0) {
      const auto variantString = variant.toString();
      if (variantString != key) {
        return tryGetInt(jsonObj, variantString, --maxRecursiveCalls);
      }
    }

    // Try parse an int.
    if (variant.isValid() && variant.canConvert<int>()) {
      return variant.toInt();
    }
  }
  return {};
}

std::optional<double> tryGetDouble(QJsonObject const& jsonObj, QString const& key, int maxRecursiveCalls = 1) {
  if (jsonObj.contains(key)) {
    const auto variant = jsonObj.value(key).toVariant();

    // Check if the value is a reference to another value.
    if (variant.type() == QVariant::String && maxRecursiveCalls > 0) {
      const auto variantString = variant.toString();
      if (variantString != key) {
        return tryGetDouble(jsonObj, variantString, --maxRecursiveCalls);
      }
    }

    // Try parse an int.
    if (variant.isValid() && variant.canConvert<double>()) {
      return variant.toDouble();
    }
  }
  return {};
}

void trySetInt(QJsonObject const& jsonObj, QString const& key, int& target) {
  if (const auto opt = tryGetInt(jsonObj, key)) {
    target = opt.value();
  }
}

void trySetDouble(QJsonObject const& jsonObj, QString const& key, double& target) {
  if (const auto opt = tryGetDouble(jsonObj, key)) {
    target = opt.value();
  }
}

QJsonDocument readJsonDoc(QString const& jsonPath) {
  QFile jsonFile(jsonPath);
  if (jsonFile.open(QIODevice::ReadOnly)) {
    const auto fileContents = jsonFile.readAll();
    QJsonParseError jsonParseError;
    const auto jsonDoc = QJsonDocument::fromJson(fileContents, &jsonParseError);
    if (jsonParseError.error == QJsonParseError::ParseError::NoError && !jsonDoc.isEmpty()) {
      return jsonDoc;
    }
  }

  return {};
}

} // namespace

Theme::Theme() {
  initializePalette();
}

Theme::Theme(QJsonDocument const& jsonDoc) {
  initializeFromJson(jsonDoc);
  initializePalette();
}

Theme::Theme(QString const& jsonPath)
  : Theme(readJsonDoc(jsonPath)) {}

void Theme::initializePalette() {
  // Shades.
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Window, backgroundColorMain1);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Dark, backgroundColorMain3);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Mid, backgroundColorMain3);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Midlight, backgroundColorMain2);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Light, backgroundColorMain2);

  // ItemViews.
  // Compute color without alpha, to avoid color blending when the QTreeView is animating.
  const auto itemViewBase = getColorSourceOver(backgroundColorMain1, adaptativeColor2);
  const auto itemViewAlternate =
    getColorSourceOver(itemViewBase, colorWithAlpha(adaptativeColor1, adaptativeColor1.alpha() / 2));
  const auto itemViewDisabled = getColorSourceOver(backgroundColorMain1, adaptativeColor1);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Base, itemViewBase);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, itemViewDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::AlternateBase, itemViewAlternate);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, itemViewDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::NoRole, backgroundColorMainTransparent);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::NoRole, backgroundColorMainTransparent);

  // Tooltips.
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::ToolTipBase, neutralColor);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::ToolTipText, neutralColorForeground);

  // Highlight.
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Highlight, primaryColor);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Highlight, primaryColorDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::HighlightedText, primaryColorForeground);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::HighlightedText, primaryColorDisabled);

  // Text.
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Text, neutralColor);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, neutralColorDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::WindowText, neutralColor);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, neutralColorDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::PlaceholderText, neutralColorDisabled);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, adaptativeColor1);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Link, primaryColor);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Link, neutralColorDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::LinkVisited, primaryColor);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::LinkVisited, neutralColorDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::BrightText, neutralAlternativeColor);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, neutralAlternativeColorDisabled);

  // Buttons.
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::ButtonText, neutralColorForeground);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, neutralColorForegroundDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Button, adaptativeColor3);
  palette.setColor(QPalette::ColorGroup::Normal, QPalette::ColorRole::Button, adaptativeColor3);
  palette.setColor(QPalette::ColorGroup::Current, QPalette::ColorRole::Button, adaptativeColor4);
  palette.setColor(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, adaptativeColor5);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, adaptativeColor1);

  //palette.setColor(QPalette::ColorGroup::, QPalette::ColorRole::Background, backgroundColor);
}

void Theme::initializeFromJson(QJsonDocument const& jsonDoc) {
  if (jsonDoc.isObject()) {
    const auto jsonObj = jsonDoc.object();
    if (!jsonObj.isEmpty()) {
      // Parse metadata.
      auto const metaObj = jsonObj.value(QStringLiteral("meta")).toObject();
      meta = ThemeMeta{
        tryGetString(metaObj, QStringLiteral("name"), {}),
        tryGetString(metaObj, QStringLiteral("version"), {}),
        tryGetString(metaObj, QStringLiteral("author"), {}),
      };

      // Parse all values.
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, backgroundColorMain1);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, backgroundColorMain2);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, backgroundColorMain3);
      backgroundColorMainTransparent = colorWithAlpha(backgroundColorMain1, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, adaptativeColor1);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, adaptativeColor2);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, adaptativeColor3);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, adaptativeColor4);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, adaptativeColor5);
      adaptativeColorTransparent = colorWithAlpha(adaptativeColor1, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, focusColor);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColor);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColorHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColorPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColorDisabled);
      primaryColorTransparent = colorWithAlpha(primaryColor, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColorForeground);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColorForegroundHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColorForegroundPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, primaryColorForegroundDisabled);
      primaryColorForegroundTransparent = colorWithAlpha(primaryColorForeground, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColor);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColorHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColorPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColorDisabled);
      neutralColorTransparent = colorWithAlpha(neutralColor, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralAlternativeColor);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralAlternativeColorHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralAlternativeColorPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralAlternativeColorDisabled);
      neutralAlternativeColorTransparent = colorWithAlpha(neutralAlternativeColor, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColorForeground);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColorForegroundHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColorForegroundPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, neutralColorForegroundDisabled);
      neutralColorForegroundTransparent = colorWithAlpha(neutralColorForeground, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorSuccess);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorSuccessHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorSuccessPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorSuccessDisabled);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorInfo);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorInfoHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorInfoPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorInfoDisabled);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorWarning);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorWarningHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorWarningPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorWarningDisabled);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorError);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorErrorHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorErrorPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorErrorDisabled);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorForeground);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorForegroundHovered);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorForegroundPressed);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, statusColorForegroundDisabled);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, borderColor1);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, borderColor2);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, borderColor3);
      borderColorTransparent = colorWithAlpha(borderColor1, 0);

      TRY_SET_COLOR_ATTRIBUTE(jsonObj, shadowColor1);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, shadowColor2);
      TRY_SET_COLOR_ATTRIBUTE(jsonObj, shadowColor3);
      shadowColorTransparent = colorWithAlpha(shadowColor1, 0);

      TRY_SET_INT_ATTRIBUTE(jsonObj, fontSize);
      TRY_SET_INT_ATTRIBUTE(jsonObj, fontSizeH1);
      TRY_SET_INT_ATTRIBUTE(jsonObj, fontSizeH2);
      TRY_SET_INT_ATTRIBUTE(jsonObj, fontSizeH3);
      TRY_SET_INT_ATTRIBUTE(jsonObj, fontSizeH4);
      TRY_SET_INT_ATTRIBUTE(jsonObj, fontSizeH5);
      TRY_SET_INT_ATTRIBUTE(jsonObj, fontSizeS1);
      TRY_SET_INT_ATTRIBUTE(jsonObj, animationDuration);
      TRY_SET_INT_ATTRIBUTE(jsonObj, focusAnimationDuration);
      TRY_SET_INT_ATTRIBUTE(jsonObj, sliderAnimationDuration);
      TRY_SET_DOUBLE_ATTRIBUTE(jsonObj, borderRadius);
      TRY_SET_DOUBLE_ATTRIBUTE(jsonObj, checkBoxBorderRadius);
      TRY_SET_DOUBLE_ATTRIBUTE(jsonObj, menuItemBorderRadius);
      TRY_SET_DOUBLE_ATTRIBUTE(jsonObj, menuBarItemBorderRadius);
      TRY_SET_INT_ATTRIBUTE(jsonObj, borderWidth);
      TRY_SET_INT_ATTRIBUTE(jsonObj, controlHeightLarge);
      TRY_SET_INT_ATTRIBUTE(jsonObj, controlHeightMedium);
      TRY_SET_INT_ATTRIBUTE(jsonObj, controlHeightSmall);
      TRY_SET_INT_ATTRIBUTE(jsonObj, controlDefaultWidth);
      TRY_SET_INT_ATTRIBUTE(jsonObj, dialMarkLength);
      TRY_SET_INT_ATTRIBUTE(jsonObj, dialMarkThickness);
      TRY_SET_INT_ATTRIBUTE(jsonObj, dialTickLength);
      TRY_SET_INT_ATTRIBUTE(jsonObj, dialTickSpacing);
      TRY_SET_INT_ATTRIBUTE(jsonObj, dialGrooveThickness);
      TRY_SET_INT_ATTRIBUTE(jsonObj, focusBorderWidth);

      auto iconExtent = 16;
      TRY_SET_INT_ATTRIBUTE(jsonObj, iconExtent);
      iconSize = QSize{ iconExtent, iconExtent };
      iconSizeMedium = iconSize * 1.5;
      iconSizeLarge = iconSize * 2;
      iconSizeExtraSmall = iconSize * 0.75;

      TRY_SET_INT_ATTRIBUTE(jsonObj, sliderTickSize);
      TRY_SET_INT_ATTRIBUTE(jsonObj, sliderTickSpacing);
      TRY_SET_INT_ATTRIBUTE(jsonObj, sliderTickThickness);
      TRY_SET_INT_ATTRIBUTE(jsonObj, sliderGrooveHeight);
      TRY_SET_INT_ATTRIBUTE(jsonObj, progressBarGrooveHeight);
      TRY_SET_INT_ATTRIBUTE(jsonObj, spacing);
      TRY_SET_INT_ATTRIBUTE(jsonObj, scrollBarThicknessFull);
      TRY_SET_INT_ATTRIBUTE(jsonObj, scrollBarThicknessSmall);
      TRY_SET_INT_ATTRIBUTE(jsonObj, scrollBarPadding);
      TRY_SET_INT_ATTRIBUTE(jsonObj, tabBarPaddingTop);

      // Fonts.
      const auto defaultFont = QFont(QStringLiteral("Inter"));
      const auto fixedFont = QFont(QStringLiteral("Roboto Mono"));
      const auto dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
      fontRegular = defaultFont;
      fontRegular.setWeight(QFont::Weight::Normal);
      fontRegular.setPointSizeF(pixelSizeToPointSize(fontSize, dpi));

      fontBold = defaultFont;
      fontBold.setWeight(QFont::Weight::Bold);
      fontBold.setPointSizeF(pixelSizeToPointSize(fontSize, dpi));

      fontH1 = defaultFont;
      fontH1.setWeight(QFont::Weight::Bold);
      fontH1.setPointSizeF(pixelSizeToPointSize(fontSizeH1, dpi));

      fontH2 = defaultFont;
      fontH2.setWeight(QFont::Weight::Bold);
      fontH2.setPointSizeF(pixelSizeToPointSize(fontSizeH2, dpi));

      fontH3 = defaultFont;
      fontH3.setWeight(QFont::Weight::Bold);
      fontH3.setPointSizeF(pixelSizeToPointSize(fontSizeH3, dpi));

      fontH4 = defaultFont;
      fontH4.setWeight(QFont::Weight::Bold);
      fontH4.setPointSizeF(pixelSizeToPointSize(fontSizeH4, dpi));

      fontH5 = defaultFont;
      fontH5.setWeight(QFont::Weight::Bold);
      fontH5.setPointSizeF(pixelSizeToPointSize(fontSizeH5, dpi));

      fontCaption = defaultFont;
      fontCaption.setWeight(QFont::Weight::Normal);
      fontCaption.setPointSizeF(pixelSizeToPointSize(fontSizeS1, dpi));

      fontMonospace = fixedFont;
      fontMonospace.setWeight(QFont::Weight::Normal);
      fontMonospace.setPointSizeF(pixelSizeToPointSize(fontSizeMonospace, dpi));
    }
  }
}

QJsonDocument Theme::toJson() const {
  QJsonObject jsonObj;

  jsonObj.insert("adaptativeColor1", toHexRGBA(adaptativeColor1));
  jsonObj.insert("adaptativeColor2", toHexRGBA(adaptativeColor2));

  // Metadata.
  QJsonObject metadataObj;
  metadataObj.insert("name", meta.name);
  metadataObj.insert("version", meta.version);
  metadataObj.insert("author", meta.author);
  jsonObj.insert("meta", metadataObj);

  QJsonDocument jsonDoc;
  jsonDoc.setObject(jsonObj);
  return jsonDoc;
}

#pragma endregion

#pragma region Operators

bool Theme::operator==(const Theme& other) const {
  // All generated values are not used in this equality test, on purpose.
  // TODO Remove this and instead use spaceship operator when C++20 is available on macOS/XCode.
  // clang-format off
  return meta == other.meta
    && backgroundColorMain1 == other.backgroundColorMain1
    && backgroundColorMain2 == other.backgroundColorMain2
    && backgroundColorMain3 == other.backgroundColorMain3

    && adaptativeColor1 == other.adaptativeColor1
    && adaptativeColor2 == other.adaptativeColor2
    && adaptativeColor3 == other.adaptativeColor3
    && adaptativeColor4 == other.adaptativeColor4
    && adaptativeColor5 == other.adaptativeColor5

    && focusColor == other.focusColor

    && primaryColor == other.primaryColor
    && primaryColorHovered == other.primaryColorHovered
    && primaryColorPressed == other.primaryColorPressed
    && primaryColorDisabled == other.primaryColorDisabled

    && primaryColorForeground == other.primaryColorForeground
    && primaryColorForegroundHovered == other.primaryColorForegroundHovered
    && primaryColorForegroundPressed == other.primaryColorForegroundPressed
    && primaryColorForegroundDisabled == other.primaryColorForegroundDisabled

    && neutralColor == other.neutralColor
    && neutralColorHovered == other.neutralColorHovered
    && neutralColorPressed == other.neutralColorPressed
    && neutralColorDisabled == other.neutralColorDisabled

    && neutralAlternativeColor == other.neutralAlternativeColor
    && neutralAlternativeColorHovered == other.neutralAlternativeColorHovered
    && neutralAlternativeColorPressed == other.neutralAlternativeColorPressed
    && neutralAlternativeColorDisabled == other.neutralAlternativeColorDisabled

    && neutralColorForeground == other.neutralColorForeground
    && neutralColorForegroundHovered == other.neutralColorForegroundHovered
    && neutralColorForegroundPressed == other.neutralColorForegroundPressed
    && neutralColorForegroundDisabled == other.neutralColorForegroundDisabled

    && statusColorSuccess == other.statusColorSuccess
    && statusColorSuccessHovered == other.statusColorSuccessHovered
    && statusColorSuccessPressed == other.statusColorSuccessPressed
    && statusColorSuccessDisabled == other.statusColorSuccessDisabled
    && statusColorInfo == other.statusColorInfo
    && statusColorInfoHovered == other.statusColorInfoHovered
    && statusColorInfoPressed == other.statusColorInfoPressed
    && statusColorInfoDisabled == other.statusColorInfoDisabled
    && statusColorWarning == other.statusColorWarning
    && statusColorWarningHovered == other.statusColorWarningHovered
    && statusColorWarningPressed == other.statusColorWarningPressed
    && statusColorWarningDisabled == other.statusColorWarningDisabled
    && statusColorError == other.statusColorError
    && statusColorErrorHovered == other.statusColorErrorHovered
    && statusColorErrorPressed == other.statusColorErrorPressed
    && statusColorErrorDisabled == other.statusColorErrorDisabled
    && statusColorForeground == other.statusColorForeground
    && statusColorForegroundHovered == other.statusColorForegroundHovered
    && statusColorForegroundPressed == other.statusColorForegroundPressed
    && statusColorForegroundDisabled == other.statusColorForegroundDisabled

    && shadowColor1 == other.shadowColor1
    && shadowColor2 == other.shadowColor2
    && shadowColor3 == other.shadowColor3

    && borderColor1 == other.borderColor1
    && borderColor2 == other.borderColor2
    && borderColor3 == other.borderColor3

    && animationDuration == other.animationDuration
    && focusAnimationDuration == other.focusAnimationDuration
    && sliderAnimationDuration == other.sliderAnimationDuration
    && borderRadius == other.borderRadius
    && checkBoxBorderRadius == other.checkBoxBorderRadius
    && menuItemBorderRadius == other.menuItemBorderRadius
    && menuBarItemBorderRadius == other.menuBarItemBorderRadius
    && borderWidth == other.borderWidth
    && controlHeightLarge == other.controlHeightLarge
    && controlHeightMedium == other.controlHeightMedium
    && controlHeightSmall == other.controlHeightSmall
    && controlDefaultWidth == other.controlDefaultWidth
    && dialMarkLength == other.dialMarkLength
    && dialMarkThickness == other.dialMarkThickness
    && dialTickLength == other.dialTickLength
    && dialTickSpacing == other.dialTickSpacing
    && dialGrooveThickness == other.dialGrooveThickness
    && focusBorderWidth == other.focusBorderWidth
    && iconSize == other.iconSize
    && iconSizeMedium == other.iconSizeMedium
    && iconSizeLarge == other.iconSizeLarge
    && iconSizeExtraSmall == other.iconSizeExtraSmall
    && sliderTickSize == other.sliderTickSize
    && sliderTickThickness == other.sliderTickThickness
    && sliderGrooveHeight == other.sliderGrooveHeight
    && progressBarGrooveHeight == other.progressBarGrooveHeight
    && spacing == other.spacing
    && scrollBarThicknessFull == other.scrollBarThicknessFull
    && scrollBarThicknessSmall == other.scrollBarThicknessSmall
    && scrollBarPadding == other.scrollBarPadding
    && tabBarPaddingTop == other.tabBarPaddingTop
    ;
  // clang-format on
}

bool Theme::operator!=(const Theme& other) const {
  return !(*this == other);
}

} // namespace oclero::qlementine
