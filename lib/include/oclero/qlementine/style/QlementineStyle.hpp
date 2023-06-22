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

#include <oclero/qlementine/style/Theme.hpp>

#include <QCommonStyle>

namespace oclero::qlementine {
class CommandLinkButtonPaintEventFilter;
class LineEditButtonEventFilter;
struct QlementineStyleImpl;

class QlementineStyle : public QCommonStyle {
  Q_OBJECT

  Q_PROPERTY(bool animationsEnabled READ animationsEnabled WRITE setAnimationsEnabled NOTIFY animationsEnabledChanged)

public:
  //static constexpr char Property_DoNotColorizeIcon[] = "DoNotColorizeIcon";

  enum StandardPixmapExt {
    SP_Check = SP_CustomBase + 1,
  };

  enum ControlElementExt {
    CE_CommandButtonLabel = CE_CustomBase + 1,
    CE_CommandButton,
  };

  enum ContentsTypeExt {
    CT_CommandButton = CT_CustomBase + 1,
  };

  enum PixelMetricExt {
    PM_MediumIconSize = PM_CustomBase + 1,
  };

  enum PrimitiveElementExt {
    PE_CommandButtonPanel = PE_CustomBase + 1,
    PE_CommandButtonLabel,
  };

public:
  explicit QlementineStyle(QObject* parent = nullptr);
  ~QlementineStyle();

  Theme const& theme() const;
  void setTheme(Theme const& theme);
  void setThemeJsonPath(QString const& jsonPath);
  Q_SIGNAL void themeChanged();

  bool animationsEnabled() const;
  void setAnimationsEnabled(bool enabled);
  Q_SIGNAL void animationsEnabledChanged();

  bool useMenuForComboBoxPopup() const;
  void setUseMenuForComboBoxPopup(bool useMenu);
  Q_SIGNAL void useMenuForComboBoxPopupChanged();

  void triggerCompleteRepaint();

  void setAutoIconColorEnabled(bool enabled);
  bool isAutoIconColorEnabled() const;

  void setAutoIconColorEnabled(QWidget* widget, bool enabled);
  bool isAutoIconColorEnabled(const QWidget* widget) const;

  static QIcon makeIcon(const QString& svgPath);

public:
  virtual void drawPrimitive(
    PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w = nullptr) const override;

  virtual void drawControl(
    ControlElement ce, const QStyleOption* opt, QPainter* p, const QWidget* w = nullptr) const override;

  virtual QRect subElementRect(SubElement se, const QStyleOption* opt, const QWidget* w = nullptr) const override;

  virtual void drawComplexControl(
    ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* w = nullptr) const override;

  virtual SubControl hitTestComplexControl(
    ComplexControl cc, const QStyleOptionComplex* opt, const QPoint& pos, const QWidget* w = nullptr) const override;

  virtual QRect subControlRect(
    ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* w = nullptr) const override;

  virtual QSize sizeFromContents(
    ContentsType ct, const QStyleOption* opt, const QSize& s, const QWidget* w = nullptr) const override;

  virtual int pixelMetric(PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* w = nullptr) const override;

  virtual int styleHint(StyleHint sh, const QStyleOption* opt = nullptr, const QWidget* w = nullptr,
    QStyleHintReturn* shret = nullptr) const override;

  virtual QPalette standardPalette() const override;

  virtual QIcon standardIcon(
    StandardPixmap sp, const QStyleOption* opt = nullptr, const QWidget* w = nullptr) const override;

  virtual QPixmap standardPixmap(
    StandardPixmap sp, const QStyleOption* opt = nullptr, const QWidget* w = nullptr) const override;

  virtual QPixmap generatedIconPixmap(QIcon::Mode im, const QPixmap& pixmap, const QStyleOption* opt) const override;

  virtual int layoutSpacing(QSizePolicy::ControlType c1, QSizePolicy::ControlType c2, Qt::Orientation o,
    const QStyleOption* opt = nullptr, const QWidget* w = nullptr) const override;

