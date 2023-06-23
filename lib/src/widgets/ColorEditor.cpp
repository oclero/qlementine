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

#include <oclero/qlementine/widgets/ColorEditor.hpp>

#include <oclero/qlementine/widgets/LineEdit.hpp>
#include <oclero/qlementine/widgets/ColorButton.hpp>
#include <oclero/qlementine/utils/ColorUtils.hpp>

#include <QHBoxLayout>

namespace oclero::qlementine {

ColorEditor::ColorEditor(QWidget* parent)
  : QWidget(parent) {
  setup(Qt::black);
}

ColorEditor::ColorEditor(const QColor& color, QWidget* parent)
  : QWidget(parent) {
  setup(color);
}

void ColorEditor::setup(const QColor& initialColor) {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  auto* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  // Button to see and choose a RGBA color.
  _colorButton = new ColorButton(this);
  _colorButton->setColor(initialColor);
  layout->addWidget(_colorButton);

  // LineEdit with an hex representation of the RGBA color.
  _lineEdit = new LineEdit(this);
  _lineEdit->setPlaceholderText(QStringLiteral("#RRGGBBAA"));
  _lineEdit->setMaxLength(9);
  _lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  _lineEdit->ensurePolished();
  _lineEdit->setFixedWidth(_lineEdit->minimumSizeHint().width());
  syncLineEditFromButton();
  layout->addWidget(_lineEdit);

  // Connect widgets together.
  QObject::connect(_colorButton, &ColorButton::colorChanged, this, [this]() {
    syncLineEditFromButton();
    emit colorChanged();
  });

  QObject::connect(_lineEdit, &QLineEdit::editingFinished, this, [this]() {
    const auto colorOptional = qlementine::tryGetColorFromHexaString(_lineEdit->text());
    const auto validString = colorOptional.has_value();
    _lineEdit->setStatus(validString ? qlementine::Status::Default : qlementine::Status::Error);
    if (validString) {
      QSignalBlocker blocker(_colorButton);
      _colorButton->setColor(colorOptional.value());
    }
  });
}

const QColor& ColorEditor::color() const {
  return _colorButton->color();
}

void ColorEditor::setColor(const QColor& color) {
  _colorButton->setColor(color);
  // Sync even if the color hasn't changed, to clear any previous wrong string.
  syncLineEditFromButton();
}

void ColorEditor::syncLineEditFromButton() {
  QSignalBlocker blocker(_lineEdit);
  _lineEdit->setText(qlementine::toHexRGBA(_colorButton->color()));
}
} // namespace oclero::qlementine
