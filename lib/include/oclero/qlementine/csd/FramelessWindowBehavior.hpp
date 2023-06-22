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

#include <QAbstractNativeEventFilter>
#include <QMargins>
#include <QSet>
#include <QWidget>

namespace oclero::qlementine {
/**
 * @brief Allows to hide the title bar in Windows' app window. NB: Windows only.
 */
class FramelessWindowBehavior
  : public QObject
  , public QAbstractNativeEventFilter {
  Q_OBJECT

  Q_PROPERTY(QMargins resizeMargins READ resizeMargins WRITE setResizeMargins NOTIFY resizeMarginsChanged)
  Q_PROPERTY(QMargins maximizedMargins READ maximizedMargins WRITE setMaximizedMargins NOTIFY maximizedMarginsChanged)
  Q_PROPERTY(int titleBarHeight READ titleBarHeight WRITE setTitleBarHeight NOTIFY titleBarHeightChanged)
  Q_PROPERTY(
    int systemMenuAreaWidth READ systemMenuAreaWidth WRITE setSystemMenuAreaWidth NOTIFY systemMenuAreaWidthChanged)

public:
  /**
   * @brief Constructs a FramelessWindowBehavior and installs it on its parent.
   * @param parent The widget to install the behavior on.
   */
  explicit FramelessWindowBehavior(QWidget* parent = nullptr);

  /**
   * @brief Destructs theFramelessWindowBehavior and uninstall it from its
   * parent.
   */
  ~FramelessWindowBehavior();

  /**
   * @brief Area around the window where the user can click to resize it.
   * @return The margins {left, top, right, bottom}, in pixels.
   */
  const QMargins& resizeMargins() const;
  void setResizeMargins(const QMargins& margins);
  Q_SIGNAL void resizeMarginsChanged();

  /**
   * @brief When the window is maximized, the title bar's height is smaller.
   * @return The difference margins {left, top, right, bottom}, in pixels, when
   * the window is maximized.
   */
  const QMargins& maximizedMargins() const;
  void setMaximizedMargins(const QMargins& margins);
  Q_SIGNAL void maximizedMarginsChanged();

  /**
   * @brief Window title bar's height when not maximized.
   * @return The title bar's height in pixels.
   */
  int titleBarHeight() const;
  void setTitleBarHeight(const int height);
  Q_SIGNAL void titleBarHeightChanged();

  /**
   * @brief Registers the widget as being interactive, i.e. not part of the
   * draggable area of the title bar.
   * @param widget The widget to register.
   */
  void addInteractiveWidget(QWidget* widget);
  void removeInteractiveWidget(QWidget* widget);

  /**
   * @brief Registers the widget as being non-interactive, i.e part of the
   * draggable area of the title bar.
   * @param widget The widget to register.
   */
  void addNonInteractiveWidget(QWidget* widget);
  void removeNonInteractiveWidget(QWidget* widget);

  /**
   * @brief The area, usually in the top left part of the title bar, with an
   * icon, where the user can make the system menu appear with a single mouse
   * press.
   * @return The area reserved for the system menu.
   */
  int systemMenuAreaWidth() const;
  void setSystemMenuAreaWidth(int area);
  Q_SIGNAL void systemMenuAreaWidthChanged();

  /**
   * @brief Shows the window's system menu at the position in parameter (relative
   * to window coordinates).
   * @param mousePos Position relative to the window coordinates.
   */
  void showSystemMenu(const QPoint& mousePos);

signals:
  /**
   * @brief Signal called when the DPI or scale changed.
   */
  void scaleFactorChanged();

protected:
  /**
   * @brief Used to filter the events on the parent window (QWidget).
   * @param obj The parent window (QWidget) in this case.
   * @param evt The event.
   * @return true if handled, false otherwise.
   */
  bool eventFilter(QObject* obj, QEvent* evt) override;

  /**
   * @brief Used to filter the events on the native parent window (HWND wrapped
   * by the parent QWindow).
   * @param eventType Native event type.
   * @param message Native event object.
   * @param result LRESULT pointer (Windows-only).
   * @return true if handled, false otherwise.
   */
  bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

private:
  /**
   * @brief Installs the behavior, by registering to the necessary events.
   */
  void install();

  /**
   * @brief Uninstall the behavior, by unregistering to the necessary events.
   */
  void uninstall();

  /**
   * @brief Updates the native window properties.
   */
  void updateNativeWindowProperties();

  /**
   * @brief Hit-tests the position to determine which part of the window does
   * contain the position in paramter.
   * @param mousePos The position to hit-test.
   * @return Value that tells which part of the window does contain mousePs
   */
  int hitTest(const QPoint& mousePos) const;

  /**
   * @brief Tests if an interactive widget is under the position {x,y}. In this
   * case, it means the position should be handled by the widget itself, and not
   * the native title bar.
   * @param mousePos The position to hit-test.
   * @return true if the position can be handled by the native title bar.
   */
  bool hitTestNativeTitleBar(const QPoint& mousePos) const;

  /**
   * @brief Returns the available geometry avialable on the window's screen.
   * @return The available geometry on the screen.
   */
  QRect availableGeometry() const;

  /**
   * @brief Computes the area that trigger the system menu when clicked inside.
   * @return The area reserved for the system menu.
   */
  QRect systemMenuArea() const;

  /**
   * @brief Sets the correct flags on the native window according to the flags
   * set on the QWindow.
   * @param window The QWindow to use to adapt the native window.
   */
  static void updateNativeWindowProperties(QWindow* const window);

  /**
   * @brief Determine if the HWND wrapped by the QWindow corresponds to a
   * maximized window.
   * @param window The window to determine if maximized or not.
   * @return true if maximized, false otherwise.
   */
  static bool isMaximized(const QWindow* const window);

private:
  QWidget* _parentWindowWidget{ nullptr };
  QWindow* _parentWindowHandle{ nullptr };
  bool _installed{ false };
  QMargins _resizeMargins{ 6, 6, 6, 6 };
  QMargins _maximizedMargins{ 0, 0, 0, 0 };
  int _titleBarHeight{ 30 };
  QSet<QWidget*> _interactiveWidgets;
  QSet<QWidget*> _nonInteractiveWidgets;
  int _systemMenuAreaWidth{ 30 };
  double _scaleFactor{ 1.0 };
  bool _systemMenuWasVisible{ false };
};
} // namespace oclero::qlementine
