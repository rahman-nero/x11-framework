#include <stdlib.h>
#include "string.h"

/**
 * Creating a new string
 * */
NeroString *NeroStringNew(const uint16_t x, const uint16_t y, char *string, uint16_t length) {
    NeroString *new = (NeroString *) malloc(sizeof(NeroString));

    new->x = x;
    new->y = y;
    new->string = string;
    new->length = length;

    return new;
}