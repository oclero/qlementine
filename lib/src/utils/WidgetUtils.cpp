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

#include <oclero/qlementine/utils/WidgetUtils.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>

#include <QFrame>
#include <QBoxLayout>
#include <QLayout>

namespace oclero::qlementine {
QWidget* makeHorizontalLine(QWidget* parentWidget, int maxWidth) {
  const auto* qlementineStyle =
    parentWidget ? qobject_cast<qlementine::QlementineStyle*>(parentWidget->style()) : nullptr;
  const auto lineThickness = qlementineStyle ? qlementineStyle->theme().borderWidth : 1;

  auto* line = new QFrame(parentWidget);
  line->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  line->setFixedHeight(lineThickness);
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Plain);

  if (maxWidth >= 0) {
    line->setMaximumWidth(maxWidth);
  }

  return line;
}

QWidget* makeVerticalLine(QWidget* parentWidget, int maxHeight) {
  const auto* qlementineStyle =
    parentWidget ? qobject_cast<qlementine::QlementineStyle*>(parentWidget->style()) : nullptr;
  const auto lineThickness = qlementineStyle ? qlementineStyle->theme().borderWidth : 1;

  auto* line = new QFrame(parentWidget);
  line->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
  line->setFixedWidth(lineThickness);
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Plain);

  if (maxHeight >= 0) {
    line->setMaximumHeight(maxHeight);
  }

  return line;
}

void centerWidget(QWidget* widget, QWidget* host) {
  if (!host)
    host = widget->parentWidget();

  if (host) {
    const auto& hostRect = host->geometry();
    widget->move(hostRect.center() - widget->rect().center());
  } else {
    const auto screenGeometry = QGuiApplication::screens()[0]->geometry();
    const auto x = (screenGeometry.width() - widget->width()) / 2;
    const auto y = (screenGeometry.height() - widget->height()) / 2;
    widget->move(x, y);
  }
}
QMargins getDefaultMargins(const QStyle* style) {
  if (!style)
    return { 0, 0, 0, 0 };

  const auto paddingLeft = style->pixelMetric(QStyle::PM_LayoutLeftMargin);
  const auto paddingRight = style->pixelMetric(QStyle::PM_LayoutRightMargin);
  const auto paddingTop = style->pixelMetric(QStyle::PM_LayoutTopMargin);
  const auto paddingBottom = style->pixelMetric(QStyle::PM_LayoutBottomMargin);
  const auto contentsMargins = QMargins{ paddingLeft, paddingTop, paddingRight, paddingBottom };
  return contentsMargins;
}
qreal getDpi(const QWidget* widget) {
  if (widget) {
    if (const auto* screen = widget->screen()) {
      return screen->logicalDotsPerInch();
    }
  }
  return 72.;
}

QWindow* getWindow(const QWidget* widget) {
  if (widget) {
    if (auto* window = widget->window()) {
      return window->windowHandle();
    }
  }
  return nullptr;
}

void clearLayout(QLayout* layout) {
  if (!layout)
    return;

  QLayoutItem* item;
  while ((item = layout->takeAt(0))) {
    if (item->layout()) {
      clearLayout(item->layout());
      delete item->layout();
    }
    if (item->widget()) {
      delete item->widget();
    }
    delete item;
  }
}
} // namespace oclero::qlementine
