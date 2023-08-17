#!/bin/bash

# General dependencies.
sudo apt install -y \
  build-essential \
  cmake \
  pkg-config \


# Qt dependencies.
declare qt_version=6

if [[ $qt_version -eq 5 ]]; then
  echo "Installing Qt$qt_version..."

  sudo apt-get install -y  \
    qtbase5-dev \
    qtbase5-dev-tools \
    qt5-qmake \
    qttools5-dev \
    qttools5-dev-tools \
    qt5svg5* \

elif [[ $qt_version -eq 6 ]]; then
  echo "Installing Qt$qt_version..."

  sudo apt-get install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qmake6 \
    libqt6svg6 \
    libqt6svg6-dev \
    # qt6-qmltooling-plugins

else
  echo "Qt$qt_version is not supported."
  exit 1

fi
