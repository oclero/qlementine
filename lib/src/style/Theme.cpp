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

// Retrieve red, green, blue and alpha channels from JSON array elements.
std::optional<QColor> tryGetColorFromVariantList(QVariantList const& variantList) {
  const auto variantListSize = variantList.size();
  if (variantListSize == 3 || variantListSize == 4) {
    int r{ 0 }, g{ 0 }, b{ 0 }, a{ 255 };

    const auto& variant_r = variantList.at(0);
    if (variant_r.isValid() && variant_r.canConvert<int>()) {
      r = variant_r.toInt();
    }
    const auto& variant_g = variantList.at(1);
    if (variant_g.isValid() && variant_g.canConvert<int>()) {
      g = variant_g.toInt();
    }
    const auto& variant_b = variantList.at(2);
    if (variant_b.isValid() && variant_b.canConvert<int>()) {
      b = variant_b.toInt();
    }
    if (variantListSize == 4) {
      const auto& variant_a = variantList.at(3);
      if (variant_a.isValid() && variant_a.canConvert<int>()) {
        a = variant_a.toInt();
      }
    }
    return QColor{ r, g, b, a };
  }

  return {};
}

std::optional<QColor> tryGetColorFromRGBAString(QString const& str) {
  // Try parse RGB ("rgb(RRR,GGG,BBB)").
  static const auto rgbRegExp = QRegularExpression(QStringLiteral("^ *rgb *\\( *(\\d{1,3}) *, *(\\d{1,3}) *, *(\\d{1,3}) *\\) *$"), QRegularExpression::CaseInsensitiveOption);
  const auto rgbMatch = rgbRegExp.match(str);
  if (rgbMatch.hasMatch()) {
    const auto r = rgbMatch.captured(1).toInt();
    const auto g = rgbMatch.captured(2).toInt();
    const auto b = rgbMatch.captured(3).toInt();
    return QColor{ r, g, b };
  }

  // Try parse RGBA ("rgba(RRR,GGG,BBB,AAA)").
  static const auto rgbaRegExp = QRegularExpression(QStringLiteral("^ *rgba *\\( *(\\d{1,3}) *, *(\\d{1,3}) *, *(\\d{1,3}) *, *(\\d{1,3})*\\) *$"), QRegularExpression::CaseInsensitiveOption);
  const auto rgbaMatch = rgbaRegExp.match(str);
  if (rgbaMatch.hasMatch()) {
    const auto r = rgbaMatch.captured(1).toInt();
    const auto g = rgbaMatch.captured(2).toInt();
    const auto b = rgbaMatch.captured(3).toInt();
    const auto a = rgbaMatch.captured(4).toInt();
    return QColor{ r, g, b, a };
  }

  return {};
}

std::optional<QColor> tryGetColorFromHexaString(QString const& str) {
  constexpr auto HEX_BASE = 16;
  constexpr auto RGB_LENGTH = 3 * 2 + 1;
  constexpr auto RGBA_LENGTH = 4 * 2 + 1;

  const auto length = str.length();
  if (str.startsWith('#') && (length == RGB_LENGTH || length == RGBA_LENGTH)) {
    auto success{ false };

    const auto r_str = str.midRef(1, 2);
    const auto r = r_str.toInt(&success, HEX_BASE);
    if (success) {
      const auto g_str = str.midRef(3, 2);
      const auto g = g_str.toInt(&success, HEX_BASE);
      if (success) {
        const auto b_str = str.midRef(5, 2);
        const auto b = b_str.toInt(&success, HEX_BASE);
        if (success) {
          QColor result{ r, g, b };

          const auto a_str = str.midRef(7, 2);
          const auto a = a_str.toInt(&success, HEX_BASE);
          if (success) {
            result.setAlpha(a);
          }

          return result;
        }
      }
    }
  }

  return {};
}

std::optional<QColor> tryGetColorFromVariant(QVariant const& variant) {
  if (variant.isValid()) {
    // Channel list ([RRR, GGG, BBB, AAA]).
    if (variant.type() == QVariant::Type::List) {
      const auto color = tryGetColorFromVariantList(variant.toList());
      if (color.has_value()) {
        return color.value();
      }
    }

    // Various ways to write color as a string.
    else if (variant.type() == QVariant::Type::String) {
      const auto variantString = variant.toString();

      // Check if color is written as hexadecimal.
      const auto hexaColor = tryGetColorFromHexaString(variantString);
      if (hexaColor.has_value()) {
        return hexaColor.value();
      }

      // Check if color is written as RGB(A).
      const auto rgbaColor = tryGetColorFromRGBAString(variantString);
      if (rgbaColor.has_value()) {
        return rgbaColor.value();
      }
    }
  }

  return {};
}

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

