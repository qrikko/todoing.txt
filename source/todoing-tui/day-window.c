#include "day-window.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <todoing-txt.h>
#include <stddef.h>

typedef struct TodoList TodoList;
typedef struct DayWindowData DayWindowData;

void day_window_handle_input(struct DayWindowData* w, int list_height, int cmd) {
}
uint8_t day_window_update(struct DayWindowData *w) {
    if (w->window.dirty) {
        werase(w->window.nc_window);
        box(w->window.nc_window, 0,0);
        wrefresh(w->window.nc_window);
        w->window.dirty = 0x0;
        return 1;
    }
    return 0;
}

void day_window_draw(DayWindowData *w, const TodoList *l) {
    for(int i=0; i<l->entry_count; i++) {
        struct TodoEntry *todo = &l->todos[i];
        for(int j=0; j<todo->metadata_count; j++) {
            struct KeyValue *kv = &todo->metadata[j];
            if(strcmp(kv->key, "start_datetime") == 0) {
                // figure out where it starts and offset drawing it
                // find duration metadata, and calculate and draw
            }
        }
    }
}

#ifdef COPY_OF_TODO
void day_window_draw(DayWindowData *w, const TodoList *l) {
    int maxw = getmaxx(w->window.nc_window) - 4;
    for(size_t i=0; i<w->visible_rows; i++) {
        int item_idx = w->scroll_offset+i;
        if(item_idx>=l->entry_count) { 
            break; 
        }

        int y = 1+i;

        if(item_idx==w->active_idx) {
            wattron(w->window.nc_window, A_REVERSE);
            char buff[512];
            todo_print_entry(&l->todos[item_idx], buff);
            mvwprintw(w->window.nc_window, y, 2, "%.*s", maxw, buff);
            wattroff(w->window.nc_window, A_REVERSE);
        } else {
            char buff[512];
            todo_print_entry(&l->todos[item_idx], buff);
            mvwprintw(w->window.nc_window, y, 2, "%.*s", maxw, buff);
        }
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
#endif
void day_window_create(DayWindowData *w) {
    todoing_nc_window_create(&w->window);
    w->window.dirty = 1;
};

