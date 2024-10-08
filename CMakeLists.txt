cmake_minimum_required(VERSION 3.21.0)

# Custom CMake modules.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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

# Temporary hack to make it work with Qt6 <6.4.2 && >6.4.2
# With Qt5, it was "path/to/Qt/6.7.0/msvc2019_64/lib/cmake/Qt6",
# but with Qt6, it is now "path/to/Qt/6.7.0/msvc2019_64".
if(WIN32)
  string(FIND "${CMAKE_PREFIX_PATH}" "/lib/cmake/Qt6" USING_Qt6_INDEX)
  if(NOT ${USING_Qt6_INDEX} EQUAL -1)
    string(REPLACE "/lib/cmake/Qt6" "" "${CMAKE_PREFIX_PATH}" "${CMAKE_PREFIX_PATH}")
  endif()
endif()

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
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Svg)

if(${QT_VERSION_MAJOR} STREQUAL "6")
  qt_standard_project_setup()
endif()

include(DeployQt)

# The library.
add_subdirectory(lib)

# Sandbox.
if(${PROJECT_IS_TOP_LEVEL})
  add_subdirectory(sandbox)
endif()