QColor const& Theme::color(MouseState const mouse, ColorRole const role) const {
  const auto primary = role == ColorRole::Primary;

  switch (mouse) {
    case MouseState::Pressed:
      return primary ? primaryColorPressed : neutralColorPressed;
    case MouseState::Hovered:
      return primary ? primaryColorHovered : neutralColorHovered;
    case MouseState::Disabled:
      return primary ? primaryColorDisabled : neutralColorDisabled;
    case MouseState::Transparent:
      return primary ? primaryColorTransparent : neutralColorTransparent;
    case MouseState::Normal:
    default:
      return primary ? primaryColor : neutralColor;
  }
}

QColor const& Theme::frameBackgroundColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return backgroundColorMainTransparent;
  else
    return backgroundColorMain1;
}

QColor const& Theme::buttonBackgroundColor(MouseState const mouse, ColorRole const role) const {
  const auto primary = role == ColorRole::Primary;

  switch (mouse) {
    case MouseState::Pressed:
      return primary ? primaryColorPressed : adaptativeColor5;
    case MouseState::Hovered:
      return primary ? primaryColorHovered : adaptativeColor4;
    case MouseState::Disabled:
      return primary ? primaryColorDisabled : adaptativeColor1;
    case MouseState::Transparent:
      return primary ? primaryColorTransparent : adaptativeColorTransparent;
    case MouseState::Normal:
    default:
      return primary ? primaryColor : adaptativeColor3;
  }
}

QColor const& Theme::buttonForegroundColor(MouseState const mouse, ColorRole const role) const {
  const auto primary = role == ColorRole::Primary;

  switch (mouse) {
    case MouseState::Pressed:
      return primary ? primaryColorForegroundPressed : neutralColor;
    case MouseState::Hovered:
      return primary ? primaryColorForegroundHovered : neutralColor;
    case MouseState::Disabled:
      return primary ? primaryColorForegroundDisabled : neutralColorDisabled;
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return primary ? primaryColorForeground : neutralColor;
  }
}

QColor const& Theme::toolButtonBackgroundColor(MouseState const mouse, ColorRole const role) const {
  const auto primary = role == ColorRole::Primary;

  switch (mouse) {
    case MouseState::Pressed:
      return primary ? primaryColorPressed : adaptativeColor3;
    case MouseState::Hovered:
      return primary ? primaryColorHovered : adaptativeColor2;
    case MouseState::Disabled:
      return primary ? primaryColorDisabled : adaptativeColorTransparent;
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return primary ? primaryColor : adaptativeColorTransparent;
  }
}

QColor const& Theme::toolButtonForegroundColor(MouseState const mouse, ColorRole const role) const {
  const auto primary = role == ColorRole::Primary;

  switch (mouse) {
    case MouseState::Disabled:
      return primary ? primaryColorForegroundDisabled : neutralColorDisabled;
    default:
      return primary ? primaryColorForeground : neutralColor;
  }
}

QColor const& Theme::toolButtonSeparatorColor(MouseState const mouse, ColorRole const role) const {
  Q_UNUSED(role)
  switch (mouse) {
    case MouseState::Pressed:
      return adaptativeColor5;
    case MouseState::Hovered:
      return adaptativeColor3;
    case MouseState::Normal:
      return adaptativeColor2;
    case MouseState::Disabled:
    default:
      return adaptativeColor1;
  }
}

QColor const& Theme::commandButtonBackgroundColor(MouseState const mouse, ColorRole const role) const {
  return toolButtonBackgroundColor(mouse, role);
}

QColor const& Theme::commandButtonTextColor(MouseState const mouse, ColorRole const role) const {
  return toolButtonForegroundColor(mouse, role);
}

QColor const& Theme::commandButtonDescriptionColor(MouseState const mouse, ColorRole const role) const {
  const auto primary = role == ColorRole::Primary;

  switch (mouse) {
    case MouseState::Disabled:
      return primary ? primaryColorForegroundDisabled : neutralAlternativeColorDisabled;
    default:
      return primary ? primaryColorForeground : neutralAlternativeColor;
  }
}

