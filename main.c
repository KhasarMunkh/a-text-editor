#include "text_buffer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

// TODO: add cursor position, selection, etc. and render them

int main(int argc, char *argv[]) {
    TextBuffer *lines[101];

    TextBuffer *tb = tb_create(100);

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
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_TEXT_INPUT) {
                tb_append(tb, event.text.text);
                TTF_SetTextString(ttf_text, tb->text, tb->length);
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_BACKSPACE && tb->length > 0) {
                tb->text[tb->length - 1] = '\0';
                tb->length--;
                TTF_SetTextString(ttf_text, tb->text, tb->length);
            }

        }

        // 2. Render frame
        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        TTF_DrawRendererText(ttf_text, 0, 0);
        SDL_RenderPresent(renderer);
    }

    // 3. Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    tb_free(tb);
    return 0;
}
