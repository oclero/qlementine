# Usage

## Installation

1. Add the library's repository as a Git submodule.

    ```bash
    git submodule add git@github.com:oclero/qlementine.git submodules/qlementine
    ```

2. Download submodules.

    ```bash
    git submodule update --init --recursive
    ```

3. Add the library to your CMake project.

    ```cmake
    add_subdirectory(submodules/qlementine)
    ```

4. Link with the library in CMake.

    ```cmake
    target_link_libraries(your_project oclero::qlementine)
    ```

5. Define the Qt version (Qt5 is the default). Two options:

    1. When calling `cmake`:

        ```bash
        cmake -B ./build -DQT_DEFAULT_MAJOR_VERSION=6
        ```

    2. Directly in a top-level CMake file:

        ```cmake
        set(QT_DEFAULT_MAJOR_VERSION 6)
        ```

## Usage

Define the `QStyle` on your `QApplication`.

```c++
#include <oclero/qlementine.hpp>

QApplication app(argc, argv);

auto* style = new oclero::qlementine::QlementineStyle(&app);
style->setThemeJsonPath(":/light.json");
QApplication::setStyle(style);
```
