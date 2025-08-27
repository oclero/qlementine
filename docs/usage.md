# Usage

## Installation

### Option 1: FetchContent (Build from Source)

1. Add the library as a dependency using CMake FetchContent:

   ```cmake
   include(FetchContent)
   FetchContent_Declare(Qlementine GIT_REPOSITORY "https://github.com/oclero/qlementine.git")
   FetchContent_MakeAvailable(Qlementine)
   ```

2. Link with the library:

   ```cmake
   target_link_libraries(your_project qlementine)
   ```

### Option 2: Find Installed Library

If Qlementine is already installed on your system (via package manager, vcpkg, or manual installation):

#### CMake

```cmake
find_package(qlementine REQUIRED)
target_link_libraries(your_project qlementine::qlementine)
```

#### Meson

```meson
qlementine_dep = dependency('qlementine')
executable('your_project',
  sources: ['main.cpp'],
  dependencies: [qlementine_dep]
)
```

### Option 3: vcpkg

```bash
vcpkg install qlementine
```

Then in CMake:
```cmake
find_package(qlementine CONFIG REQUIRED)
target_link_libraries(your_project PRIVATE qlementine::qlementine)
```

## Usage in code

Define the `QStyle` on your `QApplication`.

```c++
#include <oclero/qlementine.hpp>

QApplication app(argc, argv);

auto* style = new oclero::qlementine::QlementineStyle(&app);
QApplication::setStyle(style);
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
