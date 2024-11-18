// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/animation/WidgetAnimationManager.hpp>

#include <QTabBar>
#include <QToolButton>
#include <QAbstractItemView>
#include <QCommandLinkButton>

class QFocusFrame;

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

// Works for both QTextEdit and QPlainTextEdit
class TextEditEventFilter : public QObject {
public:
  TextEditEventFilter(QAbstractScrollArea* textEdit);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QAbstractScrollArea* _textEdit{ nullptr };
};

class WidgetWithFocusFrameEventFilter : public QObject {
  Q_OBJECT
public:
  WidgetWithFocusFrameEventFilter(QWidget* widget);

  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  QWidget* _widget{ nullptr };
  QFocusFrame* _focusFrame{ nullptr };
};

} // namespace oclero::qlementine
