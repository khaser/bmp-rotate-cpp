#ifndef BMP_GUARD
#define BMP_GUARD

#include "stdio.h"

#define BITS_PER_CHAR 5

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

struct BITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} __attribute__((packed));

typedef struct BITMAPFILEHEADER FileHeader;

struct BITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPerlsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} __attribute__((packed));

typedef struct BITMAPINFOHEADER InfoHeader;

struct Pixel {
    BYTE B;
    BYTE G;
    BYTE R;
};

typedef struct Pixel Pixel;

struct Image {
    FileHeader file_header;
    InfoHeader info_header;
    Pixel** pixels;
};

typedef struct Image Image;

Image* load_bmp(FILE* file);

Image* crop(Image* img, int x, int y, int w, int h);

Image* rotate(Image* img);

void save_bmp(Image* img, FILE* file);

void free_bmp(Image* img);

//For using in stego.c
void create_pixeldump(Image* img);
#endif
