#ifndef PARSER_H
#define PARSER_H

#include <SDL2/SDL.h>

typedef enum {
    STYLE_NORMAL = 0,
    STYLE_BOLD   = 1 << 0,
    STYLE_TILT   = 1 << 1,
    STYLE_WAVE   = 1 << 2,
    STYLE_BOUNCE = 1 << 3,
    STYLE_SHAKE  = 1 << 4,
    STYLE_BIG    = 1 << 5,
    STYLE_SMALL  = 1 << 6
} TextStyle;

typedef struct {
    char *text;
    TextStyle style;

    float wave_amount;
    float bounce_amount;
    float shake_amount;

    SDL_Color color;
} TextSegment;

int parse_text(const char *input, TextSegment **segments, SDL_Color *bgcolor, SDL_Color *text_color);

void free_segments(TextSegment *segments, int count);

SDL_Color hex_to_sdl_color(const char *hex);

#endif