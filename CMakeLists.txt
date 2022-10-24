cmake_minimum_required(VERSION 3.12)

project(
  expresso
  VERSION 0.1
  DESCRIPTION "Web Framework for C++, heavily based on ExpressJS"
  LANGUAGES CXX)

add_definitions("-std=c++17")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
