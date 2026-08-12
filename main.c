#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define FONT_SIZE 14
#define LINE_SPACING 6

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: cltext <file.clt>\n");
        return 1;
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

    TTF_Font *font = TTF_OpenFont(
        "C:\\Windows\\Fonts\\arial.ttf",
        FONT_SIZE
    );

    if (font == NULL) {
        printf("Font error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        free(text);
        return 1;
    }

    SDL_Color white = {255, 255, 255, 255};

    int line_count = 1;

    for (size_t i = 0; i < length; i++) {
        if (text[i] == '\n') {
            line_count++;
        }
    }

    SDL_Texture **textures =
        malloc(sizeof(SDL_Texture *) * line_count);

    SDL_Rect *rects =
        malloc(sizeof(SDL_Rect) * line_count);

    if (textures == NULL || rects == NULL) {
        printf("Memory allocation failed.\n");

        free(textures);
        free(rects);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        free(text);

        return 1;
    }

    int line = 0;
    char *start = text;

    for (size_t i = 0; i <= length; i++) {
        if (text[i] == '\n' || text[i] == '\0') {

            char saved = text[i];
            text[i] = '\0';

            // Remove Windows CR from CRLF newlines
            if (i > 0 && text[i - 1] == '\r') {
                text[i - 1] = '\0';
            }

            SDL_Surface *surface =
                TTF_RenderUTF8_Blended(font, start, white);

            if (surface == NULL) {
                printf("Text rendering error: %s\n", TTF_GetError());

                for (int j = 0; j < line; j++) {
                    SDL_DestroyTexture(textures[j]);
                }

                free(textures);
                free(rects);
                TTF_CloseFont(font);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                TTF_Quit();
                SDL_Quit();
                free(text);

                return 1;
            }

            textures[line] =
                SDL_CreateTextureFromSurface(renderer, surface);

            rects[line].x = 20;
            rects[line].y =
                20 + line * (FONT_SIZE + LINE_SPACING);
            rects[line].w = surface->w;
            rects[line].h = surface->h;

            SDL_FreeSurface(surface);

            line++;

            text[i] = saved;
            start = &text[i + 1];
        }
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < line_count; i++) {
            SDL_RenderCopy(
                renderer,
                textures[i],
                NULL,
                &rects[i]
            );
        }

        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < line_count; i++) {
        SDL_DestroyTexture(textures[i]);
    }

    free(textures);
    free(rects);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    free(text);

    return 0;
}