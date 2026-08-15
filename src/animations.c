#include <math.h>

#include "animations.h"

float wave_scale(float amount, float time, int character_index)
{
    float wave = (sinf(time * 5.0f - character_index * 0.4f) + 1.0f) / 2.0f;

    wave = wave * wave;

    return 1.0f + amount * wave;
}

float bounce_height(float amount, float time, int character_index)
{
    float shift_time = time + 0.4f;
    float wave = sinf(shift_time * 5.0f - character_index * 0.4f);

    //wave = wave * wave;

    return amount * 3.0f * wave;
}

void shake_offset(
    float amount,
    float time,
    int character_index,
    float *offset_x,
    float *offset_y
)
{
    *offset_x = amount * sinf(time * 30.0f + character_index * 1.7f);
    *offset_y = amount * cosf(time * 37.0f + character_index * 2.1f);
}