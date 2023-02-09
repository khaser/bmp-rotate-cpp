#include "bmp.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define PADDING 4

static int get_padding(int width) {
    return (PADDING - (width * sizeof(Pixel) % PADDING)) % PADDING;
}

static void load_pixels(FILE* file, Image* img) {
    int height = img->info_header.biHeight;
    int width = img->info_header.biWidth;
    for (int i = height - 1; i >= 0; --i) {
        fread(img->pixels[i], sizeof(Pixel), width, file);
        static char zeros[PADDING];
        fread(zeros, get_padding(width), 1, file);
    }
}

Image* load_bmp(FILE* file) {
    Image* img = malloc(sizeof(Image));
    fread(&img->file_header, sizeof(FileHeader), 1, file);
    fread(&img->info_header, sizeof(InfoHeader), 1, file);
    create_pixeldump(img);
    load_pixels(file, img);
    return img;
}

static void update_size(Image* img, int width, int height) {
    img->info_header.biHeight = height;
    img->info_header.biWidth = width;
    img->info_header.biSizeImage = get_padding(width) * height + width * height * sizeof(Pixel);
    img->file_header.bfSize = sizeof(InfoHeader) + sizeof(FileHeader) + img->info_header.biSizeImage;
}

Image* crop(Image* img, int x, int y, int w, int h) {
    Image* res = malloc(sizeof(Image));
    *res = *img;
    create_pixeldump(res);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res->pixels[i][j] = img->pixels[y + i][x + j];
        }
    }
    update_size(res, w, h);
    return res;
}

Image* rotate(Image* img) {
    //Not a misspell or bug!
    int w = img->info_header.biHeight;
    int h = img->info_header.biWidth;

    Pixel** pixel_rows = malloc(sizeof(Pixel*) * h);
    Pixel* pixel = malloc(sizeof(Pixel) * h * w);
    for (int i = 0; i < h; ++i) {
        pixel_rows[i] = &pixel[i * w];
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            pixel_rows[i][j] = img->pixels[w - j - 1][i];
        }
    }

    Image* res = malloc(sizeof(Image));
    *res = *img;
    res->pixels = pixel_rows;
    update_size(res, w, h);
    return res;
}

void save_bmp(Image *img, FILE* file) {
    fwrite(&img->file_header, sizeof(FileHeader), 1, file);
    fwrite(&img->info_header, sizeof(InfoHeader), 1, file);
    int height = img->info_header.biHeight;
    int width = img->info_header.biWidth;
    for (int i = height - 1; i >= 0; --i) {
        fwrite(img->pixels[i], sizeof(Pixel), width, file);
        static char zeros[PADDING];
        memset(zeros, 0, PADDING);
        fwrite(zeros, get_padding(width), 1, file);
    }
}

void free_bmp(Image *img) {
    free(img->pixels[0]);
    free(img->pixels);
    free(img);
}

void create_pixeldump(Image *img) {
    LONG h = img->info_header.biHeight;
    LONG w = img->info_header.biWidth;
    Pixel** pixel_rows = malloc(sizeof(Pixel*) * h);
    Pixel* pixel = malloc(sizeof(Pixel) * h * w);
    for (int i = 0; i < h; ++i) {
        pixel_rows[i] = &pixel[i * w];
    }
    img->pixels = pixel_rows;
}
