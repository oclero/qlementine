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

#include <oclero/qlementine/widgets/SegmentedControl.hpp>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/widgets/RoundedFocusFrame.hpp>
#include <oclero/qlementine/utils/ImageUtils.hpp>

namespace oclero::qlementine {
const QColor& SegmentedControl::getBgColor(const Theme& theme) const {
  return isEnabled() ? theme.backgroundColorMain3 : theme.backgroundColorMain1;
}

const QColor& SegmentedControl::getItemBgColor(MouseState mouse, const Theme& theme) const {
  switch (mouse) {
    case MouseState::Hovered:
      return theme.adaptativeColor4;
    case MouseState::Pressed:
      return theme.adaptativeColor5;
    default:
      return theme.adaptativeColorTransparent;
  }
}

const QColor& SegmentedControl::getItemFgColor(MouseState mouse, bool selected, const Theme& theme) const {
  switch (mouse) {
    case MouseState::Hovered:
      return selected ? theme.primaryColorForegroundHovered : theme.neutralColor;
    case MouseState::Pressed:
      return selected ? theme.primaryColorForegroundPressed : theme.neutralColor;
    case MouseState::Disabled:
      return selected ? theme.primaryColorForegroundDisabled : theme.neutralColorDisabled;
    default:
      return selected ? theme.primaryColorForeground : theme.neutralColor;
  }
}

const QColor& SegmentedControl::getItemBadgeBgColor(MouseState mouse, bool selected, const Theme& theme) const {
  return mouse == MouseState::Disabled ? theme.adaptativeColor1
                                       : (selected ? theme.primaryColorForeground : theme.adaptativeColor2);
}

const QColor& SegmentedControl::getItemBadgeFgColor(MouseState mouse, bool selected, const Theme& theme) const {
  if (mouse == MouseState::Disabled)
    return selected ? theme.primaryColorDisabled : theme.neutralColorDisabled;
  else
    return selected ? theme.primaryColor : theme.neutralColor;
}

void SegmentedControl::initStyleOptionFocus(QStyleOptionFocusRoundedRect& opt) const {
  const auto* style = this->style();
  const auto deltaX = style->pixelMetric(QStyle::PM_FocusFrameHMargin, &opt, this);
  const auto deltaY = style->pixelMetric(QStyle::PM_FocusFrameVMargin, &opt, this);
  opt.rect =
    getFocusedItemRect().translated(deltaX, deltaY).marginsAdded({ deltaX / 2, deltaY / 2, deltaX / 2, deltaY / 2 });
  opt.radiuses = getItemRadius();
}
} // namespace oclero::qlementine
