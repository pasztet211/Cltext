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
    float wave = (sinf(time * 5.0f - character_index * 0.4f) + 1.0f) / 2.0f;

    wave = wave * wave;

    return amount * 3.0f * wave;
}