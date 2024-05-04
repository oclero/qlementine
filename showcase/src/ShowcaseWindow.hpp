#pragma once

#include <QMainWindow>

#include <memory>

namespace oclero::qlementine {
class QlementineStyle;
}

namespace oclero::qlementine::showcase {
class ShowcaseWindow : public QWidget {
public:
  ShowcaseWindow(QWidget* parent = nullptr);
  ~ShowcaseWindow();

  void setCustomStyle(QlementineStyle* style);

private:
  struct Impl;
  std::unique_ptr<Impl> _impl{};
};
} // namespace oclero::qlementine::showcase
