#include <pebble.h>

#define ANTIALIASING true

#define KEY_BACKGROUND_COLOR 10
#define KEY_CLOCKFACE_COLOR 11
#define KEY_CLOCKSTROKE_COLOR 12
#define KEY_DISCONNECT_WARNING 2

#define HAND_MARGIN  10
#define FINAL_RADIUS 65

#define ANIMATION_DURATION 500
#define ANIMATION_DELAY    600

typedef struct {
  int hours;
  int minutes;
} Time;

static Window *s_main_window;
static Layer *s_canvas_layer;
static TextLayer *s_connection_layer;

static GColor s_color_bg, s_color_face_bg, s_color_stroke;
static GPoint s_center;
static Time s_last_time, s_anim_time;
static int s_radius = 0;
static bool s_animating = false;
static bool s_disconnect_warning = false;

/*************************** AnimationImplementation **************************/

static void animation_started(Animation *anim, void *context) {
  s_animating = true;
}

static void animation_stopped(Animation *anim, bool stopped, void *context) {
  s_animating = false;
}

static void animate(int duration, int delay, AnimationImplementation *implementation, bool handlers) {
  Animation *anim = animation_create();
  animation_set_duration(anim, duration);
  animation_set_delay(anim, delay);
  animation_set_curve(anim, AnimationCurveEaseInOut);
  animation_set_implementation(anim, implementation);
  if(handlers) {
    animation_set_handlers(anim, (AnimationHandlers) {
      .started = animation_started,
      .stopped = animation_stopped
    }, NULL);
  }
  animation_schedule(anim);
}

/************************************ UI **************************************/

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  // Store time
  s_last_time.hours = tick_time->tm_hour;
  s_last_time.hours -= (s_last_time.hours > 12) ? 12 : 0;
  s_last_time.minutes = tick_time->tm_min;

  /*for(int i = 0; i < 3; i++) {
    s_color_channels[i] = rand() % 256;
  }
*/
  // Redraw
  if(s_canvas_layer) {
    layer_mark_dirty(s_canvas_layer);
  }
}

static int hours_to_minutes(int hours_out_of_12) {
  return (int)(float)(((float)hours_out_of_12 / 12.0F) * 60.0F);
}

static void update_proc(Layer *layer, GContext *ctx) {
  // Color background?
  graphics_context_set_fill_color(ctx, s_color_bg);
  graphics_fill_rect(ctx, GRect(0, 0, 144, 168), 0, GCornerNone);

  graphics_context_set_stroke_color(ctx, s_color_stroke);
  graphics_context_set_stroke_width(ctx, 4);

  graphics_context_set_antialiased(ctx, ANTIALIASING);

  // White clockface
  graphics_context_set_fill_color(ctx, s_color_face_bg);
  graphics_fill_circle(ctx, s_center, s_radius);

  // Draw outline
  graphics_draw_circle(ctx, s_center, s_radius);

  // Don't use current time while animating
  Time mode_time = (s_animating) ? s_anim_time : s_last_time;

  // Adjust for minutes through the hour
  float minute_angle = TRIG_MAX_ANGLE * mode_time.minutes / 60;
  float hour_angle;
  if(s_animating) {
    // Hours out of 60 for smoothness
    hour_angle = TRIG_MAX_ANGLE * mode_time.hours / 60;
  } else {
    hour_angle = TRIG_MAX_ANGLE * mode_time.hours / 12;
  }
  hour_angle += (minute_angle / TRIG_MAX_ANGLE) * (TRIG_MAX_ANGLE / 12);

  // Plot hands
  GPoint minute_hand = (GPoint) {
    .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * mode_time.minutes / 60) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + s_center.x,
    .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * mode_time.minutes / 60) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + s_center.y,
  };
  GPoint hour_hand = (GPoint) {
    .x = (int16_t)(sin_lookup(hour_angle) * (int32_t)(s_radius - (2 * HAND_MARGIN)) / TRIG_MAX_RATIO) + s_center.x,
    .y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)(s_radius - (2 * HAND_MARGIN)) / TRIG_MAX_RATIO) + s_center.y,
  };

  // Draw hands with positive length only
  if(s_radius > 2 * HAND_MARGIN) {
    graphics_draw_line(ctx, s_center, hour_hand);
  } 
  if(s_radius > HAND_MARGIN) {
    graphics_draw_line(ctx, s_center, minute_hand);
  }
}

static void handle_bluetooth(bool connected) {
  text_layer_set_text(s_connection_layer, connected ? "" : "disconnected");
	if(!connected) {
		vibes_short_pulse();
	}
}

static void init_connection_warning() {
	APP_LOG(APP_LOG_LEVEL_INFO, "connection warning init");
  handle_bluetooth(bluetooth_connection_service_peek());
	bluetooth_connection_service_subscribe(handle_bluetooth);
}

