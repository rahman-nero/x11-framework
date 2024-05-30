#include <X11/X.h>
#include <err.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include "./src/config/routes.h"
#include "./src/config/config.h"
#include "./src/utils/utils.h"
#include "./src/store/state.h"
#include <pthread.h>
#include <unistd.h>

#define TITLE "Application"

NeroConfig config;

uint8_t stateGotUpdated;
StateList *stateList;

// Windows which waits until expose event is dispatched
NeroWindow *waitingExpose[10];
size_t waitingExposeCount = 0;

typedef void (*ButtonPressEventCallback)(NeroWindow window, XEvent event);

typedef void (*KeyPressEventCallback)(NeroWindow window, XEvent event);

typedef struct NeroEventListener {
    // List of Windows who listens button event
    NeroWindow *buttonPressEvent[127];

    // Callbacks of those windows that listens button event
    ButtonPressEventCallback buttonPressEventCallback[127];

    // Size of events
    size_t buttonPressEventSize;

    // List of Windows who listens button event
    NeroWindow *keyPressEvent[127];
    // Callbacks of those windows that listens button event
    KeyPressEventCallback keyPressEventCallback[127];
    // Size of events
    size_t keyPressEventSize;
} NeroEventListener;


void mappingWindows(NeroWindow *currentWin, const Window *parentWin) {
    Window win = create_sub_window(
            parentWin,
            currentWin->config.x,
            currentWin->config.y,
            currentWin->config.width,
            currentWin->config.height,
            currentWin->config.border_width,
            0,
            currentWin->config.background
    );

    XMapWindow(config.dpy, win);
    currentWin->window = win;

    if (currentWin->string != NULL) {
        waitingExpose[waitingExposeCount] = currentWin;
        waitingExposeCount += 1;
    }

    if (currentWin->subWindowSize > 0) {
        for (int i = 0; i < currentWin->subWindowSize; i++) {
            NeroWindow *subWindow = (NeroWindow *) currentWin->subWindows[i];
            mappingWindows(subWindow, &win);
        }
    }
}

void *eventHandler();

void *stateUpdateHandler();

void run() {
    // Current route
    char *route = "/main";

    // Result of matching route
    NeroWindow *result = matchRoute(route);

    mappingWindows(result, &config.mainWin);
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
                }

                if (XkbKeycodeToKeysym(config.dpy, ev.xkey.keycode, 0, 0) == XK_t) {
                    for (int i = 0; i < stateList->length; ++i) {
                        printf("State: %d\n",  stateList->list[i]->value + 1);
                        updateState(stateList->list[i], stateList->list[i]->value + 1);
                    }
                }

                break;

            case Expose:
                Window exposedWindow = ev.xany.window;
                printf("Expose \n");

                if (waitingExposeCount > 0) {
                    for (size_t i = 0; i < waitingExposeCount; i++) {
                        // Selected window
                        NeroWindow *matchedWindow = waitingExpose[i];

                        // If exposed window and selected window is equal, then draw text
                        if (exposedWindow == matchedWindow->window && matchedWindow->string != NULL) {

                            XDrawString(config.dpy,
                                        matchedWindow->window,
                                        config.gc,
                                        matchedWindow->string->x ,
                                        matchedWindow->string->y + 10,
                                        matchedWindow->string->string,
                                        matchedWindow->string->length
                            );
                        }
                    }

                }
                break;
        }
    }
}


void *stateUpdateHandler() {
    while (1) {
        if (stateGotUpdated == 1) {
            printf("State updated: %d \n", stateGotUpdated);

            // Freeing resources
            XUnmapSubwindows(config.dpy, config.mainWin);
//            XDestroySubwindows(config.dpy, config.mainWin);
//            XDestroyWindow(config.dpy, config.mainWin);

//            XFreeFont(config.dpy, config.font);
//            XFreeGC(config.dpy, config.gc);

            // Re run code
            run();

            XFlush(config.dpy);

            stateGotUpdated = 0;
        }
    }
}


int main() {

    Display *dpy = XOpenDisplay(NULL);

    // Initialize state
    stateList = (StateList *) malloc(sizeof(StateList));
    stateList->length = 0;
    stateGotUpdated = 0;

    if (dpy == NULL) {
        errx(1, "Can't open display");
    }

    // Setting default params
    config.dpy = dpy;
    config.scr = DefaultScreen(config.dpy);
    config.root = RootWindow(config.dpy, config.scr);
    config.vis = DefaultVisual(config.dpy, config.scr);
    config.displayWidth = DisplayWidth(config.dpy, config.scr);
    config.displayHeight = DisplayHeight(config.dpy, config.scr);

    // Register Routes
    registerRoutes();

    // Creating our main window
    config.mainWin = create_main_window(0, 0, 1920, config.displayHeight, 0, 0);

    // Creating Graphic context
    config.gc = create_gc(&config.mainWin);

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

    // Thread to handle state update

    // Thread to handle all events
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, eventHandler, NULL);

    pthread_t thread_id2;
    pthread_create(&thread_id2, NULL, stateUpdateHandler, NULL);

    while (1);

    // Freeing resources
    XDestroyWindow(dpy, config.mainWin);
    XFreeFont(dpy, font);
    XFreeGC(dpy, config.gc);

    // Close connection with display
    XCloseDisplay(dpy);
    return 0;
}
