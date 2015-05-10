#include "c.h"
#include "Sakura.h"

extern "C" {

struct sakura_picture_t {Sakura::Picture* rep;};

void sakura_free_picture(sakura_picture_t* pic) {
    delete pic->rep;
    delete pic;
}

sakura_picture_t* sakura_scale(sakura_picture_t* in_pic, int out_width, int out_height, int scale_filter) {
    sakura_picture_t* result = new sakura_picture_t;
    Sakura::Picture * out_pic = Sakura::Scale(in_pic->rep, out_width, out_height, static_cast<Sakura::ScaleFilter>(scale_filter));
    result->rep = out_pic;
    return result;
}

sakura_picture_t* sakura_load_png(unsigned char* input_buffer, unsigned long* buf_size) {
    sakura_picture_t* result = new sakura_picture_t;
    Sakura::Picture * pic = Sakura::LoadPng(input_buffer, buf_size);
    result->rep = pic;
    return result;
}

sakura_picture_t* sakura_load_png_file(const char* file_path) {
    sakura_picture_t* result = new sakura_picture_t;
    Sakura::Picture * pic = Sakura::LoadPng(file_path);
    result->rep = pic;
    return result;
}

sakura_picture_t* sakura_load_jpeg(unsigned char* input_buffer, unsigned long* buf_size) {
    sakura_picture_t* result = new sakura_picture_t;
    Sakura::Picture * pic = Sakura::LoadJpeg(input_buffer, buf_size);
    result->rep = pic;
    return result;
}

sakura_picture_t* sakura_load_jpeg_file(const char* file_path) {
    sakura_picture_t* result = new sakura_picture_t;
    Sakura::Picture * pic = Sakura::LoadJpeg(file_path);
    result->rep = pic;
    return result;
}

sakura_picture_t* sakura_load_webp(unsigned char* input_buffer, unsigned long* buf_size) {
    sakura_picture_t* result = new sakura_picture_t;
    Sakura::Picture * pic = Sakura::LoadWebp(input_buffer, buf_size);
    result->rep = pic;
    return result;
}

sakura_picture_t* sakura_load_webp_file(const char* file_path) {
    sakura_picture_t* result = new sakura_picture_t;
    Sakura::Picture * pic = Sakura::LoadWebp(file_path);
    result->rep = pic;
    return result;
}

void sakura_output_png(unsigned char** out_buffer, sakura_picture_t* in_pic, unsigned int comp_level) {
    Sakura::OutputPng(out_buffer, in_pic->rep, comp_level);
}

void sakura_output_png_file(const char* file_path, sakura_picture_t* in_pic, unsigned int comp_level) {
    Sakura::OutputPng(file_path, in_pic->rep, comp_level);
}

void sakura_output_jpeg(unsigned char** out_buffer, sakura_picture_t* in_pic, unsigned int quality) {
    Sakura::OutputJpeg(out_buffer, in_pic->rep, quality);
}

void sakura_output_jpeg_file(const char* file_path, sakura_picture_t* in_pic, unsigned int quality) {
    Sakura::OutputJpeg(file_path, in_pic->rep, quality);
}

void sakura_output_webp(unsigned char** out_buffer, sakura_picture_t* in_pic, unsigned int quality) {
    Sakura::OutputWebp(out_buffer, in_pic->rep, quality);
}

void sakura_output_webp_file(const char* file_path, sakura_picture_t* in_pic, unsigned int quality) {
    Sakura::OutputWebp(file_path, in_pic->rep, quality);
}

}
