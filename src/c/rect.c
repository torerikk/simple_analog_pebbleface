#if defined(PBL_RECT)

#include <pebble.h>
#include "./face.h"

#define TIME_ANGLE(time) time * (TRIG_MAX_ANGLE / 60)
#define HOUR_ANGLE(hour) hour * (TRIG_MAX_ANGLE / 12)

void draw_face(GContext *ctx, Layer *layer) {
  
}

void draw_connectWarning(GContext *ctx, Layer *layer) {
  
}
void draw_date(GContext *ctx, Layer *layer, Time time) {
  
}

#endif