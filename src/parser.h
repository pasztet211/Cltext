#ifndef PARSER_H
#define PARSER_H

typedef enum {
    STYLE_NORMAL = 0,
    STYLE_BOLD   = 1 << 0,
    STYLE_TILT   = 1 << 1,
    STYLE_WAVE   = 1 << 2,
    STYLE_BOUNCE = 1 << 3,
    STYLE_SHAKE  = 1 << 4
} TextStyle;

typedef struct {
    char *text;
    TextStyle style;

    float wave_amount;
    float bounce_amount;
    float shake_amount;
} TextSegment;

int parse_text(const char *input, TextSegment **segments);

void free_segments(TextSegment *segments, int count);

#endif