cmake_minimum_required(VERSION 3.21.0)

# Custom CMake modules.
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Enable parallel build (not enabled by default on Windows).
set(CMAKE_BUILD_PARALLEL_LEVEL $ENV{NUMBER_OF_PROCESSORS})

# Set project information.
project("qlementine"
  LANGUAGES CXX
  VERSION 1.0.0.0
  DESCRIPTION "Modern QStyle for desktop Qt applications."
)
set(PROJECT_COPYRIGHT "© Olivier Cléro, MIT License.")
set(PROJECT_NAMESPACE "oclero")

# Global flags.
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if (NOT CMAKE_OSX_DEPLOYMENT_TARGET)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "13.6")
endif()

# Find Qt.
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Svg)
qt_standard_project_setup()

# include(DeployQt)

# The library.
add_subdirectory(lib)

# Sandbox.
if(${PROJECT_IS_TOP_LEVEL})
  add_subdirectory(sandbox)
endif()
