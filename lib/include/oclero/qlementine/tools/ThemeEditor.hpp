#pragma once

#include <memory>
#include <QWidget>

#include <oclero/qlementine/style/Theme.hpp>

namespace oclero::qlementine {
class ThemeEditor : public QWidget {
  Q_OBJECT

public:
  ThemeEditor(QWidget* parent = nullptr);
  ~ThemeEditor();

public:
  const Theme& theme() const;
  void setTheme(const Theme& theme);

signals:
  void themeChanged(const oclero::qlementine::Theme& theme);

private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
} // namespace oclero::qlementine
