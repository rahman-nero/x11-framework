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

NeroConfig config;
StringRenderQueue *stringRenderQueue;
NeroWindow *currentView;

uint8_t stateGotUpdated;
StateList *stateList;

void run() {
    // Current route
    char *route = "/main";

    // Result of matching route
    currentView = matchRoute(route);

    recursiveMapWindows(currentView, config.mainWin);
}

void *eventHandler() {
    XEvent ev;

    while (XNextEvent(config.dpy, &ev) == 0) {
        switch (ev.type) {
            case ButtonPress:

                // Click left button on mouse
                if (ev.xbutton.button == Button1) {
                    printf("Button1 \n");
                }

                // Click on middle scroll on mouse
                if (ev.xbutton.button == Button2) {
                    printf("Button2 \n");
                }

                // Click right button on mouse
                if (ev.xbutton.button == Button3) {
                    printf("Button3 \n");
                }
                break;

            case KeyPress:
                if (XkbKeycodeToKeysym(config.dpy, ev.xkey.keycode, 0, 0) == XK_q) {
                    return NULL;
                }

                if (XkbKeycodeToKeysym(config.dpy, ev.xkey.keycode, 0, 0) == XK_t) {
                    for (int i = 0; i < stateList->length; ++i) {
                        printf("State: %d\n", stateList->list[i]->value + 1);
                        updateState(stateList->list[i], stateList->list[i]->value + 1);
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
                            StringRenderQueueFreeByIndex(stringRenderQueue, i);
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

    // Creating queue
    stringRenderQueue = StringRenderQueueNew();

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

    // Close connection with display
    XCloseDisplay(dpy);


    return 0;
}
