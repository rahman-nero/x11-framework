#include "../../src/window.h"
#include "../../src/store/state.h"

State *st = NULL;

NeroWindow *index_run() {
    if (st == NULL) {
        st = createState(0);
    }

    // Header
    NeroWindow *mainWin = (NeroWindow *) malloc(sizeof(NeroWindow));
    mainWin->config.width = 1920;
    mainWin->config.height = 70;
    mainWin->config.x = 0;
    mainWin->config.y = 0;
    mainWin->config.border_width = 0;
    mainWin->config.border_color = 0;
    mainWin->config.background = 0x00c324e2;
    mainWin->string = NULL;

    // Center div
    NeroWindow *subWindow = (NeroWindow *) malloc(sizeof(NeroWindow));
    subWindow->config.width = 900;
    subWindow->config.height = 30;
    subWindow->config.x = (1920 / 2) - 900 / 2;
    subWindow->config.y = (70 / 2) - 30 / 2;
    subWindow->config.border_width = 0;
    subWindow->config.border_color = 0;
    subWindow->config.background = 0x00F9F9F9;
    subWindow->string = NULL;

    // Div in center div
    NeroWindow *subSubWindow = (NeroWindow *) malloc(sizeof(NeroWindow));
    subSubWindow->config.width = 100;
    subSubWindow->config.height = 40;
    subSubWindow->config.x = 0;
    subSubWindow->config.y = 0;
    subSubWindow->config.border_width = 0;
    subSubWindow->config.border_color = 0;
    subSubWindow->config.background = 0x00CCCCCC;
    subSubWindow->string = NULL;

    // Div in center div
    NeroWindow *subSubSubWindow = (NeroWindow *) malloc(sizeof(NeroWindow));
    subSubSubWindow->config.width = 100;
    subSubSubWindow->config.height = 40;
    subSubSubWindow->config.x = 100;
    subSubSubWindow->config.y = 0;
    subSubSubWindow->config.border_width = 0;
    subSubSubWindow->config.border_color = 0;
    subSubSubWindow->config.background = 0x00FFCCCC;
    subSubSubWindow->string = NULL;

    // Text in centered div
    NeroString *subWindowString = (NeroString *) malloc(sizeof(NeroString));
    subWindowString->x = 0;
    subWindowString->y = 0;
    subWindowString->string = "Hello";
    subWindowString->length = 5;
    subSubWindow->string = subWindowString;


    // Text in centered div
    NeroString *subWindowString2 = (NeroString *) malloc(sizeof(NeroString));
    subWindowString2->x = 0;
    subWindowString2->y = 0;

    char str[10] = "World";
    char number[5];
    sprintf(number, "%d", st->value);

    char *newString = (char*) malloc(10 * sizeof(char));
    strcpy(newString, strcat(str, number));

    subWindowString2->string = newString;
    subWindowString2->length = 10;
    subSubSubWindow->string = subWindowString2;

    subWindow->subWindows[0] = subSubWindow;
    subWindow->subWindowSize = 1;

    subWindow->subWindows[1] = subSubSubWindow;
    subWindow->subWindowSize = 2;

    mainWin->subWindows[0] = subWindow;
    mainWin->subWindowSize = 1;

    return mainWin;
}

void handleKey() {

}

void main_destroy() {

}