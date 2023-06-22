#pragma once

#include <QWidget>
#include <QPointer>
#include <QVBoxLayout>
#include <QVariantAnimation>

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
  Popover(QWidget* parent = nullptr);

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

public slots:
  void openPopover();
  void closePopover();

signals:
  void aboutToOpen();
  void aboutToClose();
  void opened();
  void closed();

protected:
  void paintEvent(QPaintEvent* e) override;
  void mousePressEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void hideEvent(QHideEvent* e) override;
  void showEvent(QShowEvent* e) override;
  bool event(QEvent* e) override;
  bool eventFilter(QObject* o, QEvent* e) override;
  void focusOutEvent(QFocusEvent* e) override;

private:
  void updatePopoverGeometry();
  static const std::array<Position, 4>& positionPriority(Position const position);
  QRect getGeometryForPosition(Position const position, Alignment const alignment) const;
  QRect getFallbackGeometry() const;
  void startAnimation();
  QPixmap getFrameShape() const;

private:
  Position _preferredPosition{ Position::Left };
  Alignment _preferredAlignment{ Alignment::Begin };
  QWidget* _content{ nullptr };
  bool _opened{ false };
  QWidget* _frame{ nullptr };
  QVBoxLayout* _frameLayout{ nullptr };
  QPointer<QWidget> _anchor{ nullptr };
  int _horizontalSpacing{ 0 };
  int _verticalSpacing{ 0 };
  QVariantAnimation _opacityAnimation;
  bool _mousePressWasOnAnchor{ false };
  QMargins _screenPadding{ 10, 10, 10, 10 };
  QPixmap _dropShadowCache;
  QSize _dropShadowCacheSize;
};
} // namespace oclero::qlementine
