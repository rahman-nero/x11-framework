#include <string.h>
#include <err.h>
#include "../pages/main.h"
#include "../config/config.h"

typedef NeroWindow *(*FunctionCallback)();

const char *routePaths[10];
FunctionCallback routeControllers[10];
size_t used_cells = 0;

/**
 * Function to add route to route list
 * @param path
 * @param Callback callback
 * @return void
 * */
void addRoute(char *path, NeroWindow *(*callback)()) {
    routePaths[0] = path;
    routeControllers[0] = callback;
    used_cells = 1;
}

/**
 * Register routes
 * */
void registerRoutes() {
    addRoute("/main", main_run);
}

/**
 * Look for route and call matched route. Return Null otherwise
 * */
NeroWindow *matchRoute(const char *route) {
    for (int i = 0; i < used_cells; i++) {
        if (strcasecmp(route, routePaths[i]) == 0) {
            return routeControllers[i]();
        }
    }

    errx(1, "Can't find route");
}