QColor const& Theme::commandButtonIconColor(MouseState const mouse, ColorRole const role) const {
  return commandButtonDescriptionColor(mouse, role);
}

QColor const& Theme::checkButtonBackgroundColor(MouseState const mouse, CheckState const checked) const {
  switch (checked) {
    case CheckState::Checked:
    case CheckState::Indeterminate:
      return buttonBackgroundColor(mouse, ColorRole::Primary);
    case CheckState::NotChecked:
    default:
      return buttonBackgroundColor(mouse, ColorRole::Neutral);
  }
}

QColor const& Theme::checkButtonForegroundColor(MouseState const mouse, CheckState const checked) const {
  Q_UNUSED(checked)
  return buttonForegroundColor(mouse, ColorRole::Primary);
}

QColor const& Theme::radioButtonBackgroundColor(MouseState const mouse, CheckState const checked) const {
  return checkButtonBackgroundColor(mouse, checked);
}

QColor const& Theme::radioButtonForegroundColor(MouseState const mouse, CheckState const checked) const {
  return checkButtonForegroundColor(mouse, checked);
}

QColor const& Theme::comboBoxBackgroundColor(MouseState const mouse) const {
  return buttonBackgroundColor(mouse, ColorRole::Neutral);
}

QColor const& Theme::comboBoxForegroundColor(MouseState const mouse) const {
  return buttonForegroundColor(mouse, ColorRole::Neutral);
}

QColor const& Theme::spinBoxBackgroundColor(MouseState const mouse) const {
  return textFieldBackgroundColor(mouse);
}

QColor const& Theme::spinBoxBorderColor(MouseState const mouse, FocusState const focus) const {
  return textFieldBorderColor(mouse, focus, Status::Default);
}

QColor const& Theme::spinBoxButtonBackgroundColor(MouseState const mouse) const {
  return buttonBackgroundColor(mouse, ColorRole::Neutral);
}

QColor const& Theme::spinBoxButtonForegroundColor(MouseState const mouse) const {
  return buttonForegroundColor(mouse, ColorRole::Neutral);
}

QColor const& Theme::listItemRowBackgroundColor(MouseState const mouse, AlternateState const alternate) const {
  const auto isAlternate = alternate == AlternateState::Alternate;
  const auto isEnabled = mouse != MouseState::Disabled;
  return palette.color(isEnabled ? QPalette::ColorGroup::Normal : QPalette::ColorGroup::Disabled, isAlternate ? QPalette::ColorRole::AlternateBase : QPalette::ColorRole::Base);
}

QColor const& Theme::listItemBackgroundColor(MouseState const mouse, SelectionState const selected, FocusState const focus, ActiveState const active) const {
  Q_UNUSED(focus)
  const auto isSelected = selected == SelectionState::Selected;
  const auto isActive = active == ActiveState::Active;

  if (isActive) {
    switch (mouse) {
      case MouseState::Pressed:
        return isSelected ? primaryColor : adaptativeColor3;
      case MouseState::Hovered:
        return isSelected ? primaryColor : adaptativeColor1;
      case MouseState::Disabled:
        return isSelected ? primaryColorDisabled : adaptativeColorTransparent;
      case MouseState::Transparent:
      case MouseState::Normal:
      default:
        return isSelected ? primaryColor : adaptativeColorTransparent;
    }
  } else {
    switch (mouse) {
      case MouseState::Pressed:
        return isSelected ? adaptativeColor3 : adaptativeColor3;
      case MouseState::Hovered:
        return isSelected ? adaptativeColor3 : adaptativeColor1;
      case MouseState::Disabled:
        return isSelected ? adaptativeColor1 : adaptativeColorTransparent;
      case MouseState::Transparent:
      case MouseState::Normal:
      default:
        return isSelected ? adaptativeColor3 : adaptativeColorTransparent;
    }
  }
}

QColor const& Theme::listItemForegroundColor(MouseState const mouse, SelectionState const selected, FocusState const focus, ActiveState const active) const {
  Q_UNUSED(focus)
  const auto isSelected = selected == SelectionState::Selected;
  const auto isActive = active == ActiveState::Active;

  if (isActive) {
    switch (mouse) {
      case MouseState::Disabled:
        return isSelected ? primaryColorForegroundDisabled : neutralColorDisabled;
      case MouseState::Hovered:
      case MouseState::Pressed:
      case MouseState::Transparent:
      case MouseState::Normal:
      default:
        return isSelected ? primaryColorForeground : neutralColor;
    }
  } else {
    switch (mouse) {
      case MouseState::Disabled:
        return neutralColorDisabled;
      case MouseState::Hovered:
      case MouseState::Pressed:
      case MouseState::Transparent:
      case MouseState::Normal:
      default:
        return neutralColor;
    }
  }
}

