set(JPEG_LIBRARY jpeg)
set(TURBO_JPEG_LIBRARY turbojpeg)
project(${JPEG_LIBRARY})

# SET LIBRARY PATH
set(LIBJPEG_TURBO_HOME "${PROJECT_SOURCE_DIR}/src/third_party/libjpeg-turbo-1.4.0")
set(LIBJPEG_TURBO_SIMD_HOME "${LIBJPEG_TURBO_HOME}/simd")

set(JPEG_LIB_VERSION 62)

option(WITH_SIMD "Include SIMD extensions" TRUE)
option(WITH_ARITH_ENC "Include arithmetic encoding support" TRUE)
option(WITH_ARITH_DEC "Include arithmetic decoding support" TRUE)
option(WITH_MEM_SRCDST "Include in-memory source/destination manager functions when emulating the libjpeg v6b or v7 API/ABI" TRUE)
option(WITH_TURBOJPEG "Include the TurboJPEG wrapper library and associated test programs" TRUE)
option(WITH_12BIT "Encode/decode JPEG images with 12-bit samples (implies WITH_SIMD=0 WITH_TURBOJPEG=0 WITH_ARITH_ENC=0 WITH_ARITH_DEC=0)" FALSE)

if(WITH_12BIT)
  set(WITH_SIMD FALSE)
  set(WITH_TURBOJPEG FALSE)
  set(WITH_ARITH_ENC FALSE)
  set(WITH_ARITH_DEC FALSE)
  set(BITS_IN_JSAMPLE 12)
  message(STATUS "12-bit JPEG support enabled")
else()
  set(BITS_IN_JSAMPLE 8)
endif()

if(WITH_ARITH_ENC)
  set(C_ARITH_CODING_SUPPORTED 1)
  message(STATUS "Arithmetic encoding support enabled")
else()
  message(STATUS "Arithmetic encoding support disabled")
endif()

if(WITH_ARITH_DEC)
  set(D_ARITH_CODING_SUPPORTED 1)
  message(STATUS "Arithmetic decoding support enabled")
else()
  message(STATUS "Arithmetic decoding support disabled")
endif()

if(WITH_TURBOJPEG)
  message(STATUS "TurboJPEG C wrapper enabled")
else()
  message(STATUS "TurboJPEG C wrapper disabled")
endif()

if(WITH_JAVA)
  message(STATUS "TurboJPEG Java wrapper enabled")
else()
  message(STATUS "TurboJPEG Java wrapper disabled")
endif()

set(SO_AGE 0)
if(WITH_MEM_SRCDST)
  set(SO_AGE 1)
endif()

if(WITH_MEM_SRCDST)
  set(MEM_SRCDST_SUPPORTED 1)
  message(STATUS "In-memory source/destination managers enabled")
else()
  message(STATUS "In-memory source/destination managers disabled")
endif()

