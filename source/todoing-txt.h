#ifndef __TODOING__TXT__H_
#define __TODOING__TXT__H_

#include <stdint.h>
#include <time.h>

struct TimeBlock {
    time_t start;
    time_t duration;
};

struct Date {
    uint16_t year;
    uint16_t month;
    uint16_t day;
};

struct TagList {
    char **items;
    size_t count;
};

struct KeyValue {
    char *key;
    char *value;
};

struct TodoEntry {
    struct Date completion_date;
    struct Date creation_date;
    struct TagList project_tags;
    struct TagList context_tags;
    struct KeyValue *metadata;
    uint8_t metadata_count;
    char *desc;
    char done;
    char priority;
};

struct TodoList {
    char *file;
    struct TodoEntry *todos;
    uint8_t entry_count;
};


void todo_create_list(struct TodoList* list, const char *path);
void todo_destroy_list(struct TodoList* list);

void todo_print_entry(const struct TodoEntry* t, char *buff);

#endif
