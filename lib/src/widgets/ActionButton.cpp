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

#include <oclero/qlementine/widgets/ActionButton.hpp>

#include <QAction>

namespace oclero::qlementine {

ActionButton::ActionButton(QWidget* parent)
  : QPushButton(parent) {
  setDefault(true);
  setFocusPolicy(Qt::NoFocus);
}

ActionButton::~ActionButton() = default;

void ActionButton::setAction(QAction* action) {
  // Disconnect from previous action if any
  if (_action && (_action != action)) {
    disconnect(_action, &QAction::changed, this, &ActionButton::updateFromAction);
    disconnect(this, &ActionButton::clicked, _action, &QAction::trigger);
  }

  _action = action;
  updateFromAction();
  connect(_action, &QAction::changed, this, &ActionButton::updateFromAction);
  connect(this, &ActionButton::clicked, _action, &QAction::trigger);
}

void ActionButton::updateFromAction() {
  setText(_action->text());
  setStatusTip(_action->statusTip());
  setToolTip(_action->toolTip());
  setIcon(_action->icon());
  setEnabled(_action->isEnabled());
  setCheckable(_action->isCheckable());
  setChecked(_action->isChecked());
}

} // namespace oclero::qlementine
