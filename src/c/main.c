#include "main.h"
// #include "display_weekdays.h"

static Window *s_main_window;
static Layer *s_canvas_layer;
static Layer *s_connection_layer;
static BitmapLayer *s_connection_bmp_layer;

static GColor s_color_bg, s_color_face_bg, s_color_stroke;
static GPoint s_center, s_month_center, s_icon_center;
static Time s_last_time, s_anim_time;
static int s_radius = 0;
static int s_center_circle_radius = 0;
static bool s_animating = true;
static bool s_disconnect_warning = false;
static bool s_is_connected = false;
static int s_final_radius = 0;
static int s_bubble_radius = 20;
static GBitmap *s_bluetooth_icon;


static int find_max_face_radius(const GRect bounds) {
  int radius_w = (bounds.size.w / 2) - 6;
  int radius_h = (bounds.size.h / 2) - 6;
  if(radius_h > radius_w) {
	  return radius_w;
  } else {
    return radius_h;
  }
}

static GPoint find_month_center(const GRect bounds) {
  int pY = (bounds.size.h - s_bubble_radius - 4);
  int pX = (s_bubble_radius + 3);
  return (GPoint) {.x = pX, .y = pY };
}

static GRect find_month_rect(const GRect bounds) {
  int pY1 = bounds.size.h - (s_bubble_radius * 2) + 4;
  int pY2 = bounds.size.h - 7;
  return GRect(4, pY1, 40, pY2);
}

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
	// Store timeud
	s_last_time.hours = tick_time->tm_hour;
	s_last_time.hours -= (s_last_time.hours > 12) ? 12 : 0;
	s_last_time.minutes = tick_time->tm_min;
	strftime(s_last_time.mday, 3, "%d", tick_time);
	//s_last_time.mday = tick_time->tm_mday;

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

static void connection_update_proc(Layer *layer, GContext *ctx) {
	if(!s_animating) {
		graphics_context_set_stroke_color(ctx, s_color_stroke);
		graphics_context_set_stroke_width(ctx, 3);

		graphics_context_set_antialiased(ctx, ANTIALIASING);
	
		if(s_is_connected) {
			graphics_context_set_fill_color(ctx, s_color_face_bg);
			layer_set_hidden(bitmap_layer_get_layer(s_connection_bmp_layer), true);
		} else {
			
			graphics_context_set_fill_color(ctx, GColorRed);
			layer_set_hidden(bitmap_layer_get_layer(s_connection_bmp_layer), false);
			// Bluetooth bubble
			graphics_fill_circle(ctx, s_icon_center, s_bubble_radius);
			graphics_draw_circle(ctx, s_icon_center, s_bubble_radius);	
		}
	}
}

static void update_proc_v2(Layer *layer, GContext *ctx) {
#if defined(PBL_RECT)
  printf("This is a rectangular display!");
#elif defined(PBL_ROUND)
  GRect rect_bounds = grect_inset(layer_get_unobstructed_bounds(layer), GEdgeInsets(3));
  uint16_t inset_thickness = 10; 
  
  graphics_context_set_fill_color(ctx, GColorYellow);
  graphics_context_set_stroke_color(ctx, GColorLimerick);
  graphics_context_set_stroke_width(ctx, 40);
    
  graphics_draw_arc(ctx, rect_bounds, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(0), TRIG_MAX_ANGLE);
  // Fill a radial section of a circle
  graphics_fill_radial(ctx, rect_bounds, GOvalScaleModeFitCircle, rect_bounds.size.w , 0, TRIG_MAX_ANGLE);
  printf("This is a round display!");
#endif
}

