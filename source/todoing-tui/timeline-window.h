#ifndef __TIMELINE_WINDOW__H__
#define __TIMELINE_WINDOW__H__

#include <stdint.h>
#include <todoing-txt.h>
#include "todoing-nc-window.h"

struct TimelineWindowData{
    WindowData window;
    struct TimeStamp now;
    uint16_t step;
};

void timeline_window_create(struct TimelineWindowData *w);
void timeline_window_draw(struct TimelineWindowData *w);
uint8_t timeline_window_update(struct TimelineWindowData *w);
void timeline_window_handle_input(struct TimelineWindowData *w, int cmd);

#endif
