#include <string.h>
#include <err.h>
#include "../pages/main.h"
#include "../config/config.h"

typedef struct MainWin* (*FunctionCallback)();

char *route_names[10];
FunctionCallback route_calls[10];
size_t used_cells = 0;

/**
 * Register routes
 * */
void registerRoutes() {
    struct MainWin* (*index_callback)() = main_run;

    route_names[0] = "/main";
    route_calls[0] = index_callback;

    used_cells = 1;
}


/**
 * Look for route and call matched route. Return Null otherwise
 * */
struct MainWin* matchRoute(const char *route) {
    for (int i = 0; i < used_cells; i++) {
        if(strcasecmp(route, route_names[i]) == 0) {
            return route_calls[i]();
        }
    }

    errx(1, "Can't find route");
}