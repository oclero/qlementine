find_package(Qt${QT_DEFAULT_MAJOR_VERSION}
  COMPONENTS
    Core
    Widgets
  REQUIRED
)

add_executable(showcase WIN32 MACOSX_BUNDLE)
target_sources(showcase PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/showcase.qrc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ShowcaseWindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ShowcaseWindow.hpp
)
target_link_libraries(showcase
  PRIVATE
    oclero::qlementine
    Qt::Core
    Qt::Widgets
)

set_target_properties(showcase PROPERTIES
  INTERNAL_CONSOLE OFF
  EXCLUDE_FROM_ALL OFF
  FOLDER tools
  AUTOMOC ON
  AUTORCC ON
)

target_deploy_qt(showcase)
