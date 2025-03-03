// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/animation/WidgetAnimationManager.hpp>

#include <QTabBar>
#include <QToolButton>
#include <QListView>
#include <QComboBox>
#include <QCommandLinkButton>
#include <QPointer>

class QFocusFrame;

namespace oclero::qlementine {
class LineEditButtonEventFilter : public QObject {
public:
  LineEditButtonEventFilter(QlementineStyle* style, WidgetAnimationManager& animManager, QToolButton* button);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QPointer<QlementineStyle> _style;
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
  explicit MouseWheelBlockerEventFilter(QWidget* widget);

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
  explicit MenuEventFilter(QMenu* menu);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QMenu* _menu{ nullptr };

  // this is roughly copied from how QMenuPrivate::mouseDown detects whether the mouse is down
  bool _mousePressed{ false };
  QEvent *_mouseEventToNotFilter{}; // please do not dereference it, this is just to close properly the QMenu
};

class ComboboxItemViewFilter : public QObject {
public:
  ComboboxItemViewFilter(QComboBox* comboBox, QListView* view);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  void fixViewGeometry();
  QSize viewMinimumSizeHint() const;
  QComboBox* _comboBox{ nullptr };
  QListView* _view{ nullptr };
  int _initialMaxHeight{ 0 };
};

// Works for both QTextEdit and QPlainTextEdit
class TextEditEventFilter : public QObject {
public:
  explicit TextEditEventFilter(QAbstractScrollArea* textEdit);

  bool eventFilter(QObject* watchedObject, QEvent* evt) override;

private:
  QAbstractScrollArea* _textEdit{ nullptr };
};

class WidgetWithFocusFrameEventFilter : public QObject {
  Q_OBJECT
public:
  explicit WidgetWithFocusFrameEventFilter(QWidget* widget);

  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  QWidget* _widget{ nullptr };
  QFocusFrame* _focusFrame{ nullptr };
};

class LineEditMenuEventFilter : public QObject {
public:
  explicit LineEditMenuEventFilter(QWidget* parent);

protected:
  virtual bool eventFilter(QObject* obj, QEvent* evt) override;
};
} // namespace oclero::qlementine
