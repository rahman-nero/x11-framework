#include <X11/X.h>
#include <X11/Xft/Xft.h>

#ifndef FIRST_PROJECT_UTILS_H
#define FIRST_PROJECT_UTILS_H

Window create_main_window(
        const int x,
        const int y,
        const int width,
        const int height,
        const int border,
        const int masks
);

/**
 * Create sub-window from parent
 * */
Window create_sub_window(
        const Window *parent,
        const int x,
        const int y,
        const int width,
        const int height,
        const int border,
        const int masks,
        const unsigned long background_hex
);

/**
 * Change window's background.
 *
 * TODO: It has a memory leak

 * @param Window* window
 * @param char* hex
 * @return void
 * */
void change_window_background(const Window *window, const char *hex);


/**
 * Create Graphic Context
 * */
GC create_gc(const Window *window);

#endif //FIRST_PROJECT_UTILS_H