QColor const& Theme::listItemCaptionForegroundColor(MouseState const mouse, SelectionState const selected, FocusState const focus, ActiveState const active) const {
  Q_UNUSED(focus)
  const auto isSelected = selected == SelectionState::Selected;
  const auto isActive = active == ActiveState::Active;

  if (isActive) {
    switch (mouse) {
      case MouseState::Disabled:
        return isSelected ? primaryColorForegroundDisabled : neutralAlternativeColorDisabled;
      case MouseState::Hovered:
      case MouseState::Pressed:
      case MouseState::Transparent:
      case MouseState::Normal:
      default:
        return isSelected ? primaryColorForeground : neutralAlternativeColor;
    }
  } else {
    switch (mouse) {
      case MouseState::Disabled:
        return neutralAlternativeColorDisabled;
      case MouseState::Hovered:
      case MouseState::Pressed:
      case MouseState::Transparent:
      case MouseState::Normal:
      default:
        return neutralAlternativeColor;
    }
  }
}

QColor const& Theme::listItemCheckButtonBackgroundColor(MouseState const mouse, CheckState const checked, SelectionState const selected, ActiveState const active) const {
  Q_UNUSED(active)
  const auto isChecked = checked != CheckState::NotChecked;
  const auto isEnabled = mouse != MouseState::Disabled;
  switch (selected) {
    case SelectionState::Selected:
      if (isEnabled)
        return isChecked ? adaptativeColor5 : adaptativeColor4;
      else
        return isChecked ? adaptativeColor2 : adaptativeColor1;
    case SelectionState::NotSelected:
    default:
      if (isEnabled)
        return isChecked ? primaryColor : adaptativeColor4;
      else
        return isChecked ? primaryColorDisabled : adaptativeColor1;
  }
}

QColor const& Theme::listItemCheckButtonForegroundColor(MouseState const mouse, CheckState const checked, SelectionState const selected, ActiveState const active) const {
  Q_UNUSED(selected)
  Q_UNUSED(active)
  return checkButtonForegroundColor(mouse, checked);
}

QColor const& Theme::cellItemFocusBorderColor(FocusState const focus, SelectionState const selected, ActiveState const active) const {
  Q_UNUSED(active)
  if (selected == SelectionState::Selected)
    return focus == FocusState::Focused ? adaptativeColor5 : adaptativeColorTransparent;
  else
    return focus == FocusState::Focused ? primaryColor : primaryColorTransparent;
}

QColor const& Theme::menuBackgroundColor() const {
  return backgroundColorMain1;
}

QColor const& Theme::menuBorderColor() const {
  return borderColor3;
}

QColor const& Theme::menuSeparatorColor() const {
  return borderColor3;
}

QColor const& Theme::menuItemBackgroundColor(MouseState const mouse) const {
  switch (mouse) {
    case MouseState::Hovered:
      return primaryColor;
    case MouseState::Pressed:
      return primaryColorHovered;
    case MouseState::Disabled:
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return primaryColorTransparent;
  }
}

QColor const& Theme::menuItemForegroundColor(MouseState const mouse) const {
  switch (mouse) {
    case MouseState::Hovered:
      return primaryColorForegroundHovered;
    case MouseState::Pressed:
      return primaryColorForegroundPressed;
    case MouseState::Disabled:
      return neutralColorDisabled;
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return neutralColor;
  }
}

QColor const& Theme::menuItemSecondaryForegroundColor(MouseState const mouse) const {
  switch (mouse) {
    case MouseState::Hovered:
      return primaryColorForegroundHovered;
    case MouseState::Pressed:
      return primaryColorForegroundPressed;
    case MouseState::Disabled:
      return neutralAlternativeColorDisabled;
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return neutralAlternativeColor;
  }
}

QColor const& Theme::menuBarBackgroundColor() const {
  return backgroundColorMain1;
}

QColor const& Theme::menuBarBorderColor() const {
  return borderColor3;
}

