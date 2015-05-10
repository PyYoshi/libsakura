#ifndef SAKURA_SAKURAWRAPPER_H
#define SAKURA_SAKURAWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    sakura_pic_type_bitmap = 0x00,
    sakura_pic_type_png = 0x01,
    sakura_pic_type_jpeg = 0x02,
    sakura_pic_type_webp = 0x03,
    sakura_pic_type_gif = 0x04,
    sakura_pic_type_unknown = 0xff,
};

enum {
    sakura_scale_filter_fast_bilinear = 1,
    sakura_scale_filter_bilinear = 2,
    sakura_scale_filter_bicubic = 4,
    sakura_scale_filter_x = 8,
    sakura_scale_filter_point = 0x10,
    sakura_scale_filter_area = 0x20,
    sakura_scale_filter_bicublin = 0x40,
    sakura_scale_filter_gauss = 0x80,
    sakura_scale_filter_sinc = 0x100,
    sakura_scale_filter_lanczos = 0x200,
    sakura_scale_filter_spline = 0x400,
};

typedef struct sakura_picture_t sakura_picture_t;

extern void sakura_free_picture(sakura_picture_t* pic);

extern sakura_picture_t* sakura_scale(sakura_picture_t* in_pic, int out_width, int out_height, int scale_filter);

extern sakura_picture_t* sakura_load_png(unsigned char* input_buffer, unsigned long* buf_size);
extern sakura_picture_t* sakura_load_png_file(const char* file_path);

extern sakura_picture_t* sakura_load_jpeg(unsigned char* input_buffer, unsigned long* buf_size);
extern sakura_picture_t* sakura_load_jpeg_file(const char* file_path);

extern sakura_picture_t* sakura_load_webp(unsigned char* input_buffer, unsigned long* buf_size);
extern sakura_picture_t* sakura_load_webp_file(const char* file_path);

extern void sakura_output_png(unsigned char ** out_buffer, sakura_picture_t* in_pic, unsigned int comp_level);
extern void sakura_output_png_file(const char* file_path, sakura_picture_t* in_pic, unsigned int comp_level);

extern void sakura_output_jpeg(unsigned char ** out_buffer, sakura_picture_t* in_pic, unsigned int quality);
extern void sakura_output_jpeg_file(const char* file_path, sakura_picture_t* in_pic, unsigned int quality);

extern void sakura_output_webp(unsigned char ** out_buffer, sakura_picture_t* in_pic, unsigned int quality);
extern void sakura_output_webp_file(const char* file_path, sakura_picture_t* in_pic, unsigned int quality);

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif //SAKURA_SAKURAWRAPPER_H