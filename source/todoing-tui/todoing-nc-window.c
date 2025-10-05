#include <ncurses.h>
#include "todoing-nc-window.h"

void todoing_nc_window_resize(WindowData *w, int y, int x) {
}
void todoing_nc_window_create(WindowData *w) {
    w->nc_window = newwin(w->height, w->width, w->top, w->left);
}
