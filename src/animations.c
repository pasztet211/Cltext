#include <math.h>
#include <stdlib.h>

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

float glitch_offset(float *offset_x, float *offset_y, float *scale) {
    *offset_x = (float)rand() / 10000;
    *offset_y = (float)rand() / 10000;
    *scale    = (float)rand() / 28125;
}

float spin_angle(float amount, float time) {
    return amount * time * 360.0f;
}

float cheer_height(float amount, float time, int character_index) 
{
    int random_index = character_index + ((character_index * 37) % 10);

    float wave = (sinf(time * 10.0f - random_index * 0.4f) + 1.0f) / 2.0f;
    wave = wave * wave;

    return 1.0f + amount * 1.5f * wave;
}