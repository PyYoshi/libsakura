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

std::string SakuraUtils::GetFileName(const char *filePath) {
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

std::string SakuraUtils::GetExt(std::string fileName) {
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

SakuraPictureType SakuraUtils::GetType(std::string ext) {
    if (ext == "bmp") {
        return SakuraPictureType::BITMAP;
    } else if (ext == "png") {
        return SakuraPictureType::PNG;
    } else if (ext == "jpg" || ext == "jpeg") {
        return SakuraPictureType::JPEG;
    } else if (ext == "webp") {
        return SakuraPictureType::WEBP;
    } else if (ext == "gif") {
        return SakuraPictureType::GIF;
    } else {
        return SakuraPictureType::UNKNOWN;
    }
}

SakuraPicture::~SakuraPicture() {
    delete[]rgba;
}

Sakura::Sakura() {}

Sakura::~Sakura() {}

SakuraPicture *Sakura::Scale(SakuraPicture * pic, int outWidth, int outHeight, SakuraScaleFilter scaleMode) {
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

    SakuraPicture *oPic = new SakuraPicture();
    oPic->width = outWidth;
    oPic->height = outHeight;
    oPic->stride = stride;
    oPic->rgba = new unsigned char[oPic->height * oPic->stride];
    oPic->hasAlpha = pic->hasAlpha;

    sws_scale(scaler, (const unsigned char * const*)&pic->rgba, &pic->stride, 0, pic->height, &oPic->rgba, &oPic->stride);

    sws_freeContext(scaler);
    return oPic;
}

SakuraPicture * Sakura::LoadFromFile(const char * filePath) {
    SakuraPicture * pic;
    std::string fileName = SakuraUtils::GetFileName(filePath);
    std::string ext = SakuraUtils::GetExt(fileName);
    int type = SakuraUtils::GetType(ext);
    switch (type) {
        case SakuraPictureType::BITMAP :
            pic = Sakura::LoadBitmap(filePath);
            break;
        case SakuraPictureType::PNG :
            pic = Sakura::LoadPng(filePath);
            break;
        case SakuraPictureType::JPEG :
            pic = Sakura::LoadJpeg(filePath);
            break;
        case SakuraPictureType::WEBP :
            pic = Sakura::LoadWebp(filePath);
            break;
        case SakuraPictureType::GIF :
            pic = Sakura::LoadGif(filePath);
            break;
        default:
            std::string msg = "Unsupported image format: ";
            msg += filePath;
            throw SakuraException(msg);
    }
    return pic;
}

SakuraPicture * Sakura::LoadBitmap(const char * filePath) {
    throw "Function not yet implemented.";
}

SakuraPicture * Sakura::LoadBitmap(unsigned char * inputBuffer, unsigned long * bufSize) {
    throw "Function not yet implemented.";
}

SakuraPicture * Sakura::LoadPng(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char * fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        SakuraPicture * pic = Sakura::LoadPng(fileBuffer, &fileSize);
        delete[] fileBuffer;

        return pic;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw SakuraException(msg);
    };
}

SakuraPicture * Sakura::LoadPng(unsigned char * inputBuffer, unsigned long * bufSize) {
    SakuraPicture * pic = new SakuraPicture();
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        delete pic;
        throw SakuraException("Out of memory");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        delete pic;
        throw SakuraException("Out of memory");
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        delete pic;
        throw SakuraException("Out of memory");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete pic;
        throw SakuraException("Out of memory");
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
        throw SakuraException("Invalid PNG color coding");
    }

    if (bitDepth != 8) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        std::string msg = "Invalid PNG bit depth: ";
        msg += bitDepth;
        throw SakuraException(msg);
    }

    bool hasAlpha = (colorType == PNG_COLOR_TYPE_RGBA);

    png_read_update_info(png_ptr, info_ptr);

    png_bytep * const imgData = new png_bytep[height];
    for (unsigned int row = 0; row < height; row++)
        imgData[row] = new png_byte[4 * width];

    png_read_image(png_ptr, imgData);
    png_read_end(png_ptr, end_info);

    pic->width = width;
    pic->height = height;
    pic->hasAlpha = hasAlpha;
    if (hasAlpha) {
        pic->stride = width * 4;
    } else {
        pic->stride = width * 3;
    }
    pic->rgba = new unsigned char[height * pic->stride];

    for(int h=0; h<height; h++){
        for(int w=0; w<width; w++){
            if(colorType == PNG_COLOR_TYPE_RGBA){
                pic->rgba[(h * w) + 0] = imgData[h][(w * 4) + 0];
                pic->rgba[(h * w) + 1] = imgData[h][(w * 4) + 1];
                pic->rgba[(h * w) + 2] = imgData[h][(w * 4) + 2];
                pic->rgba[(h * w) + 3] = imgData[h][(w * 4) + 3];
            } else {
                pic->rgba[(h * w) + 0] = imgData[h][(w * 3) + 0];
                pic->rgba[(h * w) + 1] = imgData[h][(w * 3) + 1];
                pic->rgba[(h * w) + 2] = imgData[h][(w * 3) + 2];
            }
        }
        delete[] imgData[h];
    }
    delete[] imgData;

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    return pic;
}

