#include <inttypes.h>

#ifndef NERO_CONFIG_STATE_H
#define NERO_CONFIG_STATE_H

typedef struct {
    uint64_t value;
} State;

typedef struct {
    State *list[127];
    uint8_t length;
} StateList;

/**
 * Creating a new state
 * */
State *createState(uint64_t value);

/**
 * Updating state
 * */
void updateState(State *state, uint64_t  value);

#endif