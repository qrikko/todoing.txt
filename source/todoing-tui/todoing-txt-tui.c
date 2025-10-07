#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "todoing-txt.h"
#include "todo-window.h"
#include "timeline-window.h"
#include "day-window.h"

typedef struct TodoList TodoList;
typedef struct TodoWindowData TodoWindowData;
typedef struct TimelineWindowData TimelineWindowData;
typedef struct DayWindowData DayWindowData;

static void init_tui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(0);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_YELLOW, -1);
    init_pair(2, COLOR_RED, COLOR_BLACK);
}

int main(int argc, char **argv) {
    init_tui();

    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);

    TodoList todo_list;
    {
        char *home = getenv("HOME");
        char *rel = "Nextcloud/todo.txt/todo.txt";

        char path[256];
        sprintf(path, "%s/%s", home, rel);

        todo_create_list(&todo_list, path);
    }

    TodoWindowData todo_window = {
        .window = {
            .height = 10,
            .width = xmax-2,
            .top = 0,
            .left = 1,
            .active = 1
        },
        .scroll_offset = 0,
        .active_idx = 0,
        .visible_rows = 8
    };
    TimelineWindowData timeline_window = { 
        .window = {
            .height = ymax-todo_window.window.height,
            .width = 10,
            .top = todo_window.window.height,
            .left = 1,
            .active = 0
        },
        .step = 30 
    };
    DayWindowData day_window = {
        .window = {
            .height = ymax-todo_window.window.height,
            .width = xmax-timeline_window.window.width-2,
            .left = timeline_window.window.width+1,
            .top = todo_window.window.height
        },
        .scroll_offset = 0,
        .active_idx = 0,
        .visible_rows = 8
    };

    todo_window_create(&todo_window);
    timeline_window_create(&timeline_window);
    day_window_create(&day_window);

    WINDOW* focused = todo_window.window.nc_window;
    uint8_t keep_running = 0x1;
    while (keep_running == 0x1) {
        int cmd = wgetch(stdscr);

        if(todo_window.window.active==1) {
            todo_window_handle_input(&todo_window, todo_list.entry_count-1, cmd);
        } else if(timeline_window.window.active==1) {
            timeline_window_handle_input(&timeline_window, cmd);
        } else if(day_window.window.active==1) {
            day_window_handle_input(&day_window, todo_list.entry_count-1, cmd);
        }

        if(todo_window_update(&todo_window)) {
            todo_window_draw(&todo_window, &todo_list);
        }
        if(timeline_window_update(&timeline_window)) {
            timeline_window_draw(&timeline_window);
        }
        if(day_window_update(&day_window)) {
            day_window_draw(&day_window, &todo_list);
        }

        switch(cmd) {
            case 'q': 
            {
                keep_running = 0x0;
                break;
            }
            case '\t':
            {
                if(todo_window.window.active==1) {
                    todo_window.window.active = 0;
                    timeline_window.window.active = 1;
                    focused = timeline_window.window.nc_window;
                } else if(timeline_window.window.active==1) {
                    timeline_window.window.active = 0;
                    todo_window.window.active = 1;
                    focused = todo_window.window.nc_window;
                }
                todo_window.window.dirty = 1;
                timeline_window.window.dirty = 1;
                break;
            }
            case KEY_RESIZE: 
            {
                getmaxyx(stdscr, ymax, xmax);
                todoing_nc_window_resize(&todo_window.window, ymax, xmax);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    delwin(todo_window.window.nc_window);
    todo_destroy_list(&todo_list);
    endwin();
    return 0;
}
