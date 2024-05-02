#include <X11/X.h>
#include <X11/Xlib.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include "../utils/utils.h"
#include "../config/config.h"

extern Config config;

typedef struct {
    Window createdWindow;
    Window createdSubWindow;
    GC createdGc;
    u_int8_t inited;
} Controller;

Controller controller = {.inited = 0};

Window preload() {
    controller.inited = 1;

    // Creating Window
    controller.createdWindow = create_sub_window(&config.mainWin, 0, 0, 1900, 70, 0, 0);

    XMapWindow(config.dpy, controller.createdWindow);

    // Creating Sub Window
    controller.createdSubWindow = create_sub_window(&controller.createdWindow, 0, 0, 70, 10, 0, 0);

    XMapWindow(config.dpy, controller.createdSubWindow);

    // Create Graphic Context
    controller.createdGc = config.gc;

    // Change background
    change_window_background(&controller.createdWindow, "#1f1b2e");

    change_window_background(&controller.createdSubWindow, "#ff0900");

    return controller.createdWindow;
}

Window index_run(XEvent event) {
    // First call
    if (controller.inited != 1) return preload();

    // Exposed Window
    Window window = event.xany.window;

    // If our current window is exposed, we do our things
    // We do this because GC needs to remap our window to be able to work properly
    if (controller.createdWindow == window) {
        const char text[] = "Hello World";
        XDrawString(config.dpy, window, controller.createdGc, 20, 40, text, sizeof(text));
    }

    if (controller.createdSubWindow == window) {
    }

    return window;
}


void index_destroy() {

}