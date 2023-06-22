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

#include <oclero/qlementine/widgets/PopoverButton.hpp>

#include <oclero/qlementine/widgets/Popover.hpp>

#include <QPainter>
#include <QStyleOptionButton>

namespace oclero::qlementine {
PopoverButton::PopoverButton(QWidget* parent)
  : PopoverButton({}, {}, parent) {}

PopoverButton::PopoverButton(const QString& text, QWidget* parent)
  : PopoverButton(text, {}, parent) {}

PopoverButton::PopoverButton(const QString& text, const QIcon& icon, QWidget* parent)
  : QPushButton(icon, text, parent)
  , _popover(new Popover(this)) {
  _popover->setAnchorWidget(this);
  QObject::connect(this, &PopoverButton::clicked, this, [this]() {
    _popover->openPopover();
  });
  QObject::connect(_popover, &Popover::openedChanged, this, [this]() {
    update();
    emit popoverOpenedChanged(_popover->isOpened());
  });
  QObject::connect(_popover, &Popover::contentWidgetChanged, this, [this]() {
    emit popoverContentWidgetChanged();
  });
}

Popover* PopoverButton::popover() const {
  return _popover;
}

QWidget* PopoverButton::popoverContentWidget() const {
  return _popover->contentWidget();
}

void PopoverButton::setPopoverContentWidget(QWidget* widget) {
  _popover->setContentWidget(widget);
}

void PopoverButton::setPopoverOpened(bool opened) {
  _popover->setOpened(opened);
}

void PopoverButton::paintEvent(QPaintEvent* /*e*/) {
  const auto* style = this->style();
  QPainter p(this);

  auto font = this->font();
  font.setBold(false);
  p.setFont(font);

  // Background.
  QStyleOptionButton bgOpt;
  initStyleOption(&bgOpt);
  bgOpt.features.setFlag(QStyleOptionButton::ButtonFeature::HasMenu, true);
  const auto isPressed = bgOpt.state.testFlag(QStyle::State_Sunken);
  bgOpt.state.setFlag(QStyle::State_Sunken, isPressed || _popover->isOpened());
  style->drawControl(QStyle::CE_PushButtonBevel, &bgOpt, &p, this);

  // Foreground.
  QStyleOptionComboBox fgOpt;
  fgOpt.currentText = text();
  fgOpt.currentIcon = icon();
  fgOpt.editable = false;
  fgOpt.frame = false;
  fgOpt.iconSize = iconSize();
  fgOpt.subControls = QStyle::SC_ComboBoxFrame | QStyle::SC_ComboBoxArrow;
  fgOpt.activeSubControls = QStyle::SC_ComboBoxFrame;
  fgOpt.rect = bgOpt.rect;
  fgOpt.state = bgOpt.state;
  //fgOpt.fo
  style->drawControl(QStyle::CE_ComboBoxLabel, &fgOpt, &p, this);
}
} // namespace oclero::qlementine
