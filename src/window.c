#include <X11/X.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include "window.h"

extern NeroConfig config;
extern RenderQueue *stringRenderQueue;
extern RenderQueue *imageRenderQueue;

/**
 * Recursive mapping windows
 * */
void recursiveMapWindows(NeroWindow *currentWin, const Window parent) {

    // Creating window
    Window win = createWindow(
            parent,
            currentWin->config.x,
            currentWin->config.y,
            currentWin->config.width,
            currentWin->config.height,
            currentWin->config.borderWidth,
            0,
            currentWin->config.background
    );
    currentWin->window = win;

    // Mapping window
    XMapWindow(config.dpy, win);

    // Creating GC if there is something that needs to be drawn
    if (currentWin->string != NULL || currentWin->image != NULL) {
        currentWin->gc = createGc(currentWin->window);
    }

    // If there is a text that needs to be shown, we add it to queue
    if (currentWin->string != NULL) {
        RenderQueueAddWindow(stringRenderQueue, currentWin);
    }

    // If there is a text that needs to be shown, we add it to queue
    if (currentWin->image != NULL) {
        RenderQueueAddWindow(imageRenderQueue, currentWin);
    }

    // Calling function again if there is subWindows
    if (currentWin->subWindowSize > 0) {
        for (int i = 0; i < currentWin->subWindowSize; i++) {
            recursiveMapWindows(currentWin->subWindows[i], win);
        }
    }
}


/**
 * Recursive unmapping windows
 * */
void recursiveUnmapWindows(NeroWindow *currentWin) {
    // Unmapping window and destroying window
    XUnmapSubwindows(config.dpy, currentWin->window);
    XDestroySubwindows(config.dpy, currentWin->window);
    XUnmapWindow(config.dpy, currentWin->window);
    XDestroyWindow(config.dpy, currentWin->window);
    free(currentWin);
}


/**
 * Creating simple window with properties
 * */
Window createWindow(
        const Window parent,
        const uint16_t x,
        const uint16_t y,
        const uint16_t width,
        const uint16_t height,
        const uint8_t border,
        const int masks,
        const NeroBackgroundColor background
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

    win = XCreateWindow(config.dpy, parent, x, y, width, height, border, DefaultDepth(config.dpy, config.scr),
                        InputOutput, config.vis,
                        CWBackPixel | CWEventMask | CWBorderPixel, &windowAttributes);

    return win;
}

/**
 * Adding subwindow to the window
 * */
void NeroWindowAddSubWindow(NeroWindow *window, NeroWindow *subWindow) {
    window->subWindows[window->subWindowSize] = subWindow;
    window->subWindowSize += 1;
}

/**
 * Change window's background.
 *
 * @param Window* window
 * @param char* hex
 * */
void changeWindowBackground(const Window window, const char *hex) {

    // Allocation memory for color
    XftColor *color = (XftColor *) malloc(sizeof(XftColor));

    if (color == NULL) {
        errx(1, "Can't allocate memory for color");
    }

    // Assign hex color to allocated memory
    if (!XftColorAllocName(config.dpy, config.vis, config.colormap, hex, color)) {
        errx(1, "Can't allocate xft color");
    }

    XSetWindowBackground(config.dpy, window, color->pixel);

    XftColorFree(config.dpy, config.vis, config.colormap, color);
    free(color);
}


/**
 * Create Graphic Context
 * */
GC createGc(const Window window) {
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

    gc = XCreateGC(config.dpy, window, valueMask, &xgcValues);

    return gc;
}


/**
 * Constructor of RenderQueue
 * */
RenderQueue *RenderQueueNew() {
    RenderQueue *object = (RenderQueue *) malloc(sizeof(RenderQueue));
    object->length = 0;

    return object;
}

/**
 * Add a window to queue
 * */
void RenderQueueAddWindow(RenderQueue *object, NeroWindow *window) {
    object->queue[object->length] = window;
    object->length += 1;
}

/**
 * Remove value by index and shift array
 * */
void RenderQueueFreeByIndex(RenderQueue *object, uint8_t index) {
    // Checking if index is not out of array
    if (index >= object->length) {
        return;
    }

    // Shifting array elements to the left
    for (int i = index; i < object->length - 1; ++i) {
        object->queue[i] = object->queue[i + 1];
    }

    // Decrement length
    object->length -= 1;
}


/**
 * Free all elements from queue
 * */
void RenderQueueFreeQueue(RenderQueue *object) {
    for (int i = 0; i < object->length; ++i) {
        object->queue[i] = 0;
    }
    object->length = 0;
}


/**
 * Constructor of NeroWindow
 * */
NeroWindow *NeroWindowNew(
        const uint16_t width,
        const uint16_t height,
        const uint16_t x,
        const uint16_t y,
        const NeroBackgroundColor background,
        const uint8_t borderWidth,
        const NeroBorderColor borderColor,
        NeroString *string,
        NeroEventListener *event
) {

    NeroWindow *new = (NeroWindow *) malloc(sizeof(NeroWindow));

    new->config.width = width;
    new->config.height = height;
    new->config.x = x;
    new->config.y = y;
    new->config.borderWidth = borderWidth;
    new->config.borderColor = borderColor;
    new->event = event;
    new->config.background = background;
    new->string = string;
    new->subWindowSize = 0;
    new->image = NULL;

    return new;
}

/**
 * Constructor of NeroEventListener
 * */
NeroEventListener *newEventListener(char *type, EventCallback callback) {
    NeroEventListener *new = (NeroEventListener *) malloc(sizeof(NeroEventListener));

    if (type == NULL) {
        errx(1, "Type of event has to be filled");
    }

    // Checking type of event
    if (strcmp(type, ClickEvent) == 0 || strcmp(type, KeyboardEvent) == 0) {
    }

    new->type = type;
    new->callback = callback;

    return new;
}

/**
 * Constructor of NeroImage
 * */
NeroImage *NeroImageNew(
        const uint16_t width,
        const uint16_t height,
        char filename[]
) {
    NeroImage *new = (NeroImage *) malloc(sizeof(NeroImage));

    new->width = width;
    new->height = height;
    new->filename = filename;

    return new;
}


/**
 * Recursively collecting events from windows and its subwindows
 * */
void recursiveCollectWindowsWithEvents(NeroWindow *currentWin, NeroWindow *windowsWithEvents[],
                                       uint8_t *windowsWithEventsLength) {

    if (currentWin->event != NULL) {
        windowsWithEvents[*windowsWithEventsLength] = currentWin;
        *windowsWithEventsLength += 1;
    }

    // Calling function again if there is subWindows
    if (currentWin->subWindowSize > 0) {
        for (int i = 0; i < currentWin->subWindowSize; i++) {
            recursiveCollectWindowsWithEvents(currentWin->subWindows[i], windowsWithEvents, windowsWithEventsLength);
        }
    }
}



