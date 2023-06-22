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

#pragma once

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/animation/WidgetAnimationManager.hpp>

#include <QTabBar>
#include <QToolButton>
#include <QAbstractItemView>
#include <QCommandLinkButton>

namespace oclero::qlementine {
class LineEditButtonEventFilter : public QObject {
public:
  LineEditButtonEventFilter(QlementineStyle& style, WidgetAnimationManager& animManager, QToolButton* button);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QlementineStyle& _style;
  WidgetAnimationManager& _animManager;
  QToolButton* _button{ nullptr };
};

class CommandLinkButtonPaintEventFilter : public QObject {
public:
  CommandLinkButtonPaintEventFilter(
    QlementineStyle& style, WidgetAnimationManager& animManager, QCommandLinkButton* button);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QlementineStyle& _style;
  WidgetAnimationManager& _animManager;
  QCommandLinkButton* _button{ nullptr };
};

class MouseWheelBlockerEventFilter : public QObject {
public:
  MouseWheelBlockerEventFilter(QWidget* widget);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QWidget* _widget{ nullptr };
};

class TabBarEventFilter : public QObject {
public:
  TabBarEventFilter(QlementineStyle& style, QTabBar* tabBar);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QTabBar* _tabBar{ nullptr };
  QToolButton* _leftButton{ nullptr };
  QToolButton* _rightButton{ nullptr };
};

class MenuEventFilter : public QObject {
public:
  MenuEventFilter(QMenu* menu);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QMenu* _menu{ nullptr };
};

class ComboboxItemViewFilter : public QObject {
public:
  ComboboxItemViewFilter(QAbstractItemView* view);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QAbstractItemView* _view{ nullptr };
};
} // namespace oclero::qlementine