QColor const& Theme::menuBarItemBackgroundColor(MouseState const mouse, SelectionState const selected) const {
  Q_UNUSED(selected)
  switch (mouse) {
    case MouseState::Hovered:
      return adaptativeColor1;
    case MouseState::Pressed:
      return adaptativeColor3;
    case MouseState::Disabled:
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return adaptativeColorTransparent;
  }
}

QColor const& Theme::menuBarItemForegroundColor(MouseState const mouse, SelectionState const selected) const {
  Q_UNUSED(selected)
  switch (mouse) {
    case MouseState::Hovered:
      return neutralColor;
    case MouseState::Pressed:
      return neutralColor;
    case MouseState::Disabled:
      return neutralColorDisabled;
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return neutralColor;
  }
}

QColor const& Theme::tabBarBackgroundColor() const {
  return backgroundColorMain3;
}

QColor const& Theme::tabBarShadowColor() const {
  return shadowColor2;
}

QColor const& Theme::tabBarBottomShadowColor() const {
  return shadowColor1;
}

QColor const& Theme::tabBackgroundColor(MouseState const mouse, SelectionState const selected) const {
  const auto isSelected = selected == SelectionState::Selected;

  switch (mouse) {
    case MouseState::Hovered:
      return isSelected ? backgroundColorMain1 : adaptativeColor1;
    case MouseState::Pressed:
      return isSelected ? backgroundColorMain1 : adaptativeColor2;
    case MouseState::Normal:
      return isSelected ? backgroundColorMain1 : adaptativeColorTransparent;
    case MouseState::Disabled:
    case MouseState::Transparent:
    default:
      return adaptativeColorTransparent;
  }
}

QColor const& Theme::tabForegroundColor(MouseState const mouse, SelectionState const selected) const {
  Q_UNUSED(selected)
  return buttonForegroundColor(mouse, ColorRole::Neutral);
}

QColor const& Theme::tabCloseButtonBackgroundColor(MouseState const mouse, SelectionState const selected) const {
  Q_UNUSED(selected)
  switch (mouse) {
    case MouseState::Pressed:
      return adaptativeColor5;
    case MouseState::Hovered:
      return adaptativeColor3;
    case MouseState::Normal:
    case MouseState::Disabled:
    case MouseState::Transparent:
    default:
      return adaptativeColorTransparent;
  }
}

QColor const& Theme::tabCloseButtonForegroundColor(MouseState const mouse, SelectionState const selected) const {
  switch (mouse) {
    case MouseState::Pressed:
    case MouseState::Hovered:
    case MouseState::Normal:
      return neutralColor;
    case MouseState::Disabled:
    case MouseState::Transparent:
      return neutralColorTransparent;
    default:
      return selected == SelectionState::Selected ? neutralColor : neutralColorTransparent;
  }
}

QColor const& Theme::progressBarGrooveColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return adaptativeColor1;
  else
    return adaptativeColor2;
}

QColor const& Theme::progressBarValueColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return primaryColorDisabled;
  else
    return primaryColor;
}

QColor const& Theme::textFieldBackgroundColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return adaptativeColorTransparent;
  else
    return adaptativeColor1;
}

QColor const& Theme::textFieldBorderColor(MouseState const mouse, FocusState const focus, Status const status) const {
  if (mouse == MouseState::Disabled) {
    return borderColor1;
  } else {
    switch (status) {
      case Status::Error:
        if (focus == FocusState::Focused || mouse == MouseState::Hovered || mouse == MouseState::Pressed)
          return statusColorErrorHovered;
        else
          return statusColorError;
      case Status::Warning:
        if (focus == FocusState::Focused || mouse == MouseState::Hovered || mouse == MouseState::Pressed)
          return statusColorWarningHovered;
        else
          return statusColorWarning;
      case Status::Success:
        if (focus == FocusState::Focused || mouse == MouseState::Hovered || mouse == MouseState::Pressed)
          return statusColorSuccessHovered;
        else
          return statusColorSuccess;
      case Status::Info:
      case Status::Default:
      default:
        if (focus == FocusState::Focused || mouse == MouseState::Hovered || mouse == MouseState::Pressed)
          return primaryColor;
        else
          return borderColor3;
    }
  }
}

QColor const& Theme::textFieldForegroundColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return neutralColorDisabled;
  else
    return neutralColor;
}

QColor const& Theme::sliderGrooveColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return adaptativeColor1;
  else
    return adaptativeColor2;
}

