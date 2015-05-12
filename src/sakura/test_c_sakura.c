#include "c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// MacOSX
#include <unistd.h>

const char * SAMPLE_IMAGE_PATH_PNG = "./samples/sample.png";
const char * SAMPLE_IMAGE_PATH_PNG_ALPHA = "./samples/sample_alpha.png";
const char * SAMPLE_IMAGE_PATH_JPEG = "./samples/sample.jpg";
const char * SAMPLE_IMAGE_PATH_WEBP = "./samples/sample.webp";
const char * SAMPLE_IMAGE_PATH_WEBP_ALPHA = "./samples/sample_alpha.webp";

void FreeErrPtr(char ** err) {
    if (*err) {
        free(*err);
        *err = NULL;
    }
}

unsigned long LoadFile(const char * filePath, unsigned char ** outBuffer) {
    FILE *fp = fopen(filePath, "rb");
    if(fp == NULL){
        printf("Could not open this file: %s\n", filePath);
        abort();
    }
    fseek(fp, 0L, SEEK_END);
    unsigned long bufSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    *outBuffer = (unsigned char*)malloc(sizeof(unsigned char)*bufSize);
    if (*outBuffer == NULL) {
        printf("Out of memory\n");
        fclose(fp);
        abort();
    }
    size_t r = fread(*outBuffer, bufSize, 1, fp);
    if (r < bufSize) {
        printf("Could not read to this file: %s\n", filePath);
        fclose(fp);
        abort();
    }
    fclose(fp);
    return bufSize;
}

void WriteFile(const char * filePath, unsigned char * inBuffer, unsigned long bufSize) {
    FILE *fp = fopen(filePath, "wb");
    if (fp == NULL) {
        printf("Could not open this file: %s\n", filePath);
        abort();
    }
    size_t r = fwrite(inBuffer, bufSize, 1, fp);
    if (r < bufSize) {
        printf("Could not write to this file: %s\n", filePath);
        fclose(fp);
        abort();
    }
    fclose(fp);
}

char * ConcatChar(char * str1, char * str2) {
    char * str3 = (char *) malloc(1 + strlen(str1) + strlen(str2));
    strcpy(str3, str1);
    strcat(str3, str2);
    return str3;
}

void test_file_load_png_output_png(char * temp_dir) {
    printf("test_file_load_png_output_png: ");
    char* err = NULL;
    sakura_picture_t* png = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG, &err);
    if (png == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_64 = sakura_scale(png, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_1024 = sakura_scale(png, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_png_output_png.png"), png, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_png_output_png_64.png"), png_64, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_png_output_png_1024.png"), png_1024, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png);
    sakura_free_picture(png_64);
    sakura_free_picture(png_1024);
    FreeErrPtr(&err);
}
void test_file_load_png_output_jpeg(char * temp_dir) {
    printf("test_file_load_png_output_jpeg: ");
    char* err = NULL;
    sakura_picture_t* png = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG, &err);
    if (png == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_64 = sakura_scale(png, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_1024 = sakura_scale(png, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_png_output_jpeg.jpg"), png, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_png_output_jpeg_64.jpg"), png_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_png_output_jpeg_1024.jpg"), png_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png);
    sakura_free_picture(png_64);
    sakura_free_picture(png_1024);
    FreeErrPtr(&err);
}
void test_file_load_png_output_webp(char * temp_dir) {
    printf("test_file_load_png_output_webp: ");
    char* err = NULL;
    sakura_picture_t* png = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG, &err);
    if (png == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_64 = sakura_scale(png, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_1024 = sakura_scale(png, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_png_output_webp.webp"), png, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_png_output_webp_64.webp"), png_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_png_output_webp_1024.webp"), png_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png);
    sakura_free_picture(png_64);
    sakura_free_picture(png_1024);
    FreeErrPtr(&err);
}

void test_file_load_png_alpha_output_png(char * temp_dir) {
    printf("test_file_load_png_alpha_output_png: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_alpha_64 = sakura_scale(png_alpha, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_alpha_1024 = sakura_scale(png_alpha, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_png.png"), png_alpha, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_png_64.png"), png_alpha_64, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_png_1024.png"), png_alpha_1024, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    sakura_free_picture(png_alpha_64);
    sakura_free_picture(png_alpha_1024);
    FreeErrPtr(&err);
}
void test_file_load_png_alpha_output_jpeg(char * temp_dir) {
    printf("test_file_load_png_alpha_output_jpeg: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_alpha_64 = sakura_scale(png_alpha, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_alpha_1024 = sakura_scale(png_alpha, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_jpeg.jpg"), png_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_jpeg_64.jpg"), png_alpha_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_jpeg_1024.jpg"), png_alpha_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    sakura_free_picture(png_alpha_64);
    sakura_free_picture(png_alpha_1024);
    FreeErrPtr(&err);
}
void test_file_load_png_alpha_output_webp(char * temp_dir) {
    printf("test_file_load_png_alpha_output_webp: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_alpha_64 = sakura_scale(png_alpha, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* png_alpha_1024 = sakura_scale(png_alpha, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_webp.webp"), png_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_webp_64.webp"), png_alpha_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_png_alpha_output_webp_1024.webp"), png_alpha_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    sakura_free_picture(png_alpha_64);
    sakura_free_picture(png_alpha_1024);
    FreeErrPtr(&err);
}