static void deinit_connection_warning() {
	APP_LOG(APP_LOG_LEVEL_INFO, "connection warning deinit");
	bluetooth_connection_service_unsubscribe();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  s_center = grect_center_point(&window_bounds);

	s_connection_layer = text_layer_create(GRect(0, window_bounds.size.h - 20, window_bounds.size.w, 20));
	text_layer_set_text_color(s_connection_layer, s_color_stroke);
  text_layer_set_background_color(s_connection_layer, GColorClear);
  text_layer_set_font(s_connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_connection_layer, GTextAlignmentCenter);
	
  s_canvas_layer = layer_create(window_bounds);
  layer_set_update_proc(s_canvas_layer, update_proc);
	
  layer_add_child(window_layer, s_canvas_layer);
	layer_add_child(window_layer, text_layer_get_layer(s_connection_layer));
	
	if (persist_read_int(KEY_BACKGROUND_COLOR)) {
    int background_color = persist_read_int(KEY_BACKGROUND_COLOR);
    s_color_bg = GColorFromHEX(background_color);
  } else {
		s_color_bg = GColorWhite;
	}
	
	if (persist_read_int(KEY_CLOCKFACE_COLOR)) {
    int clockface_color = persist_read_int(KEY_CLOCKFACE_COLOR);
    s_color_face_bg = GColorFromHEX(clockface_color);
  } else {
		s_color_face_bg = GColorWhite;
	}
	
	if (persist_read_int(KEY_CLOCKSTROKE_COLOR)) {
    int clockstroke_color = persist_read_int(KEY_CLOCKSTROKE_COLOR);
    s_color_stroke = GColorFromHEX(clockstroke_color);
  } else {
		s_color_stroke = GColorBlack;
	}
	
	if(persist_read_bool(KEY_DISCONNECT_WARNING)) {
		s_disconnect_warning = persist_read_bool(KEY_DISCONNECT_WARNING);
		if(s_disconnect_warning) {
			init_connection_warning();
		}
	}
}

static void window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
	if(s_connection_layer) {
		text_layer_destroy(s_connection_layer);
	}
	deinit_connection_warning();
}

/*********************************** App **************************************/

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *background_color_t = dict_find(iter, KEY_BACKGROUND_COLOR);
	Tuple *clockface_color_t = dict_find(iter, KEY_CLOCKFACE_COLOR);
	Tuple *clockstroke_color_t = dict_find(iter, KEY_CLOCKSTROKE_COLOR);
	Tuple *disconnect_warning_t = dict_find(iter, KEY_DISCONNECT_WARNING);
  //Tuple *twenty_four_hour_format_t = dict_find(iter, KEY_TWENTY_FOUR_HOUR_FORMAT);
	bool updateFace = false;

	if(disconnect_warning_t) {
		s_disconnect_warning = disconnect_warning_t->value->int8;
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "disconnect warning setting changed: %d", (bool)s_disconnect_warning);
		persist_write_bool(KEY_DISCONNECT_WARNING, s_disconnect_warning);
		if(s_disconnect_warning) {
			init_connection_warning();
		} else {
			deinit_connection_warning();
		}
	}
	
  if (background_color_t) {
    int background_color = background_color_t->value->int32;

    persist_write_int(KEY_BACKGROUND_COLOR, background_color);
		
		s_color_bg = GColorFromHEX(background_color);
		updateFace = true;
  }
	
	if (clockface_color_t) {
    int clockface_color = clockface_color_t->value->int32;

    persist_write_int(KEY_CLOCKFACE_COLOR, clockface_color);
		
		s_color_face_bg = GColorFromHEX(clockface_color);
		updateFace = true;
  }
	
	if (clockstroke_color_t) {
    int clockstroke_color = clockstroke_color_t->value->int32;

    persist_write_int(KEY_CLOCKSTROKE_COLOR, clockstroke_color);
		
		s_color_stroke = GColorFromHEX(clockstroke_color);
		updateFace = true;
  }

  /*if (twenty_four_hour_format_t) {
    twenty_four_hour_format = twenty_four_hour_format_t->value->int8;

    persist_write_int(KEY_TWENTY_FOUR_HOUR_FORMAT, twenty_four_hour_format);

    update_time();
  }*/
	
	if(updateFace && s_canvas_layer) {
    layer_mark_dirty(s_canvas_layer);
 	}

}

static int anim_percentage(AnimationProgress dist_normalized, int max) {
  return (int)(float)(((float)dist_normalized / (float)ANIMATION_NORMALIZED_MAX) * (float)max);
}

static void radius_update(Animation *anim, AnimationProgress dist_normalized) {
  s_radius = anim_percentage(dist_normalized, FINAL_RADIUS);

  layer_mark_dirty(s_canvas_layer);
}

static void hands_update(Animation *anim, AnimationProgress dist_normalized) {
  s_anim_time.hours = anim_percentage(dist_normalized, hours_to_minutes(s_last_time.hours));
  s_anim_time.minutes = anim_percentage(dist_normalized, s_last_time.minutes);

  layer_mark_dirty(s_canvas_layer);
}

static void init() {
  srand(time(NULL));

  time_t t = time(NULL);
  struct tm *time_now = localtime(&t);
  tick_handler(time_now, MINUTE_UNIT);

  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Prepare animations
  AnimationImplementation radius_impl = {
    .update = radius_update
  };
  animate(ANIMATION_DURATION, ANIMATION_DELAY, &radius_impl, false);

  AnimationImplementation hands_impl = {
    .update = hands_update
  };
  animate(2 * ANIMATION_DURATION, ANIMATION_DELAY, &hands_impl, true);
	
	app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
