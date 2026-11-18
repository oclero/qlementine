// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <oclero/qlementine/qlementine_export.h>
#include <oclero/qlementine/widgets/TitleBar.hpp>

#include <QWidget>

#include <memory>

namespace oclero::qlementine {
/// A top-level window whose content extends into the title bar area.
/// macOS keeps the traffic lights at the top left, Windows keeps the caption buttons at the top right and
/// supports snap layouts, and Linux draws client-side decorations following the user's button layout.
class QLEMENTINE_EXPORT UnifiedWindow : public QWidget {
  Q_OBJECT

public:
  explicit UnifiedWindow(QWidget* parent = nullptr);
  ~UnifiedWindow() override;

  TitleBar* titleBar() const;

  /// The window takes ownership of the widget.
  QWidget* centralWidget() const;
  void setCentralWidget(QWidget* widget);

protected:
  bool event(QEvent* evt) override;
  void paintEvent(QPaintEvent* evt) override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
} // namespace oclero::qlementine
