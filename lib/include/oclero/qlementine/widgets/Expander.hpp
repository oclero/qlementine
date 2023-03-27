#pragma once

#include <QWidget>
#include <QPointer>
#include <QVariantAnimation>

namespace oclero::qlementine {
/// A QWidget that allows to expand vertically, displaying its content.
class Expander : public QWidget {
  Q_OBJECT

  Q_PROPERTY(bool expanded READ expanded WRITE setExpanded NOTIFY expandedChanged)

public:
  Expander(QWidget* parent = nullptr);

  bool expanded() const;
  Q_SLOT void setExpanded(bool expanded);
  Q_SIGNAL void expandedChanged();

  QWidget* content() const;
  void setContent(QWidget* content);
  Q_SIGNAL void contentChanged();

  QSize sizeHint() const override;

protected:
  bool event(QEvent* e) override;
  void resizeEvent(QResizeEvent* e) override;
  bool eventFilter(QObject* o, QEvent* e) override;

private:
  void updateContentGeometry();

private:
  bool _expanded{ false };
  QVariantAnimation _animation;
  QPointer<QWidget> _content{ nullptr };
};
} // namespace oclero::qlementine
