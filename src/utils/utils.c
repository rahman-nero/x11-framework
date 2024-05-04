#include <X11/X.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include "utils.h"
#include "../config/config.h"

extern NeroConfig config;

/**
 * Create main window from parent
 * */
Window create_main_window(
        const int x,
        const int y,
        const int width,
        const int height,
        const int border,
        const int masks
) {
    Window win;

    XSetWindowAttributes windowAttributes = {
            .background_pixel = 0x00FFFF00,
            .border_pixel = BlackPixel(config.dpy, config.scr),
    };

    if (masks == 0) {
        windowAttributes.event_mask =
                Button1MotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | ExposureMask;
    } else {
        windowAttributes.event_mask = masks;
    }

    win = XCreateWindow(config.dpy, config.root, x, y, width, height, border, DefaultDepth(config.dpy, config.scr),
                        InputOutput, config.vis,
                        CWBackPixel | CWEventMask | CWBorderPixel, &windowAttributes);

    return win;
}

/**
 * Create sub-window from parent
 * */
Window create_sub_window(
        const Window *parent,
        const int x,
        const int y,
        const int width,
        const int height,
        const int border,
        const int masks,
        const unsigned long background
) {
    Window win;

    XSetWindowAttributes windowAttributes = {
            .background_pixel = background,
            .border_pixel = BlackPixel(config.dpy, config.scr),
    };

    if (masks == 0) {
        windowAttributes.event_mask =
                Button1MotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | ExposureMask;
    } else {
        windowAttributes.event_mask = masks;
    }

    win = XCreateWindow(config.dpy, *parent, x, y, width, height, border, DefaultDepth(config.dpy, config.scr),
                        InputOutput, config.vis,
                        CWBackPixel | CWEventMask | CWBorderPixel, &windowAttributes);

    return win;
}

/**
 * Change window's background.
 *
 * TODO: It has a memory leak

 * @param Window* window
 * @param char* hex
 * @return void
 * */
void change_window_background(const Window *window, const char *hex) {

    // Allocation memory for color
    XftColor *color = (XftColor *) malloc(sizeof(XftColor));

    if (color == NULL) {
        errx(1, "Can't allocate memory for color");
    }

    // Assign hex color to allocated memory
    if (!XftColorAllocName(config.dpy, config.vis, DefaultColormap(config.dpy, config.scr), hex, color)) {
        errx(1, "Can't allocate xft color");
    }

    XSetWindowBackground(config.dpy, *window, color->pixel);
}


/**
 * Create Graphic Context
 * */
GC create_gc(const Window *window) {
    GC gc;

    XGCValues xgcValues;

    unsigned long valueMask;

    xgcValues.line_style = LineSolid;
    xgcValues.line_width = 4;
    xgcValues.cap_style = CapButt;
    xgcValues.join_style = JoinMiter;
    xgcValues.fill_style = FillSolid;
    xgcValues.foreground = BlackPixel(config.dpy, config.scr);
    xgcValues.background = BlackPixel(config.dpy, config.scr);

    valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCCapStyle | GCJoinStyle | GCLineWidth;

    gc = XCreateGC(config.dpy, *window, valueMask, &xgcValues);

    return gc;
}