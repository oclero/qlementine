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

#include <oclero/qlementine/csd/WindowsTitleMenuBarProxyStyle.hpp>

#include <QStyleOptionMenuItem>

namespace oclero::qlementine {
WindowsTitleMenuBarProxyStyle::WindowsTitleMenuBarProxyStyle(QStyle* parent)
  : QProxyStyle(parent) {}

QSize WindowsTitleMenuBarProxyStyle::sizeFromContents(
  QStyle::ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const {
  if (type == CT_MenuBar) {
    const auto result = QProxyStyle::sizeFromContents(type, option, size, widget);
    return QSize{ result.width(), 30 };
  } else if (type == QStyle::ContentsType::CT_MenuBarItem) {
    const auto result = QProxyStyle::sizeFromContents(type, option, size, widget);
    return QSize{ result.width(), 30 };
  }
  return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void WindowsTitleMenuBarProxyStyle::drawControl(
  QStyle::ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
  if (element == CE_MenuBarItem) {
    const auto* menuItemOption = qstyleoption_cast<const QStyleOptionMenuItem*>(option);
    if (!menuItemOption)
      return;

    const auto& state = menuItemOption->state;
    const auto isNormalState = !state.testFlag(State_MouseOver) && !state.testFlag(State_Selected)
                               && !state.testFlag(State_Sunken) && state.testFlag(State_Enabled);

    // Draw only text in default/normal mode.
    if (isNormalState) {
      const auto maybeHasAltKeyNavFocus = state & State_Selected && state & State_HasFocus;
      auto isSelected = state & State_Selected && state & State_Sunken;
      if (!isSelected && maybeHasAltKeyNavFocus && widget) {
        isSelected = widget->hasFocus();
      }

      const auto textRole = isSelected ? QPalette::HighlightedText : QPalette::Text;
      auto textFlags = Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextDontClip | Qt::TextSingleLine;
      if (proxy()->styleHint(SH_UnderlineShortcut, menuItemOption, widget)) {
        textFlags |= Qt::TextShowMnemonic;
      }
      if (widget && !widget->hasFocus()) {
        textFlags |= Qt::TextHideMnemonic;
      }
      proxy()->drawItemText(painter, option->rect, textFlags, menuItemOption->palette,
        menuItemOption->state & State_Enabled, menuItemOption->text, textRole);
      return;
    }
  } else if (element == CE_MenuBarEmptyArea) {
    // Don't draw anything.
    return;
  }

  return QProxyStyle::drawControl(element, option, painter, widget);
}

void WindowsTitleMenuBarProxyStyle::drawPrimitive(
  PrimitiveElement pe, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
  if (pe == QStyle::PE_PanelMenuBar) {
    // Don't draw anything.
    return;
  }
  return QProxyStyle::drawPrimitive(pe, option, painter, widget);
}

int WindowsTitleMenuBarProxyStyle::pixelMetric(PixelMetric m, const QStyleOption* opt, const QWidget* w) const {
  if (m == PM_MenuBarPanelWidth) {
    return 0;
  }
  return QProxyStyle::pixelMetric(m, opt, w);
}
} // namespace oclero::qlementine