SakuraPicture * Sakura::LoadJpeg(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char * fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        SakuraPicture * pic = Sakura::LoadJpeg(fileBuffer, &fileSize);
        delete[] fileBuffer;

        return pic;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw SakuraException(msg);
    };
}

SakuraPicture * Sakura::LoadJpeg(unsigned char * inputBuffer, unsigned long * bufSize) {
    SakuraPicture * pic = new SakuraPicture();
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
        throw SakuraException(msg);
    }

    tjDestroy(jpegHandle);

    return pic;
}

SakuraPicture * Sakura::LoadWebp(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char *fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        SakuraPicture * pic = Sakura::LoadWebp(fileBuffer, &fileSize);
        delete[] fileBuffer;
        return pic;

    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw SakuraException(msg);
    }
}

SakuraPicture * Sakura::LoadWebp(unsigned char * inputBuffer, unsigned long * bufSize) {
    SakuraPicture * pic = new SakuraPicture();
    WebPBitstreamFeatures *webpFeature = new WebPBitstreamFeatures();
    VP8StatusCode gRet = WebPGetFeatures(inputBuffer, *bufSize, webpFeature);
    if (gRet == VP8_STATUS_OK) {
        if (webpFeature->has_animation == 1) {
            std::string msg = "Unsupported WebP animation mode";
            delete webpFeature;
            delete pic;
            throw SakuraException(msg);
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
            throw SakuraException(msg);
        }

        delete webpFeature;
        return pic;
    } else {
        std::string msg = "Invalid WebP format";
        delete webpFeature;
        delete pic;
        throw SakuraException(msg);
    };
}

SakuraPicture * Sakura::LoadGif(const char * filePath) {
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned) ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char* fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        SakuraPicture * pic = Sakura::LoadGif(fileBuffer, &fileSize);
        delete[] fileBuffer;
        return pic;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw SakuraException(msg);
    }
}

SakuraPicture * Sakura::LoadGif(unsigned char * inputBuffer, unsigned long * bufSize) {
    SakuraPicture * pic = new SakuraPicture();
    int gifError = 0;
    GifHelperReadCbData cbData = {inputBuffer, *bufSize, 0};
    GifFileType* gif = DGifOpen((void*)&cbData, &GifHelperByteBufferReadFun, &gifError);
    if (gif == NULL) {
        std::string msg = "gif error: ";
        msg += GifErrorString(gifError);
        delete pic;
        throw SakuraException(msg);
    }
    if (gifError != D_GIF_SUCCEEDED) {
        std::string msg = "gif error: ";
        msg += GifErrorString(gif->Error);
        delete pic;
        throw SakuraException(msg);
    }

    if (DGifSlurp(gif) == GIF_ERROR) {
        std::string msg = "gif error: ";
        msg += GifErrorString(gif->Error);
        delete pic;
        throw SakuraException(msg);
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
        throw SakuraException(msg);
    }

    return pic;
}

