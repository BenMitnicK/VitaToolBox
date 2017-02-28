/*
	VitaShell
	Copyright (C) 2015-2017, TheFloW and BenmitnicK

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"
#include "config.h"
#include "language.h"

INCLUDE_EXTERN_RESOURCE(english_us_ini);

static char *lang[] ={
	"english_us",
	"japanese",
	"french",
	"spanish",
	"german",
	"italian",
	"dutch",
	"portuguese",
	"russian",
	"korean",
	"chinese_t",
	"chinese_s",
	"finnish",
	"swedish",
	"danish",
	"norwegian",
	"polish",
	"portuguese_br",
	"turkish"
};

char *language_container[LANGUAGE_CONTRAINER_SIZE];

void freeLanguageContainer() {
	int i;
	for (i = 0; i < LANGUAGE_CONTRAINER_SIZE; i++) {
		if (language_container[i]) {
			free(language_container[i]);
			language_container[i] = NULL;
		}
	}
}

void loadLanguage(int id) {
	freeLanguageContainer();

	#define LANGUAGE_ENTRY(name) { #name, CONFIG_TYPE_STRING, (void *)&language_container[name] }
	ConfigEntry language_entries[] = {
		// General strings
		LANGUAGE_ENTRY(ERROR),
		LANGUAGE_ENTRY(OK),
		LANGUAGE_ENTRY(YES),
		LANGUAGE_ENTRY(NO),
		LANGUAGE_ENTRY(CANCEL),
		LANGUAGE_ENTRY(ON),
		LANGUAGE_ENTRY(OFF),
		LANGUAGE_ENTRY(FILE_),
		LANGUAGE_ENTRY(FOLDER),

		// Progress strings
		LANGUAGE_ENTRY(MOVING),
		LANGUAGE_ENTRY(COPYING),
		LANGUAGE_ENTRY(DELETING),
		LANGUAGE_ENTRY(EXPORTING),
		LANGUAGE_ENTRY(INSTALLING),
		LANGUAGE_ENTRY(DOWNLOADING),
		LANGUAGE_ENTRY(EXTRACTING),
		LANGUAGE_ENTRY(COMPRESSING),
		LANGUAGE_ENTRY(HASHING),

		// Audio player strings
		LANGUAGE_ENTRY(TITLE),
		LANGUAGE_ENTRY(ALBUM),
		LANGUAGE_ENTRY(ARTIST),
		LANGUAGE_ENTRY(GENRE),
		LANGUAGE_ENTRY(YEAR),

		// Hex editor strings
		LANGUAGE_ENTRY(OFFSET),
		LANGUAGE_ENTRY(OPEN_HEX_EDITOR),

		// Text editor strings
		LANGUAGE_ENTRY(EDIT_LINE),
		LANGUAGE_ENTRY(ENTER_SEARCH_TERM),
		LANGUAGE_ENTRY(CUT),
		LANGUAGE_ENTRY(INSERT_EMPTY_LINE),

		// File browser context menu strings
		LANGUAGE_ENTRY(MORE),
		LANGUAGE_ENTRY(MARK_ALL),
		LANGUAGE_ENTRY(UNMARK_ALL),
		LANGUAGE_ENTRY(MOVE),
		LANGUAGE_ENTRY(COPY),
		LANGUAGE_ENTRY(PASTE),
		LANGUAGE_ENTRY(DELETE),
		LANGUAGE_ENTRY(RENAME),
		LANGUAGE_ENTRY(NEW_FOLDER),
		LANGUAGE_ENTRY(PROPERTIES),
		LANGUAGE_ENTRY(COMPRESS),
		LANGUAGE_ENTRY(INSTALL_ALL),
		LANGUAGE_ENTRY(INSTALL_FOLDER),
		LANGUAGE_ENTRY(CALCULATE_SHA1),
		LANGUAGE_ENTRY(EXPORT_MEDIA),
		LANGUAGE_ENTRY(SEARCH),

		// File browser properties strings
		LANGUAGE_ENTRY(PROPERTY_NAME),
		LANGUAGE_ENTRY(PROPERTY_TYPE),
		LANGUAGE_ENTRY(PROPERTY_FSELF_MODE),
		LANGUAGE_ENTRY(PROPERTY_SIZE),
		LANGUAGE_ENTRY(PROPERTY_COMPRESSED_SIZE),
		LANGUAGE_ENTRY(PROPERTY_CONTAINS),
		LANGUAGE_ENTRY(PROPERTY_CONTAINS_FILES_FOLDERS),
		LANGUAGE_ENTRY(PROPERTY_CREATION_DATE),
		LANGUAGE_ENTRY(PROPERTY_MODFICATION_DATE),
		LANGUAGE_ENTRY(PROPERTY_TYPE_BMP),
		LANGUAGE_ENTRY(PROPERTY_TYPE_INI),
		LANGUAGE_ENTRY(PROPERTY_TYPE_JPEG),
		LANGUAGE_ENTRY(PROPERTY_TYPE_MP3),
		LANGUAGE_ENTRY(PROPERTY_TYPE_OGG),
		LANGUAGE_ENTRY(PROPERTY_TYPE_PNG),
		LANGUAGE_ENTRY(PROPERTY_TYPE_RAR),
		LANGUAGE_ENTRY(PROPERTY_TYPE_SFO),
		LANGUAGE_ENTRY(PROPERTY_TYPE_TXT),
		LANGUAGE_ENTRY(PROPERTY_TYPE_VPK),
		LANGUAGE_ENTRY(PROPERTY_TYPE_XML),
		LANGUAGE_ENTRY(PROPERTY_TYPE_ZIP),
		LANGUAGE_ENTRY(PROPERTY_TYPE_FSELF),
		LANGUAGE_ENTRY(PROPERTY_FSELF_MODE_SAFE),
		LANGUAGE_ENTRY(PROPERTY_FSELF_MODE_UNSAFE),
		LANGUAGE_ENTRY(PROPERTY_FSELF_MODE_SCE),

		// File browser strings
		LANGUAGE_ENTRY(COPIED_FILE),
		LANGUAGE_ENTRY(COPIED_FOLDER),
		LANGUAGE_ENTRY(COPIED_FILES_FOLDERS),

		// Dialog questions
		LANGUAGE_ENTRY(DELETE_FILE_QUESTION),
		LANGUAGE_ENTRY(DELETE_FOLDER_QUESTION),
		LANGUAGE_ENTRY(DELETE_FILES_FOLDERS_QUESTION),
		LANGUAGE_ENTRY(EXPORT_FILE_QUESTION),
		LANGUAGE_ENTRY(EXPORT_FOLDER_QUESTION),
		LANGUAGE_ENTRY(EXPORT_FILES_FOLDERS_QUESTION),
		LANGUAGE_ENTRY(EXPORT_NO_MEDIA),
		LANGUAGE_ENTRY(EXPORT_SONGS_INFO),
		LANGUAGE_ENTRY(EXPORT_PICTURES_INFO),
		LANGUAGE_ENTRY(EXPORT_SONGS_PICTURES_INFO),
		LANGUAGE_ENTRY(INSTALL_ALL_QUESTION),
		LANGUAGE_ENTRY(INSTALL_FOLDER_QUESTION),
		LANGUAGE_ENTRY(INSTALL_QUESTION),
		LANGUAGE_ENTRY(INSTALL_WARNING),
		LANGUAGE_ENTRY(INSTALL_BRICK_WARNING),
		LANGUAGE_ENTRY(HASH_FILE_QUESTION),

		// HENkaku settings strings
		LANGUAGE_ENTRY(HENKAKU_SETTINGS),
		LANGUAGE_ENTRY(HENKAKU_ENABLE_PSN_SPOOFING),
		LANGUAGE_ENTRY(HENKAKU_ENABLE_UNSAFE_HOMEBREW),
		LANGUAGE_ENTRY(HENKAKU_ENABLE_VERSION_SPOOFING),
		LANGUAGE_ENTRY(HENKAKU_SPOOFED_VERSION),
		LANGUAGE_ENTRY(HENKAKU_RESTORE_DEFAULT_SETTINGS),
		LANGUAGE_ENTRY(HENKAKU_RELOAD_CONFIG),
		LANGUAGE_ENTRY(HENKAKU_RESTORE_DEFAULT_MESSAGE),
		LANGUAGE_ENTRY(HENKAKU_RELOAD_CONFIG_MESSAGE),
		LANGUAGE_ENTRY(HENKAKU_UNSAFE_HOMEBREW_MESSAGE),

		// VitaShell settings
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_MAIN),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_LANGUAGE),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_THEME),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_SELECT_BUTTON),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_NO_AUTO_UPDATE),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_DATABASE),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_DATABASE_UPDATE),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_DATABASE_REBUILD),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_POWER),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_REBOOT),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_POWEROFF),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_STANDBY),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_SELECT_BUTTON_USB),
		LANGUAGE_ENTRY(VITASHELL_SETTINGS_SELECT_BUTTON_FTP),

		// Language Settings
		LANGUAGE_ENTRY(english_us),
		LANGUAGE_ENTRY(japanese),
		LANGUAGE_ENTRY(french),
		LANGUAGE_ENTRY(spanish),
		LANGUAGE_ENTRY(german),
		LANGUAGE_ENTRY(italian),
		LANGUAGE_ENTRY(dutch),
		LANGUAGE_ENTRY(portuguese),
		LANGUAGE_ENTRY(russian),
		LANGUAGE_ENTRY(korean),
		LANGUAGE_ENTRY(chinese_t),
		LANGUAGE_ENTRY(chinese_s),
		LANGUAGE_ENTRY(finnish),
		LANGUAGE_ENTRY(swedish),
		LANGUAGE_ENTRY(danish),
		LANGUAGE_ENTRY(norwegian),
		LANGUAGE_ENTRY(polish),
		LANGUAGE_ENTRY(portuguese_br),
		LANGUAGE_ENTRY(turkish),
		
		// ThemeManager
		LANGUAGE_ENTRY(THEMEMANAGER_SELECT),
		LANGUAGE_ENTRY(THEMEMANAGER_CANCEL),
		
		// USB strings
		LANGUAGE_ENTRY(USB_CONNECTED),
		LANGUAGE_ENTRY(USB_NOT_CONNECTED),
		LANGUAGE_ENTRY(USB_CONNECTION_PERMISSION),
		LANGUAGE_ENTRY(USB_CONNECTION_NOT_AVAILABLE),

		// DataBase
		LANGUAGE_ENTRY(DATABASEUPDATEQUESTION),
		LANGUAGE_ENTRY(DATABASEREBUILDQUESTION),
		LANGUAGE_ENTRY(DATABASEUPDATECONFIRMQUESTION),
		LANGUAGE_ENTRY(DATABASEREBUILDCONFIRMQUESTION),
		
		// Others
		LANGUAGE_ENTRY(SAFE_MODE),
		LANGUAGE_ENTRY(UNSAFE_MODE),
		LANGUAGE_ENTRY(TOOLBOX),
		LANGUAGE_ENTRY(SYSINFO_TITLE),
		LANGUAGE_ENTRY(PLEASE_WAIT),
		LANGUAGE_ENTRY(SAVE_MODIFICATIONS),
		LANGUAGE_ENTRY(NO_SPACE_ERROR),
		LANGUAGE_ENTRY(WIFI_ERROR),
		LANGUAGE_ENTRY(FTP_SERVER),
		LANGUAGE_ENTRY(SYS_INFO),
		LANGUAGE_ENTRY(UPDATE_QUESTION),
		LANGUAGE_ENTRY(ARCHIVE_NAME),
		LANGUAGE_ENTRY(COMPRESSION_LEVEL),
	};

	// Load default config file
	readConfigBuffer(&_binary_resources_english_us_ini_start, (int)&_binary_resources_english_us_ini_size, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));

	// Load custom config file
	if (use_custom_config) {
		if (id >= 0 && id < (sizeof(lang) / sizeof(char *))) {
			char path[128];
			switch (vitashell_config.select_lang) {
				case 0:
					sprintf(path, "ux0:VitaToolBox/language/english_us.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
				
				case 1:
					sprintf(path, "ux0:VitaToolBox/language/japanese.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 2:
					sprintf(path, "ux0:VitaToolBox/language/french.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 3:
					sprintf(path, "ux0:VitaToolBox/language/spanish.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 4:
					sprintf(path, "ux0:VitaToolBox/language/german.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
				
				case 5:
					sprintf(path, "ux0:VitaToolBox/language/italian.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 6:
					sprintf(path, "ux0:VitaToolBox/language/dutch.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 7:
					sprintf(path, "ux0:VitaToolBox/language/portuguese.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 8:
					sprintf(path, "ux0:VitaToolBox/language/russian.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
				
				case 9:
					sprintf(path, "ux0:VitaToolBox/language/korean.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 10:
					sprintf(path, "ux0:VitaToolBox/language/chinese_t.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 11:
					sprintf(path, "ux0:VitaToolBox/language/chinese_s.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 12:
					sprintf(path, "ux0:VitaToolBox/language/finnish.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
				
				case 13:
					sprintf(path, "ux0:VitaToolBox/language/swedish.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 14:
					sprintf(path, "ux0:VitaToolBox/language/danish.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 15:
					sprintf(path, "ux0:VitaToolBox/language/norwegian.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 16:
					sprintf(path, "ux0:VitaToolBox/language/polish.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
				
				case 17:
					sprintf(path, "ux0:VitaToolBox/language/portuguese_br.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
					
				case 18:
					sprintf(path, "ux0:VitaToolBox/language/turkish.ini");
					readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
					break;
				}
			
			//sprintf(path, "ux0:VitaToolBox/language/%s.txt", lang[id]);
			//readConfig(path, language_entries, sizeof(language_entries) / sizeof(ConfigEntry));
		}
	}
}