void test_file_load_jpeg_output_png(char * temp_dir) {
    printf("test_file_load_jpeg_output_png: ");
    char* err = NULL;
    sakura_picture_t* jpg = sakura_load_jpeg_file(SAMPLE_IMAGE_PATH_JPEG, &err);
    if (jpg == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* jpg_64 = sakura_scale(jpg, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* jpg_1024 = sakura_scale(jpg, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_png.png"), jpg, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_png_64.png"), jpg_64, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_png_1024.png"), jpg_1024, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(jpg);
    sakura_free_picture(jpg_64);
    sakura_free_picture(jpg_1024);
    FreeErrPtr(&err);
}
void test_file_load_jpeg_output_jpeg(char * temp_dir) {
    printf("test_file_load_jpeg_output_jpeg: ");
    char* err = NULL;
    sakura_picture_t* jpg = sakura_load_jpeg_file(SAMPLE_IMAGE_PATH_JPEG, &err);
    if (jpg == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* jpg_64 = sakura_scale(jpg, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* jpg_1024 = sakura_scale(jpg, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_jpeg.jpg"), jpg, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_jpeg_64.jpg"), jpg_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_jpeg_1024.jpg"), jpg_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(jpg);
    sakura_free_picture(jpg_64);
    sakura_free_picture(jpg_1024);
    FreeErrPtr(&err);
}
void test_file_load_jpeg_output_webp(char * temp_dir) {
    printf("test_file_load_jpeg_output_webp: ");
    char* err = NULL;
    sakura_picture_t* jpg = sakura_load_jpeg_file(SAMPLE_IMAGE_PATH_JPEG, &err);
    if (jpg == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* jpg_64 = sakura_scale(jpg, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* jpg_1024 = sakura_scale(jpg, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_webp.webp"), jpg, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_webp_64.webp"), jpg_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_jpeg_output_webp_1024.webp"), jpg_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(jpg);
    sakura_free_picture(jpg_64);
    sakura_free_picture(jpg_1024);
    FreeErrPtr(&err);
}

void test_file_load_webp_output_png(char * temp_dir) {
    printf("test_file_load_webp_output_png: ");
    char* err = NULL;
    sakura_picture_t* webp = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP, &err);
    if (webp == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_64 = sakura_scale(webp, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_1024 = sakura_scale(webp, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_webp_output_png.png"), webp, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_webp_output_png_64.png"), webp_64, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_file_load_webp_output_png_1024.png"), webp_1024, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp);
    sakura_free_picture(webp_64);
    sakura_free_picture(webp_1024);
    FreeErrPtr(&err);
}
void test_file_load_webp_output_jpeg(char * temp_dir) {
    printf("test_file_load_webp_output_jpeg: ");
    char* err = NULL;
    sakura_picture_t* webp = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP, &err);
    if (webp == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_64 = sakura_scale(webp, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_1024 = sakura_scale(webp, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_webp_output_jpeg.jpg"), webp, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_webp_output_jpeg_64.jpg"), webp_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_webp_output_jpeg_1024.jpg"), webp_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp);
    sakura_free_picture(webp_64);
    sakura_free_picture(webp_1024);
    FreeErrPtr(&err);
}
void test_file_load_webp_output_webp(char * temp_dir) {
    printf("test_file_load_webp_output_webp: ");
    char* err = NULL;
    sakura_picture_t* webp = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP, &err);
    if (webp == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_64 = sakura_scale(webp, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_1024 = sakura_scale(webp, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_output_webp.jpg"), webp, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_output_webp_64.jpg"), webp_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_output_webp_1024.jpg"), webp_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp);
    sakura_free_picture(webp_64);
    sakura_free_picture(webp_1024);
    FreeErrPtr(&err);
}

void test_file_load_webp_alpha_output_png(char * temp_dir) {
    printf("test_file_load_webp_alpha_output_png: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_alpha_64 = sakura_scale(webp_alpha, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_alpha_1024 = sakura_scale(webp_alpha, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_png.png"), webp_alpha, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_png_64.png"), webp_alpha_64, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_png_1024.png"), webp_alpha_1024, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    sakura_free_picture(webp_alpha_64);
    sakura_free_picture(webp_alpha_1024);
    FreeErrPtr(&err);
}
void test_file_load_webp_alpha_output_jpeg(char * temp_dir) {
    printf("test_file_load_webp_alpha_output_jpeg: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_alpha_64 = sakura_scale(webp_alpha, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_alpha_1024 = sakura_scale(webp_alpha, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_jpeg.jpg"), webp_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_jpeg_64.jpg"), webp_alpha_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_jpeg_1024.jpg"), webp_alpha_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    sakura_free_picture(webp_alpha_64);
    sakura_free_picture(webp_alpha_1024);
    FreeErrPtr(&err);
}
void test_file_load_webp_alpha_output_webp(char * temp_dir) {
    printf("test_file_load_webp_alpha_output_webp: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_alpha_64 = sakura_scale(webp_alpha, 64, 64, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_picture_t* webp_alpha_1024 = sakura_scale(webp_alpha, 1024, 1024, sakura_scale_filter_lanczos, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_webp.webp"), webp_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_webp_64.webp"), webp_alpha_64, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_file_load_webp_alpha_output_webp_1024.webp"), webp_alpha_1024, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    FreeErrPtr(&err);
}

void test_load_output_file(char * temp_dir) {
    test_file_load_png_output_png(temp_dir);
    test_file_load_png_output_jpeg(temp_dir);
    test_file_load_png_output_webp(temp_dir);

    test_file_load_png_alpha_output_png(temp_dir);
    test_file_load_png_alpha_output_jpeg(temp_dir);
    test_file_load_png_alpha_output_webp(temp_dir);

    test_file_load_jpeg_output_png(temp_dir);
    test_file_load_jpeg_output_jpeg(temp_dir);
    test_file_load_jpeg_output_webp(temp_dir);

    test_file_load_webp_output_png(temp_dir);
    test_file_load_webp_output_jpeg(temp_dir);
    test_file_load_webp_output_webp(temp_dir);

    test_file_load_webp_alpha_output_png(temp_dir);
    test_file_load_webp_alpha_output_jpeg(temp_dir);
    test_file_load_webp_alpha_output_webp(temp_dir);
}

void test_to_rgb_from_rgba_bg_white_png_alpha_png(char * temp_dir) {
    printf("test_to_rgb_from_rgba_bg_white_png_alpha_png: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba_bg_white(png_alpha, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_bg_white_png_alpha_png.png"), png_alpha, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_bg_white_png_alpha_jpeg(char * temp_dir) {
    printf("test_to_rgb_from_rgba_bg_white_png_alpha_jpeg: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba_bg_white(png_alpha, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_bg_white_png_alpha_jpeg.jpg"), png_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_bg_white_png_alpha_webp(char * temp_dir) {
    printf("test_to_rgb_from_rgba_bg_white_png_alpha_webp: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba_bg_white(png_alpha, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_bg_white_png_alpha_webp.webp"), png_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    FreeErrPtr(&err);
}

void test_to_rgb_from_rgba_bg_white_webp_alpha_png(char * temp_dir) {
    printf("test_to_rgb_from_rgba_bg_white_webp_alpha_png: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba_bg_white(webp_alpha, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_bg_white_webp_alpha_png.png"), webp_alpha, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_bg_white_webp_alpha_jpeg(char * temp_dir) {
    printf("test_to_rgb_from_rgba_bg_white_webp_alpha_jpeg: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba_bg_white(webp_alpha, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_bg_white_webp_alpha_jpeg.jpg"), webp_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_bg_white_webp_alpha_webp(char * temp_dir) {
    printf("test_to_rgb_from_rgba_bg_white_webp_alpha_webp: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba_bg_white(webp_alpha, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_bg_white_webp_alpha_webp.webp"), webp_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    FreeErrPtr(&err);
}

void test_to_rgb_from_rgba_bg_white(char * temp_dir) {
    test_to_rgb_from_rgba_bg_white_png_alpha_png(temp_dir);
    test_to_rgb_from_rgba_bg_white_png_alpha_jpeg(temp_dir);
    test_to_rgb_from_rgba_bg_white_png_alpha_webp(temp_dir);

    test_to_rgb_from_rgba_bg_white_webp_alpha_png(temp_dir);
    test_to_rgb_from_rgba_bg_white_webp_alpha_jpeg(temp_dir);
    test_to_rgb_from_rgba_bg_white_webp_alpha_webp(temp_dir);
}

void test_to_rgb_from_rgba_png_alpha_png(char * temp_dir) {
    printf("test_to_rgb_from_rgba_png_alpha_png: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba(png_alpha, 200, 200, 200, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_png_alpha_png.png"), png_alpha, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_png_alpha_jpeg(char * temp_dir) {
    printf("test_to_rgb_from_rgba_png_alpha_jpeg: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba(png_alpha, 200, 200, 200, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_png_alpha_jpeg.jpg"), png_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_png_alpha_webp(char * temp_dir) {
    printf("test_to_rgb_from_rgba_png_alpha_webp: ");
    char* err = NULL;
    sakura_picture_t* png_alpha = sakura_load_png_file(SAMPLE_IMAGE_PATH_PNG_ALPHA, &err);
    if (png_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba(png_alpha, 200, 200, 200, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_png_alpha_webp.webp"), png_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(png_alpha);
    FreeErrPtr(&err);
}

void test_to_rgb_from_rgba_webp_alpha_png(char * temp_dir) {
    printf("test_to_rgb_from_rgba_webp_alpha_png: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba(webp_alpha, 200, 200, 200, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_png_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_webp_alpha_png.png"), webp_alpha, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_webp_alpha_jpeg(char * temp_dir) {
    printf("test_to_rgb_from_rgba_webp_alpha_jpeg: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba(webp_alpha, 200, 200, 200, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_jpeg_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_webp_alpha_jpeg.jpg"), webp_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    FreeErrPtr(&err);
}
void test_to_rgb_from_rgba_webp_alpha_webp(char * temp_dir) {
    printf("test_to_rgb_from_rgba_webp_alpha_webp: ");
    char* err = NULL;
    sakura_picture_t* webp_alpha = sakura_load_webp_file(SAMPLE_IMAGE_PATH_WEBP_ALPHA, &err);
    if (webp_alpha == NULL || err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_to_rgb_from_rgba(webp_alpha, 200, 200, 200, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    sakura_output_webp_file(ConcatChar(temp_dir, "/test_to_rgb_from_rgba_webp_alpha_webp.webp"), webp_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    }
    printf("success\n");
    sakura_free_picture(webp_alpha);
    FreeErrPtr(&err);
}

void test_to_rgb_from_rgba(char * temp_dir) {
    test_to_rgb_from_rgba_png_alpha_png(temp_dir);
    test_to_rgb_from_rgba_png_alpha_jpeg(temp_dir);
    test_to_rgb_from_rgba_png_alpha_webp(temp_dir);

    test_to_rgb_from_rgba_webp_alpha_png(temp_dir);
    test_to_rgb_from_rgba_webp_alpha_jpeg(temp_dir);
    test_to_rgb_from_rgba_webp_alpha_webp(temp_dir);
}

int main(int argc, char** argv) {
    char temp_dir[] = "/tmp/test_sakura_XXXXXX";
    int fd = mkdtemp(temp_dir);
    printf("%s\n", temp_dir);
    if (fd == EEXIST) {
        printf("Could not create a unique temporary filename\n");
        abort();
    } else if (fd == EINVAL) {
        printf("The last six characters of emplate were not XXXXXX\n");
        abort();
    }
    test_load_output_file(temp_dir);
    test_to_rgb_from_rgba_bg_white(temp_dir);
    test_to_rgb_from_rgba(temp_dir);
    return 0;
}
