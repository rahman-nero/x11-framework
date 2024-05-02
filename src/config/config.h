#include <X11/X.h>
#include <X11/Xlib.h>

#define BORDER 0
#define TITLE "Paint program"

typedef struct {
    // Display server connection
    Display *dpy;

    // Screen number
    int scr;

    // Root window
    Window root;

    // Visual
    Visual *vis;

    // Main Win
    Window mainWin;

    // Main Graphic context
    GC gc;
} Config;
