#include <string>
#include <stdlib.h>

#include "Sakura.h"


const char * SAMPLE_IMAGE_PATH_PNG = "./samples/sample.png";
const char * SAMPLE_IMAGE_PATH_PNG_ALPHA = "./samples/sample_alpha.png";
const char * SAMPLE_IMAGE_PATH_JPEG = "./samples/sample.jpg";
const char * SAMPLE_IMAGE_PATH_WEBP = "./samples/sample.webp";
const char * SAMPLE_IMAGE_PATH_WEBP_ALPHA = "./samples/sample_alpha.webp";

void TestToRGBFromRGBA(std::string tempDir) {
    std::cout << "TestToRGBFromRGBA: ";
    std::string outputPath = tempDir + std::string("/sample_alpha2.jpg");
    try {
        Sakura::Picture * pic = Sakura::LoadWebp(SAMPLE_IMAGE_PATH_WEBP_ALPHA);
        Sakura::ToRGBFromRGBA(pic);
        Sakura::OutputJpeg(outputPath.c_str(), pic, 100);
        std::cout << "success" << std::endl;
    } catch (Sakura::Exception &e) {
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    char temp_dir[] = "/tmp/test_sakura_XXXXXX";
    if (!mkdtemp(temp_dir)) {
        std::cout << "Could not create a unique temporary filename" << std::endl;
    }
    std::cout << temp_dir << std::endl;
    TestToRGBFromRGBA(temp_dir);
    return 0;
}
