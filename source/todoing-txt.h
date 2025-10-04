#ifndef __TODOING__TXT__H_
#define __TODOING__TXT__H_

#include <stdint.h>
#include <time.h>

typedef struct {
    time_t start;
    time_t duration;
} TimeBlock;

typedef struct {
    uint16_t year;
    uint16_t month;
    uint16_t day;
} Date;

typedef struct {
    char **items;
    size_t count;
} TagList;

typedef struct {
    char *key;
    char *value;
} KeyValue;

typedef struct {
    Date completion_date;
    Date creation_date;
    TagList project_tags;
    TagList context_tags;
    KeyValue *metadata;
    uint8_t metadata_count;
    char *desc;
    char done;
    char priority;
} TodoEntry;

typedef struct {
    char *file;
    TodoEntry *todos;
    uint8_t entry_count;
} TodoList;

extern void init();

void todo_create_list(TodoList* list, const char *path);
void todo_destroy_list(TodoList* list);

void todo_print_entry(const TodoEntry* t);

#endif
