#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "text_buffer.h"

TextBuffer *tb_create(size_t initial_capacity) {
    TextBuffer *buffer = malloc(initial_capacity);
    buffer->length = 0;
    buffer->capacity = 16;
    buffer->text = malloc(buffer->capacity);
    buffer->text[0] = '\0'; // null-terminate the string, when we add characters we will overwrite
                            // this null terminator and add a new one at the end
    return buffer;
}

void tb_free(TextBuffer *tb) {
    free(tb->text);
    tb->text = NULL;
    tb->length = 0;
    tb->capacity = 0;
    free(tb);
}

void tb_append(TextBuffer *tb, const char* text) {
    size_t text_len = strlen(text);
    // reallocate if necessary (+1 for null terminator)
    if (tb->length + text_len + 1 > tb->capacity) {
        while (tb->length + text_len + 1 > tb->capacity) {
            tb->capacity *= 2;
        }
        tb->text = realloc(tb->text, tb->capacity);
    }

    // append the new text
    // we can use memcpy here since we know the length of the text and we will add a null terminator at the end
    memcpy(tb->text + tb->length, text, text_len);
    tb->length += text_len;
    tb->text[tb->length] = '\0'; // null-terminate the string
}



