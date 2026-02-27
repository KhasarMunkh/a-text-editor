#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *text;
    size_t length;
    size_t capacity;
} TextBuffer;

TextBuffer *tb_create(size_t initial_capacity);

void tb_free(TextBuffer *tb);

void tb_append(TextBuffer *tb, const char* text);



