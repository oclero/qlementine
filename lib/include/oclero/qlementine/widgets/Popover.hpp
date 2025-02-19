// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QBitmap>
#include <QList>
#include <QPointer>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>
#include <QWidget>

namespace oclero::qlementine {
/// A MacOS-like popover.
class Popover : public QWidget {
  Q_OBJECT

  Q_PROPERTY(oclero::qlementine::Popover::Position preferredPosition READ preferredPosition WRITE setPreferredPosition
      NOTIFY preferredPositionChanged)
  Q_PROPERTY(oclero::qlementine::Popover::Alignment preferredAlignment READ preferredAlignment WRITE
      setPreferredAlignment NOTIFY preferredAlignmentChanged)
  Q_PROPERTY(bool isOpened READ isOpened WRITE setOpened NOTIFY openedChanged)
  Q_PROPERTY(QMargins padding READ padding WRITE setPadding NOTIFY paddingChanged)
  Q_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing NOTIFY horizontalSpacingChanged)
  Q_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing NOTIFY verticalSpacingChanged)
  Q_PROPERTY(QMargins screenPadding READ screenPadding WRITE setScreenPadding NOTIFY screenPaddingChanged)
  Q_PROPERTY(QColor dropShadowColor READ dropShadowColor WRITE setDropShadowColor NOTIFY dropShadowColorChanged)
  Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
  Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
  Q_PROPERTY(qreal dropShadowRadius READ dropShadowRadius WRITE setDropShadowRadius NOTIFY dropShadowRadiusChanged)
  Q_PROPERTY(bool canBeOverAnchor READ canBeOverAnchor WRITE setCanBeOverAnchor NOTIFY canBeOverAnchorChanged)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)

public:
  enum class Position {
    Left,
    Top,
    Right,
    Bottom,
  };
  Q_ENUM(Position)

  enum class Alignment {
    Begin,
    Center,
    End,
  };
  Q_ENUM(Alignment)

public:
  explicit Popover(QWidget* parent = nullptr);
  virtual ~Popover();

public:
  Position preferredPosition() const;
  void setPreferredPosition(Position position);
  Q_SIGNAL void preferredPositionChanged();

  Alignment preferredAlignment() const;
  void setPreferredAlignment(Alignment alignment);
  Q_SIGNAL void preferredAlignmentChanged();

  QWidget* contentWidget() const;
  void setContentWidget(QWidget* widget);
  Q_SIGNAL void contentWidgetChanged();

  bool isOpened() const;
  void setOpened(bool opened);
  Q_SIGNAL void openedChanged();

  QMargins padding() const;
  void setPadding(const QMargins& padding);
  Q_SIGNAL void paddingChanged();

  QMargins screenPadding() const;
  void setScreenPadding(const QMargins& padding);
  Q_SIGNAL void screenPaddingChanged();

  QWidget* anchorWidget() const;
  void setAnchorWidget(QWidget* widget);
  Q_SIGNAL void anchorWidgetChanged();

  int verticalSpacing() const;
  void setVerticalSpacing(int spacing);
  Q_SIGNAL void verticalSpacingChanged();

  int horizontalSpacing() const;
  void setHorizontalSpacing(int spacing);
  Q_SIGNAL void horizontalSpacingChanged();

  const QColor& dropShadowColor() const;
  void setDropShadowColor(const QColor&);
  Q_SIGNAL void dropShadowColorChanged();

  qreal radius() const;
  void setRadius(qreal radius);
  Q_SIGNAL void radiusChanged();

  qreal borderWidth() const;
  void setBorderWidth(qreal width);
  Q_SIGNAL void borderWidthChanged();

  qreal dropShadowRadius() const;
  void setDropShadowRadius(qreal radius);
  Q_SIGNAL void dropShadowRadiusChanged();

  bool canBeOverAnchor() const;
  void setCanBeOverAnchor(bool value);
  Q_SIGNAL void canBeOverAnchorChanged();

  const QColor& backgroundColor() const;
  void setBackgroundColor(const QColor&);
  Q_SIGNAL void backgroundColorChanged();

  const QColor& borderColor() const;
  void setBorderColor(const QColor&);
  Q_SIGNAL void borderColorChanged();

public Q_SLOTS:
  void openPopover();
  void closePopover();
  void togglePopover();

Q_SIGNALS:
  void aboutToOpen();
  void aboutToClose();
  void opened();
  void closed();
  void pressed();
  void released();

protected:
  void paintEvent(QPaintEvent* e) override;
  void mousePressEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void hideEvent(QHideEvent* e) override;
  void showEvent(QShowEvent* e) override;

private:
  QMargins dropShadowMargins() const;
  void updateDropShadowMargins();
  void updateDropShadowCache();

  void updatePopoverGeometry();
  static const std::array<Position, 4>& positionPriority(Position const position);
  QRect getGeometryForPosition(Position const position, Alignment const alignment) const;
  QRect getFallbackGeometry() const;
  void startAnimation();
  QPixmap getFrameShape() const;
  QBitmap getFrameMask() const;
  bool hitboxContainsPoint(const QPointF& pos) const;
  void clearAnchorWidget();

private:
  Position _preferredPosition{ Position::Left };
  Alignment _preferredAlignment{ Alignment::Begin };
  QWidget* _content{ nullptr };
  bool _opened{ false };
  class PopoverFrame;
  PopoverFrame* _frame{ nullptr };
  QVBoxLayout* _frameLayout{ nullptr };
  QPointer<QWidget> _anchorWidget{ nullptr };
  int _horizontalSpacing{ 0 };
  int _verticalSpacing{ 0 };
  QVariantAnimation _opacityAnimation;
  QMargins _screenPadding{ 10, 10, 10, 10 };
  struct {
    QSize size;
    QPixmap pixmap;
  } _dropShadowCache;
  bool _canBeOverAnchor{ true };
  bool _animated{ true };
  QTimer _clickTimer;
  QColor _dropShadowColor{ QColor(0, 0, 0, 144) };
  qreal _dropShadowRadius{ 24. };
  qreal _borderWidth{ 1. };
  qreal _radius{ 8. };
  QColor _backgroundColor{};
  QColor _borderColor{};
  static const bool _shouldDrawDropShadow;
};
} // namespace oclero::qlementine
