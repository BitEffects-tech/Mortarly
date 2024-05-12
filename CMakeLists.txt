cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0072 NEW)
#cmake_policy(SET CMP0135 NEW)
#cmake_policy(SET CMP0148 NEW)

set(PROJECT_NAME Mortarly)
project(${PROJECT_NAME})

# Configure CMake constants
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse2 -mavx512f")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ -DLIBCXX_ENABLE_INCOMPLETE_FEATURES=ON")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")


include(CheckCXXCompilerFlag)

# Try to compile and run the SSE2 check
try_run(RUN_RESULT_VAR COMPILE_RESULT_VAR
        ${CMAKE_BINARY_DIR}/bin
        ${CMAKE_SOURCE_DIR}/check_sse2.cpp
        CMAKE_FLAGS -DCMAKE_CXX_STANDARD=11
        RUN_OUTPUT_VARIABLE SSE2_OUTPUT)

# Check the results of the test run
if("${RUN_RESULT_VAR}" STREQUAL "0")
    check_cxx_compiler_flag("-msse2" COMPILER_SUPPORTS_MSSE2)
    if(COMPILER_SUPPORTS_MSSE2)
        message(STATUS "SSE2 is supported on this hardware.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -DENABLE_SSE2")
    else()
        message(STATUS "Compiler does not support the -msse2 flag")
    endif()
else()
    message(STATUS "SSE2 is not supported on this hardware")
endif()



# Add the submodules
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
add_subdirectory(external/gtest)
add_subdirectory(external/glad/cmake)
add_subdirectory(external/fmt)
#add_subdirectory(external/stb)

# Find required packages
#find_package(TBB REQUIRED)
#find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
#find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
#find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)
find_package(OpenGL REQUIRED)

# Configure all the include directories
#include_directories(${OpenCASCADE_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/stb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# NOTE: Globbing will disable automatic rebuild of make when adding new files
file(GLOB_RECURSE SRC_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
file(GLOB_RECURSE SRC_TEST_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
list(REMOVE_ITEM SRC_CPP_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
list(REMOVE_ITEM SRC_CPP_FILES SRC_TEST_CPP_FILES)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp ${SRC_CPP_FILES})
target_precompile_headers(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
target_link_libraries(${PROJECT_NAME}
    # INTERFACE stb
    # PRIVATE
    fmt::fmt
    # ${TBB_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    OpenGL::GL
    gtest_main
    glfw
)

# Build the tests
enable_testing()
include(GoogleTest)

add_definitions(-DUSING_GTEST)
add_executable(runUnitTests ${SRC_CPP_FILES} ${SRC_TEST_CPP_FILES})
target_precompile_headers(runUnitTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
target_link_libraries(runUnitTests
    # INTERFACE stb
    # PRIVATE
    OpenGL::GL
    fmt::fmt
    ${OpenCASCADE_LIBRARIES}
    gtest_main
    gtest
    glfw
)

gtest_discover_tests(runUnitTests)
