#include "document.h"
#include "text_buffer.h"
#include <stdlib.h>

// typedef struct {
// 	TextBuffer **text_buffer;
// 	size_t length; /* The number of lines in the document */
// 	size_t capacity;
// 	CursorPos cursor; /* The current cursor position in the document */
// } Document;
//
// typedef struct {
// 	int row;
// 	int col;
// } CursorPos;

Document *doc_create(size_t initial_capacity) {
	if (initial_capacity < DOC_MIN_CAPACITY)
		initial_capacity = DOC_MIN_CAPACITY;

	Document *doc = malloc(sizeof(*doc));
	if (!doc)
		return NULL;

	doc->capacity = initial_capacity;
	doc->length = 0;

	// Allocate memory for the array of text buffers (lines)
	doc->text_buffer = malloc(sizeof(TextBuffer *) * doc->capacity);
	if (!doc->text_buffer) {
		free(doc);
		return NULL;
	}

	// Initialize the text buffer pointers to NULL
	for (size_t i = 0; i < doc->capacity; i++) {
		doc->text_buffer[i] = NULL;
	}

	// Set the initial cursor position to the beginning of the document
	doc->cursor.row = 0;
	doc->cursor.col = 0;

	doc->text_buffer[0] = tb_create(TB_MIN_CAPACITY);
	doc->length = 1;

	return doc;
}

/*
 * Free the memory used by the document and all its text buffers. This should be called when the
 * document is no longer needed to avoid memory leaks. After calling this function, the document
 * pointer should not be used again.
 */
void doc_free(Document *doc) {
	if (!doc)
		return;

	// Free each text buffer (line) in the document
	for (size_t i = 0; i < doc->length; i++) {
		if (doc->text_buffer[i]) {
			tb_free(doc->text_buffer[i]);
			doc->text_buffer[i] = NULL;
		}
	}

	// Free the array of text buffer pointers
	free(doc->text_buffer);
	doc->text_buffer = NULL;

	// Free the document itself
	free(doc);
}

void doc_insert_text(Document *doc, const char *text) {
	tb_insert_at(doc->text_buffer[doc->cursor.row], text, doc->cursor.col);
	doc->cursor.col += strlen(text);
}

void doc_delete_text(Document *doc) {
	if (doc->cursor.col > 0) {
		doc->cursor.col--;
		tb_delete_at(doc->text_buffer[doc->cursor.row], doc->cursor.col, 1);
	} else if (doc->cursor.row > 0) {
		// If we're at the beginning of a line, we can merge it with the previous line
		size_t prev_line_length = doc->text_buffer[doc->cursor.row - 1]
					      ? doc->text_buffer[doc->cursor.row - 1]->length
					      : 0;
		size_t current_line_length = doc->text_buffer[doc->cursor.row]
						 ? doc->text_buffer[doc->cursor.row]->length
						 : 0;

		// Merge the current line into the previous line
		tb_insert_at(doc->text_buffer[doc->cursor.row - 1],
			     doc->text_buffer[doc->cursor.row]->text, prev_line_length);

		// Delete the current line
		tb_free(doc->text_buffer[doc->cursor.row]);
		doc->text_buffer[doc->cursor.row] = NULL;

		// Shift all lines after the current line up by one
		for (size_t i = doc->cursor.row; i < doc->length - 1; i++) {
			doc->text_buffer[i] = doc->text_buffer[i + 1];
		}
		doc->text_buffer[doc->length - 1] = NULL;
		doc->length--;

		// Move the cursor to the end of the previous line
		doc->cursor.row--;
		doc->cursor.col = prev_line_length;
	}
}

void doc_new_line(Document *doc) {
	TextBuffer *current_line = doc->text_buffer[doc->cursor.row];
	size_t current_line_length = current_line ? current_line->length : 0;

	// Create a new line
	TextBuffer *new_line = tb_create(TB_MIN_CAPACITY);
	if (!new_line)
		return;

	// If the cursor is not at the end of the current line, split the line
	if (doc->cursor.col < current_line_length) {
		tb_insert_at(new_line, current_line->text + doc->cursor.col,
			     current_line_length - doc->cursor.col);
		tb_delete_at(current_line, doc->cursor.col,
			     current_line_length - doc->cursor.col);
	}

	// Shift all lines after the current line down by one
	for (size_t i = doc->length; i > doc->cursor.row + 1; i--) {
		doc->text_buffer[i] = doc->text_buffer[i - 1];
	}
	doc->text_buffer[doc->cursor.row + 1] = new_line;
	doc->length++;

	// Move the cursor to the beginning of the new line
	doc->cursor.row++;
	doc->cursor.col = 0;
}

void cursor_move_up(Document *doc) {
	if (doc->cursor.row > 0) {
		doc->cursor.row--;
		size_t line_length = doc->text_buffer[doc->cursor.row]
					 ? doc->text_buffer[doc->cursor.row]->length
					 : 0;
		if (doc->cursor.col > line_length) {
			doc->cursor.col =
			    line_length; // Move to the end of the line if current column exceeds it
		}
	}
}
void cursor_move_down(Document *doc) {
	if (doc->cursor.row + 1 < doc->length) {
		doc->cursor.row++;
		size_t line_length = doc->text_buffer[doc->cursor.row]
					 ? doc->text_buffer[doc->cursor.row]->length
					 : 0;
		if (doc->cursor.col > line_length) {
			doc->cursor.col =
			    line_length; // Move to the end of the line if current column exceeds it
		}
	}
}
void cursor_move_left(Document *doc) {
	if (doc->cursor.col > 0) {
		doc->cursor.col--;
	}
}
void cursor_move_right(Document *doc) {
	size_t line_length =
	    doc->text_buffer[doc->cursor.row] ? doc->text_buffer[doc->cursor.row]->length : 0;
	if (doc->cursor.col < line_length) {
		doc->cursor.col++;
	}
}
