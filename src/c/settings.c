#include "settings.h"

void Settings_init() {
	// first, check if we have any saved settings
	if (persist_exists(SETTINGS_PERSIST_KEY)) {
		// if we do, what version of the settings was saved?
		uint32_t savedVersion = persist_read_int(SETTINGS_VERSION_KEY);

		//if(savedVersion == 2) { // in this case, we can direclty load the current version
			//persist_read_data(SETTINGS_PERSIST_KEY, &globalSettings, sizeof(Settings));
			//persist_delete(SETTINGS_PERSIST_KEY);
		//} else { // in this case, we're upgrading from the old version

		//}
	} else {
		// if there weren't any saved settings, load the defaults
		Settings_loadAllDefaults();
	}
}

void Settings_loadAllDefaults() {
	// if no persistent data is available, use the defaults
}

void Settings_deinit() {
	// save settings to persistent storage
	//persist_write_data(SETTINGS_PERSIST_KEY, &globalSettings, sizeof(Settings));
}