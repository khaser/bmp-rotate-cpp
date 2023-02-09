#ifndef STEGO_GUARD
#define STEGO_GUARD

#include "bmp.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

Image* insert_msg(Image* img, FILE* key_file, FILE* msg_file);

void extract_msg(Image* img, FILE* key_file, FILE* msg_file);

#endif
