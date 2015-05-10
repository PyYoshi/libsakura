#include "c.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
 * TODO: 各処理ごとに関数を作って入力形式とは別の形式への出力をテストするように
 */
void test_load_output_file() {
    char* err = NULL;

    ///////////////////////
    ///////// PNG /////////
    ///////////////////////
    printf("sakura_load_png_file(RGB): ");
    sakura_picture_t* png = sakura_load_png_file("./samples/sample.png", &err);
    if (png == NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    printf("sakura_output_png_file(RGB): ");
    sakura_output_png_file("./root/tmp/sample2.png", png, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    sakura_free_picture(png);

    printf("sakura_load_png_file(RGBA): ");
    sakura_picture_t* png_alpha = sakura_load_png_file("./samples/sample_alpha.png", &err);
    if (png_alpha == NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    printf("sakura_output_png_file(RGBA): ");
    sakura_output_png_file("./root/tmp/sample_alpha2.png", png_alpha, 1, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    sakura_free_picture(png_alpha);

    ///////////////////////
    //////// JPEG /////////
    ///////////////////////
    printf("sakura_load_jpeg_file(RGB): ");
    sakura_picture_t* jpg = sakura_load_jpeg_file("./samples/sample.jpg", &err);
    if (jpg == NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    printf("sakura_output_jpeg_file(RGB): ");
    sakura_output_jpeg_file("./root/tmp/sample2.jpg", jpg, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    sakura_free_picture(jpg);

    ///////////////////////
    //////// WebP /////////
    ///////////////////////
    printf("sakura_load_png_file(RGB): ");
    sakura_picture_t* webp = sakura_load_webp_file("./samples/sample.webp", &err);
    if (webp == NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    printf("sakura_output_webp_file(RGB): ");
    sakura_output_webp_file("./root/tmp/sample2.webp", webp, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    sakura_free_picture(webp);

    printf("sakura_load_png_file(RGBA): ");
    sakura_picture_t* webp_alpha = sakura_load_webp_file("./samples/sample_alpha.webp", &err);
    if (webp_alpha == NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    printf("sakura_output_webp_file(RGBA): ");
    sakura_output_webp_file("./root/tmp/sample_alpha2.webp", webp_alpha, 100, &err);
    if (err != NULL) {
        printf("%s\n", err);
        abort();
    } else {
        printf("success\n");
    }
    sakura_free_picture(webp_alpha);

    if (err != NULL) {
        free(err);
    }
}

/**
 * TODO: test_load_output_fileと同じテストを
 */
void test_load_output_memory() {
    char* err = NULL;

    ///////////////////////
    ///////// PNG /////////
    ///////////////////////

    ///////////////////////
    //////// JPEG /////////
    ///////////////////////

    ///////////////////////
    //////// WebP /////////
    ///////////////////////

    if (err != NULL) {
        free(err);
    }
}

/**
 * TODO: ダウンスケーリングだけでなくアップスケーリングもテスト
 */
void test_scale() {
    char* err = NULL;

    if (err != NULL) {
        free(err);
    }
}

int main(int argc, char** argv) {
    test_load_output_file();
    test_load_output_memory();
    test_scale();
    return 0;
}
