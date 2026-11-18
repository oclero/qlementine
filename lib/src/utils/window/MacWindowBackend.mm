// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#include "WindowBackend.hpp"

#include <oclero/qlementine/widgets/TitleBar.hpp>
#include <oclero/qlementine/widgets/UnifiedWindow.hpp>

#include <QEvent>
#include <QObject>
#include <QWidget>

#include <AppKit/AppKit.h>

#include <cmath>

namespace oclero::qlementine {
namespace {
class MacWindowBackend final
  : public QObject
  , public WindowBackend {
public:
  explicit MacWindowBackend(UnifiedWindow& window)
    : QObject(nullptr)
    , WindowBackend(window)
    , _observers([[NSMutableArray alloc] init]) {}

  ~MacWindowBackend() override {
    clearObservers();
  }

  void prepareWindow() override {
    // Qt::FramelessWindowHint would remove the traffic lights, and the Qt 6.9 expanded client area hints are not
    // reliable with QMainWindow, so only the native NSWindow configuration is used. Nothing to do before creation.
  }

  void attachNativeWindow() override {
    NSView* view = (__bridge NSView*)reinterpret_cast<void*>(_window.winId());
    NSWindow* window = view ? view.window : nil;
    if (!window) {
      return;
    }

    // Native window configuration. Qt may rewrite the style mask, so this is applied every time.
    window.styleMask |= NSWindowStyleMaskFullSizeContentView;
    window.titlebarAppearsTransparent = YES;
    window.titleVisibility = NSWindowTitleHidden;
    window.tabbingMode = NSWindowTabbingModeDisallowed;
    if (@available(macOS 11.0, *)) {
      window.titlebarSeparatorStyle = NSTitlebarSeparatorStyleNone;
    }

    // Notifications.
    if (_nsWindow != window) {
      clearObservers();
      _nsWindow = window;
      NSArray<NSNotificationName>* names = @[
        NSWindowWillEnterFullScreenNotification,
        NSWindowDidEnterFullScreenNotification,
        NSWindowWillExitFullScreenNotification,
        NSWindowDidExitFullScreenNotification,
        NSWindowDidResizeNotification,
        NSWindowDidChangeScreenNotification,
        NSWindowDidDeminiaturizeNotification,
      ];
      for (NSNotificationName name in names) {
        [_observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:name
                                                                                object:window
                                                                                 queue:[NSOperationQueue mainQueue]
                                                                            usingBlock:^(NSNotification*) {
                                                                              attachNativeWindow();
                                                                            }]];
      }
    }

    // Used to re-center the traffic lights when the title bar height changes.
    _window.titleBar()->installEventFilter(this);

    _window.titleBar()->setCaptionButtonsVisible(false);

    // Native fullscreen hides the traffic lights.
    if (window.styleMask & NSWindowStyleMaskFullScreen) {
      _window.titleBar()->setSystemInsets(0, 0);
      return;
    }

    // Vertically center the traffic lights in the title bar. They live in a private titlebar container view.
    NSButton* closeButton = [window standardWindowButton:NSWindowCloseButton];
    NSButton* minimizeButton = [window standardWindowButton:NSWindowMiniaturizeButton];
    NSButton* zoomButton = [window standardWindowButton:NSWindowZoomButton];
    const auto titleBarHeight = static_cast<CGFloat>(_window.titleBar()->height());
    NSView* titlebarView = closeButton.superview;
    NSView* container = titlebarView.superview;
    if (titleBarHeight > 0 && container && container.superview) {
      const NSRect parentBounds = container.superview.bounds;
      const CGFloat containerY = container.superview.isFlipped ? 0 : parentBounds.size.height - titleBarHeight;
      container.frame = NSMakeRect(0, containerY, parentBounds.size.width, titleBarHeight);
      titlebarView.frame = container.bounds;
      for (NSButton* button in @[ closeButton, minimizeButton, zoomButton ]) {
        if (button && button.superview == titlebarView) {
          [button setFrameOrigin:NSMakePoint(button.frame.origin.x, (titleBarHeight - button.frame.size.height) / 2)];
        }
      }
    }

    // Space reserved for the traffic lights, read from the actual button position.
    int leftInset = 0;
    if (zoomButton) {
      const NSRect zoomFrame = [zoomButton convertRect:zoomButton.bounds toView:nil];
      leftInset = static_cast<int>(std::ceil(NSMaxX(zoomFrame))) + 8;
    }
    _window.titleBar()->setSystemInsets(leftInset, 0);
  }

  void handleEvent(QEvent* evt) override {
    switch (evt->type()) {
      case QEvent::Show:
      case QEvent::Resize:
      case QEvent::WindowStateChange:
      case QEvent::ThemeChange:
      case QEvent::ScreenChangeInternal:
      case QEvent::ApplicationPaletteChange:
        attachNativeWindow();
        break;
      default:
        break;
    }
  }

  void handleTitleBarDoubleClick() override {
    NSWindow* window = _nsWindow;
    if (!window || (window.styleMask & NSWindowStyleMaskFullScreen)) {
      return;
    }
    NSString* action = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleActionOnDoubleClick"];
    if ([action isEqualToString:@"Minimize"]) {
      [window performMiniaturize:nil];
    } else if (![action isEqualToString:@"None"]) {
      [window performZoom:nil];
    }
  }

protected:
  bool eventFilter(QObject* watched, QEvent* evt) override {
    if (watched == _window.titleBar() && evt->type() == QEvent::Resize) {
      attachNativeWindow();
    }
    return false;
  }

private:
  void clearObservers() {
    for (id observer in _observers) {
      [[NSNotificationCenter defaultCenter] removeObserver:observer];
    }
    [_observers removeAllObjects];
  }

  NSMutableArray* _observers;
  NSWindow* __weak _nsWindow = nil;
};
} // namespace

std::unique_ptr<WindowBackend> createWindowBackend(UnifiedWindow& window) {
  return std::make_unique<MacWindowBackend>(window);
}
} // namespace oclero::qlementine
