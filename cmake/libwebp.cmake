set(WEBP_LIBRARY webp)
project(${WEBP_LIBRARY})

# SET LIBRARY PATH
set(LIBWEBP_HOME "${PROJECT_SOURCE_DIR}/src/third_party/libwebp-0.4.3")

# WEBP LIBRARY

## src/dec/Makefile.am

set(
  LIBWEBP_DEC_OBJS
  ${LIBWEBP_HOME}/src/dec/alpha.c
  ${LIBWEBP_HOME}/src/dec/buffer.c
  ${LIBWEBP_HOME}/src/dec/frame.c
  ${LIBWEBP_HOME}/src/dec/idec.c
  ${LIBWEBP_HOME}/src/dec/io.c
  ${LIBWEBP_HOME}/src/dec/quant.c
  ${LIBWEBP_HOME}/src/dec/tree.c
  ${LIBWEBP_HOME}/src/dec/vp8.c
  ${LIBWEBP_HOME}/src/dec/vp8l.c
  ${LIBWEBP_HOME}/src/dec/webp.c
)

## src/demux/Makefile.am

set(
  LIBWEBP_DEMUX_OBJS
  ${LIBWEBP_HOME}/src/demux/demux.c
)

## src/dsp/Makefile.am

set(
  LIBWEBP_DSP_COMMON_OBJS
  ${LIBWEBP_HOME}/src/dsp/alpha_processing.c
  ${LIBWEBP_HOME}/src/dsp/cpu.c
  ${LIBWEBP_HOME}/src/dsp/dec.c
  ${LIBWEBP_HOME}/src/dsp/dec_clip_tables.c
  ${LIBWEBP_HOME}/src/dsp/dec_neon.c
  ${LIBWEBP_HOME}/src/dsp/lossless.c
  ${LIBWEBP_HOME}/src/dsp/upsampling.c
  ${LIBWEBP_HOME}/src/dsp/yuv.c
)

if (FOUND_MIPS32)
set(
  LIBWEBP_DSP_COMMON_OBJS
  ${LIBWEBP_DSP_COMMON_OBJS}
  ${LIBWEBP_HOME}/src/dsp/dec_mips32.c
  ${LIBWEBP_HOME}/src/dsp/lossless_mips32.c
  ${LIBWEBP_HOME}/src/dsp/yuv_mips32.c
)
endif()

if (FOUND_NEON)
set(
  LIBWEBP_DSP_COMMON_OBJS
  ${LIBWEBP_DSP_COMMON_OBJS}
  ${LIBWEBP_HOME}/src/dsp/lossless_neon.c
  ${LIBWEBP_HOME}/src/dsp/upsampling_neon.c
)
endif()

if (FOUND_SSE2)
set(
  LIBWEBP_DSP_COMMON_OBJS
  ${LIBWEBP_DSP_COMMON_OBJS}
  ${LIBWEBP_HOME}/src/dsp/alpha_processing_sse2.c
  ${LIBWEBP_HOME}/src/dsp/dec_sse2.c
  ${LIBWEBP_HOME}/src/dsp/lossless_sse2.c
  ${LIBWEBP_HOME}/src/dsp/upsampling_sse2.c
  ${LIBWEBP_HOME}/src/dsp/yuv_sse2.c
)
endif()

set(
  LIBWEBP_DSP_DEC_OBJS
  ${LIBWEBP_DSP_COMMON_OBJS}
)

set(
  LIBWEBP_DSP_ENC_OBJS
  ${LIBWEBP_DSP_COMMON_OBJS}
  ${LIBWEBP_HOME}/src/dsp/enc.c
)

if (FOUND_MIPS32)
set(
  LIBWEBP_DSP_ENC_OBJS
  ${LIBWEBP_DSP_ENC_OBJS}
  ${LIBWEBP_HOME}/src/dsp/enc_mips32.c
)
endif()

if (FOUND_NEON)
set(
  LIBWEBP_DSP_ENC_OBJS
  ${LIBWEBP_DSP_ENC_OBJS}
  ${LIBWEBP_HOME}/src/dsp/enc_neon.c
)
endif()

if (FOUND_SSE2)
set(
  LIBWEBP_DSP_ENC_OBJS
  ${LIBWEBP_DSP_ENC_OBJS}
  ${LIBWEBP_HOME}/src/dsp/enc_sse2.c
)
endif()

