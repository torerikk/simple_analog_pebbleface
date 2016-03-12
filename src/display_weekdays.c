#include "display_weekdays.h"

static GPath *s_mon_path_ptr = NULL;
static GPath *s_tue_path_ptr = NULL;
static GPath *s_wed_path_ptr = NULL;
static GPath *s_thu_path_ptr = NULL;
static GPath *s_fri_path_ptr = NULL;
static GPath *s_sat_path_ptr = NULL;
static GPath *s_sun_path_ptr = NULL;

static const GPathInfo T_PATH_INFO = {
  .num_points = 8,
  .points = (GPoint []) {{0, 0}, {10, 0}, {10, 2}, {6, 2}, {6, 10}, {4, 10}, {4, 2}, {0, 2}}
};

static const GPathInfo M_PATH_INFO = {
  .num_points = 14,
  .points = (GPoint []) {{0, 0}, {2, 0}, {4, 4}, {6, 4}, {8, 0}, {10, 0}, {10, 10}, {8, 10}, {8, 4}, {6, 6}, {4, 6}, {2, 4}, {2, 10}, {0, 10}}
};

static const GPathInfo W_PATH_INFO = {
  .num_points = 14,
  .points = (GPoint []) {{0, 0}, {2, 0}, {2, 6}, {4, 4}, {6, 4}, {8, 6}, {8, 0}, {10, 0}, {10, 10}, {8, 10}, {6, 6}, {4, 6}, {2, 10}, {0, 10}}
};

static const GPathInfo F_PATH_INFO = {
  .num_points = 10,
  .points = (GPoint []) {{0, 0}, {10, 0}, {10, 2}, {2, 2}, {4, 4}, {8, 4}, {8, 6}, {2, 6}, {2, 10}, {0, 10}}
};

static const GPathInfo S_PATH_INFO = {
  .num_points = 12,
  .points = (GPoint []) {{0, 0}, {10, 0}, {10, 2}, {2, 2}, {2, 4}, {10, 4}, {10, 10}, {0, 10}, {0, 8}, {8, 8}, {8, 6}, {0, 6}}
};

void draw_weekdays(int day_of_week, Layer *layer, GContext *ctx, GColor stroke_color) {
  graphics_context_set_stroke_width(ctx, 1);
  graphics_context_set_fill_color(ctx, stroke_color);
  graphics_context_set_stroke_color(ctx, stroke_color);

  gpath_draw_filled(ctx, s_mon_path_ptr);
  gpath_draw_outline(ctx, s_mon_path_ptr);

  gpath_draw_filled(ctx, s_tue_path_ptr);
  gpath_draw_outline(ctx, s_tue_path_ptr);

  gpath_draw_filled(ctx, s_wed_path_ptr);
  gpath_draw_outline(ctx, s_wed_path_ptr);
  
  gpath_draw_filled(ctx, s_thu_path_ptr);
  gpath_draw_outline(ctx, s_thu_path_ptr);

  gpath_draw_filled(ctx, s_fri_path_ptr);
  gpath_draw_outline(ctx, s_fri_path_ptr);

  gpath_draw_filled(ctx, s_sat_path_ptr);
  gpath_draw_outline(ctx, s_sat_path_ptr);

  gpath_draw_filled(ctx, s_sun_path_ptr);
  gpath_draw_outline(ctx, s_sun_path_ptr);
  
}

void init_weekdays(void) {
  s_mon_path_ptr = gpath_create(&M_PATH_INFO);
  s_tue_path_ptr = gpath_create(&T_PATH_INFO);
  s_wed_path_ptr = gpath_create(&W_PATH_INFO);
  s_thu_path_ptr = gpath_create(&T_PATH_INFO);
  s_fri_path_ptr = gpath_create(&F_PATH_INFO);
  s_sat_path_ptr = gpath_create(&S_PATH_INFO);
  s_sun_path_ptr = gpath_create(&S_PATH_INFO);
  gpath_rotate_to(s_mon_path_ptr, TRIG_MAX_ANGLE / 360 * 37);
  gpath_rotate_to(s_tue_path_ptr, TRIG_MAX_ANGLE / 360 * 32);
  gpath_rotate_to(s_wed_path_ptr, TRIG_MAX_ANGLE / 360 * 14);
  gpath_rotate_to(s_fri_path_ptr, TRIG_MAX_ANGLE / 360 * -14);
  gpath_rotate_to(s_sat_path_ptr, TRIG_MAX_ANGLE / 360 * -24);
  gpath_rotate_to(s_sun_path_ptr, TRIG_MAX_ANGLE / 360 * -37);

  gpath_move_to(s_mon_path_ptr, GPoint(35, 118));
  gpath_move_to(s_tue_path_ptr, GPoint(45, 125));
  gpath_move_to(s_wed_path_ptr, GPoint(55, 131));  
  gpath_move_to(s_thu_path_ptr, GPoint(67, 133));
  gpath_move_to(s_fri_path_ptr, GPoint(79, 133));
  gpath_move_to(s_sat_path_ptr, GPoint(90, 130));
  gpath_move_to(s_sun_path_ptr, GPoint(101, 125));
}


void deinit_weekdays(void) {
  if(s_mon_path_ptr != NULL) {
    gpath_destroy(s_mon_path_ptr);
  }
  if(s_tue_path_ptr != NULL) {
    gpath_destroy(s_tue_path_ptr);
  }
  if(s_wed_path_ptr != NULL) {
    gpath_destroy(s_wed_path_ptr);
  }
  if(s_thu_path_ptr != NULL) {
    gpath_destroy(s_thu_path_ptr);
  }
  if(s_fri_path_ptr != NULL) {
    gpath_destroy(s_fri_path_ptr);
  }
  if(s_sat_path_ptr != NULL) {
    gpath_destroy(s_sat_path_ptr);
  }
  if(s_sun_path_ptr != NULL) {
    gpath_destroy(s_sun_path_ptr);
  }
}