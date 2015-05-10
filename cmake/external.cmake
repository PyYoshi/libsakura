include(ExternalProject)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	set(DEBUG_CFLAGS "-O0 -g")
	set(FFMPEG_DEBUG "--enable-debug")
else()
	set(DEBUG_CFLAGS "")
	set(FFMPEG_DEBUG "")
endif()

ExternalProject_Add(
	LIBJPEGTURBO
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0/ && autoreconf -fiv && ./configure CFLAGS=${DEBUG_CFLAGS} CXXFLAGS=${DEBUG_CFLAGS} --prefix=${THIRDPARTY_PREFIX_DIR} --disable-dependency-tracking --disable-shared --without-simd
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/libjpeg-turbo-1.4.0/ && make install
)

ExternalProject_Add(
	LIBWEBP
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/libwebp-0.4.3
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/libwebp-0.4.3/ && ./autogen.sh && ./configure CFLAGS=${DEBUG_CFLAGS} CXXFLAGS=${DEBUG_CFLAGS} --prefix=${THIRDPARTY_PREFIX_DIR} --disable-dependency-tracking --enable-shared=no
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/libwebp-0.4.3/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/libwebp-0.4.3/ && make install
)

ExternalProject_Add(
	GIFLIB
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/giflib-5.1.1
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/giflib-5.1.1/ && ./autogen.sh && ./configure CFLAGS=${DEBUG_CFLAGS} CXXFLAGS=${DEBUG_CFLAGS} --prefix=${THIRDPARTY_PREFIX_DIR} --disable-dependency-tracking
	BUILD_COMMAND         	cd ${THIRDPARTY_DIR_PATH}/giflib-5.1.1/ && make
	INSTALL_COMMAND       	cd ${THIRDPARTY_DIR_PATH}/giflib-5.1.1/ && make install
)

ExternalProject_Add(
	FFMPEG
	PREFIX					${THIRDPARTY_PREFIX_DIR}
	SOURCE_DIR				${THIRDPARTY_DIR_PATH}/FFmpeg-2.6.2
	CONFIGURE_COMMAND 		cd ${THIRDPARTY_DIR_PATH}/FFmpeg-2.6.2/ && ./configure --prefix=${THIRDPARTY_PREFIX_DIR} ${FFMPEG_DEBUG} --disable-programs --disable-doc --disable-everything --disable-avdevice --disable-avcodec --disable-swresample --disable-postproc --disable-avfilter --disable-shared
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
	BUILD_COMMAND         	make
	INSTALL_COMMAND       	make install
	CMAKE_CACHE_ARGS
							-DCMAKE_INSTALL_PREFIX:PATH=${THIRDPARTY_PREFIX_DIR}
							-DCMAKE_PREFIX_PATH:STRING=${THIRDPARTY_PREFIX_DIR}
							-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
							-DPNG_SHARED:BOOL=OFF
							-DPNG_STATIC:BOOL=ON
							-DPNG_NO_STDIO:BOOL=NO
	DEPENDS					ZLIB
)
