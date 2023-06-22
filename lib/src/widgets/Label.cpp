// MIT License
//
// Copyright (c) 2023 Olivier Clero
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <oclero/qlementine/widgets/Label.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/QlementineStyleOption.hpp>
#include <QApplication>

namespace oclero::qlementine {
Label::Label(QWidget* parent)
  : QLabel(parent) {
  updatePaletteFromTheme();
  qApp->installEventFilter(this);
}

Label::Label(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  updatePaletteFromTheme();
  qApp->installEventFilter(this);
}

Label::Label(const QString& text, TextRole role, QWidget* parent)
  : QLabel(text, parent)
  , _role(role) {
  updatePaletteFromTheme();
  qApp->installEventFilter(this);
}

Label::~Label() {}

TextRole Label::role() const {
  return _role;
}

void Label::setRole(TextRole role) {
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

bool Label::eventFilter(QObject* obj, QEvent* e) {
  if (e->type() == QEvent::Type::PaletteChange) {
    updatePaletteFromTheme();
  }
  return QLabel::eventFilter(obj, e);
}

void Label::updatePaletteFromTheme() {
  if (const auto* qlementineStyle = qobject_cast<QlementineStyle*>(style())) {
    const auto& font = qlementineStyle->fontForTextRole(_role);
    const auto palette = qlementineStyle->paletteForTextRole(_role);
    setFont(font);
    setPalette(palette);
    updateGeometry();
    update(contentsRect());
  }
}
} // namespace oclero::qlementine
