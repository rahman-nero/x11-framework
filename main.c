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

static Window create_win(const int x, const int y, const int w, const int h, const int b) {
    Window win;

    XColor color = {
            .red = 16,
            .green = 14,
            .blue = 25,
    };
    /* Allocate a color for the background by params from color variable*/
    XAllocColor(config.dpy, DefaultColormap(config.dpy, config.scr), &color);

    XSetWindowAttributes xwa = {
            .background_pixel = color.pixel,
            .border_pixel = BlackPixel(config.dpy, config.scr),
    };

    xwa.event_mask = Button1MotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | ExposureMask;

    win = XCreateWindow(config.dpy, config.root, x, y, w, h, b, DefaultDepth(config.dpy, config.scr), InputOutput,
                        config.vis,
                        CWBackPixel | CWEventMask | CWBorderPixel, &xwa);

    return win;
}


static void run() {
    XEvent ev;

    char *route = "/main";

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
                matchRoute(route, ev);
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

    // Register Routes
    registerRoutes();

    // Creating our main window
    config.mainWin = create_win(0, 0, DisplayWidth(config.dpy, config.scr), DisplayHeight(config.dpy, config.scr),
                                BORDER);

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
