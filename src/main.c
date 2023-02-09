#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "bmp.h"
#include "stego.h"

int main(int argc, char* argv[]) {

    if (argc < 5) {
        printf("Incorrect number of arguments\n");
        printf("At least 4 arguments expected\n");
        return 1;
    }

    char* motion = argv[1];
    FILE* input_file = fopen(argv[2], "rb");

    if (!strcmp(motion, "crop-rotate")) {
        if (argc != 8) {
            printf("Incorrect number of arguments\n");
            printf("Program signature: ./hw-01_bmp crop-rotate ‹in-bmp› ‹out-bmp› ‹x› ‹y› ‹w› ‹h›\n");
            return 1;
        }
        char* output_filename = argv[3];
        //x, y = left right corner position
        int x = atoi(argv[4]);
        int y = atoi(argv[5]);
        int w = atoi(argv[6]);
        int h = atoi(argv[7]);

        Image* origin = load_bmp(input_file);
        int img_width = origin->info_header.biWidth;
        int img_height = origin->info_header.biHeight;

        if (x < 0 || y < 0 || w < 0 || h < 0 || x + w > img_width || y + h > img_height) {
            printf("Illegal size argument\n0 <= x < x + w <= W\n0 <= y < y + h <= H\n");
            return 1;
        }

        Image* cropped = crop(origin, x, y, w, h);
        free_bmp(origin);

        Image* rotated = rotate(cropped);
        free_bmp(cropped);

        FILE* output_file = fopen(output_filename, "wb");
        save_bmp(rotated, output_file);
        fclose(output_file);

        free_bmp(rotated);
    } else if (!strcmp(motion, "insert")) {
        if (argc != 6) {
            printf("Incorrect number of arguments\n");
            printf("Program signature: ./hw-01_bmp insert ‹in-bmp› ‹out-bmp› ‹key-txt› ‹msg-txt›\n");
            return 1;
        }
        FILE* output_file = fopen(argv[3], "wb");
        FILE* key_file = fopen(argv[4], "r");
        FILE* msg_file = fopen(argv[5], "r");

        Image* empty = load_bmp(input_file);
        Image* with_secret = insert_msg(empty, key_file, msg_file);
        save_bmp(with_secret, output_file);

        free_bmp(empty);
        free_bmp(with_secret);

        fclose(output_file);
        fclose(msg_file);
        fclose(key_file);
    } else if (!strcmp(motion, "extract")) {
        if (argc != 5) {
            printf("Incorrect number of arguments\n");
            printf("Program signature: ./hw-01_bmp extract ‹in-bmp› ‹key-txt› ‹msg-txt›\n");
            return 1;
        }
        FILE* key_file = fopen(argv[3], "r");
        FILE* msg_file = fopen(argv[4], "w");

        Image* img = load_bmp(input_file);
        extract_msg(img, key_file, msg_file);

        free_bmp(img);

        fclose(key_file);
        fclose(msg_file);
    } else {
        printf("No such option %s\n", motion);
        printf("Expected: crop-rotate, insert or extract\n");
        return 1;
    }

    fclose(input_file);
    return 0;
}
