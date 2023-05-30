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

#include <QMetaType>

namespace oclero::qlementine {
/// Color family to highlght or not the widget.
enum class ColorRole {
  Neutral,
  Primary,
};

/// Mouse interaction state.
enum class MouseState {
  Transparent,
  Normal,
  Hovered,
  Pressed,
  Disabled,
};

/// Is the widget checked or not.
enum class CheckState {
  NotChecked,
  Checked,
  Indeterminate,
};

/// Has the widget keyboard focus or not.
enum class FocusState {
  NotFocused,
  Focused,
};

/// Is the list element the current item.
enum class ActiveState {
  NotActive,
  Active,
};

/// Is the widget selected or not.
enum class SelectionState {
  NotSelected,
  Selected,
};

/// Does the ListView row need to be painted in alternate color or not.
enum class AlternateState {
  NotAlternate,
  Alternate,
};

/// Does the button is the default button.
enum class DefaultState {
  NotDefault,
  Default,
};

/// Feedback status displayed to the user.
enum class Status {
  Default,
  Info,
  Success,
  Warning,
  Error,
};

enum class TextRole : int {
  Caption = -1,
  Default = 0,
  H1,
  H2,
  H3,
  H4,
  H5,
};

} // namespace oclero::qlementine

Q_DECLARE_METATYPE(oclero::qlementine::Status);
