#pragma once

#include <QMainWindow>

namespace oclero::qlementine {
class QlementineStyle;
}

namespace oclero::qlementine::sandbox {
class SandboxWindow : public QMainWindow {
public:
  SandboxWindow(QWidget* parent = nullptr);
  ~SandboxWindow();

  void setCustomStyle(QlementineStyle* style);

  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl{};
};
} // namespace oclero::qlementine::sandbox
