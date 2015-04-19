set(GIF_LIBRARY gif)
project(${GIF_LIBRARY})

# SET LIBRARY PATH
set(GIFLIB_HOME "${PROJECT_SOURCE_DIR}/src/third_party/giflib-5.1.1")

# GIF Library

set(
  GIFLIB_OBJS
  ${GIFLIB_HOME}/lib/dgif_lib.c
  ${GIFLIB_HOME}/lib/egif_lib.c
  ${GIFLIB_HOME}/lib/gif_font.c
  ${GIFLIB_HOME}/lib/gif_hash.c
  ${GIFLIB_HOME}/lib/gifalloc.c
  ${GIFLIB_HOME}/lib/gif_err.c
  ${GIFLIB_HOME}/lib/quantize.c
)

# define

include_directories("${GIFLIB_HOME}/lib")

add_library(${GIF_LIBRARY} STATIC ${GIFLIB_OBJS})

set_target_properties(
  ${GIF_LIBRARY}
  PROPERTIES OUTPUT_NAME ${GIF_LIBRARY}
  ARCHIVE_OUTPUT_DIRECTORY ${THIRDPARTY_LIBRARY_PATH}
)