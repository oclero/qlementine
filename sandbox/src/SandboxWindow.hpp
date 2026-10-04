// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <QMainWindow>

namespace oclero::qlementine {
class QlementineStyle;
class ThemeManager;
} // namespace oclero::qlementine

namespace oclero::qlementine::sandbox {
class SandboxWindow : public QMainWindow {
public:
  explicit SandboxWindow(ThemeManager* themeManager = nullptr, QWidget* parent = nullptr);
  ~SandboxWindow() override;

  bool eventFilter(QObject* watched, QEvent* event) override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl{};
};
} // namespace oclero::qlementine::sandbox
