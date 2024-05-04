#include <stdio.h>
#include <stdlib.h>
#include "../config/config.h"


#ifndef NERO_CONFIG_ROUTE_H
#define NERO_CONFIG_ROUTE_H

void registerRoutes();

NeroWindow* matchRoute(const char *route);

#endif