  virtual void polish(QPalette& palette) override;
  virtual void polish(QApplication* app) override;
  virtual void polish(QWidget* w) override;
  virtual void unpolish(QWidget* w) override;
  virtual void unpolish(QApplication* app) override;

  virtual QColor const& color(MouseState const mouse, ColorRole const role) const;

  virtual QColor const& frameBackgroundColor(MouseState const mouse) const;

  virtual QColor const& buttonBackgroundColor(MouseState const mouse, ColorRole const role) const;
  virtual QColor const& buttonForegroundColor(MouseState const mouse, ColorRole const role) const;

  virtual QColor const& toolButtonBackgroundColor(MouseState const mouse, ColorRole const role) const;
  virtual QColor const& toolButtonForegroundColor(MouseState const mouse, ColorRole const role) const;
  virtual QColor const& toolButtonSeparatorColor(MouseState const mouse, ColorRole const role) const;

  virtual QColor const& commandButtonBackgroundColor(MouseState const mouse, ColorRole const role) const;
  virtual QColor const& commandButtonTextColor(MouseState const mouse, ColorRole const role) const;
  virtual QColor const& commandButtonDescriptionColor(MouseState const mouse, ColorRole const role) const;
  virtual QColor const& commandButtonIconColor(MouseState const mouse, ColorRole const role) const;

  virtual QColor const& checkButtonBackgroundColor(MouseState const mouse, CheckState const checked) const;
  virtual QColor const& checkButtonForegroundColor(MouseState const mouse, CheckState const checked) const;

  virtual QColor const& radioButtonBackgroundColor(MouseState const mouse, CheckState const checked) const;
  virtual QColor const& radioButtonForegroundColor(MouseState const mouse, CheckState const checked) const;

  virtual QColor const& comboBoxBackgroundColor(MouseState const mouse) const;
  virtual QColor const& comboBoxForegroundColor(MouseState const mouse) const;
  virtual QColor const& comboBoxTextColor(MouseState const mouse, Status const status) const;

  virtual QColor const& spinBoxBackgroundColor(MouseState const mouse) const;
  virtual QColor const& spinBoxBorderColor(MouseState const mouse, FocusState const focus) const;
  virtual QColor const& spinBoxButtonBackgroundColor(MouseState const mouse) const;
  virtual QColor const& spinBoxButtonForegroundColor(MouseState const mouse) const;

  virtual QColor const& listItemRowBackgroundColor(MouseState const mouse, AlternateState const alternate) const;
  virtual QColor const& listItemBackgroundColor(
    MouseState const mouse, SelectionState const selected, FocusState const focus, ActiveState const active) const;
  virtual QColor const& listItemForegroundColor(
    MouseState const mouse, SelectionState const selected, FocusState const focus, ActiveState const active) const;
  virtual QColor const& listItemCaptionForegroundColor(
    MouseState const mouse, SelectionState const selected, FocusState const focus, ActiveState const active) const;
  virtual QColor const& listItemCheckButtonBackgroundColor(
    MouseState const mouse, CheckState const checked, SelectionState const selected, ActiveState const active) const;
  virtual QColor const& listItemCheckButtonForegroundColor(
    MouseState const mouse, CheckState const checked, SelectionState const selected, ActiveState const active) const;
  virtual QColor const& cellItemFocusBorderColor(
    FocusState const focus, SelectionState const selected, ActiveState const active) const;

  virtual QColor const& menuBackgroundColor() const;
  virtual QColor const& menuBorderColor() const;
  virtual QColor const& menuSeparatorColor() const;

  virtual QColor const& menuItemBackgroundColor(MouseState const mouse) const;
  virtual QColor const& menuItemForegroundColor(MouseState const mouse) const;
  virtual QColor const& menuItemSecondaryForegroundColor(MouseState const mouse) const;

  virtual QColor const& menuBarBackgroundColor() const;
  virtual QColor const& menuBarBorderColor() const;
  virtual QColor const& menuBarItemBackgroundColor(MouseState const mouse, SelectionState const selected) const;
  virtual QColor const& menuBarItemForegroundColor(MouseState const mouse, SelectionState const selected) const;

