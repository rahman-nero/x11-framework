#include <inttypes.h>
#include <malloc.h>

#ifndef NERO_CONFIG_STATE_H
#define NERO_CONFIG_STATE_H

typedef struct {
    uint8_t value;
} State;

typedef struct {
    State *list[127];
    uint8_t length;
} StateList;

extern uint8_t stateGotUpdated;
extern StateList* stateList;

/**
 * Creating a new state
 * */
State *createState(uint8_t value);

/**
 * Updating state
 * */
void updateState(State *state, uint8_t value);

#endif