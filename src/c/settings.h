#pragma once
#include <pebble.h>

#define SETTINGS_PERSIST_KEY 3
#define SETTINGS_VERSION_KEY 4
#define CURRENT_SETTINGS_VERSION 2
/*typedef struct {
  GColor timeColor;
  GColor timeBgColor;
  GColor sidebarColor;
  GColor sidebarTextColor;
  bool useMetric;
  bool sidebarOnRight;
  bool btVibe;
  bool showBatteryLevel;
  uint8_t languageId;
} Settings;

Settings globalSettings;
*/

void Settings_init();
void Settings_loadAllDefaults();
void Settings_loadV2Defaults();
void Settings_deinit();