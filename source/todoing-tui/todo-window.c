#include "todo-window.h"

#include <ncurses.h>
#include <stdint.h>
#include <todoing-txt.h>
#include <stddef.h>

typedef struct TodoList TodoList;
typedef struct TodoWindowData TodoWindowData;

uint8_t todo_window_update(struct TodoWindowData *w, int cmd, int list_height) {
    switch (cmd) {
        case 'j':
        {
            if(w->active_idx < list_height) {
                w->active_idx++;
                if(w->active_idx >= w->scroll_offset+w->visible_rows) {
                    w->scroll_offset++;
                }
                w->dirty = 1;
            }
            break;
        }
        case 'k':
        {
            if(w->active_idx > 0) {
                w->active_idx --;
                if(w->active_idx < w->scroll_offset) {
                    w->scroll_offset--;
                }
                w->dirty = 1;
            }
            break;
        }
    }

    if (w->dirty) {
        werase(w->window.nc_window);
        box(w->window.nc_window, 0,0);
        wrefresh(w->window.nc_window);
        w->dirty = 0x0;
        return 1;
    }
    return 0;
}
void todo_window_draw(TodoWindowData *w, const TodoList *l) {
    for(size_t i=0; i<w->visible_rows; i++) {
        int item_idx = w->scroll_offset+i;
        if(item_idx>=l->entry_count) { 
            break; 
        }

        int y = 1+i;

        int maxw = getmaxx(w->window.nc_window) - 4;
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
    box(w->window.nc_window, 0, 0);
    wrefresh(w->window.nc_window);
}
void todo_window_create(TodoWindowData *w) {
    todoing_nc_window_create(&w->window);
    w->dirty = 1;
};

