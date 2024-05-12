#!/bin/bash

if [ -z $"$1" ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

# Install prerequisites
sudo apt-get update
sudo apt-get -y install libgl1-mesa-dev qtbase5-dev qt5-qmake libqt5opengl5-dev libtbb-dev \
 libocct-data-exchange-dev libocct-draw-dev libocct-foundation-dev libocct-modeling-algorithms-dev \
 libocct-modeling-data-dev libocct-ocaf-dev libocct-visualization-de

# Define project name and directories
PROJECT_NAME="CatalystAD"
PROJECT_DIR="${PROJECT_NAME}"
SRC_DIR="src"
INCLUDE_DIR="include"
BUILD_DIR="build"
EXTERNAL_DIR="external"

CURR_DIR_NAME=$(basename "$(pwd)")
if [[ "$CURR_DIR_NAME" != "$PROJECT_NAME" ]]; then
  PROJECT_DIR = '.'
fi

# Create project directory and subdirectories
mkdir -p "${PROJECT_DIR}/${SRC_DIR}"
mkdir -p "${PROJECT_DIR}/${INCLUDE_DIR}"
mkdir -p "${PROJECT_DIR}/${BUILD_DIR}"
mkdir -p "${PROJECT_DIR}/${EXTERNAL_DIR}"

# Initialize Git repository
cd "${PROJECT_DIR}"
git init

git submodule add https://github.com/glfw/glfw.git external/glfw
git submodule add https://github.com/google/googletest.git external/gtest
git submodule add https://github.com/nothings/stb external/stb
git submodule add https://github.com/Dav1dde/glad external/glad

git submodule update --init --recursive

# Create a basic README file
echo "# ${PROJECT_NAME}" > README.md
echo "A custom CAD tool using OpenGL, Open CASCADE, and Qt." >> README.md

# Create main.cpp file
cat <<EOF > "${SRC_DIR}/main.cpp"
#include <iostream>

int main() {
    std::cout << "Hello, ${PROJECT_NAME}!" << std::endl;
    return 0;
}
EOF

# Create CMakeLists.txt
cat <<EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

# Set the project name
project(${PROJECT_NAME})

# Set C++ standard
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Enable automoc, autorcc, and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(TBB REQUIRED)

find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)

# Configure the build step
include_directories(\${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(\${OpenCASCADE_INCLUDE_DIR})

# NOTE: Globbing will disable automatic rebuild of make when adding new files because that only happens when
# CMakeLists.txt is modified. When adding new source files, it's necessary to manually re-run cmake
file(GLOB ROOT_CPP_FILES "\${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SRC_CPP_FILES "\${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(\${PROJECT_NAME} \${ROOT_CPP_FILES} \${SRC_CPP_FILES})

target_link_libraries(\${PROJECT_NAME} PRIVATE \${OPENGL_LIBRARIES} \${OpenCASCADE_LIBRARIES} \${TBB_LIBRARIES} Qt5::Widgets)
EOF

# Create a basic .gitignore file
cat <<EOF > .gitignore
/build/
*.o
*.so
*.exe
*~
EOF

echo "Project initialized"
