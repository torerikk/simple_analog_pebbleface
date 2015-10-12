#pragma once

#include <pebble.h>

#define ANTIALIASING true

#define KEY_BACKGROUND_COLOR 10
#define KEY_CLOCKFACE_COLOR 13
#define KEY_CLOCKSTROKE_COLOR 12
#define KEY_DISCONNECT_WARNING 2

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
  