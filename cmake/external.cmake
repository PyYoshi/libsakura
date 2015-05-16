include(ExternalProject)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(FFMPEG_DEBUG "--enable-debug")
else()
  set(FFMPEG_DEBUG "")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(FFMPEG_ENABLE_PIC "--enable-pic")
else()
  set(FFMPEG_ENABLE_PIC "")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CUSTOM_CFLAGS "-fPIC -O0 -g")
  else()
    set(CUSTOM_CFLAGS "-fPIC")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CUSTOM_CFLAGS "-O0 -g")
  else()
    set(CUSTOM_CFLAGS "")
  endif()
else()
  set(CUSTOM_CFLAGS "")
endif()

ExternalProject_Add(
	LIBJPEGTURBO
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0/ && autoreconf -fiv && ./configure CFLAGS=${CUSTOM_CFLAGS} CPPFLAGS=${CUSTOM_CFLAGS} --prefix=${THIRDPARTY_PREFIX_DIR} --disable-dependency-tracking --enable-shared --without-simd
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0/ && make install
)

ExternalProject_Add(
	LIBWEBP
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/libwebp-0.4.3
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/libwebp-0.4.3/ && ./autogen.sh && ./configure CFLAGS=${CUSTOM_CFLAGS} CPPFLAGS=${CUSTOM_CFLAGS} --prefix=${THIRDPARTY_PREFIX_DIR} --disable-dependency-tracking --enable-shared
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/libwebp-0.4.3/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/libwebp-0.4.3/ && make install
)

ExternalProject_Add(
	GIFLIB
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/giflib-5.1.1
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/giflib-5.1.1/ && ./autogen.sh && ./configure CFLAGS=${CUSTOM_CFLAGS} CPPFLAGS=${CUSTOM_CFLAGS} --prefix=${THIRDPARTY_PREFIX_DIR} --disable-dependency-tracking --enable-shared
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/giflib-5.1.1/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/giflib-5.1.1/ && make install
)

ExternalProject_Add(
	FFMPEG
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/FFmpeg-2.6.2
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/FFmpeg-2.6.2/ && ./configure --prefix=${THIRDPARTY_PREFIX_DIR} ${FFMPEG_DEBUG} --extra-cflags=${CUSTOM_CFLAGS} --extra-cxxflags=${CUSTOM_CFLAGS} --disable-programs --disable-doc --disable-everything --disable-avdevice --disable-avcodec --disable-swresample --disable-postproc --disable-avfilter --enable-static --enable-shared ${FFMPEG_ENABLE_PIC}
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/FFmpeg-2.6.2/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/FFmpeg-2.6.2/ && make install
)

ExternalProject_Add(
	ZLIB
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/zlib-1.2.8
	CMAKE_CACHE_ARGS
							-DCMAKE_INSTALL_PREFIX:PATH=${THIRDPARTY_PREFIX_DIR}
							-DCMAKE_PREFIX_PATH:STRING=${THIRDPARTY_PREFIX_DIR}
							-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)

ExternalProject_Add(
	LIBPNG
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/libpng-1.6.17
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/libpng-1.6.17/ && ./configure CFLAGS=${CUSTOM_CFLAGS} CPPFLAGS=${CUSTOM_CFLAGS} --prefix=${THIRDPARTY_PREFIX_DIR} --with-zlib-prefix=${THIRDPARTY_PREFIX_DIR} --disable-dependency-tracking --enable-shared
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/libpng-1.6.17/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/libpng-1.6.17/ && make install
	DEPENDS					ZLIB
)
