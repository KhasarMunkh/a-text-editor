#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "text_buffer.h"

#define DOC_MIN_CAPACITY 64

typedef struct {
	int row;
	int col;
} CursorPos;

/* A document consists of multiple lines of text, each represented by a TextBuffer. The document
 * also tracks the current cursor position (c_row and c_col) to allow for editing operations. The
 * document can be extended to include additional features such as selection, undo/redo history, etc
 */
typedef struct {
	TextBuffer **text_buffer;
	size_t length; /* The number of lines in the document */
	size_t capacity;
	CursorPos cursor; /* The current cursor position in the document */
} Document;

/*
 * Create a new document with the specified initial capacity for lines. The document will be
 * initialized with empty lines and the cursor position set to the beginning (0, 0). The caller
 * is responsible for freeing the memory used by the document when it is no longer needed.
 */
Document *doc_create(size_t initial_capacity);

/*
 * Free the memory used by the document and all its text buffers. This should be called when the
 * document is no longer needed to avoid memory leaks. After calling this function, the document
 * pointer should not be used again.
 */
void doc_free(Document *doc);

/*
 * Insert the given text at the current cursor position in the document. The text will be inserted
 * into the appropriate line based on the current cursor row (c_row) and column (c_col). If the
 * text contains newline characters, it will be split into multiple lines accordingly. After
 * insertion, the cursor position will be updated to reflect the new position after the inserted
 * text.
 */
void doc_insert_text(Document *doc, const char *text);

/*
 * Delete text at the current cursor position in the document. The deletion will affect the line
 * at the current cursor row and column. 
 */
void doc_delete_text(Document *doc);

/*
 * Insert a new line at the current cursor position in the document. The text after the cursor
 * position on the current line will be moved to the new line, and the cursor will be moved to
 * the beginning of the new line. If the current line is empty, a new empty line will be inserted
 * below it.
 */
void doc_new_line(Document *doc);

void cursor_move_up(Document *doc);
void cursor_move_down(Document *doc);
void cursor_move_left(Document *doc);
void cursor_move_right(Document *doc);

#endif // DOCUMENT_H
