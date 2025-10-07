#ifndef __DAY_WINDOW__H__
#define __DAY_WINDOW__H__

#include <stdint.h>
#include "todoing-nc-window.h"

struct TodoList;

struct DayWindowData{
    WindowData window;
    uint16_t scroll_offset;
    uint16_t active_idx;
    uint16_t visible_rows;
};

void day_window_create(struct DayWindowData *w);
void day_window_draw(struct DayWindowData *w, const struct TodoList *l);
void day_window_handle_input(struct DayWindowData *w, int list_height, int cmd);
uint8_t day_window_update(struct DayWindowData *w);

#endif
