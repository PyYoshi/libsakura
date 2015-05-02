# Base: https://github.com/tarantool/tarantool/blob/master/cmake/simd.cmake
# http://qiita.com/yohhoy/items/5850e707f01cc905a272

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_COMPILER_IS_CLANG TRUE)
endif()

if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|^i[3,9]86$")
    return()
endif()

include(CheckCXXSourceRuns)

#
# Check compiler for SSE intrinsics
#
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG )
    set(CMAKE_REQUIRED_FLAGS "-msse")
    check_cxx_source_runs("
    #include <immintrin.h>

    int main()
    {
    __m128i a = _mm_setzero_ps();
    return 0;
    }"
    CC_HAS_SSE_INTRINSICS)
endif()

#
# Check compiler for SSE2 intrinsics
#
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG )
    set(CMAKE_REQUIRED_FLAGS "-msse2")
    check_cxx_source_runs("
    #include <immintrin.h>

    int main()
    {
    __m128i a = _mm_setzero_si128();
    return 0;
    }"
    CC_HAS_SSE2_INTRINSICS)
endif()

#
# Check compiler for SSE3 intrinsics
#
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG )
    set(CMAKE_REQUIRED_FLAGS "-msse3")
    check_cxx_source_runs("
    #include <immintrin.h>

    int main()
    {
    const int vals[4] = {0,0,0,0};
    __m128i a = _mm_lddqu_si128((const __m128i*)vals);
    return 0;
    }"
    CC_HAS_SSE3_INTRINSICS)
endif()

#
# Check compiler for SSE4.1 intrinsics
#
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG )
    set(CMAKE_REQUIRED_FLAGS "-msse4 -msse4.1")
    check_cxx_source_runs("
    #include <smmintrin.h>
    int main ()
    {
    __m128 a, b;
    float vals[4] = {1, 2, 3, 4};
    const int mask = 123;
    a = _mm_loadu_ps (vals);
    b = a;
    b = _mm_dp_ps (a, a, mask);
    _mm_storeu_ps (vals,b);
    return (0);
    }"
    CC_HAS_SSE4_1_INTRINSICS)
endif()

#
# Check compiler for SSE4.2 intrinsics
#
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG )
    set(CMAKE_REQUIRED_FLAGS "-msse4 -msse4.2")
    check_cxx_source_runs("
    #include <emmintrin.h>
    #include <nmmintrin.h>
    int main ()
    {
    long long a[2] = {  1, 2 };
    long long b[2] = { -1, 3 };
    long long c[2];
    __m128i va = _mm_loadu_si128 ((__m128i*)a);
    __m128i vb = _mm_loadu_si128 ((__m128i*)b);
    __m128i vc = _mm_cmpgt_epi64 (va, vb);
    _mm_storeu_si128 ((__m128i*)c, vc);
    if (c[0] == -1LL && c[1] == 0LL)
    return (0);
    else
    return (1);
    }"
    CC_HAS_SSE4_2_INTRINSICS)
endif()

#
# Check compiler for AVX intrinsics
#
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG )
    set(CMAKE_REQUIRED_FLAGS "-mavx")
    check_cxx_source_runs("
    #include <immintrin.h>

    int main()
    {
    __m256i a = _mm256_setzero_si256();
    return 0;
    }"
    CC_HAS_AVX_INTRINSICS)
endif()

#
# Check compiler for AVX2 intrinsics
#
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG )
    set(CMAKE_REQUIRED_FLAGS "-mavx2")
    check_cxx_source_runs("
    #include <immintrin.h>

    int main()
    {
    __m256i a = _mm256_setzero_si256();
    __m256i result = _mm256_and_si256(a, a);
    return 0;
    }"
    CC_HAS_AVX2_INTRINSICS)
endif()

IF (CC_HAS_SSE_INTRINSICS)
   set(SSE_FOUND true CACHE BOOL "SSE available on host")
ELSE (CC_HAS_SSE_INTRINSICS)
   set(SSE_FOUND false CACHE BOOL "SSE available on host")
ENDIF (CC_HAS_SSE_INTRINSICS)

IF (CC_HAS_SSE2_INTRINSICS)
   set(SSE2_FOUND true CACHE BOOL "SSE2 available on host")
ELSE (CC_HAS_SSE2_INTRINSICS)
   set(SSE2_FOUND false CACHE BOOL "SSE2 available on host")
ENDIF (CC_HAS_SSE2_INTRINSICS)

IF (CC_HAS_SSE3_INTRINSICS)
   set(SSE3_FOUND true CACHE BOOL "SSE3 available on host")
ELSE (CC_HAS_SSE3_INTRINSICS)
   set(SSE3_FOUND false CACHE BOOL "SSE3 available on host")
ENDIF (CC_HAS_SSE3_INTRINSICS)

IF (CC_HAS_SSE4_1_INTRINSICS)
   set(SSE4_1_FOUND true CACHE BOOL "SSE4.1 available on host")
ELSE (CC_HAS_SSE4_1_INTRINSICS)
   set(SSE4_1_FOUND false CACHE BOOL "SSE4.1 available on host")
ENDIF (CC_HAS_SSE4_1_INTRINSICS)

IF (CC_HAS_SSE4_2_INTRINSICS)
   set(SSE4_2_FOUND true CACHE BOOL "SSE4.2 available on host")
ELSE (CC_HAS_SSE4_2_INTRINSICS)
   set(SSE4_2_FOUND false CACHE BOOL "SSE4.2 available on host")
ENDIF (CC_HAS_SSE4_2_INTRINSICS)

IF (CC_HAS_AVX_INTRINSICS)
   set(AVX_FOUND true CACHE BOOL "AVX available on host")
ELSE (CC_HAS_AVX_INTRINSICS)
   set(AVX_FOUND false CACHE BOOL "AVX available on host")
ENDIF (CC_HAS_AVX_INTRINSICS)

IF (CC_HAS_AVX2_INTRINSICS)
   set(AVX2_FOUND true CACHE BOOL "AVX2 available on host")
ELSE (CC_HAS_AVX2_INTRINSICS)
   set(AVX2_FOUND false CACHE BOOL "AVX2 available on host")
ENDIF (CC_HAS_AVX2_INTRINSICS)

if(NOT SSE_FOUND)
   MESSAGE(STATUS "Could not find hardware support for SSE on this machine.")
endif(NOT SSE_FOUND)

if(NOT SSE2_FOUND)
   MESSAGE(STATUS "Could not find hardware support for SSE2 on this machine.")
endif(NOT SSE2_FOUND)

if(NOT SSE3_FOUND)
   MESSAGE(STATUS "Could not find hardware support for SSE3 on this machine.")
endif(NOT SSE3_FOUND)

if(NOT SSE4_1_FOUND)
   MESSAGE(STATUS "Could not find hardware support for SSE4.1 on this machine.")
endif(NOT SSE4_1_FOUND)

if(NOT SSE4_2_FOUND)
   MESSAGE(STATUS "Could not find hardware support for SSE4.2 on this machine.")
endif(NOT SSE4_2_FOUND)

if(NOT AVX_FOUND)
   MESSAGE(STATUS "Could not find hardware support for AVX on this machine.")
endif(NOT AVX_FOUND)

if(NOT AVX2_FOUND)
   MESSAGE(STATUS "Could not find hardware support for AVX2 on this machine.")
endif(NOT AVX2_FOUND)

mark_as_advanced(SSE_FOUND SSE2_FOUND SSE3_FOUND SSE4_1_FOUND AVX_FOUND AVX2_FOUND)
