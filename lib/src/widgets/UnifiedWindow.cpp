// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include <oclero/qlementine/widgets/UnifiedWindow.hpp>

#include "utils/window/WindowBackend.hpp"

#include <QEvent>
#include <QPainter>
#include <QPointer>
#include <QVBoxLayout>

namespace oclero::qlementine {
struct UnifiedWindow::Impl {
  TitleBar* titleBar{ nullptr };
  QVBoxLayout* layout{ nullptr };
  QPointer<QWidget> centralWidget;
  std::unique_ptr<WindowBackend> backend;
  bool shown{ false };
};

UnifiedWindow::UnifiedWindow(QWidget* parent)
  : QWidget(parent, Qt::Window)
  , _impl(std::make_unique<Impl>()) {
  // Layout: title bar on top, central widget below.
  _impl->layout = new QVBoxLayout(this);
  _impl->layout->setContentsMargins(0, 0, 0, 0);
  _impl->layout->setSpacing(0);
  _impl->titleBar = new TitleBar(this);
  _impl->layout->addWidget(_impl->titleBar);

  // Native window configuration, before anything is shown.
  _impl->backend = createWindowBackend(*this);
  _impl->backend->prepareWindow();

  // Title bar requests.
  connect(_impl->titleBar, &TitleBar::closeRequested, this, &QWidget::close);
  connect(_impl->titleBar, &TitleBar::minimizeRequested, this, &QWidget::showMinimized);
  connect(_impl->titleBar, &TitleBar::maximizeRequested, this, [this]() {
    if (isMaximized()) {
      showNormal();
    } else {
      showMaximized();
    }
  });
  connect(_impl->titleBar, &TitleBar::moveRequested, this, [this]() {
    _impl->backend->startMove();
  });
  connect(_impl->titleBar, &TitleBar::doubleClicked, this, [this]() {
    _impl->backend->handleTitleBarDoubleClick();
  });
  connect(_impl->titleBar, &TitleBar::menuRequested, this, [this](const QPoint& globalPos) {
    _impl->backend->showSystemMenu(globalPos);
  });
}

UnifiedWindow::~UnifiedWindow() {
  _impl->backend.reset();
}

TitleBar* UnifiedWindow::titleBar() const {
  return _impl->titleBar;
}

QWidget* UnifiedWindow::centralWidget() const {
  return _impl->centralWidget.data();
}

void UnifiedWindow::setCentralWidget(QWidget* widget) {
  if (widget == _impl->centralWidget.data()) {
    return;
  }

  if (_impl->centralWidget) {
    _impl->layout->removeWidget(_impl->centralWidget.data());
    _impl->centralWidget->deleteLater();
  }

  _impl->centralWidget = widget;
  if (widget) {
    _impl->layout->addWidget(widget, 1);
  }
}

bool UnifiedWindow::event(QEvent* evt) {
  const auto result = QWidget::event(evt);

  if (_impl && _impl->backend) {
    switch (evt->type()) {
      case QEvent::WindowTitleChange:
        _impl->titleBar->setTitle(windowTitle());
        break;
      case QEvent::WinIdChange:
        _impl->backend->attachNativeWindow();
        break;
      case QEvent::Show:
        if (!_impl->shown) {
          _impl->shown = true;
          _impl->backend->attachNativeWindow();
        }
        break;
      default:
        break;
    }

    _impl->backend->handleEvent(evt);
  }

  return result;
}

void UnifiedWindow::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  if (!_impl->backend->paintFrame(painter)) {
    painter.fillRect(rect(), palette().window());
  }
}
} // namespace oclero::qlementine
