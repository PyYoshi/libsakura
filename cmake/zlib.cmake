set(ZLIB_LIBRARY zlib)
project(${ZLIB_LIBRARY})

# SET LIBRARY PATH
set(ZLIB_HOME "${PROJECT_SOURCE_DIR}/src/third_party/zlib-1.2.8")

option(ASM686 "Enable building i686 assembly implementation")
option(AMD64 "Enable building amd64 assembly implementation")

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)

#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif()
if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif()
if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif()
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

if(NOT ZLIB_HOME STREQUAL ZLIB_HOME)
    # If we're doing an out of source build and the user has a zconf.h
    # in their source tree...
    if(EXISTS ${ZLIB_HOME}/zconf.h)
        message(STATUS "Renaming")
        message(STATUS "    ${ZLIB_HOME}/zconf.h")
        message(STATUS "to 'zconf.h.included' because this file is included with zlib")
        message(STATUS "but CMake generates it automatically in the build directory.")
        file(RENAME ${ZLIB_HOME}/zconf.h ${ZLIB_HOME}/zconf.h.included)
  endif()
endif()

set(ZLIB_PC ${ZLIB_HOME}/zlib.pc)
configure_file( ${ZLIB_HOME}/zlib.pc.cmakein
		${ZLIB_PC} @ONLY)
configure_file(	${ZLIB_HOME}/zconf.h.cmakein
		${ZLIB_HOME}/zconf.h @ONLY)

#============================================================================
# zlib
#============================================================================

set(ZLIB_PUBLIC_HDRS
    ${ZLIB_HOME}/zconf.h
    ${ZLIB_HOME}/zlib.h
)
set(ZLIB_PRIVATE_HDRS
    ${ZLIB_HOME}/crc32.h
    ${ZLIB_HOME}/deflate.h
    ${ZLIB_HOME}/gzguts.h
    ${ZLIB_HOME}/inffast.h
    ${ZLIB_HOME}/inffixed.h
    ${ZLIB_HOME}/inflate.h
    ${ZLIB_HOME}/inftrees.h
    ${ZLIB_HOME}/trees.h
    ${ZLIB_HOME}/zutil.h
)
set(ZLIB_SRCS
    ${ZLIB_HOME}/adler32.c
    ${ZLIB_HOME}/compress.c
    ${ZLIB_HOME}/crc32.c
    ${ZLIB_HOME}/deflate.c
    ${ZLIB_HOME}/gzclose.c
    ${ZLIB_HOME}/gzlib.c
    ${ZLIB_HOME}/gzread.c
    ${ZLIB_HOME}/gzwrite.c
    ${ZLIB_HOME}/inflate.c
    ${ZLIB_HOME}/infback.c
    ${ZLIB_HOME}/inftrees.c
    ${ZLIB_HOME}/inffast.c
    ${ZLIB_HOME}/trees.c
    ${ZLIB_HOME}/uncompr.c
    ${ZLIB_HOME}/zutil.c
)

if(CMAKE_COMPILER_IS_GNUCC)
    if(ASM686)
        set(ZLIB_ASMS ${ZLIB_HOME}/contrib/asm686/match.S)
    elseif (AMD64)
        set(ZLIB_ASMS ${ZLIB_HOME}/contrib/amd64/amd64-match.S)
    endif ()

	if(ZLIB_ASMS)
		add_definitions(-DASMV)
		set_source_files_properties(${ZLIB_ASMS} PROPERTIES LANGUAGE C COMPILE_FLAGS -DNO_UNDERLINE)
	endif()
endif()

if(MSVC)
    if(ASM686)
		ENABLE_LANGUAGE(ASM_MASM)
        set(ZLIB_ASMS
			${ZLIB_HOME}/contrib/masmx86/inffas32.asm
			${ZLIB_HOME}/contrib/masmx86/match686.asm
		)
    elseif (AMD64)
		ENABLE_LANGUAGE(ASM_MASM)
        set(ZLIB_ASMS
			${ZLIB_HOME}/contrib/masmx64/gvmat64.asm
			${ZLIB_HOME}/contrib/masmx64/inffasx64.asm
		)
    endif()

	if(ZLIB_ASMS)
		add_definitions(-DASMV -DASMINF)
	endif()
endif()

# parse the full version number from zlib.h and include in ZLIB_FULL_VERSION
file(READ ${ZLIB_HOME}/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*"
    "\\1" ZLIB_FULL_VERSION ${_zlib_h_contents})

add_library(${ZLIB_LIBRARY} STATIC ${ZLIB_SRCS} ${ZLIB_ASMS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
set_target_properties(
  ${ZLIB_LIBRARY}
  PROPERTIES OUTPUT_NAME z
  ARCHIVE_OUTPUT_DIRECTORY ${THIRDPARTY_LIBRARY_PATH}
)
