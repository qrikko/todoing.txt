#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "todoing-txt.h"
#include "todo-window.h"

typedef struct TodoList TodoList;
typedef struct TodoWindowData TodoWindowData;

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
    TodoList todo_list;
    {
        char *home = getenv("HOME");
        char *rel = "Nextcloud/todo.txt/todo.txt";

        char path[256];
        sprintf(path, "%s/%s", home, rel);

        todo_create_list(&todo_list, path);
    }

    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);

    TodoWindowData todo_window = {
        .window = {
            .height = 10,
            .width = xmax-2,
            .top = 0,
            .left = 1
        },
        .scroll_offset = 0,
        .active_idx = 0,
        .visible_rows = 8
    };

    todo_window_create(&todo_window);
    
    uint8_t keep_running = 0x1;
    while (keep_running == 0x1) {
        int cmd = wgetch(stdscr);

        if(todo_window_update(&todo_window, cmd, todo_list.entry_count-1)) {
            todo_window_draw(&todo_window, &todo_list);
        }
        switch(cmd) {
            case 'q': 
            {
                keep_running = 0x0;
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
