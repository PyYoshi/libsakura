#include "Sakura.h"

extern "C"
{
#include "gif_lib.h"

#include "webp/encode.h"
#include "webp/decode.h"

#include "png.h"
#include "pngconf.h"

#include "jpeglib.h"
#include "jpegint.h"

#include "libavutil/parseutils.h"
#include "libavformat/avformat.h"
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
            fprintf(stderr, "Unsupported image format: %s\n", filePath);
            exit(1);
    }
}

Sakura::~Sakura() {
    delete &this->_pic;
}

SakuraPicture * Sakura::Scale(int outWidth, int outHeight, int scaleMode) {
    // ignore message: Warning: data is not aligned! This can lead to a speedloss
    av_log_set_level(AV_LOG_ERROR);

    struct SwsContext * scaler;
    scaler = sws_getContext(this->_pic->width, this->_pic->height, PIX_FMT_RGBA, outWidth, outHeight, PIX_FMT_RGBA, scaleMode, NULL, NULL, NULL);

    SakuraPicture * oPic = new SakuraPicture();
    oPic->width = outWidth;
    oPic->height = outHeight;
    oPic->stride = outWidth * 4; // 32bit
    oPic->rgba = new unsigned char[oPic->height * oPic->stride];

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

    png_image_finish_read(&png, NULL, buf, -stride, NULL);
    if (PNG_IMAGE_FAILED(png)) {
        throw std::runtime_error(png.message);
    }

    this->_pic->width = png.width;
    this->_pic->height = png.height;
    this->_pic->stride = stride;
    this->_pic->rgba = buf;

    png_image_free(&png);

}

void Sakura::loadJpeg(const char * filePath){
    throw "Function not yet implemented.";
}

void Sakura::loadWebp(const char * filePath){
    throw "Function not yet implemented.";
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
    png.format = PNG_FORMAT_RGBA;

    uint32_t stride = PNG_IMAGE_ROW_STRIDE(png);

    png_image_write_to_file(&png, filePath, 0, pic->rgba, -stride, NULL);
    if (PNG_IMAGE_FAILED(png)) {
        throw std::runtime_error(png.message);
    }
    png_image_free(&png);
}

void Sakura::OutputJpeg(const char * filePath, SakuraPicture * pic) {
    throw "Function not yet implemented.";
}

void Sakura::OutputWebp(const char * filePath, SakuraPicture * pic) {
    throw "Function not yet implemented.";
}
