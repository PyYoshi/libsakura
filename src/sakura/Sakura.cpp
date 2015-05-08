#include "Sakura.h"

extern "C"
{
#include "gif_lib.h"

#include "webp/encode.h"
#include "webp/decode.h"

#include "zlib.h"
#include "png.h"

#include "turbojpeg.h"

#include "libavutil/log.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
}

typedef struct {
    unsigned char * src;
    size_t size;
    size_t read;
} GifHelperReadCbData;

typedef struct {
    unsigned char * src;
    size_t size;
    size_t read;
} PngHelperReadCbData;

typedef struct {
    unsigned char * src;
    size_t size;
} PngHelperWriteCbData;

int GifHelperByteBufferReadFun(GifFileType* gif, GifByteType* bytes, int size) {
    GifHelperReadCbData* cbData = (GifHelperReadCbData*) gif->UserData;
    if (cbData->read + size > cbData->size) {
        memcpy(bytes, cbData->src + cbData->read, cbData->size - cbData->read);
        cbData->read = cbData->read;
        return (int) (cbData->size - cbData->read);
    }
    memcpy(bytes, cbData->src + cbData->read, (size_t) size);
    cbData->read += size;
    return size;
}

void PngHelperByteBufferReadFun(png_structp png_ptr, png_bytep data, png_size_t size) {
    PngHelperReadCbData * cbData = (PngHelperReadCbData *) png_get_io_ptr(png_ptr);
    if (cbData->read + size > cbData->size) {
        png_error(png_ptr, "PNG source buffer is missing data");
        return;
    }
    memcpy(data, cbData->src + cbData->read, size);
    cbData->read += size;
    return;
}

void PngHelperByteBufferWriteFun(png_structp png_ptr, png_bytep data, png_size_t size) {
    PngHelperWriteCbData * cbData = (PngHelperWriteCbData *) png_get_io_ptr(png_ptr);
    size_t totalSize = cbData->size + size;
    if (cbData->src) {
        cbData->src = (unsigned char *) realloc(cbData->src, totalSize * sizeof(unsigned char));
    } else {
        cbData->src = (unsigned char *) malloc(totalSize * sizeof(unsigned char));
    }
    if (!cbData->src) {
        png_error(png_ptr, "Out of memory");
        return;
    }
    memcpy(cbData->src + cbData->size, data, size);
    cbData->size += size;
}

std::string Sakura::Utils::GetFileName(const char *filePath) {
    std::string path = filePath;

    char sep = '/';
#ifdef _WIN32
   sep = '\\';
#endif

    size_t i = path.rfind(sep, path.length());
    if (i != std::string::npos) {
        return (path.substr(i + 1, path.length() - i));
    }
    return ("");
}

std::string Sakura::Utils::GetExt(std::string fileName) {
    std::string ext;
    size_t pos = fileName.rfind('.', fileName.length());
    if (pos != std::string::npos) {
        ext = fileName.substr(pos + 1, fileName.size() - pos);
        std::string::iterator itr = ext.begin();
        while (itr != ext.end()) {
            *itr = tolower(*itr);
            itr++;
        }
        itr = ext.end() - 1;
        while (itr != ext.begin()) {
            if (*itr == 0 || *itr == 32) {
                ext.erase(itr--);
            }
            else {
                itr--;
            }
        }
    }
    return ext;
}

Sakura::PictureType Sakura::Utils::GetType(std::string ext) {
    if (ext == "bmp") {
        return Sakura::PictureType::BITMAP;
    } else if (ext == "png") {
        return Sakura::PictureType::PNG;
    } else if (ext == "jpg" || ext == "jpeg") {
        return Sakura::PictureType::JPEG;
    } else if (ext == "webp") {
        return Sakura::PictureType::WEBP;
    } else if (ext == "gif") {
        return Sakura::PictureType::GIF;
    } else {
        return Sakura::PictureType::UNKNOWN;
    }
}

Sakura::Picture::~Picture() {
    delete[]rgba;
}

