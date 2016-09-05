#pragma once

#include <pebble.h>
#include "src/c/models.h"

void draw_face(GContext *ctx, Layer *layer);
void draw_connectWarning(GContext *ctx, Layer *layer);
void draw_date(GContext *ctx, Layer *layer, Time time);