  virtual QColor const& tabBarBackgroundColor() const;
  virtual QColor const& tabBarShadowColor() const;
  virtual QColor const& tabBarBottomShadowColor() const;
  virtual QColor const& tabBackgroundColor(MouseState const mouse, SelectionState const selected) const;
  virtual QColor const& tabForegroundColor(MouseState const mouse, SelectionState const selected) const;
  virtual QColor const& tabCloseButtonBackgroundColor(MouseState const mouse, SelectionState const selected) const;
  virtual QColor const& tabCloseButtonForegroundColor(MouseState const mouse, SelectionState const selected) const;

  virtual QColor const& progressBarGrooveColor(MouseState const mouse) const;
  virtual QColor const& progressBarValueColor(MouseState const mouse) const;

  virtual QColor const& textFieldBackgroundColor(MouseState const mouse, Status const status) const;
  virtual QColor const& textFieldBorderColor(MouseState const mouse, FocusState const focus, Status const status) const;
  virtual QColor const& textFieldForegroundColor(MouseState const mouse) const;

  virtual QColor const& sliderGrooveColor(MouseState const mouse) const;
  virtual QColor const& sliderValueColor(MouseState const mouse) const;
  virtual QColor const& sliderHandleColor(MouseState const mouse) const;
  virtual QColor const& sliderTickColor(MouseState const mouse) const;

  virtual QColor const& dialHandleColor(MouseState const mouse) const;
  virtual QColor const& dialGrooveColor(MouseState const mouse) const;
  virtual QColor const& dialValueColor(MouseState const mouse) const;
  virtual QColor const& dialTickColor(MouseState const mouse) const;
  virtual QColor const& dialMarkColor(MouseState const mouse) const;
  virtual QColor const& dialBackgroundColor(MouseState const mouse) const;

  virtual QColor const& labelForegroundColor(MouseState const mouse) const;
  virtual QColor const& labelCaptionForegroundColor(MouseState const mouse) const;

  virtual QColor const& toolBarBackgroundColor() const;
  virtual QColor const& toolBarBorderColor() const;
  virtual QColor const& toolBarSeparatorColor() const;

  virtual QColor const& toolTipBackgroundColor() const;
  virtual QColor const& toolTipBorderColor() const;
  virtual QColor const& toolTipForegroundColor() const;

  virtual QColor const& scrollBarGrooveColor(MouseState const mouse) const;
  virtual QColor const& scrollBarHandleColor(MouseState const mouse) const;
  virtual int getScrollBarThickness(MouseState const mouse) const;

  virtual QColor const& groupBoxTitleColor(MouseState const mouse) const;
  virtual QColor const& groupBoxBorderColor(MouseState const mouse) const;
  virtual QColor const& groupBoxBackgroundColor(MouseState const mouse) const;

  virtual QColor const& focusBorderColor() const;
  virtual QColor const& frameBorderColor() const;

  virtual QColor const& colorForTextRole(TextRole role, MouseState const mouse) const;
  virtual int pixelSizeForTextRole(TextRole role) const;
  virtual QFont const& fontForTextRole(TextRole role) const;
  virtual QPalette paletteForTextRole(TextRole role) const;

  virtual QColor const& switchGrooveColor(MouseState const mouse, CheckState const checked) const;
  virtual QColor const& switchHandleColor(MouseState const mouse, CheckState const checked) const;

  virtual QColor const& tableHeaderBgColor(MouseState const mouse, CheckState const checked) const;
  virtual QColor const& tableHeaderFgColor(MouseState const mouse, CheckState const checked) const;
  virtual QColor const& tableLineColor() const;

  virtual Status widgetStatus(QWidget const* widget) const;

  friend class CommandLinkButtonPaintEventFilter;
  friend class LineEditButtonEventFilter;

private:
  std::unique_ptr<QlementineStyleImpl> _impl;
};
} // namespace oclero::qlementine
