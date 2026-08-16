#ifndef ANIMATIONS_H
#define ANIMATIONS_H

float wave_scale(float amount, float time, int character_index);
float bounce_height(float amount, float time, int character_index);
void shake_offset(float amount, float time, int character_index, float *offset_x, float *offset_y);
float glitch_offset(float ammount);

#endif