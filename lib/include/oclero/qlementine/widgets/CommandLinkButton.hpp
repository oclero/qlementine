#pragma once

#include <QCommandLinkButton>

namespace oclero::qlementine {
class QStyleOptionCommandLinkButton;

/// A QCommandLinkButton that just doesn't force its size to Windows Vista's
/// system-default CommandLinkButtton size. It actually lets content decide.
class CommandLinkButton : public QCommandLinkButton {
public:
  using QCommandLinkButton::QCommandLinkButton;

public:
  QSize sizeHint() const override;
  int heightForWidth(int) const override;
  bool hasHeightForWidth() const override;

protected:
  void paintEvent(QPaintEvent*) override;
  void initStyleOption(QStyleOptionCommandLinkButton* option) const;
};
} // namespace oclero::qlementine
