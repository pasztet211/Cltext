#include <ctype.h>

#include "check_valid_tag.h"

int is_valid_tag(const char *pos) {
    const char *p = pos;

    if (*p != '<') {
        return 0;
    }

    p++;

    if (*p == '/') {
        p++;
    }

    if (p[0] == '?' && p[1] == '=' && p[2] == '=') {
        p += 3;
    }

    if (!isalpha((unsigned char)*p)) {
        return 0;
    }

    /* tag name */
    while (isalnum((unsigned char)*p) || *p == '_' || *p == '-') {
        p++;
    }

    /* directly ends */
    if (*p == '>') {
        return 1;
    }

    /* parameters must start after whitespace */
    if (!isspace((unsigned char)*p)) {
        return 0;
    }

    while (*p != '\0' && *p != '>') {
        p++;
    }

    return *p == '>';
}