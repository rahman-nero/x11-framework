#include "../utils/utils.h"
#include "../config/config.h"

extern NeroConfig config;

NeroWindow *main_run() {

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

    // Text in centered div
    NeroString *subWindowString = (NeroString *) malloc(sizeof(NeroString));
    subWindowString->x = (900 / 2) - 50 / 2;
    subWindowString->y = 0;
    subWindowString->string = "Hello";
    subWindowString->length = 5;

    subWindow->string = subWindowString;
    mainWin->subWindows[0] = subWindow;
    mainWin->subWindowSize = 1;

    return mainWin;
}

void main_destroy() {

}