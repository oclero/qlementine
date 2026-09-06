# Usage

## Build Options

Qlementine can be built as a static library, as a shared library, or as both variants in the same build tree. The default build is static, which preserves the behavior of existing consumers unless `BUILD_SHARED_LIBS` is enabled explicitly.

```cmake
# Static library, default.
-DBUILD_SHARED_LIBS=OFF

# Shared library.
-DBUILD_SHARED_LIBS=ON

# Shared and static libraries together.
-DBUILD_SHARED_LIBS=ON -DQLEMENTINE_BUILD_STATIC=ON
```

When both variants are enabled, the exported CMake targets are `qlementine::qlementine_shared` and `qlementine::qlementine_static`. Otherwise, the installed target is `qlementine::qlementine` and matches the selected build type.

The Qt style plugin is built in all build configurations, as `qlementinestyleplugin`, and keeps the style key `"qlementine"`.

## Installation

### Option 1: FetchContent

Add the library as a dependency using CMake FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(Qlementine GIT_REPOSITORY "https://github.com/oclero/qlementine.git")
FetchContent_MakeAvailable(Qlementine)
```

Link with the library:

```cmake
target_link_libraries(your_project PRIVATE qlementine)
```

To build Qlementine as a shared library through FetchContent, set `BUILD_SHARED_LIBS` before `FetchContent_MakeAvailable`.

```cmake
set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(Qlementine)
```

### Option 2: Find Installed Library

If Qlementine is already installed on your system, use the installed CMake package:

```cmake
find_package(qlementine REQUIRED)
target_link_libraries(your_project PRIVATE qlementine::qlementine)
```

If the package was installed with both shared and static variants, link the variant explicitly:

```cmake
target_link_libraries(your_project PRIVATE qlementine::qlementine_shared)
# or
target_link_libraries(your_project PRIVATE qlementine::qlementine_static)
```

### Option 3: Meson or pkg-config

On platforms where pkg-config metadata is installed, Qlementine can also be consumed from Meson or another pkg-config aware build system.

```meson
qlementine_dep = dependency('qlementine')
executable('your_project',
   sources: ['main.cpp'],
   dependencies: [qlementine_dep]
)
```

### Option 4: vcpkg

```bash
vcpkg install qlementine
```

Then in CMake:

```cmake
find_package(qlementine CONFIG REQUIRED)
target_link_libraries(your_project PRIVATE qlementine::qlementine)
```

## Usage in Code

### Direct Style Creation

Create the `QStyle` and assign it to your `QApplication`.

```c++
#include <oclero/qlementine.hpp>

QApplication app(argc, argv);

auto* style = new oclero::qlementine::QlementineStyle(&app);
QApplication::setStyle(style);
```

### Qt Style Plugin

Qlementine can also be used through its Qt style plugin. Install or deploy `qlementinestyleplugin` in Qt's `styles` plugin directory, then select the `qlementine` style key.

```c++
QApplication app(argc, argv);
QApplication::setStyle("qlementine");
```

## Themes

You may want to use your own JSON theme.

```c++
style->setThemeJsonPath(":/path/to/your/theme.json");
```

Additionnally, you can also use `ThemeManager` to handle that for you.

```c++
// Link a ThemeManager to a QlementineStyle.
auto* themeManager = new oclero::qlementine::ThemeManager(style);

// Load the directory where you store your own JSON themes.
themeManager->loadDirectory(":/themes");

// Define theme on QStyle.
themeManager->setCurrentTheme("Light");
```