static void update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  s_center = grect_center_point(&bounds);
  
  s_final_radius = find_max_face_radius(bounds);
  s_month_center = find_month_center(bounds);
  
	graphics_context_set_stroke_color(ctx, s_color_stroke);
	graphics_context_set_stroke_width(ctx, 3);

	graphics_context_set_antialiased(ctx, ANTIALIASING);
	
	// Same color used for clockface and month bubble.
	graphics_context_set_fill_color(ctx, s_color_face_bg);
	if(!s_animating) {
    s_radius = s_final_radius;
		// Month bubble
		
    // TYPE A:
		graphics_fill_circle(ctx, s_month_center, s_bubble_radius);
    
		graphics_draw_circle(ctx, s_month_center, s_bubble_radius);
	
    graphics_context_set_text_color(ctx, s_color_stroke);
		// TYPE A:
    //graphics_draw_text(ctx, s_last_time.mday, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), GRect(4, 132, 40, 161), GTextOverflowModeFill, GTextAlignmentCenter, NULL);
	}
	// White clockface
	
	//graphics_context_set_fill_color(ctx, s_color_face_bg);
	graphics_fill_circle(ctx, s_center, s_radius);

	// Draw outline
	//graphics_draw_circle(ctx, s_center, s_radius);
  graphics_context_set_fill_color(ctx, s_color_stroke);
  graphics_fill_radial(ctx, grect_inset(layer_get_bounds(layer), GEdgeInsets(3)), GOvalScaleModeFitCircle, 5, 0, TRIG_MAX_ANGLE);

  if(!s_animating) {
    // TYPE B:
    graphics_fill_circle(ctx, s_month_center, s_bubble_radius - 2);
    GRect month_rect = find_month_rect(bounds);
    graphics_draw_text(ctx, s_last_time.mday, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), month_rect, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  }
	// Don't use current time while animating
	Time mode_time = (s_animating) ? s_anim_time : s_last_time;

  // Uncomment to show weekday letters. Does not indicate current day yet.
  //draw_weekdays(0, layer, ctx, s_color_stroke);
  
	graphics_context_set_stroke_width(ctx, 5);
	
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
		.x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * mode_time.minutes / 60) * (int32_t)(s_radius - LONG_HAND_MARGIN) / TRIG_MAX_RATIO) + s_center.x,
		.y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * mode_time.minutes / 60) * (int32_t)(s_radius - LONG_HAND_MARGIN) / TRIG_MAX_RATIO) + s_center.y,
	};
	GPoint hour_hand = (GPoint) {
		.x = (int16_t)(sin_lookup(hour_angle) * (int32_t)(s_radius - SHORT_HAND_MARGIN) / TRIG_MAX_RATIO) + s_center.x,
		.y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)(s_radius - SHORT_HAND_MARGIN) / TRIG_MAX_RATIO) + s_center.y,
	};

	// Draw hands with positive length only
	if(s_radius > SHORT_HAND_MARGIN) {
		graphics_draw_line(ctx, s_center, hour_hand);
	} 
	if(s_radius > LONG_HAND_MARGIN) {
		graphics_draw_line(ctx, s_center, minute_hand);
	}

	// Center circle fill and outline
	graphics_context_set_fill_color(ctx, s_color_face_bg);
	graphics_fill_circle(ctx, s_center, s_center_circle_radius);
	graphics_draw_circle(ctx, s_center, s_center_circle_radius);
}

static void handle_bluetooth(bool connected) {
	//text_layer_set_text(s_connection_layer, connected ? "" : "disconnected");
	s_is_connected = connected;
	if(s_connection_layer) {
		layer_mark_dirty(s_connection_layer);
	}
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
  GRect unobstructed_bounds = layer_get_unobstructed_bounds(window_layer);

	//GRect window_bounds = layer_get_bounds(window_layer);
  s_final_radius = find_max_face_radius(unobstructed_bounds);

  s_center = grect_center_point(&unobstructed_bounds);
	s_month_center = find_month_center(unobstructed_bounds);

	s_bluetooth_icon = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_ICON);
	if (!s_bluetooth_icon) {
		APP_LOG(APP_LOG_LEVEL_ERROR, "Image is NULL!");
	}

  // TODO: find s_icon_center based on unobstructed view.
	s_icon_center = (GPoint) {.x = 121, .y = 23 };
	GSize image_size = gbitmap_get_bounds(s_bluetooth_icon).size;
	GRect image_frame = GRect(s_icon_center.x, s_icon_center.y, image_size.w, image_size.h);
	image_frame.origin.x -= image_size.w / 2;
	image_frame.origin.y -= image_size.h / 2;

	s_connection_bmp_layer = bitmap_layer_create(image_frame);
	bitmap_layer_set_compositing_mode(s_connection_bmp_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_connection_bmp_layer, s_bluetooth_icon);
	layer_set_hidden(bitmap_layer_get_layer(s_connection_bmp_layer), true);

	s_canvas_layer = layer_create(unobstructed_bounds);
	layer_set_update_proc(s_canvas_layer, update_proc_v2);
	
	s_connection_layer = layer_create(unobstructed_bounds);
	layer_set_update_proc(s_connection_layer, connection_update_proc);
	
	layer_add_child(window_layer, s_connection_layer);
	
	layer_add_child(window_layer, s_canvas_layer);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_connection_bmp_layer));
	
	if (persist_exists(MESSAGE_KEY_backgroundColor)) {
		int background_color = persist_read_int(MESSAGE_KEY_backgroundColor);
		s_color_bg = GColorFromHEX(background_color);
	} else {
		s_color_bg = GColorWhite;
	}
	
	window_set_background_color(window, s_color_bg);
	
	if (persist_exists(MESSAGE_KEY_clockfaceColor)) {
		APP_LOG(APP_LOG_LEVEL_INFO, "face color set");
		int clockface_color = persist_read_int(MESSAGE_KEY_clockfaceColor);
		s_color_face_bg = GColorFromHEX(clockface_color);
	} else {
		s_color_face_bg = GColorWhite;
		APP_LOG(APP_LOG_LEVEL_INFO, "face default color");
	}
	
	if (persist_exists(MESSAGE_KEY_clockStrokeColor)) {
		APP_LOG(APP_LOG_LEVEL_INFO, "clock stroke set");
		int clockstroke_color = persist_read_int(MESSAGE_KEY_clockStrokeColor);
		s_color_stroke = GColorFromHEX(clockstroke_color);
	} else {
		APP_LOG(APP_LOG_LEVEL_INFO, "clock stroke default");
		s_color_stroke = GColorBlack;
	}
	
	if(persist_exists(MESSAGE_KEY_disconnectWarning)) {
		s_disconnect_warning = persist_read_bool(MESSAGE_KEY_disconnectWarning);
		if(s_disconnect_warning) {
			init_connection_warning();
		}
	}
}

