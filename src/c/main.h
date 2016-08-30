#pragma once

#include <pebble.h>

#define ANTIALIASING true

#define LONG_HAND_MARGIN  10
#define SHORT_HAND_MARGIN 30
#define FINAL_RADIUS 65
#define CENTER_RADIUS 8

#define ANIMATION_DURATION 500
#define ANIMATION_DELAY    600

typedef struct {
	int hours;
	int minutes;
	char mday[3];
} Time;
  