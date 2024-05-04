#include "../utils/utils.h"
#include "../config/config.h"

extern Config config;

struct MainWin* main_run() {

    struct MainWin *mainWin = (struct MainWin*) malloc(sizeof(struct MainWin));
    mainWin->config.w = 1920;
    mainWin->config.h = 70;
    mainWin->config.x = 0;
    mainWin->config.y = 0;
    mainWin->config.b = 0;
    mainWin->config.background =  0x00324EFF;
    mainWin->sub_window_size = 0;

    struct SubWindow *subWindow = (struct SubWindow*) malloc(sizeof(struct SubWindow));
    subWindow->config.w = 40;
    subWindow->config.h = 40;
    subWindow->config.x = 0;
    subWindow->config.y = 0;
    subWindow->config.b = 0;
    subWindow->config.background = 0x00F9F9F9;
    subWindow->sub_window_size = 0;
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

    mainWin->sub_windows[0] = subWindow;
    mainWin->sub_window_size = 1;

    return mainWin;
}

void main_destroy() {

}