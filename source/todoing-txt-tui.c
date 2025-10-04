#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "todoing-txt.h"

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

void todo_window_draw(WINDOW *w, const TodoList *l, int scroll_offset, int cursor_index, int h) {
        for(size_t i=0; i<h; i++) {
            int item_idx = scroll_offset+i;
            if(item_idx>=l->entry_count) { 
                break; 
            }

            int y = 1+i;

            int maxw = getmaxx(w) - 4;
            if(item_idx==cursor_index) {
                wattron(w, A_REVERSE);
                char buff[512];
                todo_print_entry(&l->todos[item_idx], buff);
                mvwprintw(w, y, 2, "%.*s", maxw, buff);
                wattroff(w, A_REVERSE);
            } else {
                char buff[512];
                todo_print_entry(&l->todos[item_idx], buff);
                mvwprintw(w, y, 2, "%.*s", maxw, buff);
            }
        }
        box(w, 0, 0);
        wrefresh(w);
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
    int scroll_offset = 0;
    int active_idx = 0;
    int height = 10;

    WINDOW *todo_win;
    {
        int width = xmax - 2;
        int top = 0;
        int left = 1;

        todo_win = newwin(height, width, top, left);
    }
    int visible_rows = height-2;
    todo_window_draw(todo_win, &todo_list, scroll_offset, active_idx, visible_rows);

    uint8_t keep_running = 0x1;
    uint8_t needs_redraw = 0x0;
    while (keep_running == 0x1) {
        if (needs_redraw) {
            werase(todo_win);
            box(todo_win, 0,0);
            wrefresh(todo_win);
            todo_window_draw(todo_win, &todo_list, scroll_offset, active_idx, visible_rows);
            needs_redraw = 0x0;
        }

        int cmd = wgetch(todo_win);
        switch(cmd) {
            case 'j':
            {
                if(active_idx < todo_list.entry_count-1) {
                    active_idx++;
                    if(active_idx >= scroll_offset+visible_rows) {
                        scroll_offset++;
                    }
                    needs_redraw = 1;
                }
                break;
            }
            case 'k':
            {
                if(active_idx > 0) {
                    active_idx --;
                    if(active_idx < scroll_offset) {
                        scroll_offset--;
                    }
                    needs_redraw = 1;
                }
                break;
            }
            case 'q': 
            {
                keep_running = 0x0;
                break;
            }
            case KEY_RESIZE: 
            {
                getmaxyx(stdscr, ymax, xmax);
                //wresize(todo_win, ymax/2, xmax-20);
                //mvwin(todo_win, ymax/4, 10);
                needs_redraw = true;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    delwin(todo_win);
    todo_destroy_list(&todo_list);
    endwin();
    return 0;
}
