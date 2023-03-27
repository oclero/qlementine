#pragma once

#include <QPushButton>

namespace oclero::qlementine {

class ActionButton : public QPushButton {
  Q_OBJECT

public:
  ActionButton(QWidget* parent = nullptr);
  virtual ~ActionButton();

public:
  void setAction(QAction* action);
  void updateFromAction();

private:
  QAction* _action{ nullptr };
};
} // namespace oclero::qlementine
