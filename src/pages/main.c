#include "../utils/utils.h"
#include "../config/config.h"

extern NeroConfig config;

NeroWindow *main_run() {

    NeroWindow *mainWin = (NeroWindow *) malloc(sizeof(NeroWindow));
    mainWin->config.width = 1920;
    mainWin->config.height = 70;
    mainWin->config.x = 0;
    mainWin->config.y = 0;
    mainWin->config.border_width = 0;
    mainWin->config.border_color = 0;
    mainWin->config.background = 0x00324EFF;
    mainWin->string = NULL;

    // String
    NeroString *subWindowString = (NeroString *) malloc(sizeof(NeroString));
    subWindowString->x = 5;
    subWindowString->y = 5;
    subWindowString->string = "Hello";
    subWindowString->length = 5;

    NeroWindow *subWindow = (NeroWindow *) malloc(sizeof(NeroWindow));
    subWindow->config.width = 700;
    subWindow->config.height = 70;
    subWindow->config.x = 0;
    subWindow->config.y = 0;
    subWindow->config.border_width = 0;
    subWindow->config.border_color = 0;
    subWindow->config.background = 0x00F9F9F9;
    subWindow->string = subWindowString;

    mainWin->subWindows[0] = subWindow;
    mainWin->subWindowSize = 1;

    return mainWin;
}

void main_destroy() {

}