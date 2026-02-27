#include "text_buffer.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

TextBuffer *tb_create(size_t initial_capacity) {
	if (initial_capacity < TB_MIN_CAPACITY)
		initial_capacity = TB_MIN_CAPACITY;
	TextBuffer *buffer = malloc(sizeof(*buffer));
	buffer->length = 0;
	buffer->capacity = initial_capacity;
	buffer->text = malloc(buffer->capacity + 1);
	buffer->text[0] = '\0'; // null-terminate the string, when we add characters we will
				// overwrite this null terminator and add a new one at the end
	return buffer;
}

/*
 * Create a new text buffer initialized with the given string. The text should be a null-terminated string.
 * The text buffer will automatically resize if necessary to accommodate the initial text.
 */
TextBuffer *tb_create_from_string(const char *text) {
	size_t text_len = strlen(text);
	size_t capacity = TB_MIN_CAPACITY;
	while (capacity < text_len + 1) { // +1 for null terminator
		capacity *= 2;
	}
	TextBuffer *buffer = malloc(sizeof(*buffer));
	buffer->length = text_len;
	buffer->capacity = capacity;
	buffer->text = malloc(buffer->capacity);
	memcpy(buffer->text, text, text_len + 1); // +1 to copy the null terminator
	return buffer;
}

void tb_free(TextBuffer *tb) {
	free(tb->text);
	tb->text = NULL;
	tb->length = 0;
	tb->capacity = 0;
	free(tb);
}

void tb_append(TextBuffer *tb, const char *text) {
	size_t text_len = strlen(text);
	// reallocate if necessary (+1 for null terminator)
	if (tb->length + text_len + 1 > tb->capacity) {
		while (tb->length + text_len + 1 > tb->capacity) {
			tb->capacity *= 2;
		}
		tb->text = realloc(tb->text, tb->capacity);
	}

	// append the new text
	// we can use memcpy here since we know the length of the text and we will add
	// a null terminator at the end
	memcpy(tb->text + tb->length, text, text_len);
	tb->length += text_len;
	tb->text[tb->length] = '\0'; // null-terminate the string
}

void tb_insert_at(TextBuffer *tb, const char *text, size_t index) {
	if (index > tb->length) {
		index = tb->length; // insert at the end if index is out of bounds
	}
	size_t text_len = strlen(text);
	// reallocate if necessary (+1 for null terminator)
	if (tb->length + text_len + 1 > tb->capacity) {
		while (tb->length + text_len + 1 > tb->capacity) {
			tb->capacity *= 2;
		}
		tb->text = realloc(tb->text, tb->capacity);
	}

	// move the existing text to make room for the new text
	memmove(tb->text + index + text_len, tb->text + index,
		tb->length - index + 1); // +1 to move the null terminator
	// copy the new text into the buffer
	memcpy(tb->text + index, text, text_len);
	tb->length += text_len;
}

void tb_delete_at(TextBuffer *tb, size_t index, size_t length) {
	if (index >= tb->length) {
		return; // index out of bounds, nothing to delete
	}
	if (index + length > tb->length) {
		length =
		    tb->length - index; // adjust length to delete only up to the end of the buffer
	}

	// move the text after the deleted portion to fill the gap
	memmove(tb->text + index, tb->text + index + length,
		tb->length - index - length + 1); // +1 to move the null terminator
	tb->length -= length;
}
