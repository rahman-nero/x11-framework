#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <inttypes.h>
#include "string.h"

#ifndef NERO_SRC_WINDOW_H
#define NERO_SRC_WINDOW_H

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
    uint16_t displayWidth;

    // Current display Height
    uint16_t displayHeight;
} NeroConfig;

typedef struct {
    // Position from left to right
    uint16_t x;

    // Position from top to bottom
    uint16_t y;

    // Width of window
    uint16_t width;

    // Height of window
    uint16_t height;

    // Border
    uint8_t border_width;

    // Border color (RGB). Example: 0x00FFFFFF - white
    NeroBorderColor border_color;

    // Background color based on RGB. Example: 0x00FF0000 - red
    NeroBackgroundColor background;
} NeroWindowConfig;

typedef struct NeroWindow {
    // Filled when window has been created
    Window window;

    // Config
    NeroWindowConfig config;

    // String
    NeroString *string;

    // Color of text
    NeroStringColor color;

    // Sub windows
    struct NeroWindow *subWindows[127];

    // Count of windows
    uint8_t subWindowSize;
} NeroWindow;

typedef struct {
    NeroWindow *queue[127];
    uint8_t length;
} StringRenderQueue;

/**
 * Constructor of StringRenderQueue
 * */
StringRenderQueue *StringRenderQueueNew();

/**
 * Add a window to queue
 * */
void StringRenderQueueAddWindow(StringRenderQueue *object, NeroWindow *window);

/**
 * Remove value by index and shift array
 * */
void StringRenderQueueFreeByIndex(StringRenderQueue *object, uint8_t index);

/**
 * Free all elements from queue
 * */
void StringRenderQueueFreeQueue(StringRenderQueue *object);

/**
 * Recursive mapping windows
 * */
void recursiveMapWindows(NeroWindow *currentWin, Window parent);

/**
 * Recursive unmapping windows
 * */
void recursiveUnmapWindows(NeroWindow *currentWin);

/**
 * Creating simple window with properties
 * */
Window createWindow(
        Window parent,
        uint16_t x,
        uint16_t y,
        uint16_t width,
        uint16_t height,
        uint8_t border,
        int masks,
        NeroBackgroundColor background
);

/**
 * Change window's background.
 *
 * @param Window* window
 * @param char* hex
 * */
void changeWindowBackground(const Window window, const char *hex);

/**
 * Create Graphic Context
 * */
GC createGc(const Window window);

#endif //NERO_SRC_WINDOW_H