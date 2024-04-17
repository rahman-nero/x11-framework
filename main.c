//
// Created by nero on 4/15/24.
//
#include <X11/Xlib.h>
#include <stdio.h>
#include <err.h>

// Display server connenction
static Display *dpy;
// Screen number
static int scr;
static Window root;

#define POSX 500
#define POSY 500
#define WIDTH 500
#define HEIGHT 500
#define BORDER 15

static Window create_win(const int x, const int y, const int w, const int h, const int b) {
    Window win;
    XSetWindowAttributes xwa;

    xwa.background_pixel = WhitePixel(dpy, scr);
    xwa.border_pixel = BlackPixel(dpy, scr);
    xwa.event_mask = ButtonPress;

    win = XCreateWindow(dpy, root, x, y, w, h, b, DefaultDepth(dpy, scr), InputOutput, DefaultVisual(dpy, scr),
                        CWBackPixel | CWBorderPixel | CWEventMask, &xwa);

    return win;
}

static void run() {
    XEvent ev;

    while (XNextEvent(dpy, &ev) == 0) {

        switch (ev.type) {
            case ButtonPress:
                return;
        }

    }
}

int main() {

    Window win;

    dpy = XOpenDisplay(NULL);

    if (dpy == NULL) {
        errx(1, "Can't open display");
    }

    // get default screen and root window
    scr = DefaultScreen(dpy);
    root = RootWindow(dpy, scr);

    // Creating our simple window
    win = create_win(POSX, POSY, WIDTH, HEIGHT, BORDER);

    // Mapping window to display server
    XMapWindow(dpy, win);

    run();

    // Unmap window
    XUnmapWindow(dpy, win);
    // Freeing resources
    XDestroyWindow(dpy, win);
    // Close connection with display
    XCloseDisplay(dpy);

    return 0;
}