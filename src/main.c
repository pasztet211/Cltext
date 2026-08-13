#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "parser.h"
#include "animations.h"

#define FONT_SIZE 14
#define LINE_SPACING 6

#define VERSION "v0.0.5" 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: cltext <file.clt>\n");
        return 1;
    }

    if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)) {
        printf("Cltext %s\n", VERSION);
        return 0;
    }

    FILE *file = fopen(argv[1], "rb");

    if (file == NULL) {
        printf("Could not open file: %s\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if (file_size < 0) {
        printf("Could not determine file size.\n");
        fclose(file);
        return 1;
    }

    char *text = malloc(file_size + 1);

    if (text == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    size_t length = fread(text, 1, file_size, file);
    text[length] = '\0';

    fclose(file);

    TextSegment *segments;
    int segment_count = parse_text(text, &segments);

    if (segment_count < 0) {
        printf("Parser error.\n");
        free(text);
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        free(text);
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        SDL_Quit();
        free(text);
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Cltext",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("Window error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        free(text);
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == NULL) {
        printf("Renderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        free(text);
        return 1;
    }

    TTF_Font *normal_font = TTF_OpenFont(
        "C:\\Windows\\Fonts\\arial.ttf",
        FONT_SIZE
    );

    TTF_Font *bold_font = TTF_OpenFont(
        "C:\\Windows\\Fonts\\arialbd.ttf",
        FONT_SIZE
    );

    TTF_Font *italic_font = TTF_OpenFont(
        "C:\\Windows\\Fonts\\ariali.ttf",
        FONT_SIZE
    );

    if (normal_font == NULL || bold_font == NULL || italic_font == NULL) {
        printf("Font error: %s\n", TTF_GetError());

        if (normal_font != NULL)
            TTF_CloseFont(normal_font);

        if (bold_font != NULL)
            TTF_CloseFont(bold_font);

        if (italic_font != NULL)
            TTF_CloseFont(italic_font);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        free(text);

        return 2;
    }

    SDL_Color white = {255, 255, 255, 255};

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        float time = SDL_GetTicks() / 1000.0f;

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        int x = 20;
        int y = 20;

        for (int i = 0; i < segment_count; i++) {

            TTF_Font *font;

            if (segments[i].style == STYLE_BOLD) {
                font = bold_font;
            }
            else if (segments[i].style == STYLE_TILT) {
                font = italic_font;
            }
            else {
                font = normal_font;
            }

            char *text_start = segments[i].text;
            int character_index = 0;

            while (*text_start != '\0') {

                /* Handle newlines */
                if (*text_start == '\n') {
                    x = 20;
                    y += FONT_SIZE + LINE_SPACING;
                    text_start++;
                    character_index = 0;
                    continue;
                }

                if (*text_start == '\r') {
                    text_start++;
                    continue;
                }


                char character[2];

                character[0] = *text_start;
                character[1] = '\0';

                float scale = 1.0f;
                float height_b = 0.0f;

                if (segments[i].style == STYLE_BOUNCE) {
                    scale = wave_scale(
                        segments[i].amount,
                        time,
                        character_index
                    );
                }
                else if (segments[i].style == STYLE_WAVE) {
                    height_b = bounce_height(
                        segments[i].amount,
                        time,
                        character_index
                    );
                }

                SDL_Surface *surface =
                    TTF_RenderUTF8_Blended(font, character, white);

                if (surface == NULL) {
                    printf(
                        "Text rendering error: %s\n",
                        TTF_GetError()
                    );
                    break;
                }

                SDL_Texture *texture =
                    SDL_CreateTextureFromSurface(
                        renderer,
                        surface
                    );

                int width = (int)(surface->w * scale);
                float height = (int)(surface->h * scale);

                SDL_Rect rect = {
                    x,
                    y + (surface->h - height) / 2 - height_b,
                    width,
                    height
                };

                SDL_RenderCopy(
                    renderer,
                    texture,
                    NULL,
                    &rect
                );

                /*
                * Advance by the ORIGINAL character width,
                * not the scaled width.
                */
                x += surface->w;

                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);

                text_start++;
                character_index++;
            }
        }
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(bold_font);
    TTF_CloseFont(normal_font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    free_segments(segments, segment_count);
    free(text);

    return 0;
}