static void window_unload(Window *window) {
	layer_destroy(s_canvas_layer);
	if(s_connection_layer) {
		layer_destroy(s_connection_layer);
	}
	bitmap_layer_destroy(s_connection_bmp_layer);
	gbitmap_destroy(s_bluetooth_icon);

	deinit_connection_warning();
}

/*********************************** App **************************************/

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *background_color_t = dict_find(iter, MESSAGE_KEY_backgroundColor);
	Tuple *clockface_color_t = dict_find(iter, MESSAGE_KEY_clockfaceColor);
	Tuple *clockstroke_color_t = dict_find(iter, MESSAGE_KEY_clockStrokeColor);
	Tuple *disconnect_warning_t = dict_find(iter, MESSAGE_KEY_disconnectWarning);
	//Tuple *twenty_four_hour_format_t = dict_find(iter, KEY_TWENTY_FOUR_HOUR_FORMAT);
	bool updateFace = false;

	if(disconnect_warning_t && disconnect_warning_t->value->int32 > 0) {
		if(s_disconnect_warning == false) {
			init_connection_warning();
		}
		s_disconnect_warning = true;
		persist_write_bool(MESSAGE_KEY_disconnectWarning, s_disconnect_warning);
	} else {
		if(s_disconnect_warning == true) {
			deinit_connection_warning();
		}
		s_disconnect_warning = false;
		persist_write_bool(MESSAGE_KEY_disconnectWarning, s_disconnect_warning);
	}
	
	if (background_color_t) {
		int background_color = background_color_t->value->int32;

		persist_write_int(MESSAGE_KEY_backgroundColor, background_color);
		
		s_color_bg = GColorFromHEX(background_color);
		window_set_background_color(s_main_window, s_color_bg);
		//updateFace = true;
	}
	
	if (clockface_color_t) {
		
		int clockface_color = clockface_color_t->value->int32;
		
		persist_write_int(MESSAGE_KEY_clockfaceColor, clockface_color);
		
		s_color_face_bg = GColorFromHEX(clockface_color);
		APP_LOG(APP_LOG_LEVEL_INFO,  "clock face color set and saved: %d", clockface_color );
		updateFace = true;
	}
	
	if (clockstroke_color_t) {
		int clockstroke_color = clockstroke_color_t->value->int32;

		persist_write_int(MESSAGE_KEY_clockStrokeColor, clockstroke_color);
		
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
	s_radius = anim_percentage(dist_normalized, s_final_radius);
	
	if(s_radius < (int)CENTER_RADIUS) {
		s_center_circle_radius = s_radius;
	} else {
		s_center_circle_radius = (int)CENTER_RADIUS;
	}
	
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
  
  // Uncomment to show weekday letters. Does not indicate current day yet.
  //init_weekdays();
}

static void deinit() {
  // Uncomment to show weekday letters. Does not indicate current day yet.
  //deinit_weekdays();
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}