Sakura::Picture *Sakura::Scale(Sakura::Picture * pic, int outWidth, int outHeight, Sakura::ScaleFilter scaleMode) {
    // ignore message: Warning: data is not aligned! This can lead to a speedloss
    av_log_set_level(AV_LOG_ERROR);

    int stride;
    AVPixelFormat format;
    if (pic->hasAlpha) {
        format = PIX_FMT_RGBA;
        stride = outWidth * 4;
    } else {
        format = PIX_FMT_RGB24;
        stride = outWidth * 3;
    }

    struct SwsContext *scaler;
    scaler = sws_getContext(pic->width, pic->height, format, outWidth, outHeight, format, scaleMode, NULL,
                            NULL, NULL);

    Sakura::Picture *oPic = new Sakura::Picture();
    oPic->width = outWidth;
    oPic->height = outHeight;
    oPic->stride = stride;
    oPic->rgba = new unsigned char[oPic->height * oPic->stride];
    oPic->hasAlpha = pic->hasAlpha;

    sws_scale(scaler, (const unsigned char * const*)&pic->rgba, &pic->stride, 0, pic->height, &oPic->rgba, &oPic->stride);

    sws_freeContext(scaler);
    return oPic;
}

Sakura::Picture * Sakura::LoadFromFile(const char * filePath) {
    Sakura::Picture * pic;
    std::string fileName = Sakura::Utils::GetFileName(filePath);
    std::string ext = Sakura::Utils::GetExt(fileName);
    int type = Sakura::Utils::GetType(ext);
    switch (type) {
        case Sakura::PictureType::BITMAP :
            pic = Sakura::LoadBitmap(filePath);
            break;
        case Sakura::PictureType::PNG :
            pic = Sakura::LoadPng(filePath);
            break;
        case Sakura::PictureType::JPEG :
            pic = Sakura::LoadJpeg(filePath);
            break;
        case Sakura::PictureType::WEBP :
            pic = Sakura::LoadWebp(filePath);
            break;
        case Sakura::PictureType::GIF :
            pic = Sakura::LoadGif(filePath);
            break;
        default:
            std::string msg = "Unsupported image format: ";
            msg += filePath;
            throw Sakura::Exception(msg);
    }
    return pic;
}

Sakura::Picture * Sakura::LoadBitmap(const char * filePath) {
    throw "Function not yet implemented.";
}

Sakura::Picture * Sakura::LoadBitmap(unsigned char * inputBuffer, unsigned long * bufSize) {
    throw "Function not yet implemented.";
}

Sakura::Picture * Sakura::LoadPng(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char * fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        Sakura::Picture * pic = Sakura::LoadPng(fileBuffer, &fileSize);
        delete[] fileBuffer;

        return pic;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw Sakura::Exception(msg);
    };
}

Sakura::Picture * Sakura::LoadPng(unsigned char * inputBuffer, unsigned long * bufSize) {
    Sakura::Picture * pic = new Sakura::Picture();
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        delete pic;
        throw Sakura::Exception("Out of memory");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        delete pic;
        throw Sakura::Exception("Out of memory");
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        delete pic;
        throw Sakura::Exception("Out of memory");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete pic;
        throw Sakura::Exception("Out of memory");
    }

    PngHelperReadCbData cbData = {inputBuffer, *bufSize, 0};
    png_set_read_fn(png_ptr, (voidp) &cbData, PngHelperByteBufferReadFun);

    png_read_info(png_ptr, info_ptr);
    png_set_sig_bytes(png_ptr, 8);

    unsigned int width=0, height=0;
    int bitDepth=0, colorType=0;
    bool isGray = false;
    png_get_IHDR(png_ptr, info_ptr, (png_uint_32p)&width, (png_uint_32p)&height, &bitDepth, &colorType, NULL, NULL, NULL);

    if (colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
        colorType = PNG_COLOR_TYPE_RGB;
        bitDepth = 8;
    }

    if (colorType == PNG_COLOR_TYPE_GRAY && colorType < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
        isGray = true;
        bitDepth = 8;
    }

    if (png_get_valid(png_ptr, info_ptr, (png_uint_32)PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
        colorType |= PNG_COLOR_MASK_ALPHA;
    }

    if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
        colorType |= PNG_COLOR_MASK_COLOR;
        isGray = true;
    }

    if (colorType == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, (png_uint_32)0xffffU, PNG_FILLER_AFTER);

    if (!(colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_RGB_ALPHA)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        throw Sakura::Exception("Invalid PNG color coding");
    }

    if (bitDepth != 8) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        std::string msg = "Invalid PNG bit depth: ";
        msg += bitDepth;
        throw Sakura::Exception(msg);
    }

    bool hasAlpha = (colorType == PNG_COLOR_TYPE_RGBA);

    png_read_update_info(png_ptr, info_ptr);

    pic->width = width;
    pic->height = height;
    pic->hasAlpha = true;
    pic->stride = width * 4;

    pic->rgba = new unsigned char[pic->height * pic->stride];
    if(!pic->rgba)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        throw Sakura::Exception("Out of memory");
    }

    png_bytep* rowsPtr = new png_bytep[height];
    if(!rowsPtr)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        throw Sakura::Exception("Out of memory");
    }

    for (int i = 0; i < height; ++i)
        rowsPtr[i] = &pic->rgba[i * pic->stride];

    png_read_image(png_ptr, rowsPtr);
    png_read_end(png_ptr, end_info);

    delete[] rowsPtr;
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    return pic;
}

