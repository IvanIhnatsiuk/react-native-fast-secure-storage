cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "react-native-fast-secure-storage")
project(PACKAGE_NAME)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)
set (BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

add_library(
      ${PACKAGE_NAME}
      SHARED
      src/main/cpp/bindings.h
      src/main/cpp/bindings.cpp
      src/main/cpp/cpp-adapter.cpp
      ..cpp/macros.h
)

include_directories(
  ../cpp
)

set_target_properties(
        ${PACKAGE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)

find_package(ReactAndroid REQUIRED CONFIG)
find_package(fbjni REQUIRED CONFIG)
find_library(log-lib log)

target_link_libraries(
        ${PACKAGE_NAME}
        ${log-lib}
        fbjni::fbjni
        ReactAndroid::jsi
        ReactAndroid::turbomodulejsijni
        ReactAndroid::react_nativemodule_core
        android
)
