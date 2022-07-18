#pragma once

#include <QWidget>
#include <QScreen>
#include <QGuiApplication>

namespace oclero::qlementine {
QWidget* makeVerticalLine(QWidget* parentWidget, int maxHeight = -1);
QWidget* makeHorizontalLine(QWidget* parentWidget, int maxWidth = -1);

void centerWidget(QWidget* widget, QWidget* host = nullptr);

QMargins getDefaultMargins(const QStyle* style);

qreal getDpi(const QWidget* widget);

QWindow* getWindow(const QWidget* widget);

void clearLayout(QLayout* layout);
} // namespace oclero::qlementine
