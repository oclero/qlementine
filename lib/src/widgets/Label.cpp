#include <oclero/qlementine/widgets/Label.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/QlementineStyleOption.hpp>
#include <QApplication>

namespace oclero::qlementine {
Label::Label(QWidget* parent)
  : QLabel(parent) {
  qApp->installEventFilter(this);
}

Label::Label(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  qApp->installEventFilter(this);
}

Label::~Label() {}

Theme::TextRole Label::role() const {
  return _role;
}

void Label::setRole(Theme::TextRole role) {
  if (role != _role) {
    _role = role;
    // Change text font, size and color.
    updatePaletteFromTheme();
    emit roleChanged();
  }
}

bool Label::event(QEvent* e) {
  // Ensure the label repaints when the theme changes.
  if (e->type() == QEvent::Type::PaletteChange && !_changingPaletteFlag) {
    _changingPaletteFlag = true;
    updatePaletteFromTheme();
    _changingPaletteFlag = false;
  }
  return QLabel::event(e);
}

bool Label::eventFilter(QObject*, QEvent* e) {
  if (e->type() == QEvent::Type::PaletteChange) {
    updatePaletteFromTheme();
  }
  return false;
}

void Label::updatePaletteFromTheme() {
  if (const auto* qlementineStyle = qobject_cast<QlementineStyle*>(style())) {
    const auto& theme = qlementineStyle->theme();
    const auto& font = theme.fontForTextRole(_role);
    const auto palette = theme.paletteForTextRole(_role);
    setFont(font);
    setPalette(palette);
    updateGeometry();
    update(contentsRect());
  }
}
} // namespace oclero::qlementine
