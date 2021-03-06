project(sakura)

set(SAKURA_LIBRARY sakura)
set(SAKURA_STATIC_LIBRARY sakurastatic)
set(SAKURA_SHARED_LIBRARY sakura)
set(SAKURA_EXECUTABLE test_sakura)

# SET LIBRARY PATH
set(LIBSAKURA_HOME "${PROJECT_SOURCE_DIR}/src/sakura")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

set(CMAKE_C_FLAGS_DEBUG "-O -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O -g")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif()

include_directories(${LIBSAKURA_HOME} ${THIRDPARTY_INCLUDE_PATH})
link_directories(${THIRDPARTY_LIBRARY_PATH})

set(SAKURA_DEPENDENCIES
  ${THIRDPARTY_LIBRARY_PATH}/libavutil.a
  ${THIRDPARTY_LIBRARY_PATH}/libswscale.a
  ${THIRDPARTY_LIBRARY_PATH}/libz.a
  ${THIRDPARTY_LIBRARY_PATH}/libgif.a
  ${THIRDPARTY_LIBRARY_PATH}/libjpeg.a
  ${THIRDPARTY_LIBRARY_PATH}/libturbojpeg.a
  ${THIRDPARTY_LIBRARY_PATH}/libpng.a
  ${THIRDPARTY_LIBRARY_PATH}/libwebp.a
)

set(SOURCE_FILES ${LIBSAKURA_HOME}/Sakura.cpp ${LIBSAKURA_HOME}/c.cpp)
set(HEADER_FILES ${LIBSAKURA_HOME}/Sakura.h ${LIBSAKURA_HOME}/c.h)

add_library(${SAKURA_STATIC_LIBRARY} STATIC ${HEADER_FILES} ${SOURCE_FILES})
add_dependencies(${SAKURA_STATIC_LIBRARY} LIBJPEGTURBO LIBWEBP GIFLIB FFMPEG ZLIB LIBPNG)
target_link_libraries(${SAKURA_STATIC_LIBRARY} ${SAKURA_DEPENDENCIES})
set_target_properties(
  ${SAKURA_STATIC_LIBRARY}
  PROPERTIES OUTPUT_NAME ${SAKURA_LIBRARY}
)

if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

add_library(${SAKURA_SHARED_LIBRARY} SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_dependencies(${SAKURA_SHARED_LIBRARY} LIBJPEGTURBO LIBWEBP GIFLIB FFMPEG ZLIB LIBPNG)
target_link_libraries(${SAKURA_SHARED_LIBRARY} ${SAKURA_DEPENDENCIES})
set_target_properties(
  ${SAKURA_SHARED_LIBRARY}
  PROPERTIES OUTPUT_NAME ${SAKURA_LIBRARY}
)

#set_directory_properties(PROPERTIES
#    ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/libSakuraScaler.a
#)
#add_custom_command(
#    OUTPUT COMBINED_LIBSAKURA
#    COMMAND rm -f ${PROJECT_SOURCE_DIR}/libSakuraScaler.a
#    COMMAND ${CMAKE_AR} rc ${PROJECT_SOURCE_DIR}/libSakuraScaler.a ${PROJECT_SOURCE_DIR}/libsakura.a ${SAKURA_DEPENDENCIES}
#    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#    COMMENT "combined libsakura"
#)
#add_custom_target(combined_libsakura ALL
#  DEPENDS ${SAKURA_STATIC_LIBRARY} COMBINED_LIBSAKURA
#)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(TEST_C_SOURCE_FILES ${LIBSAKURA_HOME}/test_c_sakura.c)
  add_executable(test_c_sakura ${TEST_C_SOURCE_FILES})
  add_dependencies(test_c_sakura ${SAKURA_STATIC_LIBRARY})
  target_link_libraries(test_c_sakura ${SAKURA_DEPENDENCIES} ${PROJECT_SOURCE_DIR}/libsakura.a ${SAKURA_DEPENDENCIES})

  set(TEST_SOURCE_FILES ${LIBSAKURA_HOME}/test_sakura.cpp)
  add_executable(test_sakura ${TEST_SOURCE_FILES})
  add_dependencies(test_sakura ${SAKURA_STATIC_LIBRARY})
  target_link_libraries(test_sakura ${SAKURA_DEPENDENCIES} ${PROJECT_SOURCE_DIR}/libsakura.a ${SAKURA_DEPENDENCIES})
endif()

install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/sakura)
install(TARGETS ${SAKURA_STATIC_LIBRARY} ${SAKURA_SHARED_LIBRARY}
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}")

