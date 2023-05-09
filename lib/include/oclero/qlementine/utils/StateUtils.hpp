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

#include <oclero/qlementine/style/Theme.hpp>

#include <QStyle>
#include <QStyleOption>

namespace oclero::qlementine {
MouseState getMouseState(QStyle::State const& state);
MouseState getMouseState(bool const pressed, bool const hovered, bool const enabled);
MouseState getToolButtonMouseState(QStyle::State const& state);
MouseState getMenuItemMouseState(QStyle::State const& state);
MouseState getTabItemMouseState(QStyle::State const& state, const bool tabIsHovered);
ColorRole getColorRole(QStyle::State const& state, bool const isDefault);
ColorRole getColorRole(bool checked, bool const isDefault);
ColorRole getColorRole(CheckState const checked);
MouseState getSliderHandleState(QStyle::State const& state, QStyle::SubControls const activeSubControls);
MouseState getScrollBarHandleState(QStyle::State const& state, QStyle::SubControls const activeSubControls);
FocusState getFocusState(QStyle::State const& state);
CheckState getCheckState(QStyle::State const& state);
CheckState getCheckState(Qt::CheckState const& state);
CheckState getCheckState(bool checked);
ActiveState getActiveState(QStyle::State const& state);
SelectionState getSelectionState(QStyle::State const& state);
AlternateState getAlternateState(QStyleOptionViewItem::ViewItemFeatures const& state);
QStyle::State getState(const bool enabled, const bool hover, const bool pressed);
QIcon::Mode getIconMode(MouseState const mouse);
QIcon::State getIconState(CheckState const checked);
QPalette::ColorGroup getPaletteColorGroup(QStyle::State const& state);
QPalette::ColorGroup getPaletteColorGroup(MouseState const mouse);

QString mouseStateToString(MouseState const state);
QString focusStateToString(FocusState const state);
QString activeStateToString(ActiveState const state);
QString selectionStateToString(SelectionState const state);
QString checkStateToString(CheckState const state);
QString printState(QStyle::State const& state);
} // namespace oclero::qlementine
