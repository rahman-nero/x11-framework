#include <stdio.h>
#include <stdlib.h>
#include "../window.h"

#ifndef NERO_CONFIG_ROUTE_H
#define NERO_CONFIG_ROUTE_H

void registerRoutes();

NeroWindow* matchRoute(const char *route);

#endif