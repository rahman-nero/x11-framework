#include <X11/X.h>
#include <err.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include "./src/config/routes.h"
#include "./src/config/config.h"
#include "./src/utils/utils.h"

#define TITLE "Application"

NeroConfig config;

void mappingWindows(NeroWindow *currentWin, const Window *parentWin) {
    Window win = create_sub_window(
            parentWin,
            currentWin->config.x,
            currentWin->config.y,
            currentWin->config.width,
            currentWin->config.height,
            currentWin->config.border_width,
            0,
            currentWin->config.background
    );

    XMapWindow(config.dpy, win);

    if (currentWin->subWindowSize > 0) {
        for (int i = 0; i < currentWin->subWindowSize; i++) {
            NeroWindow subWindow = *(NeroWindow *) currentWin->subWindows[i];
            mappingWindows(&subWindow, &win);
        }
    }
}

static void run() {
    char *route = "/main";

    NeroWindow *result = matchRoute(route);

    mappingWindows(result, &config.mainWin);

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
    config.displayWidth = DisplayWidth(config.dpy, config.scr);
    config.displayHeight = DisplayHeight(config.dpy, config.scr);

    printf("Weight: %d and Height: %d \n", config.displayWidth, config.displayHeight);

    // Register Routes
    registerRoutes();

    // Creating our main window
    config.mainWin = create_main_window(0, 0, 1920, config.displayHeight, 0, 0);

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


    // Freeing resources
    XDestroyWindow(dpy, config.mainWin);
    XFreeFont(dpy, font);
    XFreeGC(dpy, config.gc);

    // Close connection with display
    XCloseDisplay(dpy);
    return 0;
}
