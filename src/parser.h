#ifndef PARSER_H
#define PARSER_H

typedef enum {
    STYLE_NORMAL,
    STYLE_BOLD,
    STYLE_TILT
} TextStyle;

typedef struct {
    char *text;
    TextStyle style;
} TextSegment;

int parse_text(const char *input, TextSegment **segments);
void free_segments(TextSegment *segments, int count);

#endif