#include <inttypes.h>

#ifndef NERO_SRC_STRING_H
#define NERO_SRC_STRING_H

typedef struct {
    // Position from left to right
    uint16_t x;

    // Position from top to bottom
    uint16_t y;

    // String
    char *string;

    // Length of Text
    uint16_t length;
} NeroString;

/**
 * Creating a new string
 * */
NeroString *NeroStringNew(uint16_t x, uint16_t y, char *string, uint16_t length);

#endif