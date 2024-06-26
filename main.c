#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <X11/X.h>
#include <err.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <pthread.h>
#include <unistd.h>
#include "./src/routes/routes.h"
#include "./src/window.h"
#include "./src/store/state.h"

#define TITLE "Application"

// Main config
NeroConfig config;

// Holds texts that needs to be drawn
RenderQueue *stringRenderQueue;

// Holds images that needs to be drawn
RenderQueue *imageRenderQueue;

// Result of matched route's controller
NeroWindow *currentView;

// State
StateList *stateList;
uint8_t stateGotUpdated;

// Events
NeroWindow *windowsWithEvents[127];
uint8_t windowsWithEventsLength = 0;

void run() {
    // Current route
    char *route = "/main";

    // Result of matching route
    currentView = matchRoute(route);

    // Recursive mapping windows
    recursiveMapWindows(currentView, config.mainWin);

    // Recursive collection windows
    recursiveCollectWindowsWithEvents(currentView, windowsWithEvents, &windowsWithEventsLength);

    printf("Events : %d\n", windowsWithEventsLength);
}

void show_image(NeroWindow *window) {
    int width, height, channels;
    unsigned char *img = stbi_load(window->image->filename, &width, &height, &channels, 4);
    if (img == NULL) {
        fprintf(stderr, "Error loading image: %s\n", window->image->filename);
        return;
    }

    XImage *ximage = XCreateImage(config.dpy, config.vis, DefaultDepth(config.dpy, config.scr), ZPixmap, 0,
                                  (char *) img, width,
                                  height, 32, 0);
    if (ximage == NULL) {
        fprintf(stderr, "Error creating XImage\n");
        stbi_image_free(img);
        return;
    }

    XPutImage(config.dpy, window->window, window->gc, ximage, 0, 0, 0, 0, width, height);

//     Free resources
    ximage->data = NULL;  // Prevent XDestroyImage from freeing the image data
    XDestroyImage(ximage);
    stbi_image_free(img);
}

void *eventHandler() {
    XEvent ev;

    while (XNextEvent(config.dpy, &ev) == 0) {
        switch (ev.type) {
            case ButtonPress:

                // If there is registered event from window
                if (windowsWithEventsLength > 0) {
                    for (int i = 0; i < windowsWithEventsLength; ++i) {
                        // Check if it is Click event
                        if (ev.xany.window == windowsWithEvents[i]->window &&
                            strcmp(windowsWithEvents[i]->event->type, ClickEvent) == 0) {
                            windowsWithEvents[i]->event->callback(ev);
                        }
                    }

                }
                break;

            case KeyPress:
                if (XkbKeycodeToKeysym(config.dpy, ev.xkey.keycode, 0, 0) == XK_q) {
                    return NULL;
                }

                // If there is registered event from window
                if (windowsWithEventsLength > 0) {
                    for (int i = 0; i < windowsWithEventsLength; ++i) {
                        // Check if it is Click event
                        if (ev.xany.window == windowsWithEvents[i]->window &&
                            strcmp(windowsWithEvents[i]->event->type, KeyboardEvent) == 0) {
                            windowsWithEvents[i]->event->callback(ev);
                        }
                    }
                }

                break;

            case Expose:
                Window exposedWindow = ev.xany.window;

                if (stringRenderQueue->length > 0) {
                    for (size_t i = 0; i < stringRenderQueue->length; i++) {
                        // Selected window
                        NeroWindow *matchedWindow = stringRenderQueue->queue[i];

                        // If exposed window and selected window is equal, then draw text
                        if (exposedWindow == matchedWindow->window && matchedWindow->string != NULL) {
                            XDrawString(config.dpy,
                                        matchedWindow->window,
                                        config.gc,
                                        matchedWindow->string->x,
                                        matchedWindow->string->y + 10,
                                        matchedWindow->string->string,
                                        matchedWindow->string->length
                            );

                            // Remove after rendering
                            RenderQueueFreeByIndex(stringRenderQueue, i);
                        }
                    }
                }

                if (imageRenderQueue->length > 0) {
                    for (size_t i = 0; i < imageRenderQueue->length; i++) {
                        // Selected window
                        NeroWindow *matchedWindow = imageRenderQueue->queue[i];

                        // If exposed window and selected window is equal, then draw text
                        if (exposedWindow == matchedWindow->window) {
                            show_image(matchedWindow);

                            // Remove after rendering
                            RenderQueueFreeByIndex(imageRenderQueue, i);
                        }
                    }
                }

                break;
        }
    }

    return NULL;
}

void *stateUpdateHandler() {
    while (1) {
        if (stateGotUpdated == 1) {
            printf("State updated \n");

            // Unmapping all windows with subwindows and freeing them
            recursiveUnmapWindows(currentView);

            // Resetting events
            windowsWithEventsLength = 0;

            // Regenerating windows again
            run();

            // Flushing result to apply changes
            XFlush(config.dpy);

            // Setting state event to default
            stateGotUpdated = 0;
        }
    }
}

int main() {
    // Display
    Display *dpy = XOpenDisplay(NULL);

    if (dpy == NULL) {
        errx(1, "Can't open display");
    }

    // Initialize state
    stateList = (StateList *) malloc(sizeof(StateList));
    stateList->length = 0;
    stateGotUpdated = 0;

    // Setting default params
    config.dpy = dpy;
    config.scr = DefaultScreen(config.dpy);
    config.root = RootWindow(config.dpy, config.scr);
    config.vis = DefaultVisual(config.dpy, config.scr);
    config.displayWidth = DisplayWidth(config.dpy, config.scr);
    config.displayHeight = DisplayHeight(config.dpy, config.scr);
    config.colormap = DefaultColormap(config.dpy, config.scr);

    // Creating queue
    stringRenderQueue = RenderQueueNew();

    // Creating queue
    imageRenderQueue = RenderQueueNew();

    // Register Routes
    registerRoutes();

    // Creating our main window
    config.mainWin = createWindow(config.root, 0, 0, 1920, config.displayHeight, 0, 0, 0x00FFFFFF);

    // Creating Graphic context
    config.gc = createGc(config.mainWin);

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

    // Thread to handle all events
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, eventHandler, NULL);

    // Thread to handle state update
    pthread_t thread_id2;
    pthread_create(&thread_id2, NULL, stateUpdateHandler, NULL);

    pthread_join(thread_id, NULL);

    // Freeing resources
    XDestroyWindow(dpy, config.mainWin);
    XFreeFont(dpy, font);
    XFreeGC(dpy, config.gc);
    free(stringRenderQueue);
    free(currentView);

    // Close connection with display
    XCloseDisplay(dpy);

    return 0;
}
