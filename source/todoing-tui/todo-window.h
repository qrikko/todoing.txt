#ifndef __TODO_WINDOW__H__
#define __TODO_WINDOW__H__

#include <stdint.h>
#include "todoing-nc-window.h"

struct TodoList;

struct TodoWindowData{
    WindowData window;
    uint16_t scroll_offset;
    uint16_t active_idx;
    uint16_t visible_rows;
    uint8_t dirty;
};

void todo_window_create(struct TodoWindowData *w);
void todo_window_draw(struct TodoWindowData *w, const struct TodoList *l);
uint8_t todo_window_update(struct TodoWindowData *w, int cmd, int list_height);

#endif
