#include <stdio.h>
#include <stdlib.h>
#include "todoing-txt.h"

void init() {
    char *home = getenv("HOME");
    char *rel = "Nextcloud/todo.txt/todo.txt";

    char path[256];
    sprintf(path, "%s/%s", home, rel);

    TodoList todo_list;
    todo_create_list(&todo_list, path);

    system("clear");
    int i=0;
    while(1) {
        printf("\nThere are %d entries\nChoose which to show (-1 to exit): ", todo_list.entry_count);
        scanf("%d", &i);

        system("clear");
        printf("\n\n");

        if(i==-1) {
            break;
        }
        todo_print_entry(&todo_list.todos[i]);
    }
}
