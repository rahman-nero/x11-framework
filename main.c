//
// Created by nero on 4/15/24.
//
#include <X11/X.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Xft/Xft.h>

#define POSX 500
#define POSY 500
#define WIDTH 500
#define HEIGHT 500
#define BORDER 15
#define LINE 4
#define TITLE "Paint program"
#define BL "#0000ff"
#define GR "#00ff00"
#define RE "#ff0000"

#define CHILD_W 50
#define CHILD_H 50
#define CHILD_B 1


enum {
    BLUE, GREEN, RED
};

typedef struct {
    Window win;
    XftColor *color;
} ColorButtons;


// Display server connenction
static Display *dpy;
// Screen number
static int scr;
static Window root;
static Visual *vis;
static ColorButtons colorButtons[3];
static const char *colors[3] = {BL, GR, RE};

static void create_color(XftColor *color, const char *name) {
    if (!XftColorAllocName(dpy, vis, DefaultColormap(dpy, scr), name, color)) {
        errx(1, "Can't allocate color");
    }

    // color->pixel != 0xff << 24;
}

static Window create_win(const int x, const int y, const int w, const int h, const int b, Window *parent, int idx) {
    Window win;
    XSetWindowAttributes xwa = {
            .background_pixel = WhitePixel(dpy, scr),
            .border_pixel = BlackPixel(dpy, scr),
    };

    if (!parent) {
        xwa.event_mask = Button1MotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask;

        win = XCreateWindow(dpy, root, x, y, w, h, b, DefaultDepth(dpy, scr), InputOutput, vis,
                            CWBackPixel | CWEventMask | CWBorderPixel, &xwa);

    } else {
        xwa.event_mask = ButtonPressMask;
        xwa.background_pixel = colorButtons[idx].color->pixel;

        win = XCreateWindow(dpy, *parent, x, y, w, h, b, DefaultDepth(dpy, scr), InputOutput, vis,
                            CWBackPixel | CWEventMask | CWBorderPixel, &xwa);
    }

    return win;
}

static GC create_gc(int line_width) {
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

    valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCLineWidth | GCCapStyle | GCJoinStyle;

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
                    if (ev.xbutton.window == colorButtons[RED].win) {
                        XSetForeground(dpy, gc, colorButtons[RED].color->pixel);
                    } else if (ev.xbutton.window == colorButtons[BLUE].win) {
                        XSetForeground(dpy, gc, colorButtons[BLUE].color->pixel);
                    } else if (ev.xbutton.window == colorButtons[GREEN].win) {
                        XSetForeground(dpy, gc, colorButtons[GREEN].color->pixel);
                    } else {
                        // Drawing a point
                        XDrawPoint(dpy, ev.xbutton.window, gc, ev.xbutton.x, ev.xbutton.y);
                    }
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
                if (XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0) == XK_w) {
                    XClearWindow(dpy, ev.xbutton.window);
                }

//                if (XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0) == XK_1) {
//                    create_color(color, "#000000");
//                    XSetForeground(dpy, gc, color->pixel);
//                }
//
//                if (XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0) == XK_2) {
//                    create_color(color, "#ff0000");
//                    XSetForeground(dpy, gc, color->pixel);
//                }
//
//                if (XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0) == XK_3) {
//                    create_color(color, "#008000");
//                    XSetForeground(dpy, gc, color->pixel);
//                }
//                if (XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0) == XK_4) {
//                    create_color(color, "#ffa000");
//                    XSetForeground(dpy, gc, color->pixel);
//                }

                break;
        }

    }
}


int main() {

    Window main_win;

    dpy = XOpenDisplay(NULL);
    GC gc;
    int y = 0;

    if (dpy == NULL) {
        errx(1, "Can't open display");
    }

    // get default screen and root window
    scr = DefaultScreen(dpy);
    root = RootWindow(dpy, scr);
    vis = DefaultVisual(dpy, scr);

    // Creating our simple window
    main_win = create_win(POSX, POSY, WIDTH, HEIGHT, BORDER, NULL, 0);

    for (int i = 0; i < 3; ++i) {
        colorButtons[i].color = malloc(sizeof(XftColor));
        if (!colorButtons[i].color) {
            errx(1, "Can't allocate memory for color");
        }

        if (!XftColorAllocName(dpy, vis, DefaultColormap(dpy, scr), colors[i], colorButtons[i].color)) {
            errx(1, "Can't allocate xft color");
        }

        colorButtons[i].win = create_win(0, y, CHILD_W, CHILD_W, CHILD_B, &main_win, i);

        // Each time increase margins between windows
        y += CHILD_H + CHILD_B;
    }

    gc = create_gc(LINE);

    // Setting Title
    XStoreName(dpy, main_win, TITLE);

    // Mapping window to display server
    XMapWindow(dpy, main_win);
    XMapSubwindows(dpy, main_win);

    run(gc);

    // Unmap window
    XUnmapWindow(dpy, main_win);
    XUnmapSubwindows(dpy, main_win);
    // Freeing resources
    XDestroyWindow(dpy, main_win);
    XDestroySubwindows(dpy, main_win);
    XFreeGC(dpy, gc);

    for (int i = 0; i < 3; ++i) {
        XftColorFree(dpy, vis, DefaultColormap(dpy, scr), colorButtons[i].color);
    }

    // Close connection with display
    XCloseDisplay(dpy);

    return 0;
}