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

#include <oclero/qlementine/widgets/Action.hpp>

namespace oclero::qlementine {
Action::Action(QObject* parent)
  : QAction(parent) {}

Action::Action(const QString& text, QObject* parent)
  : QAction(text, parent) {}

Action::Action(const QIcon& icon, const QString& text, QObject* parent)
  : QAction(icon, text, parent) {}

Action::Action(const QKeySequence& shortcut, QObject* parent)
  : QAction(parent) {
  setShortcut(shortcut);
  setShortcutContext(Qt::ApplicationShortcut);
}

Action::Action(const QKeySequence& shortcut, Qt::ShortcutContext shortcutContext, QObject* parent)
  : QAction(parent) {
  setShortcut(shortcut);
  setShortcutContext(shortcutContext);
}

Action::Action(
  const QString& text, const QKeySequence& shortcut, const Qt::ShortcutContext shortcutContext, QObject* parent)
  : QAction(text, parent) {
  setShortcut(shortcut);
  setShortcutContext(shortcutContext);
}

Action::Action(const QString& text, const QKeySequence& shortcut, QObject* parent)
  : QAction(text, parent) {
  setShortcut(shortcut);
  setShortcutContext(Qt::ApplicationShortcut);
}

Action::Action(const QIcon& icon, const QString& text, const QKeySequence& shortcut, QObject* parent)
  : QAction(icon, text, parent) {
  setShortcut(shortcut);
  setShortcutContext(Qt::ApplicationShortcut);
}

Action::Action(const QIcon& icon, const QString& text, const QKeySequence& shortcut,
  const Qt::ShortcutContext shortcutContext, QObject* parent)
  : QAction(icon, text, parent) {
  setShortcut(shortcut);
  setShortcutContext(shortcutContext);
}

void Action::setCallback(const std::function<void()>& cb) {
  _triggeredCb = cb;
  QObject::disconnect(_triggerdConnection);
  _triggerdConnection = QObject::connect(this, &QAction::triggered, this, _triggeredCb);
}

void Action::setEnabledPredicate(const std::function<bool()>& cb) {
  _updateEnabledCb = cb;
  updateEnabled();
}

void Action::setCheckedPredicate(const std::function<bool()>& cb) {
  _updateCheckedCb = cb;
  updateChecked();
}

void Action::setCheckablePredicate(const std::function<bool()>& cb) {
  _updateCheckableCb = cb;
  updateCheckable();
}

void Action::setVisiblePredicate(const std::function<bool()>& cb) {
  _updateVisibleCb = cb;
  updateVisible();
}

void Action::updateEnabled() {
  if (_updateEnabledCb) {
    setEnabled(_updateEnabledCb());
  }
}

void Action::updateChecked() {
  if (_updateCheckedCb) {
    setChecked(_updateCheckedCb());
  }
}

void Action::updateCheckable() {
  if (_updateCheckableCb) {
    setCheckable(_updateCheckableCb());
  }
}

void Action::updateVisible() {
  if (_updateVisibleCb) {
    setVisible(_updateVisibleCb());
  }
}

void Action::update() {
  updateEnabled();
  updateCheckable();
  updateChecked();
  updateVisible();
}

bool Action::shortcutEditable() const {
  return _shortcutEditable;
}

void Action::setShortcutEditable(bool editable) {
  if (editable != _shortcutEditable) {
    _shortcutEditable = editable;
    emit shortcutEditableChanged();
    emit changed();
  }
}

const QKeySequence& Action::userShortcut() const {
  return _userShortcut;
}

void Action::setUserShortcut(const QKeySequence& shortcut) {
  if (_shortcutEditable && shortcut != _userShortcut) {
    if (!_shortcutEditedByUser) {
      //  Save the default shortcut.
      _defaultShortcut = this->shortcut();
      _shortcutEditedByUser = true;
      emit shortcutEditedByUserChanged();
    }

    _userShortcut = shortcut;

    // Set the new shortcut on the action.
    setShortcut(_userShortcut);

    emit userShortcutChanged();
  }
}

void Action::resetShortcut() {
  if (_shortcutEditedByUser) {
    _userShortcut = {};
    _shortcutEditedByUser = false;

    emit userShortcutChanged();
    emit shortcutEditedByUserChanged();
    setShortcut(_defaultShortcut);
  }
}

bool Action::shortcutEditedByUser() const {
  return _shortcutEditedByUser;
}

const QString& Action::description() const {
  return _description;
}

void Action::setDescription(const QString& description) {
  if (description != _description) {
    _description = description;
    emit descriptionChanged();
    emit changed();
  }
}
} // namespace oclero::qlementine
