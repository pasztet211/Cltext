#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static void add_segment(
    TextSegment **segments,
    int *count,
    int *capacity,
    const char *text,
    size_t length,
    TextStyle style,
    float wave_amount,
    float bounce_amount
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

    (*count)++;
}

int parse_text(const char *input, TextSegment **segments) {
    int capacity = 8;
    int count = 0;

    *segments = malloc(sizeof(TextSegment) * capacity);

    if (*segments == NULL) {
        return -1;
    }

    TextStyle current_style = STYLE_NORMAL;

    float current_wave_amount = 0.0f;
    float current_bounce_amount = 0.0f;

    const char *text_start = input;
    const char *pos = input;

    while (*pos != '\0') {
        if (strncmp(pos, "<?==", 4) == 0) {
            const char *comment_end = strstr(pos + 4, "==?>");

            if (comment_end != NULL) {
                pos = comment_end + 4;
                text_start = pos;
                continue;
            }

            break;
        }
        
        if (*pos != '<') {
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
            current_bounce_amount
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
            else {
                add_segment(
                    segments,
                    &count,
                    &capacity,
                    pos,
                    tag_end - pos + 1,
                    current_style,
                    current_wave_amount,
                    current_bounce_amount
                );
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
        current_bounce_amount
    );

    return count;
}

void free_segments(TextSegment *segments, int count) {
    for (int i = 0; i < count; i++) {
        free(segments[i].text);
    }

    free(segments);
}