#include <stdio.h>
#include <stdlib.h>
#include "state.h"

extern uint8_t stateGotUpdated;
extern StateList* stateList;

/**
 * Creating a new state
 * */
State *createState(uint8_t value) {
    State *state = (State *) malloc(sizeof(State));

    if (stateList->length == 0) {
        stateList->list[0] = state;
        stateList->length = 1;
    } else {
        stateList->list[stateList->length] = state;
        stateList->length += 1;
    }

    printf("Create State called, %d \n", stateList->length);

    state->value = value;
    return state;
}

/**
 * Updating state
 * */
void updateState(State *state, uint8_t value) {
    stateGotUpdated = 1;
    state->value = value;
}