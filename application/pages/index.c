#include <stdlib.h>
#include <pthread.h>
#include "../../src/window.h"
#include "../../src/store/state.h"

extern NeroConfig config;
State *state = NULL;
State *isModal = NULL;

uint16_t getUpTime() {

    FILE *file;
    char time[5];
    file = fopen("/proc/uptime", "r");
    fgets(time, 5, file);
    fclose(file);

    return atoi(time);
}

void windowEventListener(XEvent event) {
    if (isModal->value == 0) {
        updateState(isModal, 1);

    } else {
        updateState(isModal, 0);

    }
}


NeroWindow *index_run() {
    if (state == NULL || isModal == NULL) {
        state = createState(0);
        isModal = createState(0);
    }

    // Header
    NeroWindow *root = NeroWindowNew(1920, 900, 0, 0, 0x00FFFFFF, 0, 0, NULL, NULL);

    // Header
    NeroWindow *mainWin = NeroWindowNew(1920, 70, 0, 0, 0x00c324e2, 0, 0, NULL, NULL);

    // Center div
    NeroWindow *subWindow = NeroWindowNew(900, 30, XCenter(mainWin->config.width, 900), YCenter(mainWin->config.height, 30), 0x00F9F9F9, 0, 0, NULL,
                                          NULL);
    // Div in center 1
    NeroWindow *subSubWindow = NeroWindowNew(100, 40, 0, 0, 0x00CCCCCC, 0, 0,
                                             NeroStringNew(0, 0, "Update", sizeof("Update")),
                                             newEventListener(ClickEvent, windowEventListener));
    char *str = " Seconds";
    char number[15];
    sprintf(number, "%lu", state->value);

    char *newString = (char *) malloc(30 * sizeof(char));
    strcpy(newString, strcat(number, str));

    // Div in center 2
    NeroWindow *subSubSubWindow = NeroWindowNew(100, 40, 100, 0, 0x00FFCCCC, 0, 0, NeroStringNew(0, 0, newString, 30),
                                                NULL);

    /// Center Section
    NeroWindow *centerMainWin = NeroWindowNew(1920, 400, 0, 80, 0x00ffa409, 0, 0, NULL, NULL);

    if (isModal->value == 1) {
        NeroWindowAddSubWindow(root, centerMainWin);
    }


    NeroWindowAddSubWindow(root, mainWin);
    NeroWindowAddSubWindow(mainWin, subWindow);
    NeroWindowAddSubWindow(subWindow, subSubWindow);
    NeroWindowAddSubWindow(subWindow, subSubSubWindow);

    return root;
}


void main_destroy() {
    free(state);
}

