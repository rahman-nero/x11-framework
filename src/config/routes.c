#include <string.h>
#include <err.h>
#include "../pages/index.h"
#include "../config/config.h"

typedef NeroWindow *(*FunctionCallback)();

const char *routePaths[10];
FunctionCallback routeControllers[10];
size_t routeCount = 0;

/**
 * Function to add route to route list
 * @param path
 * @param Callback callback
 * @return void
 * */
void addRoute(char *path, NeroWindow *(*callback)()) {
    routePaths[routeCount] = path;
    routeControllers[routeCount] = callback;
    routeCount += 1;
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
    for (int i = 0; i < routeCount; i++) {
        if (strcasecmp(route, routePaths[i]) == 0) {
            return routeControllers[i]();
        }
    }

    errx(1, "Can't find route");
}
