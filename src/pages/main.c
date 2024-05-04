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

    NeroWindow *subWindow = (NeroWindow *) malloc(sizeof(NeroWindow));
    subWindow->config.width = 40;
    subWindow->config.height = 40;
    subWindow->config.x = 0;
    subWindow->config.y = 0;
    subWindow->config.border_width = 0;
    subWindow->config.border_color = 0;
    subWindow->config.background = 0x00F9F9F9;

    //
//    struct SubWindow *subSubWindow = (struct SubWindow*) malloc(sizeof(struct SubWindow));
//    subSubWindow->config.w = 50;
//    subSubWindow->config.h = 50;
//    subSubWindow->config.x = 10;
//    subSubWindow->config.y = 10;
//    subSubWindow->config.b = 0;
//    subSubWindow->config.background_hex = "#f9f9f9";
//    subSubWindow->sub_window_size = 0;
//
//
//    subWindow->sub_windows[0] = subSubWindow;
//    subWindow->sub_window_size = 1;

    mainWin->subWindows[0] = subWindow;
    mainWin->subWindowSize = 1;

    return mainWin;
}

void main_destroy() {

}