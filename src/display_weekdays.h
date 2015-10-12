#pragma once
#include <pebble.h>

void draw_weekdays(int day_of_week, Layer *layer, GContext *ctx, GColor stroke_color);
void init_weekdays(void);
void deinit_weekdays(void);