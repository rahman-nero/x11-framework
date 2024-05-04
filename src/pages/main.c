#include <X11/X.h>
#include <X11/Xlib.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include "../utils/utils.h"
#include "../config/config.h"

extern Config config;

struct MainWin* main_run() {

    struct MainWin *mainWin = (struct MainWin*) malloc(sizeof(struct MainWin));
    mainWin->config.w = 1920;
    mainWin->config.h = config.height;
    mainWin->config.x = 0;
    mainWin->config.y = 0;
    mainWin->config.b = 0;
    mainWin->config.background_hex = "#cccccc";
    mainWin->sub_window_size = 0;

    struct SubWindow *subWindow = (struct SubWindow*) malloc(sizeof(struct SubWindow));
    subWindow->config.w = 1920;
    subWindow->config.h = 70;
    subWindow->config.x = 0;
    subWindow->config.y = 0;
    subWindow->config.b = 0;
    subWindow->config.background_hex = "#ffa099";
    subWindow->sub_window_size = 0;

    struct SubWindow *subSubWindow = (struct SubWindow*) malloc(sizeof(struct SubWindow));
    subSubWindow->config.w = 5;
    subSubWindow->config.h = 5;
    subSubWindow->config.x = 5;
    subSubWindow->config.y = 5;
    subSubWindow->config.b = 0;
    subSubWindow->config.background_hex = "#000000";
    subSubWindow->sub_window_size = 0;


    subWindow->sub_windows[0] = subSubWindow;
    subWindow->sub_window_size = 1;

    mainWin->sub_windows[0] = subWindow;
    mainWin->sub_window_size = 1;

    return mainWin;
}

void main_destroy() {

}