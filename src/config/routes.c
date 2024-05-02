#include <string.h>
#include <err.h>
#include <stdio.h>
#include "../pages/index.h"
#include <X11/X.h>
#include <X11/Xlib.h>


typedef Window (*FunctionCallback)(XEvent XEvent);

char *route_names[10];
FunctionCallback route_calls[10];
size_t used_cells = 0;

/**
 * Register routes
 * */
void registerRoutes() {
    Window (*index_callback)(XEvent XEvent) = index_run;

    route_names[0] = "/main";
    route_calls[0] = index_callback;

    used_cells = 1;
}


/**
 * Look for route and call matched route. Return Null otherwise
 * */
Window matchRoute(const char *route, XEvent event) {

    for (int i = 0; i < used_cells; i++) {
        if(strcasecmp(route, route_names[i]) == 0) {
            return route_calls[i](event);
        }
    }

    errx(1, "Can't find route");
}