set(PNG_LIBRARY png)
project(${PNG_LIBRARY})

# SET LIBRARY PATH
set(LIBPNG_HOME "${PROJECT_SOURCE_DIR}/src/third_party/libpng-1.6.9")

set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")

if(NOT WIN32)
  find_library(M_LIBRARY
    NAMES m
    PATHS /usr/lib /usr/local/lib
  )
  if(NOT M_LIBRARY)
    message(STATUS
      "math library 'libm' not found - floating point support disabled")
  endif()
else()
  # not needed on windows
  set(M_LIBRARY "")
endif()

find_library(M_LIBRARY
  NAMES m
  PATHS /usr/lib /usr/local/lib
)

find_library(Z_LIBRARY
  NAMES z
  PATHS ${THIRDPARTY_LIBRARY_PATH}
)

configure_file(${LIBPNG_HOME}/scripts/pnglibconf.h.prebuilt
               ${LIBPNG_HOME}/pnglibconf.h)

set(LIBPNG_HEADERS
  ${LIBPNG_HOME}/png.h
  ${LIBPNG_HOME}/pngconf.h
  ${LIBPNG_HOME}/pnglibconf.h
  ${LIBPNG_HOME}/pngdebug.h
  ${LIBPNG_HOME}/pnginfo.h
  ${LIBPNG_HOME}/pngpriv.h
  ${LIBPNG_HOME}/pngstruct.h
)

set(LIBPNG_OBJS
  ${LIBPNG_HOME}/png.c
  ${LIBPNG_HOME}/pngerror.c
  ${LIBPNG_HOME}/pngget.c
  ${LIBPNG_HOME}/pngmem.c
  ${LIBPNG_HOME}/pngpread.c
  ${LIBPNG_HOME}/pngread.c
  ${LIBPNG_HOME}/pngrio.c
  ${LIBPNG_HOME}/pngrtran.c
  ${LIBPNG_HOME}/pngrutil.c
  ${LIBPNG_HOME}/pngset.c
  ${LIBPNG_HOME}/pngtrans.c
  ${LIBPNG_HOME}/pngwio.c
  ${LIBPNG_HOME}/pngwrite.c
  ${LIBPNG_HOME}/pngwtran.c
  ${LIBPNG_HOME}/pngwutil.c
)

if(NEON_FOUND)
set(LIBPNG_OBJS
  ${LIBPNG_OBJS}
  ${LIBPNG_HOME}/arm/arm_init.c
  ${LIBPNG_HOME}/arm/filter_neon.S
  ${LIBPNG_HOME}/arm/filter_neon_intrinsics.c
)
endif()

include_directories("${LIBPNG_HOME}")
include_directories("${ZLIB_HOME}")

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

# TODO: プロジェクトで用意したzlibとの連携がうまく行かない
add_library(${PNG_LIBRARY} STATIC ${LIBPNG_OBJS} ${LIBPNG_HEADERS})
add_dependencies(${PNG_LIBRARY} ${ZLIB_LIBRARY})
target_link_libraries(${PNG_LIBRARY} ${Z_LIBRARY} ${M_LIBRARY})

set_target_properties(
  ${PNG_LIBRARY}
  PROPERTIES OUTPUT_NAME ${PNG_LIBRARY}
  ARCHIVE_OUTPUT_DIRECTORY ${THIRDPARTY_LIBRARY_PATH}
)