Sakura::Picture * Sakura::LoadJpeg(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char * fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        Sakura::Picture * pic = Sakura::LoadJpeg(fileBuffer, &fileSize);
        delete[] fileBuffer;

        return pic;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw Sakura::Exception(msg);
    };
}

Sakura::Picture * Sakura::LoadJpeg(unsigned char * inputBuffer, unsigned long * bufSize) {
    Sakura::Picture * pic = new Sakura::Picture();
    tjhandle jpegHandle = tjInitDecompress();
    int jpegSubsample = 0;
    int width, height;

    tjDecompressHeader2(jpegHandle, inputBuffer, *bufSize, &width, &height, &jpegSubsample);

    unsigned char *rgbBuffer = new unsigned char[tjBufSize(width, height, jpegSubsample)];

    int stride = width * tjPixelSize[TJPF_RGB];

    pic->width = width;
    pic->height = height;
    pic->stride = stride;
    pic->hasAlpha = false;

    const int result = tjDecompress2(jpegHandle, inputBuffer, *bufSize, rgbBuffer,
                                     width, stride, height, TJPF_RGB, TJFLAG_FASTUPSAMPLE | TJFLAG_FASTDCT);
    pic->rgba = rgbBuffer;

    if (result != 0) {
        std::string msg = "Failed while tjCompress2: ";
        msg += tjGetErrorStr();
        tjDestroy(jpegHandle);
        delete pic;
        throw Sakura::Exception(msg);
    }

    tjDestroy(jpegHandle);

    return pic;
}

Sakura::Picture * Sakura::LoadWebp(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char *fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        Sakura::Picture * pic = Sakura::LoadWebp(fileBuffer, &fileSize);
        delete[] fileBuffer;
        return pic;

    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw Sakura::Exception(msg);
    }
}

Sakura::Picture * Sakura::LoadWebp(unsigned char * inputBuffer, unsigned long * bufSize) {
    Sakura::Picture * pic = new Sakura::Picture();
    WebPBitstreamFeatures *webpFeature = new WebPBitstreamFeatures();
    VP8StatusCode gRet = WebPGetFeatures(inputBuffer, *bufSize, webpFeature);
    if (gRet == VP8_STATUS_OK) {
        if (webpFeature->has_animation == 1) {
            std::string msg = "Unsupported WebP animation mode";
            delete webpFeature;
            delete pic;
            throw Sakura::Exception(msg);
        }

        int stride = 0;
        if (webpFeature->has_alpha == 1) {
            pic->rgba = WebPDecodeRGBA(inputBuffer, *bufSize, &pic->width, &pic->height);
            pic->hasAlpha = true;
            stride = pic->width * 4;
        } else {
            pic->rgba = WebPDecodeRGB(inputBuffer, *bufSize, &pic->width, &pic->height);
            pic->hasAlpha = false;
            stride = pic->width * 3;
        }
        pic->stride = stride;

        if (pic->rgba == NULL) {
            std::string msg = "Invalid WebP format";
            delete webpFeature;
            delete pic;
            throw Sakura::Exception(msg);
        }

        delete webpFeature;
        return pic;
    } else {
        std::string msg = "Invalid WebP format";
        delete webpFeature;
        delete pic;
        throw Sakura::Exception(msg);
    };
}

