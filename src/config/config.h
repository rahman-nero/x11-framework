#include <X11/X.h>
#include <X11/Xlib.h>

#ifndef FIRST_PROJECT_CONFIG_H
#define FIRST_PROJECT_CONFIG_H
#define BORDER 0
#define TITLE "Paint program"

typedef struct {
    // Display server connection
    Display *dpy;

    // Screen number
    int scr;

    // Root window
    Window root;

    // Visual
    Visual *vis;

    // Main Win
    Window mainWin;

    // Main Graphic context
    GC gc;

    // Display Width
    // If you have 3 monitors, it will be considered as one here.
    // So you will get around 5760 in this property
    u_int16_t width;

    // Display Height
    u_int16_t height;
} Config;

struct WindowConfig {
    int x;
    int y;
    int w;
    int h;
    int b;
    char *background_hex;
};

struct SubWindow {
    struct WindowConfig config;
    void *sub_windows[127];
    u_int8_t sub_window_size;
};

struct MainWin {
    struct WindowConfig config;
    void *sub_windows[127];
    u_int8_t sub_window_size;
};


#endif //FIRST_PROJECT_CONFIG_H
