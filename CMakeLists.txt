cmake_minimum_required(VERSION 2.8.12)
project(libsakura)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_INCLUDE_PATH})

set(THIRDPARTY_DIR_PATH "${PROJECT_SOURCE_DIR}/src/third_party")
set(THIRDPARTY_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/src/third_party/root/lib")
set(THIRDPARTY_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/src/third_party/root/include")

if (CMAKE_BUILD_TYPE_INITIALIZED_TO_DEFAULT)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(cmake/external.cmake)
include(cmake/BuildSakura.cmake)
