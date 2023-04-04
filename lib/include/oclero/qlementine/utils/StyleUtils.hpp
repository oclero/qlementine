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

#include <QWidget>

namespace oclero::qlementine {
/// Hover events (enter/leave events) are disabled by default on widgets.
/// However, some widgets need them.
bool shouldHaveHoverEvents(const QWidget* w);

/// Mouse tracking events (mouse moved events) are disabled by default on widgets.
/// However, some widgets need them.
bool shouldHaveMouseTracking(const QWidget* w);

/// Should the widget text be displayed in bold.
bool shouldHaveBoldFont(const QWidget* w);

/// Focus border outside the widget.
bool shouldHaveExternalFocusFrame(const QWidget* w);

/// Should the widget be focusable only with Tab.
bool shouldHaveTabFocus(const QWidget* w);

/// Should we prevent the widget to resize vertically.
bool shouldNotBeVerticallyCompressed(const QWidget* w);

/// Horizontal paddings (left, right) are different according to the content of the widget.
std::tuple<int, int> getHPaddings(const bool hasIcon, const bool hasText, const bool hasIndicator, const int padding);

/// Should the widget not receive wheel events when not focused.
bool shouldNotHaveWheelEvents(const QWidget* w);
} // namespace oclero::qlementine
