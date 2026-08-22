#ifndef FONTS_H
#define FONTS_H

#include <SDL2/SDL_ttf.h>

typedef struct {
    TTF_Font *normal;
    TTF_Font *bold;
    TTF_Font *italic;

    TTF_Font *big_normal;
    TTF_Font *big_bold;
    TTF_Font *big_italic;

    TTF_Font *small_normal;
    TTF_Font *small_bold;
    TTF_Font *small_italic;
} Fonts;

int load_fonts(Fonts *fonts,const char name[64]);
void free_fonts(Fonts *fonts);

#endif