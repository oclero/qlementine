// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <oclero/qlementine/widgets/UnifiedWindow.hpp>

#include <memory>

namespace oclero::qlementine {
class QlementineStyle;
class ThemeManager;
} // namespace oclero::qlementine

namespace oclero::qlementine::showcase {
class ShowcaseWindow : public UnifiedWindow {
public:
  explicit ShowcaseWindow(ThemeManager* themeManager = nullptr, QWidget* parent = nullptr);
  ~ShowcaseWindow() override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl{};
};
} // namespace oclero::qlementine::showcase
