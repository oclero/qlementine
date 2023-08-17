cmake_minimum_required(VERSION 3.17.5)

# Check for Qt version in given arguments. Fallback to Qt5.
if (NOT DEFINED QT_DEFAULT_MAJOR_VERSION)
  set(QT_DEFAULT_MAJOR_VERSION 5)
endif()
message("-- Using Qt${QT_DEFAULT_MAJOR_VERSION}")

# Custom CMake modules.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DeployQt)

# Set project information.
project("qlementine"
  LANGUAGES CXX
  VERSION 1.0.0.0
)
set(PROJECT_NAMESPACE "oclero")

# Global flags.
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# The library.
add_subdirectory(lib)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  # Sandbox.
  add_subdirectory(sandbox)
endif()