QColor const& Theme::sliderValueColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return primaryColorDisabled;
  else
    return primaryColor;
}

QColor const& Theme::sliderHandleColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return primaryColorDisabled;
  else if (mouse == MouseState::Pressed)
    return primaryColorPressed;
  else if (mouse == MouseState::Hovered)
    return primaryColorHovered;
  else
    return primaryColor;
}

QColor const& Theme::sliderTickColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return borderColor1;
  else
    return borderColor3;
}

QColor const& Theme::dialHandleColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return adaptativeColor1;
  else
    return adaptativeColor2;
}

QColor const& Theme::dialGrooveColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return adaptativeColor1;
  else
    return adaptativeColor5;
}

QColor const& Theme::dialValueColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return primaryColorDisabled;
  else
    return primaryColor;
}

QColor const& Theme::dialTickColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return adaptativeColor1;
  else
    return adaptativeColor5;
}

QColor const& Theme::dialMarkColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return neutralColorDisabled;
  else
    return neutralColor;
}

QColor const& Theme::dialBackgroundColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return adaptativeColor1;
  else
    return adaptativeColor5;
}

QColor const& Theme::labelForegroundColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return neutralColorDisabled;
  else
    return neutralColor;
}

QColor const& Theme::labelCaptionForegroundColor(MouseState const mouse) const {
  if (mouse == MouseState::Disabled)
    return neutralAlternativeColorDisabled;
  else
    return neutralAlternativeColor;
}

QColor const& Theme::toolBarBackgroundColor() const {
  return backgroundColorMain1;
}

QColor const& Theme::toolBarBorderColor() const {
  return borderColor3;
}

QColor const& Theme::toolBarSeparatorColor() const {
  return neutralColorDisabled;
}

QColor const& Theme::toolTipBackgroundColor() const {
  return neutralColor;
}

QColor const& Theme::toolTipBorderColor() const {
  return neutralColorPressed;
}

QColor const& Theme::toolTipForegroundColor() const {
  return neutralColorForeground;
}

QColor const& Theme::scrollBarGrooveColor(MouseState const mouse) const {
  switch (mouse) {
    case MouseState::Hovered:
    case MouseState::Pressed:
      return adaptativeColor4;
    default:
      return adaptativeColorTransparent;
  }
}

QColor const& Theme::scrollBarHandleColor(MouseState const mouse) const {
  switch (mouse) {
    case MouseState::Hovered:
      return neutralColorHovered;
    case MouseState::Pressed:
      return neutralColorPressed;
    case MouseState::Disabled:
      return adaptativeColorTransparent;
    case MouseState::Normal:
      return adaptativeColor5;
    case MouseState::Transparent:
    default:
      return adaptativeColor4;
  }
}

int Theme::getScrollBarThickness(MouseState const mouse) const {
  switch (mouse) {
    case MouseState::Hovered:
    case MouseState::Pressed:
      return scrollBarThicknessFull;
    default:
      return scrollBarThicknessSmall;
  }
}

QColor const& Theme::groupBoxBorderColor(MouseState const mouse) const {
  switch (mouse) {
    case MouseState::Disabled:
      return borderColor1;
    default:
      return borderColor3;
  }
}

QColor const& Theme::groupBoxTitleColor(MouseState const mouse) const {
  return labelForegroundColor(mouse);
}

QColor const& Theme::focusBorderColor() const {
  return focusColor;
}

QColor const& Theme::frameBorderColor() const {
  return borderColor3;
}

const QFont& Theme::fontForTextRole(TextRole role) const {
  switch (role) {
    case TextRole::Caption:
      return fontCaption;
    case TextRole::H1:
      return fontH1;
    case TextRole::H2:
      return fontH2;
    case TextRole::H3:
      return fontH3;
    case TextRole::H4:
      return fontH4;
    case TextRole::H5:
      return fontH5;
    default:
      return fontRegular;
  }
}

QPalette Theme::paletteForTextRole(TextRole role) const {
  auto result = QPalette{ palette };

  const auto& textColor = colorForTextRole(role, MouseState::Normal);
  const auto& textColorDisabled = colorForTextRole(role, MouseState::Disabled);

  result.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Text, textColor);
  result.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::WindowText, textColor);
  result.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::BrightText, textColor);

  result.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, textColorDisabled);
  result.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, textColorDisabled);
  result.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, textColorDisabled);

  return result;
}

