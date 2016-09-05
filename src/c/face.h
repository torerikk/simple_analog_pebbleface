#pragma once

#include <pebble.h>
#include "./models.h"

void draw_face(GContext *ctx, Layer *layer);
void draw_connectWarning(GContext *ctx, Layer *layer);
void draw_date(GContext *ctx, Layer *layer, Time time);