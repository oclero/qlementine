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

#include <oclero/qlementine/widgets/CommandLinkButton.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/QlementineStyleOption.hpp>

#include <QStylePainter>

namespace oclero::qlementine {
QSize CommandLinkButton::sizeHint() const {
  if (qobject_cast<QlementineStyle*>(style())) {
    ensurePolished();
    QStyleOptionCommandLinkButton opt;
    initStyleOption(&opt);
    const auto ct = static_cast<QStyle::ContentsType>(QlementineStyle::CT_CommandButton);
    const auto minSize = style()->sizeFromContents(ct, &opt, { 0, 0 }, this);
    return minSize;
  } else {
    return QCommandLinkButton::sizeHint();
  }
}

int CommandLinkButton::heightForWidth(int w) const {
  return w;
}

bool CommandLinkButton::hasHeightForWidth() const {
  return false;
}

void CommandLinkButton::paintEvent(QPaintEvent* e) {
  if (qobject_cast<QlementineStyle*>(style())) {
    QStylePainter p(this);
    QStyleOptionCommandLinkButton opt;
    initStyleOption(&opt);
    const auto ce = static_cast<QStyle::ControlElement>(QlementineStyle::CE_CommandButton);
    p.drawControl(ce, opt);
  } else {
    QCommandLinkButton::paintEvent(e);
  }
}

void CommandLinkButton::initStyleOption(QStyleOptionCommandLinkButton* option) const {
  QCommandLinkButton::initStyleOption(option);
  const auto pm = static_cast<QStyle::PixelMetric>(QlementineStyle::PM_MediumIconSize);
  const auto iconExtent = style()->pixelMetric(pm, option, this);
  option->description = description();
  option->iconSize = QSize{ iconExtent, iconExtent };
}
} // namespace oclero::qlementine
