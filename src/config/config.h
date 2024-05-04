#include <X11/X.h>
#include <X11/Xlib.h>

#ifndef NERO_CONFIG_CONFIG_H
#define NERO_CONFIG_CONFIG_H

typedef unsigned long NeroBorderColor;
typedef unsigned long NeroBackgroundColor;
typedef unsigned long NeroStringColor;

typedef struct {
    // Display server connection
    Display *dpy;

    // Screen number
    int scr;

    // Root window (doesn't show up)
    Window root;

    // Visual
    Visual *vis;

    // Main Win
    Window mainWin;

    // Graphic context to be able to write something (with text)
    GC gc;

    // Current display Width
    // NOTE: If you have 3 monitors, it will be considered as one here.
    // So you will get around 5760 in this property, so be aware of it
    u_int16_t displayWidth;

    // Current display Height
    u_int16_t displayHeight;
} NeroConfig;

typedef struct {
    // Position from left to right
    u_int16_t x;

    // Position from top to bottom
    u_int16_t y;

    // Width of window
    u_int16_t width;

    // Height of window
    u_int16_t height;

    // Border
    int border_width;

    // Border color (RGB). Example: 0x00FFFFFF - white
    NeroBorderColor border_color;

    // Background color based on RGB. Example: 0x00FF0000 - red
    NeroBackgroundColor background;
} NeroWindowConfig;


typedef struct {
    // Position from left to right
    u_int16_t x;

    // Position from top to bottom
    u_int16_t y;

    // String
    char *string;

    // Length of Text
    u_int16_t length;
} NeroString;


typedef struct NeroWindow{
    // Filled when window has been created
    Window window;

    // Config
    NeroWindowConfig config;

    // String
    NeroString* string;

    // Color of text
    NeroStringColor color;

    // Sub windows
    struct NeroWindow *subWindows[127];

    // Count of windows
    u_int8_t subWindowSize;
} NeroWindow;

#endif //NERO_CONFIG_CONFIG_H

