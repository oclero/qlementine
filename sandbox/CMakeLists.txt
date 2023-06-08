find_package(Qt5
  REQUIRED
    Core
    Widgets
)

add_executable(sandbox WIN32 MACOSX_BUNDLE)
target_sources(sandbox PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/sandbox.qrc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/SandboxWindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/SandboxWindow.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CsdWindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CsdWindow.hpp
)
target_link_libraries(sandbox
  PRIVATE
    oclero::qlementine
    Qt5::Core
    Qt5::Widgets
)

set_target_properties(sandbox PROPERTIES
  INTERNAL_CONSOLE OFF
  EXCLUDE_FROM_ALL OFF
  FOLDER sandbox
  AUTOMOC ON
  AUTORCC ON
)

############# Minimal example ends here #############
target_deploy_qt(sandbox)
