libsakura
=========

# Usage

# Build: Debug

## OSX

Download LLVM 3.6.0 Pre-built Binary from [link](http://llvm.org/releases/download.html#3.6.0)

```bash
$ export PATH=$HOME/llvm/3.6.0/bin:$PATH
```

```bash
$ cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug .
$ make -j4
$ ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer) ./sakura
```

## Linux

```bash
$ cmake -DCMAKE_BUILD_TYPE=Debug .
$ make -j4
$ ASAN_OPTIONS=symbolize=1:detect_leaks=1 ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer) ./sakura
```

# Feature

- Use libswscale
- Supported format: PNG, JPEG, WEBP
- Supported Alpha Channel
- Supported OS: OSX, Linux

# Third Party Libraries

- giflib: git://git.code.sf.net/p/giflib/code
- libjpeg-turbo: http://svn.code.sf.net/p/libjpeg-turbo/code
- libwebp: https://chromium.googlesource.com/webm/libwebp
- zlib: git://github.com/madler/zlib.git
- libpng: git://git.code.sf.net/p/libpng/code
- libavutil: git://github.com/FFmpeg/FFmpeg.git
- libswscale: git://github.com/FFmpeg/FFmpeg.git

# Sample Images

http://www.eecs.qmul.ac.uk/~phao/CIP/Images/