Sakura::Picture * Sakura::LoadGif(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char* fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        Sakura::Picture * pic = Sakura::LoadGif(fileBuffer, &fileSize);
        delete[] fileBuffer;
        return pic;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw Sakura::Exception(msg);
    }
}

Sakura::Picture * Sakura::LoadGif(unsigned char * inputBuffer, unsigned long * bufSize) {
    Sakura::Picture * pic = new Sakura::Picture();
    int gifError = 0;
    GifHelperReadCbData cbData = {inputBuffer, *bufSize, 0};
    GifFileType* gif = DGifOpen((void*)&cbData, &GifHelperByteBufferReadFun, &gifError);
    if (gif == NULL) {
        std::string msg = "gif error: ";
        msg += GifErrorString(gifError);
        delete pic;
        throw Sakura::Exception(msg);
    }
    if (gifError != D_GIF_SUCCEEDED) {
        std::string msg = "gif error: ";
        msg += GifErrorString(gif->Error);
        delete pic;
        throw Sakura::Exception(msg);
    }

    if (DGifSlurp(gif) == GIF_ERROR) {
        std::string msg = "gif error: ";
        msg += GifErrorString(gif->Error);
        delete pic;
        throw Sakura::Exception(msg);
    }

    GraphicsControlBlock gcb;
    bool hasGCB = DGifSavedExtensionToGCB(gif, 0, &gcb) != GIF_ERROR;

    // Unsupported Animation GIF File
    SavedImage * gifimg = &gif->SavedImages[0];

    int stride = gif->SWidth * 4;
    pic->width = gif->SWidth;
    pic->height = gif->SHeight;
    pic->stride = stride;
    pic->hasAlpha = true;

    GifByteType ci;
    pic->rgba = new unsigned char[pic->stride * pic->height];
    int len = pic->width * pic->height;
    unsigned char alpha;
    GifColorType c;
    GifColorType c_trans = {0, 0, 0};
    ColorMapObject * cmap = NULL != gifimg->ImageDesc.ColorMap
                            ? gifimg->ImageDesc.ColorMap
                            : gif->SColorMap;
    for (int i=0; i<len; i++){
        ci = gifimg->RasterBits[i];
        if (hasGCB && ci == gcb.TransparentColor){
            c = c_trans;
            alpha = 0;
        } else {
            c = cmap->Colors[ci];
            alpha = 255;
        }
        pic->rgba[i * 4] = c.Red;
        pic->rgba[i * 4 + 1] = c.Green;
        pic->rgba[i * 4 + 2] = c.Blue;
        pic->rgba[i * 4 + 3] = alpha;
    }

    if (DGifCloseFile(gif, &gifError) == GIF_ERROR) {
        std::string msg = "gif error: ";
        msg += GifErrorString(gifError);
        delete pic;
        throw Sakura::Exception(msg);
    }

    return pic;
}

void Sakura::OutputBitmap(const char *filePath, Sakura::Picture *pic) {
    throw "Function not yet implemented.";
}

unsigned long Sakura::OutputBitmap(unsigned char ** outputBuffer, Sakura::Picture *pic) {
    throw "Function not yet implemented.";
}

void Sakura::OutputPng(const char *filePath, Sakura::Picture *pic, int compLevel) {
    unsigned char *pngBuf = NULL;
    unsigned long pngSize = Sakura::OutputPng(&pngBuf, pic, compLevel);

    FILE *fp = fopen(filePath, "wb");
    if (fp != NULL) {
        fwrite(pngBuf, pngSize, 1, fp);
        fclose(fp);
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw Sakura::Exception(msg);
    }
}