if (FOUND_AVX2)
set(
  LIBWEBP_DSP_ENC_OBJS
  ${LIBWEBP_DSP_ENC_OBJS}
  ${LIBWEBP_HOME}/src/dsp/enc_avx2.c
)
endif()

## src/enc/Makefile.am

set(
  LIBWEBP_ENC_OBJS
  ${LIBWEBP_HOME}/src/enc/alpha.c
  ${LIBWEBP_HOME}/src/enc/analysis.c
  ${LIBWEBP_HOME}/src/enc/backward_references.c
  ${LIBWEBP_HOME}/src/enc/config.c
  ${LIBWEBP_HOME}/src/enc/cost.c
  ${LIBWEBP_HOME}/src/enc/filter.c
  ${LIBWEBP_HOME}/src/enc/frame.c
  ${LIBWEBP_HOME}/src/enc/histogram.c
  ${LIBWEBP_HOME}/src/enc/iterator.c
  ${LIBWEBP_HOME}/src/enc/picture.c
  ${LIBWEBP_HOME}/src/enc/picture_csp.c
  ${LIBWEBP_HOME}/src/enc/picture_psnr.c
  ${LIBWEBP_HOME}/src/enc/picture_rescale.c
  ${LIBWEBP_HOME}/src/enc/picture_tools.c
  ${LIBWEBP_HOME}/src/enc/quant.c
  ${LIBWEBP_HOME}/src/enc/syntax.c
  ${LIBWEBP_HOME}/src/enc/token.c
  ${LIBWEBP_HOME}/src/enc/tree.c
  ${LIBWEBP_HOME}/src/enc/vp8l.c
  ${LIBWEBP_HOME}/src/enc/webpenc.c
)

## src/mux/Makefile.am

set(
  LIBWEBP_MUX_OBJS
  ${LIBWEBP_HOME}/src/mux/muxedit.c
  ${LIBWEBP_HOME}/src/mux/muxinternal.c
  ${LIBWEBP_HOME}/src/mux/muxread.c
)

## src/utils/Makefile.am

set(
  LIBWEBP_UTILS_COMMON_OBJS
  ${LIBWEBP_HOME}/src/utils/bit_reader.c
  ${LIBWEBP_HOME}/src/utils/color_cache.c
  ${LIBWEBP_HOME}/src/utils/filters.c
  ${LIBWEBP_HOME}/src/utils/huffman.c
  ${LIBWEBP_HOME}/src/utils/quant_levels_dec.c
  ${LIBWEBP_HOME}/src/utils/random.c
  ${LIBWEBP_HOME}/src/utils/rescaler.c
  ${LIBWEBP_HOME}/src/utils/thread.c
  ${LIBWEBP_HOME}/src/utils/utils.c
)

set(
  LIBWEBP_UTILS_ENC_OBJS
  ${LIBWEBP_UTILS_COMMON_OBJS}
  ${LIBWEBP_HOME}/src/utils/bit_writer.c
  ${LIBWEBP_HOME}/src/utils/huffman_encode.c
  ${LIBWEBP_HOME}/src/utils/quant_levels.c
)

set(
  LIBWEBP_UTILS_DEC_OBJS
  ${LIBWEBP_UTILS_COMMON_OBJS}
)

## src/Makefile.am

set(
  LIBWEBP_OBJS
  ${LIBWEBP_DEC_OBJS}
  ${LIBWEBP_DSP_ENC_OBJS}
  ${LIBWEBP_ENC_OBJS}
  ${LIBWEBP_UTILS_ENC_OBJS}
)

set(
  LIBWEBPDECODER_OBJS
  ${LIBWEBP_DEC_OBJS}
  ${LIBWEBP_DSP_DEC_OBJS}
  ${LIBWEBP_UTILS_DEC_OBJS}
)

# define

add_definitions(-DWEBP_USE_THREAD)

include_directories("${LIBWEBP_HOME}/src")

add_library(${WEBP_LIBRARY} STATIC ${LIBWEBP_OBJS} ${LIBWEBPDECODER_OBJS})

set_target_properties(
  ${WEBP_LIBRARY}
  PROPERTIES OUTPUT_NAME ${WEBP_LIBRARY}
  ARCHIVE_OUTPUT_DIRECTORY ${THIRDPARTY_LIBRARY_PATH}
)
