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

#include <oclero/qlementine/utils/StyleUtils.hpp>

#include <QAbstractButton>
#include <QComboBox>
#include <QMenuBar>
#include <QScrollBar>
#include <QSplitterHandle>
#include <QTabBar>
#include <QAbstractSlider>
#include <QLineEdit>
#include <QAbstractSpinBox>
#include <qabstractitemview.h>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QToolButton>

#include <oclero/qlementine/widgets/ColorButton.hpp>

namespace oclero::qlementine {
bool shouldHaveHoverEvents(const QWidget* w) {
  return qobject_cast<const QAbstractButton*>(w) || qobject_cast<const QComboBox*>(w)
         || qobject_cast<const QMenuBar*>(w) || qobject_cast<const QScrollBar*>(w)
         || qobject_cast<const QSplitterHandle*>(w) || qobject_cast<const QTabBar*>(w)
         || qobject_cast<const QAbstractSlider*>(w) || qobject_cast<const QLineEdit*>(w)
         || qobject_cast<const QAbstractSpinBox*>(w) || qobject_cast<const QAbstractItemView*>(w)
         || qobject_cast<const QGroupBox*>(w) || w->inherits("QDockSeparator") || w->inherits("QDockWidgetSeparator");
}

bool shouldHaveMouseTracking(const QWidget* w) {
  return qobject_cast<const QAbstractItemView*>(w);
}

bool shouldHaveBoldFont(const QWidget* w) {
  return qobject_cast<const QPushButton*>(w) || qobject_cast<const QToolButton*>(w);
}

bool shouldHaveExternalFocusFrame(const QWidget* w) {
  return (w && qobject_cast<const QAbstractButton*>(w) && !qobject_cast<const QTabBar*>(w->parentWidget()))
         || qobject_cast<const QComboBox*>(w) || qobject_cast<const QLineEdit*>(w)
         || (!qobject_cast<const QScrollBar*>(w) && qobject_cast<const QAbstractSlider*>(w))
         || qobject_cast<const QGroupBox*>(w);
}

bool shouldHaveTabFocus(const QWidget* w) {
  return w && (w->focusPolicy() == Qt::StrongFocus || w->focusPolicy() == Qt::ClickFocus)
         && (qobject_cast<const QAbstractButton*>(w) || qobject_cast<const QGroupBox*>(w));
}

bool shouldNotBeVerticallyCompressed(const QWidget* w) {
  return qobject_cast<const QAbstractButton*>(w) || qobject_cast<const QComboBox*>(w)
         || qobject_cast<const QLineEdit*>(w) || qobject_cast<const QAbstractSpinBox*>(w);
}

std::tuple<int, int> getHPaddings(const bool hasIcon, const bool hasText, const bool hasIndicator, const int padding) {
  if (hasText) {
    if (!hasIcon && hasIndicator)
      return { padding * 2, padding };
    else if (hasIcon && !hasIndicator)
      return { padding, padding * 2 };
    else if (hasIcon && hasIndicator)
      return { padding, padding };
    else
      return { padding * 2, padding * 2 };
  }
  return { padding, padding };
}

bool shouldNotHaveWheelEvents(const QWidget* w) {
  return (!qobject_cast<const QScrollBar*>(w) && qobject_cast<const QAbstractSlider*>(w))
         || qobject_cast<const QAbstractSpinBox*>(w);
}
} // namespace oclero::qlementine
