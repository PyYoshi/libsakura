cmake_minimum_required(VERSION 2.8.12)
project(libsakura)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_INCLUDE_PATH})

set(THIRDPARTY_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/src/third_party/lib")

include(cmake/FindSIMD.cmake)
include(cmake/FindARM.cmake)

include(cmake/giflib.cmake)
include(cmake/libwebp.cmake)
include(cmake/zlib.cmake)
include(cmake/libpng.cmake)
#include(cmake/libjpeg_turbo.cmake)
include(cmake/mozjpeg.cmake)

include_directories(${PROJECT_SOURCE_DIR}/src/sakura)
