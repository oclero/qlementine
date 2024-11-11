#pragma once

#include <QMainWindow>

#include <memory>

namespace oclero::qlementine {
class QlementineStyle;
class ThemeManager;
} // namespace oclero::qlementine

namespace oclero::qlementine::showcase {
class ShowcaseWindow : public QWidget {
public:
  ShowcaseWindow(ThemeManager* themeManager, QWidget* parent = nullptr);
  ~ShowcaseWindow();

private:
  struct Impl;
  std::unique_ptr<Impl> _impl{};
};
} // namespace oclero::qlementine::showcase
