#ifndef SAKURA_SAKURA_H
#define SAKURA_SAKURA_H

#include <iostream>
#include <string>
#include <exception>

namespace Sakura {
    class Exception: public std::exception
    {
        public:
            explicit Exception(const char* message): msg_(message) {}
            explicit Exception(const std::string& message): msg_(message) {}
            virtual ~Exception() throw (){}

            virtual const char* what() const throw (){
                return msg_.c_str();
            }

        protected:
            std::string msg_;
    };

    enum PictureType {
        BITMAP=0x00,
        PNG=0x01,
        JPEG=0x02,
        WEBP=0x03,
        GIF=0x04,
        UNKNOWN=0xff
    };

    enum ScaleFilter {
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

    class Utils {
        public:
            static std::string GetFileName(const char * filePath);
            static std::string GetExt(std::string fileName);
            static PictureType GetType(std::string ext);
    };

    class Picture {
        public:
            ~Picture();

            int width;
            int height;
            int stride;
            unsigned char *rgba;
            bool hasAlpha;
    };

    Picture * Scale(Picture * pic, int outWidth, int outHeight, ScaleFilter scaleMode);
    Picture * LoadFromFile(const char * filePath);
    Picture * LoadBitmap(const char * filePath);
    Picture * LoadBitmap(unsigned char * inputBuffer, unsigned long * bufSize);
    Picture * LoadPng(const char * filePath);
    Picture * LoadPng(unsigned char * inputBuffer, unsigned long * bufSize);
    Picture * LoadJpeg(const char * filePath);
    Picture * LoadJpeg(unsigned char * inputBuffer, unsigned long * bufSize);
    Picture * LoadWebp(const char * filePath);
    Picture * LoadWebp(unsigned char * inputBuffer, unsigned long * bufSize);
    Picture * LoadGif(const char * filePath);
    Picture * LoadGif(unsigned char * inputBuffer, unsigned long * bufSize);

    void OutputBitmap(const char * filePath, Picture * pic);
    unsigned long OutputBitmap(unsigned char ** outputBuffer, Picture * pic);
    void OutputPng(const char * filePath, Picture * pic, int compLevel);
    unsigned long OutputPng(unsigned char ** outputBuffer, Picture * pic, int compLevel);
    void OutputJpeg(const char * filePath, Picture * pic, unsigned int quality);
    unsigned long OutputJpeg(unsigned char ** outputBuffer, Picture * pic, unsigned int quality);
    void OutputWebp(const char * filePath, Picture * pic, unsigned int quality);
    unsigned long OutputWebp(unsigned char ** outputBuffer, Picture * pic, unsigned int quality);
    void OutputGif(const char * filePath, Picture * pic);
    unsigned long OutputGif(unsigned char ** outputBuffer, Picture * pic);
}

#endif //SAKURA_SAKURA_H
