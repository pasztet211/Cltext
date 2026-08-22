#include "fonts.h"
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <string.h>

#define FONT_SIZE 14

static int find_font(const char *name, char *path, size_t path_size)
{
    HKEY key;
    char font_name[256];
    DWORD font_name_size;
    DWORD type;
    char file_name[MAX_PATH];
    DWORD file_name_size;

    if (RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
        0,
        KEY_READ,
        &key
    ) != ERROR_SUCCESS) {
        return 0;
    }

    for (DWORD i = 0; ; i++) {
        font_name_size = sizeof(font_name);
        file_name_size = sizeof(file_name);

        if (RegEnumValueA(
            key,
            i,
            font_name,
            &font_name_size,
            NULL,
            &type,
            (BYTE *)file_name,
            &file_name_size
        ) != ERROR_SUCCESS) {
            break;
        }

        if (_strnicmp(font_name, name, strlen(name)) == 0) {
            snprintf(
                path,
                path_size,
                "C:\\Windows\\Fonts\\%s",
                file_name
            );

            RegCloseKey(key);
            return 1;
        }
    }

    RegCloseKey(key);
    return 0;
}

int load_fonts(Fonts *fonts, const char *name) {
    char normal_path[256];
    char bold_path[256];
    char italic_path[256];

    if (!find_font(name, normal_path, sizeof(normal_path)) ||
        !find_font(name, bold_path, sizeof(bold_path)) ||
        !find_font(name, italic_path, sizeof(italic_path))) {

        printf("Font not found: %s\n", name);
        return 0;
    }

    fonts->normal = TTF_OpenFont(normal_path, FONT_SIZE);
    fonts->bold = TTF_OpenFont(bold_path, FONT_SIZE);
    fonts->italic = TTF_OpenFont(italic_path, FONT_SIZE);

    fonts->big_normal = TTF_OpenFont(normal_path, FONT_SIZE + 4);
    fonts->big_bold = TTF_OpenFont(bold_path, FONT_SIZE + 4);
    fonts->big_italic = TTF_OpenFont(italic_path, FONT_SIZE + 4);

    fonts->small_normal = TTF_OpenFont(normal_path, FONT_SIZE - 2);
    fonts->small_bold = TTF_OpenFont(bold_path, FONT_SIZE - 2);
    fonts->small_italic = TTF_OpenFont(italic_path, FONT_SIZE - 2);

    if (fonts->normal == NULL ||
        fonts->bold == NULL ||
        fonts->italic == NULL ||
        fonts->big_normal == NULL ||
        fonts->big_bold == NULL ||
        fonts->big_italic == NULL ||
        fonts->small_normal == NULL ||
        fonts->small_bold == NULL ||
        fonts->small_italic == NULL) {

        printf("Font error: %s\n", TTF_GetError());
        free_fonts(fonts);
        return 0;
    }

    return 1;
}
void free_fonts(Fonts *fonts) {
    if (fonts->normal != NULL)
        TTF_CloseFont(fonts->normal);

    if (fonts->bold != NULL)
        TTF_CloseFont(fonts->bold);

    if (fonts->italic != NULL)
        TTF_CloseFont(fonts->italic);

    if (fonts->big_normal != NULL)
        TTF_CloseFont(fonts->big_normal);

    if (fonts->big_bold != NULL)
        TTF_CloseFont(fonts->big_bold);

    if (fonts->big_italic != NULL)
        TTF_CloseFont(fonts->big_italic);

    if (fonts->small_normal != NULL)
        TTF_CloseFont(fonts->small_normal);

    if (fonts->small_bold != NULL)
        TTF_CloseFont(fonts->small_bold);

    if (fonts->small_italic != NULL)
        TTF_CloseFont(fonts->small_italic);
}

