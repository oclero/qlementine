#pragma once

#include <oclero/qlementine/widgets/FramelessWindow.hpp>

class CsdWindow : public oclero::qlementine::FramelessWindow {
  Q_OBJECT

public:
  explicit CsdWindow(QWidget* parent = nullptr);
  ~CsdWindow() = default;

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  void populateMenuBar(QMenuBar* menuBar);
  void setupUi();

private:
  QColor _backgroundColor{ 255, 192, 0 };
  bool _useDefaultColor{ false };
};
