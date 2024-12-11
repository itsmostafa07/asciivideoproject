#include "./frame.h"
#include "./helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
/*
 * Creating a new frame.
 */
frame *frame_new(uint64_t id, pixel pixels[], unsigned int width, unsigned int height) {
    // Allocate memory for the frame structure and its pixel array
    frame *new_frame = (frame*)malloc(sizeof(frame));
    if (!new_frame) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Initialize the frame fields
    new_frame->id = id;
    new_frame->width = width;
    new_frame->height = height;
    new_frame-> pixels = (pixel*) malloc(sizeof(pixel) * width * height);

    // Copy the pixel data into the frame

    memcpy(new_frame->pixels, pixels, sizeof(pixel) * width * height);
    printf("==> %c \n", pixel_to_ascii(new_frame->pixels[0]));
    return new_frame;
}

/*
 * Converting the frame into Ascii Art.
 */
char *frame_to_ascii(frame *frame) {
    unsigned int width = frame->width;
    unsigned int height = frame->height;

    // Allocate memory for the ASCII art string
    char *ascii_art =(char*) malloc((width + 1) * height + 1); // Extra space for newlines and '\0'
    // if (!ascii_art) {
    //     return NULL; // Return NULL if memory allocation fails
    // }

    int index = 0;
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            index += 1;
            pixel p = frame->pixels[i * width + j];
            char ascii_char = pixel_to_ascii(p); 
            
            strcat(ascii_art, (char*)ascii_char);
            printf("D: %c , i=%i, j=%i\n", ascii_art[index], j, i);

        }
        strcat(ascii_art, (char*)'\n') ; // Add newline after each row
    }


    return ascii_art;
}

/*
 * Writing the frame that converted into Ascii Art to the path of the
 * destination file.
 */
int frame_ascii_write_to(frame *frame, char *filename) {
    char *ascii_art = frame_to_ascii(frame);
    if (!ascii_art) {
        return -1; // Return -1 if ASCII conversion fails
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        free(ascii_art); // Free the ASCII art if file cannot be opened
        return -1;
    }

    fprintf(file, "%s", ascii_art); // Write the ASCII art to the file
    fclose(file); // Close the file
    free(ascii_art); // Free the ASCII art memory

    return 0; // Return 0 on success
}
