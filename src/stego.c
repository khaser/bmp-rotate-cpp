#include "stego.h"
#include "bmp.h"
#include "assert.h"
#include "stdlib.h"

static unsigned char char_code(char c) {
    if ('A' <= c && c <= 'Z') {
        return c - 'A';
    } else if (c == ' ') {
        return 26;
    } else if (c == '.') {
        return 27;
    } else if (c == ',') {
        return 28;
    }
    assert(0);
    return 255;
}

static char code_char(unsigned char c) {
    if (c <= 25) {
        return c + 'A';
    } else if (c == 26) {
        return ' ';
    } else if (c == 27) {
        return '.';
    } else if (c == 28) {
        return ',';
    }
    assert(0);
}


static BYTE set_bit(BYTE x, BYTE bit) {
    return (x & ~(bit ^ 1)) | bit;
}

Image* insert_msg(Image* img, FILE* key_file, FILE* msg_file) {
    Image* res = malloc(sizeof(Image));
    *res = *img;
    create_pixeldump(res);
    int height = res->info_header.biHeight;
    int width = res->info_header.biWidth;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            res->pixels[i][j] = img->pixels[i][j];
        }
    }
    char c;
    while ((c = fgetc(msg_file)) != EOF && c != '\n') {
        char cc = char_code(c);
        /* printf("c: %c cc: %d\n", c, cc); */
        for (int bit_order = 0; bit_order < BITS_PER_CHAR; ++bit_order) {
            int x, y;
            char channel;
            fscanf(key_file, "%d %d %c\n", &x, &y, &channel);
            BYTE bit = (((1 << bit_order) & cc) != 0);
            /* printf("x: %d y: %d channel: %c bit: %d\n", x, y, channel, bit); */
            switch (channel) {
                case 'R':
                    res->pixels[y][x].R = set_bit(img->pixels[y][x].R, bit);
                    break;
                case 'G':
                    res->pixels[y][x].G = set_bit(img->pixels[y][x].G, bit);
                    break;
                case 'B':
                    res->pixels[y][x].B = set_bit(img->pixels[y][x].B, bit);
                    break;
                default:
                    printf("Incorrect channel '%c'. 'R', 'G' or 'B' only", channel);
                    return NULL;
            }
        }
    }
    return res;
}

void extract_msg(Image* img, FILE* key_file, FILE* msg_file) {
    int x, y;
    char channel;
    BYTE buf[BITS_PER_CHAR];
    int it = 0;
    while (fscanf(key_file, "%d %d %c", &x, &y, &channel) && !feof(key_file)) {
        /* printf("%d %d %c\n", x, y, channel); */
        switch (channel) {
            case 'R':
                buf[it] = img->pixels[y][x].R;
                break;
            case 'G':
                buf[it] = img->pixels[y][x].G;
                break;
            case 'B':
                buf[it] = img->pixels[y][x].B;
                break;
            default:
                printf("Incorrect channel '%c'. 'R', 'G' or 'B' only", channel);
                break;
        }
        it = (it + 1) % BITS_PER_CHAR;
        if (it == 0) {
            int cc = 0;
            for (int i = 0; i < BITS_PER_CHAR; ++i) {
                cc += ((1 & buf[i]) << i);
            }
            fputc(code_char(cc), msg_file);
        }
    }
    fputc('\n', msg_file);
}
