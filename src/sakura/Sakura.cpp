#include "Sakura.h"

extern "C"
{
#include "gif_lib.h"

#include "webp/encode.h"
#include "webp/decode.h"

#include "png.h"
#include "pngconf.h"

#include "turbojpeg.h"

#include "libavutil/log.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
}

std::string SakuraUtils::GetFileName(const char *filePath) {
    std::string path = filePath;

    char sep = '/';
#ifdef _WIN32
   sep = '\\';
#endif

    size_t i = path.rfind(sep, path.length());
    if (i != std::string::npos) {
        return(path.substr(i+1, path.length() - i));
    }
    return("");
}

std::string SakuraUtils::GetExt(std::string fileName) {
    std::string ext;
    size_t pos = fileName.rfind('.', fileName.length());
    if(pos != std::string::npos){
        ext = fileName.substr(pos+1, fileName.size()-pos);
        std::string::iterator itr = ext.begin();
        while(itr != ext.end()){
            *itr = tolower(*itr);
            itr++;
        }
        itr = ext.end()-1;
        while(itr != ext.begin()){
            if(*itr == 0 || *itr == 32){
                ext.erase(itr--);
            }
            else{
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
    } else {
        return SakuraPictureType::UNKNOWN;
    }
}

SakuraPicture::~SakuraPicture() {
    delete []rgba;
}

Sakura::Sakura(const char * filePath) {
    this->_pic = new SakuraPicture();
    std::string fileName = SakuraUtils::GetFileName(filePath);
    std::string ext = SakuraUtils::GetExt(fileName);
    int type = SakuraUtils::GetType(ext);
    switch (type) {
        case SakuraPictureType::BITMAP :
            this->loadBitmap(filePath);
            break;
        case SakuraPictureType::PNG :
            this->loadPng(filePath);
            break;
        case SakuraPictureType::JPEG :
            this->loadJpeg(filePath);
            break;
        case SakuraPictureType::WEBP :
            this->loadWebp(filePath);
            break;
        default:
            std::string msg = "Unsupported image format: ";
            msg += filePath;
            throw SakuraException(msg);
    }
}

Sakura::~Sakura() {
    delete &this->_pic;
}

SakuraPicture * Sakura::Scale(int outWidth, int outHeight, int scaleMode) {
    // ignore message: Warning: data is not aligned! This can lead to a speedloss
    av_log_set_level(AV_LOG_ERROR);

    int stride;
    AVPixelFormat format;
    if (this->_pic->hasAlpha) {
        format = PIX_FMT_RGBA;
        stride = outWidth * 4;
    } else {
        format = PIX_FMT_RGB24;
        stride = outWidth * 3;
    }

    struct SwsContext * scaler;
    scaler = sws_getContext(this->_pic->width, this->_pic->height, format, outWidth, outHeight, format, scaleMode, NULL, NULL, NULL);

    SakuraPicture * oPic = new SakuraPicture();
    oPic->width = outWidth;
    oPic->height = outHeight;
    oPic->stride = stride;
    oPic->rgba = new unsigned char[oPic->height * oPic->stride];
    oPic->hasAlpha = this->_pic->hasAlpha;

    sws_scale(scaler, &this->_pic->rgba, &this->_pic->stride, 0, this->_pic->height, &oPic->rgba, &oPic->stride);

    sws_freeContext(scaler);
    return oPic;
}

void Sakura::loadBitmap(const char * filePath) {
    throw "Function not yet implemented.";
}

void Sakura::loadPng(const char * filePath){
    png_image png;
    volatile png_structp png2;
    memset(&png, 0, sizeof(png));
    png.version = PNG_IMAGE_VERSION;
    png_image_begin_read_from_file(&png, filePath);
    if (PNG_IMAGE_FAILED(png)) {
        png_image_free(&png);
        throw std::runtime_error(png.message);
    }

    uint32_t stride = PNG_IMAGE_ROW_STRIDE(png);
    uint8_t * buf = new unsigned char[PNG_IMAGE_BUFFER_SIZE(png, stride)];

    png_image_finish_read(&png, NULL, buf, stride, NULL);
    if (PNG_IMAGE_FAILED(png)) {
        throw std::runtime_error(png.message);
    }

    this->_pic->width = png.width;
    this->_pic->height = png.height;
    this->_pic->stride = stride;
    this->_pic->rgba = buf;

    if (png.format == PNG_FORMAT_RGB) {
        this->_pic->hasAlpha = false;
    } else if (png.format == PNG_FORMAT_RGBA) {
        this->_pic->hasAlpha = true;
    } else {
        throw "Supported Only RGB and RGBA";
    }

    png_image_free(&png);
}

void Sakura::loadJpeg(const char * filePath){
    FILE* fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned)ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char * fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        tjhandle jpegHandle = tjInitDecompress();
        int jpegSubsample = 0;
        int width, height;

        tjDecompressHeader2(jpegHandle, fileBuffer, fileSize, &width, &height, &jpegSubsample);

        unsigned char * rgbBuffer = new unsigned char[tjBufSize(width, height, jpegSubsample)];

        int stride = width * tjPixelSize[TJPF_RGB];

        this->_pic->width = width;
        this->_pic->height = height;
        this->_pic->stride = stride;
        this->_pic->hasAlpha = false;

        const int result = tjDecompress2(jpegHandle, fileBuffer, fileSize, rgbBuffer,
                                   width, stride, height, TJPF_RGB, TJFLAG_FASTUPSAMPLE | TJFLAG_FASTDCT);
        this->_pic->rgba = rgbBuffer;

        if (result != 0) {
            std::string msg = "Failed while tjCompress2: ";
            msg += tjGetErrorStr();
            delete[] fileBuffer; fileBuffer = NULL;
            tjDestroy(jpegHandle);
            throw SakuraException(msg);
        }

        delete[] fileBuffer; fileBuffer = NULL;
        tjDestroy(jpegHandle);
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw SakuraException(msg);
    }
}

