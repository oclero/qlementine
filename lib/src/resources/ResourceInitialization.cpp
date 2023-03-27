#include <oclero/qlementine/resources/ResourceInitialization.hpp>

#include <qglobal.h>

// This must be done outside of any namespace.
void qlementineResourceInitialization() {
  // Loads the QRC content.
  Q_INIT_RESOURCE(qlementine);

#if defined(WIN32)
  Q_INIT_RESOURCE(qlementine_fonts_windows);
#else
  Q_INIT_RESOURCE(qlementine_fonts);
#endif
}

namespace oclero::qlementine::resources {
void initializeResources() {
  qlementineResourceInitialization();
}
} // namespace oclero::qlementine::resources
