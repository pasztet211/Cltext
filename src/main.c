#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "parser.h"
#include "animations.h"
#include "info.h"
#include "fonts.h"

#define FONT_SIZE 14
#define LINE_SPACING 6

#define VERSION "v0.2.5"
#define EXTENSION_VER "v0.2.5"
SDL_Color base_text_color = {255,255,255,255};

int scroll_y = 0;
int content_height = 0;

char font_name[64] = "Arial";
int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));
    if (argc < 2) {
        printf("Usage: cltext <file.clt> or cltext [-v [extension | cltext]| --version [extension | cltext]] [-h |--help]\n");
        return 1;
    }

    if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)) {
        printf("Cltext %s\n", VERSION);
        printf("Cltext VS Code extension %s\n", EXTENSION_VER);
        return 0;
    }
    else if (argc == 3 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) && (strcmp(argv[2], "cltext") == 0 || strcmp(argv[2], "extension") == 0)) {
        if (strcmp(argv[2], "cltext") == 0) {
            printf("Cltext %s\n", VERSION);
        }
        else if (strcmp(argv[2], "extension") == 0) {
            printf("Cltext VS Code extension %s\n", EXTENSION_VER);
        }
        return 0;
    }
    else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        printf("Cltext %s\n\n", VERSION);
        printf("%s", info);
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
    SDL_Color bgcolor = {20, 20, 20, 255};
    SDL_Color text_color = {255, 255, 255, 255};
    char title[256] = "Cltext";

    int segment_count = parse_text(text, &segments, &bgcolor, &text_color, &base_text_color, &title, &font_name);

    int content_height = 20;

    for (int i = 0; i < segment_count; i++) {
        for (char *p = segments[i].text; *p != '\0'; p++) {
            if (*p == '\n') {
                content_height += FONT_SIZE + LINE_SPACING;
            }
        }
    }

    content_height += FONT_SIZE;

    if (content_height > 600)
        content_height += 20;

    if (segment_count < 0) {
        printf("Parser error.\n");
        free(text);
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        free_segments(segments, segment_count);
        free(text);
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        SDL_Quit();
        free_segments(segments, segment_count);
        free(text);
        return 1;
    }

    SDL_Surface *icon = SDL_LoadBMP("C:/Cltext/icon.bmp");

    if (!icon) {
        printf("Icon failed to load: %s\n", SDL_GetError());
    }

    SDL_Window *window = SDL_CreateWindow(
        title,
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
        free_segments(segments, segment_count);
        free(text);
        return 1;
    }

    if (icon != NULL) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
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
        free_segments(segments, segment_count);
        free(text);
        return 1;
    }

    Fonts fonts = {0};

    if (!load_fonts(&fonts, font_name)) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        free_segments(segments, segment_count);
        free(text);

        return 2;
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_MOUSEWHEEL) {
                scroll_y -= event.wheel.y * 40;
            }
        }

        float time = SDL_GetTicks() / 1000.0f;

        SDL_SetRenderDrawColor(
            renderer,
            bgcolor.r,
            bgcolor.g,
            bgcolor.b,
            bgcolor.a
        );

        SDL_RenderClear(renderer);

        int x = 20;
        int y = 20 - scroll_y;

        int max_scroll_y = content_height - 600;

        if (max_scroll_y < 0)
            max_scroll_y = 0;

        if (scroll_y < 0)
            scroll_y = 0;

        if (scroll_y > max_scroll_y)
            scroll_y = max_scroll_y;

        for (int render_pass = 0; render_pass < 2; render_pass++) {

            int outline_pass = (render_pass == 0);

            x = 20;
            y = 20 - scroll_y;

            for (int i = 0; i < segment_count; i++) {

                TTF_Font *font = fonts.normal;

                if (segments[i].style & STYLE_BIG) {
                    font = fonts.big_normal;

                    if (segments[i].style & STYLE_BOLD) {
                        font = fonts.big_bold;
                    }
                    else if (segments[i].style & STYLE_TILT) {
                        font = fonts.big_italic;
                    }
                }
                else if (segments[i].style & STYLE_SMALL) {
                    font = fonts.small_normal;

                    if (segments[i].style & STYLE_BOLD) {
                        font = fonts.small_bold;
                    }
                    else if (segments[i].style & STYLE_TILT) {
                        font = fonts.small_italic;
                    }
                }
                else {
                    font = fonts.normal;

                    if (segments[i].style & STYLE_BOLD) {
                        font = fonts.bold;
                    }
                    else if (segments[i].style & STYLE_TILT) {
                        font = fonts.italic;
                    }
                }

                int font_style = TTF_STYLE_NORMAL;

                if (segments[i].style & STYLE_BOLD) {
                    font_style |= TTF_STYLE_BOLD;
                }

                if (segments[i].style & STYLE_TILT) {
                    font_style |= TTF_STYLE_ITALIC;
                }

                TTF_SetFontStyle(font, font_style);

                char reversed_text[8192];
                char *text_start;

                if (segments[i].style & STYLE_REVERSE) {
                    int len = strlen(segments[i].text);

                    for (int j = 0; j < len; j++) {
                        reversed_text[j] = segments[i].text[len - 1 - j];
                    }

                    reversed_text[len] = '\0';
                    text_start = reversed_text;
                }
                else {
                    text_start = segments[i].text;
                }

                int character_index = 0;

                while (*text_start != '\0') {

                    if (*text_start == '\n') {
                        x = 20;
                        y += FONT_SIZE + LINE_SPACING;

                        int actual_y = y + scroll_y;

                        if (actual_y > content_height)
                            content_height = actual_y;

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
                    float shake_offset_x = 0.0f;
                    float shake_offset_y = 0.0f;
                    float glitch_offset_x = 0.0f;
                    float glitch_offset_y = 0.0f;
                    float angle = 0.0f;

                    if (segments[i].style & STYLE_BOUNCE) {
                        scale = wave_scale(
                            segments[i].bounce_amount,
                            time,
                            character_index
                        );
                    }

                    if (segments[i].style & STYLE_WAVE) {
                        height_b = bounce_height(
                            segments[i].wave_amount,
                            time,
                            character_index
                        );
                    }

                    if (segments[i].style & STYLE_SHAKE) {
                        shake_offset(
                            segments[i].shake_amount,
                            time,
                            character_index,
                            &shake_offset_x,
                            &shake_offset_y
                        );
                    }

                    if (segments[i].style & STYLE_SPIN) {
                        angle = spin_angle(
                            segments[i].spin_ammount,
                            time
                        );
                    }

                    if (segments[i].style & STYLE_GLITCH) {
                        glitch_offset(
                            &glitch_offset_x,
                            &glitch_offset_y,
                            &scale
                        );
                    }

                    SDL_Surface *surface =
                        TTF_RenderUTF8_Blended(
                            font,
                            character,
                            segments[i].color
                        );

                    SDL_Surface *outline_surface =
                        TTF_RenderUTF8_Blended(
                            font,
                            character,
                            (SDL_Color){255, 255, 255, 255}
                        );

                    if (surface == NULL || outline_surface == NULL) {
                        printf(
                            "Text rendering error: %s\n",
                            TTF_GetError()
                        );

                        if (surface != NULL)
                            SDL_FreeSurface(surface);

                        if (outline_surface != NULL)
                            SDL_FreeSurface(outline_surface);

                        break;
                    }

                    SDL_Texture *texture =
                        SDL_CreateTextureFromSurface(
                            renderer,
                            surface
                        );

                    SDL_Texture *outline_texture =
                        SDL_CreateTextureFromSurface(
                            renderer,
                            outline_surface
                        );

                    if (texture == NULL || outline_texture == NULL) {
                        printf(
                            "Texture creation error: %s\n",
                            SDL_GetError()
                        );

                        if (texture != NULL)
                            SDL_DestroyTexture(texture);

                        if (outline_texture != NULL)
                            SDL_DestroyTexture(outline_texture);

                        SDL_FreeSurface(surface);
                        SDL_FreeSurface(outline_surface);

                        break;
                    }

                    int width = (int)(surface->w * scale);
                    int height = (int)(surface->h * scale);

                    SDL_Rect rect = {
                        x + shake_offset_x + glitch_offset_x,
                        y + (surface->h - height) / 2
                            - (int)height_b
                            + shake_offset_y
                            + glitch_offset_y,
                        width,
                        height
                    };

                    /* OUTLINE PASS */
                    if (outline_pass &&
                        (segments[i].style & STYLE_OUTLINE)) {

                        SDL_SetTextureColorMod(
                            outline_texture,
                            segments[i].outline_color.r,
                            segments[i].outline_color.g,
                            segments[i].outline_color.b
                        );

                        int thickness = segments[i].outline_thickness;

                        for (int ox = -thickness; ox <= thickness; ox++) {
                            for (int oy = -thickness; oy <= thickness; oy++) {

                                if (ox == 0 && oy == 0)
                                    continue;

                                SDL_Rect outline_rect = rect;

                                outline_rect.x += ox;
                                outline_rect.y += oy;

                                SDL_RenderCopyEx(
                                    renderer,
                                    outline_texture,
                                    NULL,
                                    &outline_rect,
                                    angle,
                                    NULL,
                                    SDL_FLIP_NONE
                                );
                            }
                        }

                        SDL_SetTextureColorMod(
                            outline_texture,
                            255,
                            255,
                            255
                        );
                    }

                    /* TEXT PASS */
                    if (!outline_pass) {
                        SDL_RenderCopyEx(
                            renderer,
                            texture,
                            NULL,
                            &rect,
                            angle,
                            NULL,
                            SDL_FLIP_NONE
                        );
                    }

                    /*
                    * Advance by the ORIGINAL character width,
                    * not the scaled width.
                    */
                    x += surface->w;

                    SDL_DestroyTexture(texture);
                    SDL_DestroyTexture(outline_texture);

                    SDL_FreeSurface(surface);
                    SDL_FreeSurface(outline_surface);

                    text_start++;
                    character_index++;
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    TTF_SetFontStyle(fonts.normal, TTF_STYLE_NORMAL);
    free_fonts(&fonts);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    free_segments(segments, segment_count);
    free(text);

    return 0;
}