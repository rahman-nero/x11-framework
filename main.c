//
// Created by nero on 4/15/24.
//
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
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
#define LINE 4

static Window create_win(const int x, const int y, const int w, const int h, const int b) {
    Window win;
    XSetWindowAttributes xwa = {
            .background_pixel = WhitePixel(dpy, scr),
            .border_pixel = BlackPixel(dpy, scr),
    };

    xwa.event_mask = Button1MotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask;

    win = XCreateWindow(dpy, root, x, y, w, h, b, DefaultDepth(dpy, scr), InputOutput, DefaultVisual(dpy, scr),
                        CWBackPixel | CWBorderPixel | CWEventMask, &xwa);

    return win;
}

static GC create_gc(int line_width)
{
    GC gc;
    XGCValues xgcValues;

    unsigned long valueMask;

    xgcValues.line_style = LineSolid;
    xgcValues.line_width = line_width;
    xgcValues.cap_style = CapButt;
    xgcValues.join_style = JoinMiter;
    xgcValues.fill_style = FillSolid;
    xgcValues.foreground = BlackPixel(dpy, scr);
    xgcValues.background = WhitePixel(dpy, scr);

    valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCCapStyle | GCJoinStyle | GCLineWidth;

    gc = XCreateGC(dpy, root, valueMask, &xgcValues);

    return gc;
}

static void run(GC gc) {
    XEvent ev;

    int init = 0;

    // Previous positions
    int prev_x = 0;
    int prev_y = 0;


    while (XNextEvent(dpy, &ev) == 0) {

        switch (ev.type) {
            case ButtonPress:
                if (ev.xbutton.button == Button1) {
                    // Drawing a point
                    XDrawPoint(dpy, ev.xbutton.window, gc, ev.xbutton.x, ev.xbutton.y);
                }
                break;

            // If we drag our mouse in clicked state (as mousepress in js)
            // We will draw a line or point depending on whether it pressed or not
            case MotionNotify:

                if (init) {
                    XDrawLine(dpy, ev.xbutton.window, gc, prev_x, prev_y, ev.xbutton.x, ev.xbutton.y);
                } else {
                    XDrawPoint(dpy, ev.xbutton.window, gc, ev.xbutton.x, ev.xbutton.y);
                    init = 1;
                }

                prev_x = ev.xbutton.x;
                prev_y = ev.xbutton.y;

                break;

            case ButtonRelease:
                init = 0;
                break;

            case KeyPress:
                if (XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0) == XK_q) {
                    return;
                }
        }

    }
}

int main() {

    Window main_win;

    dpy = XOpenDisplay(NULL);
    GC gc = create_gc(LINE);

    if (dpy == NULL) {
        errx(1, "Can't open display");
    }

    // get default screen and root window
    scr = DefaultScreen(dpy);
    root = RootWindow(dpy, scr);

    // Creating our simple window
    main_win = create_win(POSX, POSY, WIDTH, HEIGHT, BORDER);

    // Mapping window to display server
    XMapWindow(dpy, main_win);

    run(gc);

    // Unmap window
    XUnmapWindow(dpy, main_win);
    // Freeing resources
    XDestroyWindow(dpy, main_win);
    XFreeGC(dpy, gc);
    // Close connection with display
    XCloseDisplay(dpy);

    return 0;
}