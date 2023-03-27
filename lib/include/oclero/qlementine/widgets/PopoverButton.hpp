#pragma once

#include <QPushButton>

namespace oclero::qlementine {
class Popover;

/// A Button that looks like a ComboBox but opens a Popover.
class PopoverButton : public QPushButton {
  Q_OBJECT

public:
  PopoverButton(QWidget* parent = nullptr);
  PopoverButton(const QString& text, QWidget* parent = nullptr);
  PopoverButton(const QString& text, const QIcon& icon, QWidget* parent = nullptr);

public:
  QWidget* popoverContentWidget() const;
  void setPopoverContentWidget(QWidget* widget);
  Q_SIGNAL void popoverContentWidgetChanged();

  Popover* popover() const;

public slots:
  void setPopoverOpened(bool opened);

signals:
  void popoverOpenedChanged(bool opened);

protected:
  void paintEvent(QPaintEvent* e) override;

private:
  Popover* _popover{ nullptr };
};
} // namespace oclero::qlementine
