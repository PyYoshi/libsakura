#ifndef SAKURA_SAKURA_H
#define SAKURA_SAKURA_H

#include <iostream>
#include <string>
#include <exception>

class SakuraException: public std::exception
{
public:
    explicit SakuraException(const char* message): msg_(message) {}
    explicit SakuraException(const std::string& message): msg_(message) {}
    virtual ~SakuraException() throw (){}

    virtual const char* what() const throw (){
        return msg_.c_str();
    }

protected:
    std::string msg_;
};

enum SakuraPictureType {
    BITMAP,
    PNG,
    JPEG,
    WEBP,
    GIF,
    UNKNOWN
};

enum SakuraScaleFilter {
    FAST_BILINEAR = 1,
    BILINEAR = 2,
    BICUBIC = 4,
    X = 8,
    POINT = 0x10,
    AREA = 0x20,
    BICUBLIN = 0x40,
    GAUSS = 0x80,
    SINC = 0x100,
    LANCZOS = 0x200,
    SPLINE = 0x400,
};

class SakuraUtils {
    public:
        static std::string GetFileName(const char * filePath);
        static std::string GetExt(std::string fileName);
        static SakuraPictureType GetType(std::string ext);
};

class SakuraPicture {
    public:
        ~SakuraPicture();

        int width;
        int height;
        int stride;
        unsigned char *rgba;
        bool hasAlpha;
};

class Sakura {
    public:
        Sakura();
        ~Sakura();

        static SakuraPicture * Scale(SakuraPicture * pic, int outWidth, int outHeight, SakuraScaleFilter scaleMode);
        static SakuraPicture * LoadFromFile(const char * filePath);
        static SakuraPicture * LoadBitmap(const char * filePath);
        static SakuraPicture * LoadBitmap(unsigned char * inputBuffer, unsigned long * bufSize);
        static SakuraPicture * LoadPng(const char * filePath);
        static SakuraPicture * LoadPng(unsigned char * inputBuffer, unsigned long * bufSize);
        static SakuraPicture * LoadJpeg(const char * filePath);
        static SakuraPicture * LoadJpeg(unsigned char * inputBuffer, unsigned long * bufSize);
        static SakuraPicture * LoadWebp(const char * filePath);
        static SakuraPicture * LoadWebp(unsigned char * inputBuffer, unsigned long * bufSize);
        static SakuraPicture * LoadGif(const char * filePath);
        static SakuraPicture * LoadGif(unsigned char * inputBuffer, unsigned long * bufSize);

        static void OutputBitmap(const char * filePath, SakuraPicture * pic);
        static void OutputBitmap(unsigned char ** outputBuffer, SakuraPicture * pic);
        static void OutputPng(const char * filePath, SakuraPicture * pic);
        static void OutputPng(unsigned char ** outputBuffer, SakuraPicture * pic);
        static void OutputJpeg(const char * filePath, SakuraPicture * pic, unsigned int quality);
        static void OutputJpeg(unsigned char ** outputBuffer, SakuraPicture * pic, unsigned int quality);
        static void OutputWebp(const char * filePath, SakuraPicture * pic, unsigned int quality);
        static void OutputWebp(unsigned char ** outputBuffer, SakuraPicture * pic, unsigned int quality);
        static void OutputGif(const char * filePath, SakuraPicture * pic);
        static void OutputGif(unsigned char ** outputBuffer, SakuraPicture * pic);
};

#endif //SAKURA_SAKURA_H
