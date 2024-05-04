//
// Created by nero on 4/15/24.
//
#include <X11/X.h>
#include <err.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include "./src/config/routes.h"
#include "./src/config/config.h"
#include "./src/utils/utils.h"

Config config;

void runMapping(struct MainWin *mainWin, const Window *rootWin);
void recursiveMappingSubWindow(struct SubWindow *controller, const Window *rootWin);

static void run() {
    char *route = "/main";

    struct MainWin *outPut = matchRoute(route);

    runMapping(outPut, &config.mainWin);

    XEvent ev;
    while (XNextEvent(config.dpy, &ev) == 0) {
        switch (ev.type) {
            case ButtonPress:
                // Click left button on mouse
                if (ev.xbutton.button == Button1) {
                }

                // Click on middle scroll on mouse
                if (ev.xbutton.button == Button2) {

                }

                // Click right button on mouse
                if (ev.xbutton.button == Button3) {

                }
                break;

            case KeyPress:
                if (XkbKeycodeToKeysym(config.dpy, ev.xkey.keycode, 0, 0) == XK_q) {
                    return;
                }
                break;

            case Expose:
                break;
        }
    }
}


void runMapping(struct MainWin *mainWin, const Window *rootWin) {
    Window win = create_sub_window(
            rootWin,
            mainWin->config.x,
            mainWin->config.y,
            mainWin->config.w,
            mainWin->config.h,
            mainWin->config.b,
            0,
            mainWin->config.background
    );

    XMapWindow(config.dpy, win);

    if (mainWin->sub_window_size > 0) {
        for (int i = 0; i < mainWin->sub_window_size; i++) {
            struct SubWindow subWindow = *(struct SubWindow *) mainWin->sub_windows[i];
            recursiveMappingSubWindow(&subWindow, &win);
        }
    }
}

void recursiveMappingSubWindow(struct SubWindow *controller, const Window *rootWin) {
    Window win = create_sub_window(
            rootWin,
            controller->config.x,
            controller->config.y,
            controller->config.w,
            controller->config.h,
            controller->config.b,
            0,
            controller->config.background
    );

    XMapWindow(config.dpy, win);

    if (controller->sub_window_size > 0) {
        for (int i = 0; i < controller->sub_window_size; i++) {
            struct SubWindow subWindow = *(struct SubWindow *) controller->sub_windows[i];
            recursiveMappingSubWindow(&subWindow, &win);
        }
    }

}

int main() {

    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        errx(1, "Can't open display");
    }

    // Setting default params
    config.dpy = dpy;
    config.scr = DefaultScreen(config.dpy);
    config.root = RootWindow(config.dpy, config.scr);
    config.vis = DefaultVisual(config.dpy, config.scr);
    config.width = DisplayWidth(config.dpy, config.scr);
    config.height = DisplayHeight(config.dpy, config.scr);

    printf("Weight: %d and Height: %d \n", config.width, config.height);

    // Register Routes
    registerRoutes();

    // Creating our main window
    config.mainWin = create_main_window(0, 0, 1920, config.height, BORDER, 0);

    // Creating Graphic context
    config.gc = create_gc(&config.mainWin);

    // Setting Title for window
    XStoreName(dpy, config.mainWin, TITLE);

    // Mapping window to display server
    XMapWindow(dpy, config.mainWin);

    // Load font
    XFontStruct *font = XLoadQueryFont(config.dpy, "-*-*-medium-r-*-*-12-*-*-*-m-*-iso8859-1");
    if (font == NULL) errx(1, "Can't load font");

    // Set font for the graphics context
    XSetFont(config.dpy, config.gc, font->fid);

    run();

    // Unmap window
    XUnmapWindow(dpy, config.mainWin);
    XUnmapSubwindows(dpy, config.mainWin);

    // Freeing resources
    XDestroyWindow(dpy, config.mainWin);
    XFreeFont(dpy, font);
    XFreeGC(dpy, config.gc);

    // Close connection with display
    XCloseDisplay(dpy);
    return 0;
}
