#pragma once

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
} // namespace oclero::qlementine
