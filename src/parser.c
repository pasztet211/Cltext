#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "parser.h"
#include "check_valid_tag.h"

static void add_segment(
    TextSegment **segments,
    int *count,
    int *capacity,
    const char *text,
    size_t length,
    TextStyle style,
    float wave_amount,
    float bounce_amount,
    float shake_amount,
    SDL_Color *color
) {
    if (length == 0) {
        return;
    }

    if (*count >= *capacity) {
        *capacity *= 2;

        TextSegment *new_segments =
            realloc(*segments, sizeof(TextSegment) * (*capacity));

        if (new_segments == NULL) {
            return;
        }

        *segments = new_segments;
    }

    (*segments)[*count].text = malloc(length + 1);

    if ((*segments)[*count].text == NULL) {
        return;
    }

    memcpy((*segments)[*count].text, text, length);
    (*segments)[*count].text[length] = '\0';

    (*segments)[*count].style = style;
    (*segments)[*count].wave_amount = wave_amount;
    (*segments)[*count].bounce_amount = bounce_amount;
    (*segments)[*count].shake_amount = shake_amount;
    (*segments)[*count].color = *color;

    (*count)++;
}

int parse_text(const char *input, TextSegment **segments, SDL_Color *bgcolor, SDL_Color *text_color, SDL_Color *base_text_color) {
    int capacity = 8;
    int count = 0;

    *segments = malloc(sizeof(TextSegment) * capacity);

    if (*segments == NULL) {
        return -1;
    }

    TextStyle current_style = STYLE_NORMAL;

    float current_wave_amount = 0.0f;
    float current_bounce_amount = 0.0f;
    float current_shake_amount = 0.0f;
    float current_glitch_amount = 0.0f;
    int current_size = 0;

    const char *text_start = input;
    const char *pos = input;

    while (*pos != '\0') {
        int line_start = (pos == input || *(pos - 1) == '\n');

        if (*pos != '<') {
            pos++;
            continue;
        }

        if (strncmp(pos, "<?==", 4) == 0) {
            const char *comment_end = strstr(pos + 4, "==?>");

            if (comment_end != NULL) {
                pos = comment_end + 4;
                text_start = pos;
                continue;
            }

            break;
        }

        if (!is_valid_tag(pos)) {
            pos++;
            continue;
        }

        const char *tag_end = strchr(pos, '>');

        if (tag_end == NULL) {
            pos++;
            continue;
        }

        add_segment(
            segments,
            &count,
            &capacity,
            text_start,
            pos - text_start,
            current_style,
            current_wave_amount,
            current_bounce_amount,
            current_shake_amount,
            text_color
        );

        size_t tag_length = tag_end - pos - 1;

        char tag[128];

        if (tag_length < sizeof(tag)) {
            memcpy(tag, pos + 1, tag_length);
            tag[tag_length] = '\0';

            if (strcmp(tag, "bold") == 0) {
                current_style |= STYLE_BOLD;
            }
            else if (strcmp(tag, "/bold") == 0) {
                current_style &= ~STYLE_BOLD;
            }
            else if (strcmp(tag, "tilt") == 0) {
                current_style |= STYLE_TILT;
            }
            else if (strcmp(tag, "/tilt") == 0) {
                current_style &= ~STYLE_TILT;
            }
            else if (strncmp(tag, "wave", 4) == 0) {
                current_style |= STYLE_WAVE;
                current_wave_amount = 1.0f;

                char *a = strstr(tag, "a=");

                if (a != NULL) {
                    current_wave_amount = strtof(a + 2, NULL);
                }
            }
            else if (strcmp(tag, "/wave") == 0) {
                current_style &= ~STYLE_WAVE;
                current_wave_amount = 0.0f;
            }
            else if (strncmp(tag, "bounce", 6) == 0) {
                current_style |= STYLE_BOUNCE;
                current_bounce_amount = 1.0f;

                char *a = strstr(tag, "a=");

                if (a != NULL) {
                    current_bounce_amount = strtof(a + 2, NULL);
                }
            }
            else if (strcmp(tag, "/bounce") == 0) {
                current_style &= ~STYLE_BOUNCE;
                current_bounce_amount = 0.0f;
            }
            else if (strncmp(tag, "shake", 5) == 0) {
                current_style |= STYLE_SHAKE;
                current_shake_amount = 1.0f;

                char *a = strstr(tag, "a=");

                if (a != NULL) {
                    current_shake_amount = strtof(a + 2, NULL);
                }
            }
            else if (strcmp(tag, "/shake") == 0) {
                current_style &= ~STYLE_SHAKE;
                current_shake_amount = 0.0f;
            }
            else if (strncmp(tag, "bgcolor", 7) == 0) {
                char bg_color[8] = "";

                char *c = strstr(tag, "c=\"");

                if (c != NULL) {
                    strncpy(bg_color, c + 3, 7);
                    bg_color[7] = '\0';

                    *bgcolor = hex_to_sdl_color(bg_color);
                }
            }
            else if (strncmp(tag, "color", 5) == 0) {
                char textcolor[8] = "";

                char *c = strstr(tag, "c=\"");

                if (c != NULL) {
                    strncpy(textcolor, c + 3, 7);
                    textcolor[7] = '\0';

                    *text_color = hex_to_sdl_color(textcolor);
                }
            }
            else if (strcmp(tag, "/color") == 0) {
                *text_color = *base_text_color;
            }
            else if (strcmp(tag, "big") == 0) {
                current_style |= STYLE_BIG;
            }
            else if (strcmp(tag, "/big") == 0) {
                current_style &= ~STYLE_BIG;
            }
            else if (strcmp(tag, "small") == 0) {
                current_style |= STYLE_SMALL;
            }
            else if (strcmp(tag, "/small") == 0) {
                current_style &= ~STYLE_SMALL;
            }
            else if (strncmp(tag, "txtcolor", 8) == 0) {
                char textcolor[8] = "";

                char *c = strstr(tag, "c=\"");

                if (c != NULL) {
                    strncpy(textcolor, c + 3, 7);
                    textcolor[7] = '\0';
                    
                    *base_text_color = hex_to_sdl_color(textcolor);
                    *text_color = hex_to_sdl_color(textcolor);
                }
                
            }
            else if (strcmp(tag, "glitch") == 0) {
                current_style |= STYLE_GLITCH;
                current_glitch_amount = 10.0f;
            }
            else if (strcmp(tag, "/glitch") == 0) {
                current_style &= ~STYLE_GLITCH;
                current_glitch_amount = 0.0f;
            }
            else {
                add_segment(
                    segments,
                    &count,
                    &capacity,
                    pos,
                    tag_end - pos + 1,
                    current_style,
                    current_wave_amount,
                    current_bounce_amount,
                    current_shake_amount,
                    text_color
                );
            }

            if (
                line_start &&
                (strncmp(tag, "txtcolor", 8) == 0 ||
                strncmp(tag, "bgcolor", 7) == 0)
            ) {
                const char *line_end = tag_end + 1;

                while (*line_end != '\0' && *line_end != '\n' && *line_end != '\r') {
                    line_end++;
                }

                pos = line_end;

                if (*pos == '\r') pos++;
                if (*pos == '\n') pos++;

                text_start = pos;
                continue;
            }
        }

        pos = tag_end + 1;
        text_start = pos;
    }

    add_segment(
        segments,
        &count,
        &capacity,
        text_start,
        pos - text_start,
        current_style,
        current_wave_amount,
        current_bounce_amount,
        current_shake_amount,
        text_color
    );

    return count;
}

void free_segments(TextSegment *segments, int count) {
    for (int i = 0; i < count; i++) {
        free(segments[i].text);
    }

    free(segments);
}

SDL_Color hex_to_sdl_color(const char *hex)
{
    SDL_Color color = {0, 0, 0, 255};

    if (hex == NULL || hex[0] != '#' || strlen(hex) != 7) {
        return color;
    }

    unsigned long value = strtoul(hex + 1, NULL, 16);

    color.r = (value >> 16) & 0xFF;
    color.g = (value >> 8) & 0xFF;
    color.b = value & 0xFF;

    return color;
}