if(MSVC)
  # Use the static C library for all build types
  foreach(var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
    endif()
  endforeach()

  add_definitions(-W3 -wd4996)
endif()

# Detect whether compiler is 64-bit
if(MSVC AND CMAKE_CL_64)
  set(SIMD_X86_64 1)
  set(64BIT 1)
elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
  set(SIMD_X86_64 1)
  set(64BIT 1)
endif()

if(64BIT)
  message(STATUS "64-bit build")
else()
  message(STATUS "32-bit build")
endif()


message(STATUS "Install directory = ${CMAKE_INSTALL_PREFIX}")

configure_file(${LIBJPEG_TURBO_HOME}/win/jconfig.h.in ${LIBJPEG_TURBO_HOME}/jconfig.h)
configure_file(${LIBJPEG_TURBO_HOME}/win/jconfigint.h.in ${LIBJPEG_TURBO_HOME}/jconfigint.h)

include_directories(${LIBJPEG_TURBO_HOME})

set(JPEG_SOURCES
  ${LIBJPEG_TURBO_HOME}/jcapimin.c
  ${LIBJPEG_TURBO_HOME}/jcapistd.c
  ${LIBJPEG_TURBO_HOME}/jccoefct.c
  ${LIBJPEG_TURBO_HOME}/jccolor.c
  ${LIBJPEG_TURBO_HOME}/jcdctmgr.c
  ${LIBJPEG_TURBO_HOME}/jchuff.c
  ${LIBJPEG_TURBO_HOME}/jcinit.c
  ${LIBJPEG_TURBO_HOME}/jcmainct.c
  ${LIBJPEG_TURBO_HOME}/jcmarker.c
  ${LIBJPEG_TURBO_HOME}/jcmaster.c
  ${LIBJPEG_TURBO_HOME}/jcomapi.c
  ${LIBJPEG_TURBO_HOME}/jcparam.c
  ${LIBJPEG_TURBO_HOME}/jcphuff.c
  ${LIBJPEG_TURBO_HOME}/jcprepct.c
  ${LIBJPEG_TURBO_HOME}/jcsample.c
  ${LIBJPEG_TURBO_HOME}/jctrans.c
  ${LIBJPEG_TURBO_HOME}/jdapimin.c
  ${LIBJPEG_TURBO_HOME}/jdapistd.c
  ${LIBJPEG_TURBO_HOME}/jdatadst.c
  ${LIBJPEG_TURBO_HOME}/jdatasrc.c
  ${LIBJPEG_TURBO_HOME}/jdcoefct.c
  ${LIBJPEG_TURBO_HOME}/jdcolor.c
  ${LIBJPEG_TURBO_HOME}/jddctmgr.c
  ${LIBJPEG_TURBO_HOME}/jdhuff.c
  ${LIBJPEG_TURBO_HOME}/jdinput.c
  ${LIBJPEG_TURBO_HOME}/jdmainct.c
  ${LIBJPEG_TURBO_HOME}/jdmarker.c
  ${LIBJPEG_TURBO_HOME}/jdmaster.c
  ${LIBJPEG_TURBO_HOME}/jdmerge.c
  ${LIBJPEG_TURBO_HOME}/jdphuff.c
  ${LIBJPEG_TURBO_HOME}/jdpostct.c
  ${LIBJPEG_TURBO_HOME}/jdsample.c
  ${LIBJPEG_TURBO_HOME}/jdtrans.c
  ${LIBJPEG_TURBO_HOME}/jerror.c
  ${LIBJPEG_TURBO_HOME}/jfdctflt.c
  ${LIBJPEG_TURBO_HOME}/jfdctfst.c
  ${LIBJPEG_TURBO_HOME}/jfdctint.c
  ${LIBJPEG_TURBO_HOME}/jidctflt.c
  ${LIBJPEG_TURBO_HOME}/jidctfst.c
  ${LIBJPEG_TURBO_HOME}/jidctint.c
  ${LIBJPEG_TURBO_HOME}/jidctred.c
  ${LIBJPEG_TURBO_HOME}/jquant1.c
  ${LIBJPEG_TURBO_HOME}/jquant2.c
  ${LIBJPEG_TURBO_HOME}/jutils.c
  ${LIBJPEG_TURBO_HOME}/jmemmgr.c
  ${LIBJPEG_TURBO_HOME}/jmemnobs.c
)

if(WITH_ARITH_ENC OR WITH_ARITH_DEC)
  set(JPEG_SOURCES ${JPEG_SOURCES} ${LIBJPEG_TURBO_HOME}/jaricom.c)
endif()

if(WITH_ARITH_ENC)
  set(JPEG_SOURCES ${JPEG_SOURCES} ${LIBJPEG_TURBO_HOME}/jcarith.c)
endif()

if(WITH_ARITH_DEC)
  set(JPEG_SOURCES ${JPEG_SOURCES} ${LIBJPEG_TURBO_HOME}/jdarith.c)
endif()

if(WITH_SIMD)
  add_definitions(-DWITH_SIMD)

  if(NOT DEFINED NASM)
    set(NASM nasm CACHE PATH "Path to NASM/YASM executable")
  endif()

  if(SIMD_X86_64)
    set(NAFLAGS -fwin64 -DWIN64 -D__x86_64__)
  else()
    if(BORLAND)
      set(NAFLAGS -fobj -DOBJ32)
    else()
      set(NAFLAGS -fwin32 -DWIN32)
    endif()
  endif()
  set(NAFLAGS ${NAFLAGS} -I${LIBJPEG_TURBO_HOME}/win/ -I${LIBJPEG_TURBO_HOME}/ -I${LIBJPEG_TURBO_SIMD_HOME}/)

  if(SIMD_X86_64)
    set(SIMD_BASENAMES
      jfdctflt-sse-64
      jccolor-sse2-64
      jcgray-sse2-64
      jcsample-sse2-64
      jdcolor-sse2-64
      jdmerge-sse2-64
      jdsample-sse2-64
      jfdctfst-sse2-64
      jfdctint-sse2-64
      jidctflt-sse2-64
      jidctfst-sse2-64
      jidctint-sse2-64
      jidctred-sse2-64
      jquantf-sse2-64
      jquanti-sse2-64
    )
    message(STATUS "Building x86_64 SIMD extensions")
  else()
    set(SIMD_BASENAMES
      jsimdcpu
      jfdctflt-3dn
      jidctflt-3dn
      jquant-3dn
      jccolor-mmx
      jcgray-mmx
      jcsample-mmx
      jdcolor-mmx
      jdmerge-mmx
      jdsample-mmx
      jfdctfst-mmx
      jfdctint-mmx
      jidctfst-mmx
      jidctint-mmx
      jidctred-mmx
      jquant-mmx
      jfdctflt-sse
      jidctflt-sse
      jquant-sse
      jccolor-sse2
      jcgray-sse2
      jcsample-sse2
      jdcolor-sse2
      jdmerge-sse2
      jdsample-sse2
      jfdctfst-sse2
      jfdctint-sse2
      jidctflt-sse2
      jidctfst-sse2
      jidctint-sse2
      jidctred-sse2
      jquantf-sse2
      jquanti-sse2
    )
    message(STATUS "Building i386 SIMD extensions")
  endif()

  set(OBJDIR ${LIBJPEG_TURBO_SIMD_HOME})

  file(GLOB INC_FILES ${LIBJPEG_TURBO_SIMD_HOME}/*.inc)

  foreach(file ${SIMD_BASENAMES})
    set(DEPFILE "")
    set(SIMD_SRC ${LIBJPEG_TURBO_SIMD_HOME}/${file}.asm)
    if(${file} MATCHES jccolor)
      set(DEPFILE ${file})
      string(REGEX REPLACE "jccolor" "jccolext" DEPFILE ${DEPFILE})
      set(DEPFILE ${LIBJPEG_TURBO_SIMD_HOME}/${DEPFILE}.asm)
    endif()
    if(${file} MATCHES jcgray)
      set(DEPFILE ${file})
      string(REGEX REPLACE "jcgray" "jcgryext" DEPFILE ${DEPFILE})
      set(DEPFILE ${LIBJPEG_TURBO_SIMD_HOME}/${DEPFILE}.asm)
    endif()
    if(${file} MATCHES jdcolor)
      set(DEPFILE ${file})
      string(REGEX REPLACE "jdcolor" "jdcolext" DEPFILE ${DEPFILE})
      set(DEPFILE ${LIBJPEG_TURBO_SIMD_HOME}/${DEPFILE}.asm)
    endif()
    if(${file} MATCHES jdmerge)
      set(DEPFILE ${file})
      string(REGEX REPLACE "jdmerge" "jdmrgext" DEPFILE ${DEPFILE})
      set(DEPFILE ${LIBJPEG_TURBO_SIMD_HOME}/${DEPFILE}.asm)
    endif()
    set(SIMD_OBJ ${OBJDIR}/${file}.obj)
    add_custom_command(OUTPUT ${SIMD_OBJ}
      DEPENDS ${SIMD_SRC} ${DEPFILE} ${INC_FILES}
      COMMAND ${NASM} ${NAFLAGS} ${SIMD_SRC} -o${SIMD_OBJ})
    set(SIMD_OBJS ${SIMD_OBJS} ${SIMD_OBJ})
  endforeach()

  if(SIMD_X86_64)
    set(JPEG_SOURCES ${JPEG_SOURCES} ${LIBJPEG_TURBO_HOME}/simd/jsimd_x86_64.c)
  else()
    set(JPEG_SOURCES ${JPEG_SOURCES} ${LIBJPEG_TURBO_HOME}/simd/jsimd_i386.c)
  endif()
  # This tells CMake that the "source" files haven't been generated yet
  set_source_files_properties(${SIMD_OBJS} PROPERTIES GENERATED 1)
else()
  set(JPEG_SOURCES ${JPEG_SOURCES} ${LIBJPEG_TURBO_HOME}/jsimd_none.c)
  message(STATUS "Not using SIMD acceleration")
endif()


add_library(${JPEG_LIBRARY} STATIC ${JPEG_SOURCES} ${SIMD_OBJS})
if(NOT MSVC)
  set_target_properties(${JPEG_LIBRARY}
    PROPERTIES OUTPUT_NAME ${JPEG_LIBRARY}
    ARCHIVE_OUTPUT_DIRECTORY ${THIRDPARTY_LIBRARY_PATH}
  )
endif()

if(WITH_TURBOJPEG)
  add_library(${TURBO_JPEG_LIBRARY} STATIC ${JPEG_SOURCES} ${SIMD_OBJS}
    ${LIBJPEG_TURBO_HOME}/turbojpeg.c
    ${LIBJPEG_TURBO_HOME}/transupp.c
    ${LIBJPEG_TURBO_HOME}/jdatadst-tj.c
    ${LIBJPEG_TURBO_HOME}/jdatasrc-tj.c
  )
  if(NOT MSVC)
    set_target_properties(${TURBO_JPEG_LIBRARY}
      PROPERTIES OUTPUT_NAME ${TURBO_JPEG_LIBRARY}
      ARCHIVE_OUTPUT_DIRECTORY ${THIRDPARTY_LIBRARY_PATH}
    )
  endif()
endif()
