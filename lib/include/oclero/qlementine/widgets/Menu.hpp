#pragma once

#include <functional>

#include <QMenu>
#include <QAction>

namespace oclero::qlementine {
class Menu : public QMenu {
  Q_OBJECT

public:
  using QMenu::QMenu;

  void setEnabledPredicate(const std::function<bool()>& cb);
  void setVisiblePredicate(const std::function<bool()>& cb);

  void updateEnabled();
  void updateVisible();
  void update();

private:
  std::function<bool()> _updateEnabledCb;
  std::function<bool()> _updateVisibleCb;
};
} // namespace oclero::qlementine