void Sakura::OutputBitmap(const char *filePath, SakuraPicture *pic) {
    throw "Function not yet implemented.";
}

void Sakura::OutputBitmap(unsigned char ** outputBuffer, SakuraPicture *pic) {
    throw "Function not yet implemented.";
}

void Sakura::OutputPng(const char *filePath, SakuraPicture *pic) {
    png_image png;
    memset(&png, 0, sizeof(png));
    png.version = PNG_IMAGE_VERSION;

    png.width = pic->width;
    png.height = pic->height;

    if (pic->hasAlpha) {
        png.format = PNG_FORMAT_RGBA;
    } else {
        png.format = PNG_FORMAT_RGB;
    }

    uint32_t stride = PNG_IMAGE_ROW_STRIDE(png);

    png_image_write_to_file(&png, filePath, 0, pic->rgba, stride, NULL);
    if (PNG_IMAGE_FAILED(png)) {
        throw std::runtime_error(png.message);
    }
    png_image_free(&png);
}

void Sakura::OutputPng(unsigned char ** outputBuffer, SakuraPicture *pic) {
    throw "Function not yet implemented.";
}

void Sakura::OutputJpeg(const char *filePath, SakuraPicture *pic, unsigned int quality) {
    tjhandle jpegHandle = tjInitCompress();

    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;

    TJPF format;
    if (pic->hasAlpha) {
        format = TJPF_RGBA;
    } else {
        format = TJPF_RGB;
    }

    const int result = tjCompress2(jpegHandle, pic->rgba, pic->width, pic->stride, pic->height, format,
                                   &jpegBuf, &jpegSize, TJSAMP_444, quality, TJFLAG_FASTUPSAMPLE | TJFLAG_FASTDCT);
    if (result != 0) {
        std::string msg = "Failed while tjCompress2: ";
        msg += tjGetErrorStr();
        tjFree(jpegBuf);
        tjDestroy(jpegHandle);
        throw SakuraException(msg);
    }

    FILE *fp = fopen(filePath, "wb");
    if (fp != NULL) {
        fwrite(jpegBuf, jpegSize, 1, fp);
        fclose(fp);
        tjFree(jpegBuf);
        tjDestroy(jpegHandle);
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        tjFree(jpegBuf);
        tjDestroy(jpegHandle);
        throw SakuraException(msg);
    }
}

void Sakura::OutputJpeg(unsigned char ** outputBuffer, SakuraPicture *pic, unsigned int quality) {
    throw "Function not yet implemented.";
}

void Sakura::OutputWebp(const char *filePath, SakuraPicture *pic, unsigned int quality) {
    unsigned char *webpBuffer = NULL;
    size_t buffSize = 0;
    if (pic->hasAlpha) {
        buffSize = WebPEncodeRGBA(pic->rgba, pic->width, pic->height, pic->stride, quality, &webpBuffer);
    } else {
        buffSize = WebPEncodeRGB(pic->rgba, pic->width, pic->height, pic->stride, quality, &webpBuffer);
    }

    if (buffSize == 0) {
        std::string msg = "WebP encode error";
        throw SakuraException(msg);
    }

    FILE *fp = fopen(filePath, "wb");
    if (fp != NULL) {
        fwrite(webpBuffer, buffSize, 1, fp);
        fclose(fp);
        delete[] webpBuffer;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        delete[] webpBuffer;
        throw SakuraException(msg);
    }
}

void Sakura::OutputWebp(unsigned char ** outputBuffer, SakuraPicture *pic, unsigned int quality) {
    throw "Function not yet implemented.";
}

void Sakura::OutputGif(const char *filePath, SakuraPicture *pic) {
    throw "Function not yet implemented.";
}

void Sakura::OutputGif(unsigned char ** outputBuffer, SakuraPicture *pic) {
    throw "Function not yet implemented.";
}
