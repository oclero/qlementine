<div align="center">
	<img height="50" src="branding/logo.svg">
</div>

# Qlementine

[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://mit-license.org/)
[![CMake version](https://img.shields.io/badge/CMake-3.17.5+-064F8C?logo=cmake)](https://www.qt.io)
[![C++ version](https://img.shields.io/badge/C++-17-00599C?logo=++)](https://www.qt.io)
[![Qt version](https://img.shields.io/badge/Qt-5.15.3+-41CD52?logo=qt)](https://www.qt.io)

Modern QStyle for desktop Qt5 applications.

---

### Table of Contents

- [Requirements](#requirements)
- [Features](#features)
- [Usage](#usage)
- [Author](#author)
- [License](#license)

---

## Requirements

- Windows, MacOS or Linux.
- [CMake 3.17.5+](https://cmake.org/download/)
- [Qt 5.15.3+](https://www.qt.io/download-qt-installer)

## Features

This library contains:

- A custom `QStyle` named `QlementineStyle`, that implements all the necessary API to give a modern look and feel to your Qt application. It's a drop-in replacement for the default `QStyle`.
- An actual way to have client-side decoration (CSD) on your Qt window, with actual OS window animations and effects. *(Windows only, at the moment)*
- Lots of utilities to help you write beautiful Qt widgets.

## Usage

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

5. Define the `QStyle` on your `QApplication`.

   ```c++
   #include <oclero/qlementine.hpp>

   QApplication app(argc, argv);

   auto* style = new oclero::qlementine::QlementineStyle(&app);
   style->setThemeJsonPath(":/light.json");
   QApplication::setStyle(style);
   ```

## Author

**Olivier Cléro** | [email](mailto:oclero@pm.me) | [website](https://www.olivierclero.com) | [github](https://www.github.com/oclero) | [gitlab](https://www.gitlab.com/oclero)

## License

**Qlementine** is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
