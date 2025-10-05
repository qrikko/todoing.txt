#include "timeline-window.h"
#include <ncurses.h>

#define MINUTES_PER_HOUR 1440;

void timeline_window_draw(struct TimelineWindowData *w) {
    int maxw = getmaxx(w->window.nc_window) - 4;
    mvwprintw(w->window.nc_window, 0, 2, "11:02.22");
    for(int i=0; i<10; i++) {
        mvwprintw(w->window.nc_window, i+1, 2, "%02d:%02d", i+7, 0);
    }
    if(w->window.active) {
        wattron(w->window.nc_window, A_REVERSE);
        box(w->window.nc_window, 0, 0);
        wattroff(w->window.nc_window, A_REVERSE);
    } else {
        box(w->window.nc_window, 0, 0);
    }
    wrefresh(w->window.nc_window);
}
void timeline_window_handle_input(struct TimelineWindowData *w, int cmd) {
}
uint8_t timeline_window_update(struct TimelineWindowData *w) {
    if(w->window.dirty) {
        w->window.dirty = 0;
        return 1;
    }
    return 0;
}
void timeline_window_create(struct TimelineWindowData *w) {
    todoing_nc_window_create(&w->window);
    w->window.dirty = 1;
};
