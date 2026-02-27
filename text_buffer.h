#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *text;
    size_t length;
    size_t capacity;
} TextBuffer;

TextBuffer *tb_create(size_t initial_capacity);

/*
 * Free the memory used by the text buffer. This should be called when the text buffer is no longer
 * needed to avoid memory leaks. After calling this function, the text buffer pointer should not be
 * used again.
 */
void tb_free(TextBuffer *tb);

/*
 * Append the given text to the end of the text buffer. The text buffer will automatically resize
 * if necessary to accommodate the new text. The text should be a null-terminated string.
 */
void tb_append(TextBuffer *tb, const char *text);

/*
 * Insert the given text at the specified index in the text buffer.
 * The text buffer will automatically resize if necessary to accommodate the new text.
 * The text should be a null-terminated string.
 * If the index is greater than the current length of the text buffer,
 * the text will be appended to the end.
 */
void tb_insert_at(TextBuffer *tb, const char *text, size_t index);

/*
 * Delete a portion of the text buffer starting at the specified index and spanning the specified length.
 * If the index is greater than the current length of the text buffer, no action will be taken.
 * If the length extends beyond the end of the text buffer, it will be truncated to delete only up to the end.
 */
void tb_delete_at(TextBuffer *tb, size_t index, size_t length);

#endif // TEXT_BUFFER_H