void Sakura::loadWebp(const char * filePath){
    FILE* fp = fopen(filePath, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        unsigned long fileSize = (unsigned)ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char * fileBuffer = new unsigned char[fileSize];

        fread(fileBuffer, fileSize, 1, fp);
        fclose(fp);

        WebPBitstreamFeatures * webpFeature = new WebPBitstreamFeatures();
        VP8StatusCode gRet = WebPGetFeatures(fileBuffer, fileSize, webpFeature);
        if (gRet == VP8_STATUS_OK) {
            if (webpFeature->has_animation == 1) {
                std::string msg = "Unsupported WebP animation mode: ";
                msg += filePath;
                delete[] fileBuffer; fileBuffer = NULL;
                delete webpFeature; webpFeature = NULL;
                throw SakuraException(msg);
            }

            int stride = 0;
            if (webpFeature->has_alpha == 1) {
                this->_pic->rgba = WebPDecodeRGBA(fileBuffer, fileSize, &this->_pic->width, &this->_pic->height);
                this->_pic->hasAlpha = true;
                stride = this->_pic->width * 4;
            } else {
                this->_pic->rgba = WebPDecodeRGB(fileBuffer, fileSize, &this->_pic->width, &this->_pic->height);
                this->_pic->hasAlpha = false;
                stride = this->_pic->width * 3;
            }
            this->_pic->stride = stride;

            if (this->_pic->rgba == NULL) {
                std::string msg = "Invalid WebP format: ";
                msg += filePath;
                delete[] fileBuffer; fileBuffer = NULL;
                delete webpFeature; webpFeature = NULL;
                throw SakuraException(msg);
            }
            delete[] fileBuffer; fileBuffer = NULL;
        } else {
            std::string msg = "Invalid WebP format: ";
            msg += filePath;
            delete[] fileBuffer; fileBuffer = NULL;
            delete webpFeature; webpFeature = NULL;
            throw SakuraException(msg);
        };
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        throw SakuraException(msg);
    }
}

void Sakura::OutputBitmap(const char * filePath, SakuraPicture * pic) {
    throw "Function not yet implemented.";
}

void Sakura::OutputPng(const char * filePath, SakuraPicture * pic) {
    png_image png;
    memset(&png, 0, sizeof(png));
    png.version = PNG_IMAGE_VERSION;

    png.width  = pic->width;
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

void Sakura::OutputJpeg(const char * filePath, SakuraPicture * pic, unsigned int quality) {
    tjhandle jpegHandle = tjInitCompress();

    unsigned char * jpegBuf = NULL;
    unsigned long jpegSize = 0;

    const int result = tjCompress2(jpegHandle, pic->rgba, pic->width, pic->stride, pic->height, TJPF_RGB,
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

void Sakura::OutputWebp(const char * filePath, SakuraPicture * pic, unsigned int quality) {
    unsigned char * webpBuffer = NULL;
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
        delete[] webpBuffer; webpBuffer = NULL;
    } else {
        std::string msg = "Could not open file: ";
        msg += filePath;
        delete[] webpBuffer; webpBuffer = NULL;
        throw SakuraException(msg);
    }
}
