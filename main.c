#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "SDL3/SDL_keycode.h"
#include "document.h"
#include "text_buffer.h"

// TODO: add cursor position, selection, etc. and render them

int main(int argc, char *argv[]) {
	float cursor_x;
	float cursor_y;
	Document *doc = doc_create(100);

	SDL_Init(SDL_INIT_VIDEO);
	if (!TTF_Init()) {
		SDL_Log("Failed to initialize SDL_ttf: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Editor", 1024, 768, SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	TTF_TextEngine *text_engine = TTF_CreateRendererTextEngine(renderer);
	SDL_Log("text_engine: %p", (void *)text_engine);

	TTF_Font *font = TTF_OpenFont("fonts/ttf/JetBrainsMono-Medium.ttf", 32);
	SDL_Log("font: %p (%s)", (void *)font, font ? "ok" : SDL_GetError());

	int line_height = TTF_GetFontLineSkip(font);
	int font_width;
	int font_height;
	TTF_GetStringSize(font, "T", 1, &font_width, &font_height);

	TTF_Text *ttf_text = TTF_CreateText(text_engine, font, "Hello world", 0);
	SDL_Log("ttf_text: %p (%s)", (void *)ttf_text, ttf_text ? "ok" : SDL_GetError());

	TTF_SetTextColor(ttf_text, 255, 255, 255, 255);
	bool running = true;
	SDL_Event event;
	SDL_StartTextInput(window);
	SDL_TextInputEvent text; /**< Text input event data */
	while (running) {
		// 1. Poll all pending events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				running = false;

			if (event.type == SDL_EVENT_TEXT_INPUT)
				doc_insert_text(doc, event.text.text);

			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_BACKSPACE)
				doc_delete_text(doc);

			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RETURN)
				doc_new_line(doc);

			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_TAB)
				doc_insert_text(doc, "    ");

			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_UP)
				cursor_move_up(doc);
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_LEFT)
				cursor_move_left(doc);
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RIGHT)
				cursor_move_right(doc);
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_DOWN)
				cursor_move_down(doc);
		}

		// 2. Render frame
		// loop through doc: TTF_SetTextString(ttf_text, tb->text, tb->length);
		SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (size_t i = 0; i < doc->length; i++) {
			TTF_SetTextString(ttf_text, doc->text_buffer[i]->text,
					  doc->text_buffer[i]->length);
			TTF_DrawRendererText(ttf_text, 0, i * line_height);
		}

		cursor_x = doc->cursor.col * font_width;
		cursor_y = doc->cursor.row * line_height;

		// enable blending for transparency
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);  // semi-transparent white
		SDL_FRect cursor_rect = { cursor_x, cursor_y, font_width, line_height };
		SDL_RenderFillRect(renderer, &cursor_rect);

		SDL_RenderPresent(renderer);
	}

	// 3. Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	doc_free(doc);
	return 0;
}
