#include <oclero/qlementine/widgets/RoundedFocusFrame.hpp>

namespace oclero::qlementine {
const RadiusesF& RoundedFocusFrame::radiuses() const {
	return _radiuses;
}

void RoundedFocusFrame::setRadiuses(const RadiusesF& radiuses) {
	if (radiuses != _radiuses) {
		_radiuses = radiuses;
		emit radiusesChanged();
		update();
	}
}
} // namespace oclero::qlementine