QColor const& Theme::switchGrooveColor(MouseState const mouse, CheckState const checked) const {
  const auto primary = checked == CheckState::Checked;

  switch (mouse) {
    case MouseState::Pressed:
      return primary ? primaryColorPressed : adaptativeColor5;
    case MouseState::Hovered:
      return primary ? primaryColorHovered : adaptativeColor4;
    case MouseState::Disabled:
      return primary ? primaryColorDisabled : adaptativeColor1;
    case MouseState::Normal:
    default:
      return primary ? primaryColor : adaptativeColor3;
  }
}

QColor const& Theme::switchHandleColor(MouseState const mouse, CheckState const checked) const {
  const auto primary = checked == CheckState::Checked;

  switch (mouse) {
    case MouseState::Pressed:
      return primary ? primaryColorForegroundPressed : neutralAlternativeColorPressed;
    case MouseState::Hovered:
      return primary ? primaryColorForegroundHovered : neutralAlternativeColorHovered;
    case MouseState::Disabled:
      return primary ? primaryColorForegroundDisabled : neutralAlternativeColorDisabled;
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return primary ? primaryColorForeground : neutralAlternativeColor;
  }
}

QColor const& Theme::tableHeaderBgColor(MouseState const mouse, CheckState const) const {
  switch (mouse) {
    case MouseState::Pressed:
      return adaptativeColor5;
    case MouseState::Hovered:
      return adaptativeColor2;
    case MouseState::Disabled:
      return adaptativeColor3;
    case MouseState::Transparent:
    case MouseState::Normal:
    default:
      return adaptativeColor3;
  }
}

QColor const& Theme::tableHeaderFgColor(MouseState const mouse, CheckState const) const {
  switch (mouse) {
    case MouseState::Disabled:
      return neutralColorDisabled;
    default:
      return neutralColor;
  }
}

QColor const& Theme::tableLineColor() const {
  return neutralAlternativeColor;
}

QColor const& Theme::colorForTextRole(TextRole role, MouseState const mouse) const {
  switch (role) {
    case TextRole::Caption:
      return mouse == MouseState::Disabled ? neutralAlternativeColorDisabled : neutralAlternativeColor;
    case TextRole::H1:
    case TextRole::H2:
    case TextRole::H3:
    case TextRole::H4:
    case TextRole::H5:
    case TextRole::Default:
    default:
      return mouse == MouseState::Disabled ? neutralColorDisabled : neutralColor;
  }
}

int Theme::pixelSizeForTextRole(TextRole role) const {
  switch (role) {
    case TextRole::Caption:
      return fontSizeS1;
    case TextRole::H1:
      return fontSizeH1;
    case TextRole::H2:
      return fontSizeH2;
    case TextRole::H3:
      return fontSizeH3;
    case TextRole::H4:
      return fontSizeH4;
    case TextRole::H5:
      return fontSizeH5;
    default:
      return fontSize;
  }
}

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
  const auto itemViewAlternate = getColorSourceOver(itemViewBase, colorWithAlpha(adaptativeColor1, adaptativeColor1.alpha() / 2));
  const auto itemViewDisabled = getColorSourceOver(backgroundColorMain1, adaptativeColor1);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Base, itemViewBase);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, itemViewDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::AlternateBase, itemViewAlternate);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, itemViewDisabled);
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::NoRole, backgroundColorMainTransparent);
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::NoRole, backgroundColorMainTransparent);

  // Tooltips.
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::ToolTipBase, toolTipBackgroundColor());
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::ToolTipText, toolTipForegroundColor());

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
  palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Button, buttonBackgroundColor(MouseState::Normal, ColorRole::Neutral));
  palette.setColor(QPalette::ColorGroup::Normal, QPalette::ColorRole::Button, buttonBackgroundColor(MouseState::Normal, ColorRole::Neutral));
  palette.setColor(QPalette::ColorGroup::Current, QPalette::ColorRole::Button, buttonBackgroundColor(MouseState::Hovered, ColorRole::Neutral));
  palette.setColor(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, buttonBackgroundColor(MouseState::Pressed, ColorRole::Neutral));
  palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, buttonBackgroundColor(MouseState::Disabled, ColorRole::Neutral));

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

      fontMedium = defaultFont;
      fontMedium.setWeight(QFont::Weight::Medium);
      fontMedium.setPointSizeF(pixelSizeToPointSize(fontSize, dpi));

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
#pragma endregion
} // namespace oclero::qlementine
