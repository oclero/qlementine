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

#include <QString>
#include <QWidget>

#include <oclero/qlementine/csd/WindowsTitleBarCommon.hpp>

class QHBoxLayout;
class QMenuBar;
class QPushButton;
class QLabel;
class QAbstractButton;

namespace oclero::qlementine {
class WindowsTitleBarButton;
class SystemMenuButton;

class WindowsTitleBar : public QWidget {
  Q_OBJECT

  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(oclero::qlementine::SystemTheme buttonsSystemTheme READ buttonsSystemTheme WRITE setButtonsSystemTheme
      NOTIFY buttonsSystemThemeChanged)
  Q_PROPERTY(oclero::qlementine::SystemWindowType systemWindowType READ systemWindowType WRITE setSystemWindowType
      NOTIFY systemWindowTypeChanged)

public:
  explicit WindowsTitleBar(QWidget* parent = nullptr);

  QWidget* iconButton() const;
  QAbstractButton* minimizeButton() const;
  QAbstractButton* maximizeButton() const;
  QAbstractButton* closeButton() const;
  QMenuBar* menuBar() const;

  const QColor& backgroundColor() const;
  void setBackgroundColor(const QColor& color);
  Q_SIGNAL void backgroundColorChanged();

  SystemTheme buttonsSystemTheme() const;
  void setButtonsSystemTheme(const SystemTheme systemTheme);
  Q_SIGNAL void buttonsSystemThemeChanged();

  SystemWindowType systemWindowType() const;
  void setSystemWindowType(const SystemWindowType type);
  Q_SIGNAL void systemWindowTypeChanged();

private:
  void setupUi();
  void updateTitleGeometry();
  void updateWindowTitle();
  void minimizeWindow();
  void maximizeWindow();
  void closeWindow();

protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void showEvent(QShowEvent* event) override;
  bool event(QEvent* event) override;

signals:
  void systemMenuTriggered();

private:
  QColor _backgroundColor{ QColor::Invalid };
  QHBoxLayout* _rootLayout;
  SystemMenuButton* _iconButton;
  QMenuBar* _menuBar;
  WindowsTitleBarButton* _minimizeButton;
  WindowsTitleBarButton* _maximizeButton;
  WindowsTitleBarButton* _closeButton;
  QLabel* _titleLabel;
};
} // namespace oclero::qlementine
