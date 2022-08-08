#pragma once

#include <oclero/qlementine/style/Theme.hpp>
#include <oclero/qlementine/utils/RadiusesF.hpp>

#include <QPainter>
#include <QRect>
#include <QWidget>
#include <QKeySequence>
#include <QAbstractSpinBox>
#include <QStyle>

namespace oclero::qlementine {
#pragma region Shapes

/// Draws an antialiased pixel-perfect border for the ellipsis.
void drawEllipseBorder(QPainter* p, QRectF const& rect, QColor const& color, qreal const borderWidth);

/// Generates a QPainterPath that contains a rounded rectangle with different radiuses for each angle.
QPainterPath getMultipleRadiusesRectPath(QRectF const& rect, RadiusesF const& radiuses);

/// Draws an antialiased rect.
void drawRoundedRect(QPainter* p, QRectF const& rect, QColor const& color, qreal const radius = 0.);

// Draws an antialiased rect with different radiuses.
void drawRoundedRect(QPainter* p, QRectF const& rect, QColor const& color, RadiusesF const& radiuses);

/// Draws an antialiased rect.
void drawRoundedRect(QPainter* p, QRect const& rect, QColor const& color, qreal const radius = 0.);

// Draws an antialiased rect with different radiuses.
void drawRoundedRect(QPainter* p, QRect const& rect, QColor const& color, RadiusesF const& radiuses);

/// Draws an antialiased pixel-perfect border for the rounded rect.
void drawRoundedRectBorder(QPainter* p, QRectF const& rect, QColor const& color, qreal const borderWidth, qreal const radius = 0.);

/// Draws an antialiased pixel-perfect border for the rounded rect.
void drawRoundedRectBorder(QPainter* p, QRect const& rect, QColor const& color, qreal const borderWidth, qreal const radius = 0.);

/// Draws an antialiased pixel-perfect border for the rounded rect.
void drawRoundedRectBorder(QPainter* p, QRectF const& rect, QColor const& color, qreal const borderWidth, RadiusesF const& radiuses = {});

/// Draws an antialiased pixel-perfect border for the rounded rect.
void drawRoundedRectBorder(QPainter* p, QRect const& rect, QColor const& color, qreal const borderWidth, RadiusesF const& radiuses = {});

/// Draws a pixel-perfect border for the rect.
void drawRectBorder(QPainter* p, QRect const& rect, QColor const& color, qreal const borderWidth);

/// Draws a pixel-perfect border for the rect.
void drawRectBorder(QPainter* p, QRectF const& rect, QColor const& color, qreal const borderWidth);

/// Draws an antialiased triangle.
void drawRoundedTriangle(QPainter* p, QRectF const& rect, qreal const radius = 0.);

/// Draws the value of a progress bar. A clipping mask is used to ensure the rectangle radiuses are respected
/// even for values close to min or max.
void drawProgressBarValueRect(QPainter* p, QRect const& rect, QColor const& color, qreal min, qreal max, qreal value, qreal const radius = 0., bool inverted = false);

/// Gets the color with the new alpha channel (from 0.0 to 1.0).
QColor colorWithAlphaF(QColor const& color, qreal alpha);

/// Gets the color the new alpha chanel (from 0 to 255).
QColor colorWithAlpha(QColor const& color, int alpha);

/// Gets the resulting color by applying the foreground color over the background color with 'SourceOver' composition mode.
QColor getColorSourceOver(const QColor& bg, const QColor& fg);

/// Draws a semi-transparent red rectangle.
void drawDebugRect(const QRect& rect, QPainter* p);

/// Gets the device pixel ratio for the QWidget.
double getPixelRatio(QWidget const* w);

/// Parses the text to detect the MenuItem's label and shortcut, usually separated by a tab.
std::tuple<QString, QString> getMenuLabelAndShortcut(QString const& text);

using PixmapMakerFunc = std::function<QPixmap(const QSize& s, const QColor& c)>;

/// Utility to add QPixmaps to all states of the QIcon. The callback in parameter will be called to draw each QPixmap.
void updateUncheckableButtonIconPixmap(QIcon& icon, const QSize& size, Theme const& theme, const PixmapMakerFunc& func);

#pragma endregion

#pragma region Indicators

/// Draws the menu arrow in a Button.
QPainterPath getMenuIndicatorPath(const QRect& rect);

/// Draws the combobox double arrow.
void drawComboBoxIndicator(const QRect& rect, QPainter* p);

/// Draws the checkbox indicator (i.e. a check mark).
void drawCheckBoxIndicator(const QRect& rect, QPainter* p, qreal progress = 1.);

/// Draws the radiobutton indicator (i.e. a circle).
void drawRadioButtonIndicator(const QRect& rect, QPainter* p, qreal progress = 1.);

/// Draws a spinbox up/down indicator (i.e. +/- or up/down arrow).
void drawSpinBoxArrowIndicator(const QRect& rect, QPainter* p, QAbstractSpinBox::ButtonSymbols buttonSymbol, QStyle::SubControl subControl, QSize const& iconSize);

/// Draws an arrow that points to the right.
void drawArrowRight(const QRect& rect, QPainter* p);

/// Draws an arrow that points to the left.
void drawArrowLeft(const QRect& rect, QPainter* p);

/// Draws an arrow that points down.
void drawArrowDown(const QRect& rect, QPainter* p);

/// Draws an arrow that points up.
void drawArrowUp(const QRect& rect, QPainter* p);

/// Draws the sub-menu arrow.
void drawSubMenuIndicator(const QRect& rect, QPainter* p);

/// Draws a double right arrow.
void drawDoubleArrowRightIndicator(const QRect& rect, QPainter* p);

/// Draws a small double right arrow.
void drawToolBarExtensionIndicator(const QRect& rect, QPainter* p);

/// Draws a close indicator (i.e. the cross).
void drawCloseIndicator(const QRect& rect, QPainter* p);

/// Draws a treeview indicator.
void drawTreeViewIndicator(const QRect& rect, QPainter* p, bool open);

#pragma endregion

#pragma region Pixmap / Icons

QPixmap getPixmap(QIcon const& icon, const QSize& iconSize, double const pixelRatio, MouseState const mouse, CheckState const checked);

/// Generates a pixmap for a specific state of QLineEdit's clear button.
QPixmap makeClearButtonPixmap(QSize const& size, QColor const& color);

/// Generates an icon for QLineEdit's clear button.
void updateClearButtonIcon(QIcon& icon, QSize const& size, Theme const& theme);

/// Generates a pixmap that contains a check mark.
QPixmap makeCheckPixmap(QSize const& size, QColor const& color);

/// Generates an icon that contains a check mark.
void updateCheckIcon(QIcon& icon, QSize const& size, Theme const& theme);

/// Generates a pixmap that contains a double right arrow.
QPixmap makeDoubleArrowRightPixmap(QSize const& size, QColor const& color);

/// Generates a pixmap that contains a small double right arrow.
QPixmap makeToolBarExtensionPixmap(QSize const& size, QColor const& color);

/// Generates an icon that contains a double right arrow.
void updateToolBarExtensionIcon(QIcon& icon, QSize const& size, Theme const& theme);

/// Generates a pixmap that contains a left arrow.
QPixmap makeArrowLeftPixmap(QSize const& size, QColor const& color);

/// Generates a pixmap that contains a right arrow.
QPixmap makeArrowRightPixmap(QSize const& size, QColor const& color);

/// Generates an icon that contains a left arrow.
void updateArrowLeftIcon(QIcon& icon, QSize const& size, Theme const& theme);

/// Generates an icon that contains a right arrow.
void updateArrowRightIcon(QIcon& icon, QSize const& size, Theme const& theme);

QPixmap makeMessageBoxWarningPixmap(QSize const& size, QColor const& bgColor, QColor const& fgColor);
QPixmap makeMessageBoxCriticalPixmap(QSize const& size, QColor const& bgColor, QColor const& fgColor);
QPixmap makeMessageBoxQuestionPixmap(QSize const& size, QColor const& bgColor, QColor const& fgColor);
QPixmap makeMessageBoxInformationPixmap(QSize const& size, QColor const& bgColor, QColor const& fgColor);

void updateMessageBoxWarningIcon(QIcon& icon, QSize const& size, Theme const& theme);
void updateMessageBoxCriticalIcon(QIcon& icon, QSize const& size, Theme const& theme);
void updateMessageBoxQuestionIcon(QIcon& icon, QSize const& size, Theme const& theme);
void updateMessageBoxInformationIcon(QIcon& icon, QSize const& size, Theme const& theme);

#pragma endregion

#pragma region SubControls

/// Draws a RadioButton indicator according to its checked state.
void drawRadioButton(QPainter* p, const QRect& rect, QColor const& bgColor, const QColor& borderColor, QColor const& fgColor, const qreal borderWidth, qreal progress);

/// Draws a CheckButton indicator according to its checked state.
void drawCheckButton(QPainter* p, const QRect& rect, qreal radius, const QColor& bgColor, const QColor& borderColor, const QColor& fgColor, const qreal borderWidth, qreal progress);

/// Draws a menu separator.
void drawMenuSeparator(QPainter* p, const QRect& rect, QColor const& color, const int thickness);

/// Gets the tick interval according to the length of steps, range and available length.
int getTickInterval(int tickInterval, int singleStep, int pageStep, int min, int max, int sliderLength);

/// Draws the Slider tick marks.
void drawSliderTickMarks(
	QPainter* p, QRect const& tickmarksRect, QColor const& tickColor, const int min, const int max, const int interval, const int tickThickness, const int singleStep, const int pageStep);

/// Draws the Dial tick marks.
void drawDialTickMarks(QPainter* p, QRect const& tickmarksRect, QColor const& tickColor, const int min, const int max, const int tickThickness, const int tickLength, const int singleStep,
	const int pageStep, const int minArcLength);

/// Draws a Dial.
void drawDial(QPainter* p, QRect const& rect, int min, int max, double value, QColor const& bgColor, QColor const& handleColor, QColor const& grooveColor, QColor const& valueColor,
	QColor const& markColor, const int grooveThickness, const int markLength, const int markThickness);

/// Gets the path for a rounded tab. Specify negative radiuses if you want the tab to overlap its bounds.
QPainterPath getTabPath(QRect const& rect, const RadiusesF& radiuses);

/// Draws a rounded tab. Specify negative radiuses if you want the tab to overlap its bounds.
void drawTab(QPainter* p, QRect const& rect, const RadiusesF& radiuses, const QColor& bgColor);
#pragma endregion

#pragma region Text

/// Draws an elided text (with an ellipsis "…" at the end if necessary) inside a QRect.
/// The difference with Qt's method is the ellipsis (Qt doesn't draw one and just cuts the text).
void drawElidedMultiLineText(QPainter& p, const QRect& rect, const QString& text, const QPaintDevice* paintDevice);

/// Removes the trailing whitespaces at the end.
QString removeTrailingWhitespaces(const QString& str);

/// Draws a keyboard shortcut.
void drawShortcut(QPainter& p, const QKeySequence& shortcut, const QRect& rect, const Theme& theme, bool enabled, Qt::Alignment alignment = { Qt::AlignLeft | Qt::AlignVCenter });

/// Gets the necessary size to display the whole shortcut.
QSize shortcutSizeHint(const QKeySequence& shortcut, const Theme& theme);

#pragma endregion
} // namespace oclero::qlementine
