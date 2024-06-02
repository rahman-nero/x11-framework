#include "../../src/window.h"
#include "../../src/store/state.h"

State *st = NULL;

void windowEventListener(XEvent event) {
    printf("Got Clicked " __FILE__ "\n");
    updateState(st, st->value + 1);
}

NeroWindow *index_run() {
    if (st == NULL) {
        st = createState(0);
    }

    // Header
    NeroWindow *mainWin = NeroWindowNew(1920, 70, 0, 0, 0x00c324e2, 0, 0, NULL, NULL);

    // Center div
    NeroWindow *subWindow = NeroWindowNew(900, 30, (1920 / 2) - 900 / 2, (70 / 2) - 30 / 2, 0x00F9F9F9, 0, 0, NULL,
                                          NULL);

    // Click Event
    NeroEventListener *event = newEventListener(ClickEvent, windowEventListener);

    // Div in center 1
    NeroWindow *subSubWindow = NeroWindowNew(100, 40, 0, 0, 0x00CCCCCC, 0, 0, NeroStringNew(0, 0, "Hello", 5), event);


    char str[30] = "World";
    char number[15];
    sprintf(number, "%lu", st->value);

    char *newString = (char *) malloc(30 * sizeof(char));
    strcpy(newString, strcat(str, number));

    // Div in center 2
    NeroWindow *subSubSubWindow = NeroWindowNew(100, 40, 100, 0, 0x00FFCCCC, 0, 0, NeroStringNew(0, 0, newString, 30),
                                                NULL);

    NeroWindowAddSubWindow(mainWin, subWindow);
    NeroWindowAddSubWindow(subWindow, subSubWindow);
    NeroWindowAddSubWindow(subWindow, subSubSubWindow);

    return mainWin;
}


void main_destroy() {
    free(st);
}