unsigned long Sakura::OutputPng(unsigned char ** outputBuffer, Sakura::Picture *pic, int compLevel) {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr) {
        throw Sakura::Exception("Out of memory");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        throw Sakura::Exception("Out of memory");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        throw Sakura::Exception("PNG compression error");
    }

    png_bytep * const rowPnts = new png_bytep[pic->height];
    if (!rowPnts) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        throw Sakura::Exception("Out of memory");
    }
    for (unsigned int row = 0; row < pic->height; row++) {
        rowPnts[row] = new png_byte[pic->stride];
        if (!rowPnts[row]) {
            for (unsigned int p = 0 ; p < row ; p++) delete[] rowPnts[p];
            delete[] rowPnts;
            png_destroy_write_struct(&png_ptr, &info_ptr);
            throw Sakura::Exception("Out of memory");
        }
    }

    png_set_IHDR(png_ptr, info_ptr, (png_uint_32)pic->width, (png_uint_32)pic->height, 8, pic->hasAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_compression_level(png_ptr, compLevel);

    PngHelperWriteCbData cbData = {NULL, 0};
    png_set_write_fn(png_ptr, (voidp) &cbData, PngHelperByteBufferWriteFun, NULL);

    for(int h=0; h < pic->height; h++){
        for(int w=0; w < pic->width; w++){
            if (pic->hasAlpha) {
                rowPnts[h][(w * 4) + 0] = pic->rgba[(h * w) + 0];
                rowPnts[h][(w * 4) + 1] = pic->rgba[(h * w) + 1];
                rowPnts[h][(w * 4) + 2] = pic->rgba[(h * w) + 2];
                rowPnts[h][(w * 4) + 3] = pic->rgba[(h * w) + 3];
            } else {
                rowPnts[h][(w * 3) + 0] = pic->rgba[(h * w) + 0];
                rowPnts[h][(w * 3) + 1] = pic->rgba[(h * w) + 1];
                rowPnts[h][(w * 3) + 2] = pic->rgba[(h * w) + 2];
            };
        }
    }

    png_set_rows(png_ptr, info_ptr, rowPnts);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    for (unsigned int r = 0; r < pic->height; r++) delete[] rowPnts[r];
    delete[] rowPnts;

    outputBuffer = &cbData.src;
    return cbData.size;
}

void Sakura::OutputJpeg(const char *filePath, Sakura::Picture *pic, unsigned int quality) {
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = Sakura::OutputJpeg(&jpegBuf, pic, quality);

    FILE *fp = fopen(filePath, "wb");
    if (fp != NULL) {
        fwrite(jpegBuf, jpegSize, 1, fp);
        fclose(fp);
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw Sakura::Exception(msg);
    }
}

unsigned long Sakura::OutputJpeg(unsigned char ** outputBuffer, Sakura::Picture *pic, unsigned int quality) {
    unsigned long jpegSize = 0;
    tjhandle jpegHandle = tjInitCompress();
    TJPF format;
    if (pic->hasAlpha) {
        format = TJPF_RGBA;
    } else {
        format = TJPF_RGB;
    }

    const int result = tjCompress2(jpegHandle, pic->rgba, pic->width, pic->stride, pic->height, format,
                                   outputBuffer, &jpegSize, TJSAMP_444, quality, TJFLAG_FASTUPSAMPLE | TJFLAG_FASTDCT);
    if (result != 0) {
        std::string msg = "Failed while tjCompress2: ";
        msg += tjGetErrorStr();
        tjDestroy(jpegHandle);
        throw Sakura::Exception(msg);
    }

    tjDestroy(jpegHandle);

    return jpegSize;
}

void Sakura::OutputWebp(const char *filePath, Sakura::Picture *pic, unsigned int quality) {
    unsigned char *webpBuffer = NULL;
    size_t buffSize = Sakura::OutputWebp(&webpBuffer, pic, quality);

    FILE *fp = fopen(filePath, "wb");
    if (fp != NULL) {
        fwrite(webpBuffer, buffSize, 1, fp);
        fclose(fp);
        delete[] webpBuffer;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        delete[] webpBuffer;
        throw Sakura::Exception(msg);
    }
}

unsigned long Sakura::OutputWebp(unsigned char ** outputBuffer, Sakura::Picture *pic, unsigned int quality) {
    size_t buffSize = 0;
    if (pic->hasAlpha) {
        buffSize = WebPEncodeRGBA(pic->rgba, pic->width, pic->height, pic->stride, quality, outputBuffer);
    } else {
        buffSize = WebPEncodeRGB(pic->rgba, pic->width, pic->height, pic->stride, quality, outputBuffer);
    }

    if (buffSize == 0) {
        std::string msg = "WebP encode error";
        throw Sakura::Exception(msg);
    }

    return buffSize;
}

void Sakura::OutputGif(const char *filePath, Sakura::Picture *pic) {
    throw "Function not yet implemented.";
}

unsigned long Sakura::OutputGif(unsigned char ** outputBuffer, Sakura::Picture *pic) {
    throw "Function not yet implemented.";
}
