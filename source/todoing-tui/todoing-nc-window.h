#ifndef __TODOING_NC_WINDOW__H__
#define __TODOING_NC_WINDOW__H__

#include <stdint.h>

typedef struct _win_st WINDOW;
typedef struct {
    WINDOW *nc_window;
    uint16_t top;
    uint16_t left;
    uint16_t width;
    uint16_t height;
} WindowData;

void todoing_nc_window_create(WindowData *w);
void todoing_nc_window_resize(WindowData *w, int y, int x);

#endif
