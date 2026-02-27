#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "text_buffer.h"

/* A document consists of multiple lines of text, each represented by a TextBuffer. The document
 * also tracks the current cursor position (c_row and c_col) to allow for editing operations. The
 * document can be extended to include additional features such as selection, undo/redo history, etc
 */
typedef struct {
    TextBuffer **text_buffer; // Array of pointers to TextBuffer, each representing a line of text
    size_t length; /* The number of lines in the document */
    size_t capacity;
    int c_row;
    int c_col;
} Document;

#endif // DOCUMENT_H
