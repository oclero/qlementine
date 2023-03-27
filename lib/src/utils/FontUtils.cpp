#include <oclero/qlementine/utils/FontUtils.hpp>

namespace oclero::qlementine {
double pointSizeToPixelSize(double pointSize, double dpi) {
  return pointSize / 72. * dpi;
}

double pixelSizeToPointSize(double pixelSize, double dpi) {
  return pixelSize * 72. / dpi;
}
} // namespace oclero::qlementine
