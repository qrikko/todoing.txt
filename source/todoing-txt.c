#include "todoing-txt.h"
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t is_date(const char *v) {
    return 
        strlen(v)==10 &&
        isdigit(v[0]) && isdigit(v[1]) && isdigit(v[2]) && isdigit(v[3]) &&
        v[4] == '-' &&
        isdigit(v[5]) && isdigit(v[6]) &&
        v[7] == '-' &&
        isdigit(v[8]) && isdigit(v[9])
    ;
}
static Date parse_date(const char* date) {
    Date d = {0};
    sscanf(date, "%hd-%hd-%hd", &d.year, &d.month, &d.day);
    return d;
}
static void add_tag(TagList *list, const char *tag) {
    list->items = realloc(list->items, sizeof(char*) * (list->count+1));
    list->items[list->count++] = strdup(tag);
}
static void add_metadata(TodoEntry *todo, const char *kv) {
    char *sep = strchr(kv, ':');
    if(!sep) { return; }
    *sep = '\0';
    todo->metadata[todo->metadata_count].key = strdup(kv);
    todo->metadata[todo->metadata_count].value = strdup(sep+1);
    todo->metadata_count++;
}

void todo_print_entry(const TodoEntry *t) {
    printf("Completed: %d\n", t->done);
    printf("Priority: %c\n", t->priority ? t->priority : '_');
    if(t->completion_date.year) {
        printf("Completion date: %04d-%02d-%02d\n",
            t->completion_date.year, 
            t->completion_date.month, 
            t->completion_date.day
        );
    }
    if(t->creation_date.year) {
        printf("Creation date: %04d-%02d-%02d\n",
            t->creation_date.year, 
            t->creation_date.month, 
            t->creation_date.day
        );
    }
    printf("Description: %s\n", t->desc);

    printf("Project tags: [");
    for(size_t i=0; i<t->project_tags.count-1; i++) {
        printf("+%s, ", t->project_tags.items[i]);
    }
    printf("+%s]\n", t->project_tags.items[t->project_tags.count-1]);

    printf("Context tags: [");
    for(size_t i=0; i<t->context_tags.count-1; i++) {
        printf("@%s, ", t->context_tags.items[i]);
    }
    printf("@%s]\n", t->context_tags.items[t->context_tags.count-1]);

    printf("Metadata: {\n");
    for(size_t i=0; i<t->metadata_count; i++) {
        printf("\t%s=%s\n", t->metadata[i].key, t->metadata[i].value);
    }
    printf("}\n");
}


void todo_create_list(TodoList* list, const char *path) {
    FILE *f = fopen(path, "r");
    if(f == NULL) {
        printf("Error: Could not open file '%s' for reading!", path);
    }

    char *line;
    size_t len;
    ssize_t read;
    while((read = getline(&line, &len, f) != -1)) {
        TodoEntry todo = {0};
        char *copy = strdup(line);
        char *token = strtok(copy, " ");

        int stage = 0;
        char desc_buff[512] = {0};
        size_t desc_len = 0;

        while(token) {
            if(stage == 0 && (strcmp(token, "x") == 0 || strcmp(token, "X") == 0)) {
                todo.done = 1;
            } else if(stage<=1 && token[0]=='(' && token[2]==')' && isalpha(token[1])) {
                todo.priority = token[1];
            } else if(is_date(token)) {
                if(todo.completion_date.year == 0) {
                    todo.completion_date = parse_date(token);
                } else {
                    todo.creation_date = parse_date(token);
                }
            } else if(token[0] == '+') {
                add_tag(&todo.project_tags, token+1);
            } else if(token[0] == '@') {
                add_tag(&todo.context_tags, token+1);
            } else if(strchr(token, ':')) {
                add_metadata(&todo, token);
            } else {
                if(desc_len+strlen(token)+2 < sizeof(desc_buff)) {
                    strcat(desc_buff, token);
                    strcat(desc_buff, " ");
                    desc_len+=strlen(token)+1;
                }
            }

            token = strtok(NULL, " ");
            stage++;
        }

        todo.desc = strdup(desc_buff);
        free(copy);

        list->todos = realloc(list->todos, sizeof(TodoEntry)*(list->entry_count+1));
        memcpy(&list->todos[list->entry_count++], &todo, sizeof(todo));
    }
    fclose(f);
    if(line) {
        free(line);
    }
}
int main(int argc, char **argv) {
    init();

    return 0;
}
