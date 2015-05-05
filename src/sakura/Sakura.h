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
        int width;
        int height;
        int stride;
        unsigned char *rgba;
        bool hasAlpha;

    private:
        ~SakuraPicture();
};

class Sakura {
    public:
        Sakura(const char * filePath);
        SakuraPicture * Scale(int outWidth, int outHeight, int scaleMode);
        static void OutputBitmap(const char * filePath, SakuraPicture * pic);
        static void OutputPng(const char * filePath, SakuraPicture * pic);
        static void OutputJpeg(const char * filePath, SakuraPicture * pic, unsigned int quality);
        static void OutputWebp(const char * filePath, SakuraPicture * pic);

    private:
        SakuraPicture * _pic;

        ~Sakura();

        void loadBitmap(const char * filePath);
        void loadPng(const char * filePath);
        void loadJpeg(const char * filePath);
        void loadWebp(const char * filePath);
};

#endif //SAKURA_SAKURA_H
