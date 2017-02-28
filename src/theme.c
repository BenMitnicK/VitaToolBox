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
#include "init.h"
#include "file.h"
#include "config.h"
#include "theme.h"
#include "utils.h"

INCLUDE_EXTERN_RESOURCE(bg_wallpaper_png);

INCLUDE_EXTERN_RESOURCE(colors_ini);
INCLUDE_EXTERN_RESOURCE(colors_ini_size);

INCLUDE_EXTERN_RESOURCE(param_ini);
INCLUDE_EXTERN_RESOURCE(param_ini_size);

INCLUDE_EXTERN_RESOURCE(folder_icon_png);
INCLUDE_EXTERN_RESOURCE(file_icon_png);
INCLUDE_EXTERN_RESOURCE(archive_icon_png);
INCLUDE_EXTERN_RESOURCE(image_icon_png);
INCLUDE_EXTERN_RESOURCE(audio_icon_png);
INCLUDE_EXTERN_RESOURCE(sfo_icon_png);
INCLUDE_EXTERN_RESOURCE(text_icon_png);
INCLUDE_EXTERN_RESOURCE(wifi_png);
INCLUDE_EXTERN_RESOURCE(ftp_png);
INCLUDE_EXTERN_RESOURCE(battery_png);
INCLUDE_EXTERN_RESOURCE(battery_bar_red_png);
INCLUDE_EXTERN_RESOURCE(battery_bar_green_png);
INCLUDE_EXTERN_RESOURCE(battery_bar_charge_png);

INCLUDE_EXTERN_RESOURCE(bg_browser_png);
INCLUDE_EXTERN_RESOURCE(bg_hexeditor_png);
INCLUDE_EXTERN_RESOURCE(bg_texteditor_png);
INCLUDE_EXTERN_RESOURCE(bg_photoviewer_png);
INCLUDE_EXTERN_RESOURCE(bg_audioplayer_png);
INCLUDE_EXTERN_RESOURCE(bg_selecttheme_png);
INCLUDE_EXTERN_RESOURCE(context_png);
INCLUDE_EXTERN_RESOURCE(context_more_png);
INCLUDE_EXTERN_RESOURCE(dialog_png);

INCLUDE_EXTERN_RESOURCE(cover_png);
INCLUDE_EXTERN_RESOURCE(play_png);
INCLUDE_EXTERN_RESOURCE(pause_png);
INCLUDE_EXTERN_RESOURCE(fastforward_png);
INCLUDE_EXTERN_RESOURCE(fastrewind_png);

// THEME POSITION
int SQUARE_x_POSITION;
int SQUARE_y_POSITION;
int TRIANGLE_x_POSITION;
int TRIANGLE_y_POSITION;
int SELECT_x_POSITION;
int SELECT_y_POSITION;
int CANCEL_x_POSITION;
int CANCEL_y_POSITION;
int NAME_x_POSITION;
int NAME_y_POSITION;
int NAME_CENTER_DIM;
int NAME_Size;
int PREVIEW_x_POSITION;
int PREVIEW_y_POSITION;
int SCALE_x_POSITION;
int SCALE_y_POSITION;
int FILTER_R_COLOR;
int FILTER_G_COLOR;
int FILTER_B_COLOR;
int FILTER_A_COLOR;
int FILTER_rW;
int FILTER_rH;
int FILTER_ACTIVE;
int FONT_SIZE_CUSTOM;

// Shell colors
int BACKGROUND_COLOR;
int TITLE_COLOR;
int PATH_COLOR;
int DATE_TIME_COLOR;

// Settings color
int SETTINGS_MENU_COLOR;
int SETTINGS_MENU_FOCUS_COLOR;
int SETTINGS_MENU_TITLE_COLOR;
int SETTINGS_MENU_ITEM_COLOR;
int SETTINGS_MENU_OPTION_COLOR;

// DataBase color
int DATABASE_MENU_COLOR;
int DATABASE_MENU_FOCUS_COLOR;
int DATABASE_MENU_TITLE_COLOR;
int DATABASE_MENU_ITEM_COLOR;
int DATABASE_MENU_OPTION_COLOR;

// PluginsManager colors
int PLUGINSMANAGER_MENU_COLOR;
int PLUGINSMANAGER_MENU_FOCUS_COLOR;
int PLUGINSMANAGER_MENU_TITLE_COLOR;
int PLUGINSMANAGER_NAME_COLOR;
int PLUGINSMANAGER_TYPE_COLOR;
int PLUGINSMANAGER_STATUS_COLOR;

// ThemeManager colors
int THEMEMANAGER_SELECT_COLOR;
int THEMEMANAGER_CANCEL_COLOR;
int THEMEMANAGER_ITEM_COLOR;

// Vitaident colors
int VITAIDENT_KERNEL_NAME_COLOR;
int VITAIDENT_KERNEL_ITEM_COLOR;
int VITAIDENT_SYSTEM_NAME_COLOR;
int VITAIDENT_SYSTEM_ITEM_COLOR;
int VITAIDENT_BATTERY_NAME_COLOR;
int VITAIDENT_BATTERY_ITEM_COLOR;
int VITAIDENT_MISC_NAME_COLOR;
int VITAIDENT_MISC_ITEM_COLOR;

// File browser colors
int FOCUS_COLOR;
int FILE_COLOR;
int SFO_COLOR;
int TXT_COLOR;
int FOLDER_COLOR;
int IMAGE_COLOR;
int ARCHIVE_COLOR;
int SCROLL_BAR_COLOR;
int SCROLL_BAR_BG_COLOR;
int MARKED_COLOR;

// Context menu colors
int CONTEXT_MENU_TEXT_COLOR;
int CONTEXT_MENU_FOCUS_COLOR;
int CONTEXT_MENU_COLOR;
int CONTEXT_MENU_MORE_COLOR;
int INVISIBLE_COLOR;

// Dialog colors
int DIALOG_COLOR;
int DIALOG_BG_COLOR;
int PROGRESS_BAR_COLOR;
int PROGRESS_BAR_BG_COLOR;

// Hex editor colors
int HEX_COLOR;
int HEX_OFFSET_COLOR;
int HEX_NIBBLE_COLOR;

// Text editor colors
int TEXT_COLOR;
int TEXT_FOCUS_COLOR;
int TEXT_LINE_NUMBER_COLOR;
int TEXT_LINE_NUMBER_COLOR_FOCUS;
int TEXT_HIGHLIGHT_COLOR;

// Photo viewer colors
int PHOTO_ZOOM_COLOR;

// Audio player colors
int AUDIO_INFO_ASSIGN;
int AUDIO_INFO;
int AUDIO_SPEED;
int AUDIO_TIME_CURRENT;
int AUDIO_TIME_SLASH;
int AUDIO_TIME_TOTAL;
int AUDIO_TIME_BAR;
int AUDIO_TIME_BAR_BG;

vita2d_texture *default_wallpaper = NULL;

vita2d_texture *folder_icon = NULL, *file_icon = NULL, *archive_icon = NULL, *image_icon = NULL, *audio_icon = NULL, *sfo_icon = NULL, *text_icon = NULL,
			   *wifi_image = NULL, *ftp_image = NULL, *dialog_image = NULL, *context_image = NULL, *context_more_image = NULL, *settings_image = NULL, *database_image = NULL, *p_manager_image = NULL, *battery_image = NULL,
			   *battery_bar_red_image = NULL, *battery_bar_green_image = NULL, *battery_bar_charge_image = NULL, *bg_browser_image = NULL, *bg_hex_image = NULL, *bg_text_image = NULL,
			   *bg_photo_image = NULL, *bg_select_theme_image = NULL, *bg_audio_image = NULL, *cover_image = NULL, *play_image = NULL, *pause_image = NULL, *fastforward_image = NULL, *fastrewind_image = NULL;

vita2d_texture *wallpaper_image[MAX_WALLPAPERS];

vita2d_texture *previous_wallpaper_image = NULL, *current_wallpaper_image = NULL;

int wallpaper_count = 0;

float SIZE_FONT_CUSTOM;

vita2d_pgf *font = NULL;
char font_size_cache[256];

typedef struct {
	char *name;
	void *default_buf;
	vita2d_texture **texture;
} ThemeImage;

ThemeImage theme_images[] = {
	{ "folder_icon.png", &_binary_resources_folder_icon_png_start, &folder_icon },
	{ "file_icon.png", &_binary_resources_file_icon_png_start, &file_icon },
	{ "archive_icon.png", &_binary_resources_archive_icon_png_start, &archive_icon },
	{ "image_icon.png", &_binary_resources_image_icon_png_start, &image_icon },
	{ "audio_icon.png", &_binary_resources_audio_icon_png_start, &audio_icon },
	{ "sfo_icon.png", &_binary_resources_sfo_icon_png_start, &sfo_icon },
	{ "text_icon.png", &_binary_resources_text_icon_png_start, &text_icon },
	{ "wifi.png", &_binary_resources_wifi_png_start, &wifi_image },
	{ "ftp.png", &_binary_resources_ftp_png_start, &ftp_image },
	{ "dialog.png", &_binary_resources_dialog_png_start, &dialog_image },
	{ "context.png", &_binary_resources_context_png_start, &context_image },
	{ "context_more.png", &_binary_resources_context_more_png_start, &context_more_image },
	{ "settings.png", NULL, &settings_image },
	{ "manager.png", NULL, &p_manager_image },
	{ "database.png", NULL, &database_image },
	{ "battery.png", &_binary_resources_battery_png_start, &battery_image },
	{ "battery_bar_red.png", &_binary_resources_battery_bar_red_png_start, &battery_bar_red_image },
	{ "battery_bar_green.png", &_binary_resources_battery_bar_green_png_start, &battery_bar_green_image },
	{ "battery_bar_charge.png", &_binary_resources_battery_bar_charge_png_start, &battery_bar_charge_image },
	{ "bg_browser.png", NULL, &bg_browser_image },
	{ "bg_hexeditor.png", NULL, &bg_hex_image },
	{ "bg_texteditor.png", NULL, &bg_text_image },
	{ "bg_photoviewer.png", NULL, &bg_photo_image },
	{ "bg_audioplayer.png", NULL, &bg_audio_image },
	{ "bg_selecttheme.png", NULL, &bg_select_theme_image },
	{ "cover.png", &_binary_resources_cover_png_start, &cover_image },
	{ "play.png", &_binary_resources_play_png_start, &play_image },
	{ "pause.png", &_binary_resources_pause_png_start, &pause_image },
	{ "fastforward.png", &_binary_resources_fastforward_png_start, &fastforward_image },
	{ "fastrewind.png", &_binary_resources_fastrewind_png_start, &fastrewind_image },
	//{ "wallpaper.png", &_binary_resources_bg_wallpaper_png_start, &default_wallpaper },
};

#define N_THEME_IMAGES (sizeof(theme_images) / sizeof(ThemeImage))

void loadTheme() {
	#define COLOR_ENTRY(name) { #name, CONFIG_TYPE_HEXDECIMAL, (void *)&name }
	ConfigEntry colors_entries[] = {
		// Shell colors
		COLOR_ENTRY(BACKGROUND_COLOR),
		COLOR_ENTRY(TITLE_COLOR),
		COLOR_ENTRY(PATH_COLOR),
		COLOR_ENTRY(DATE_TIME_COLOR),

		// Settings colors
		COLOR_ENTRY(SETTINGS_MENU_COLOR),
		COLOR_ENTRY(SETTINGS_MENU_FOCUS_COLOR),
		COLOR_ENTRY(SETTINGS_MENU_TITLE_COLOR),
		COLOR_ENTRY(SETTINGS_MENU_ITEM_COLOR),
		COLOR_ENTRY(SETTINGS_MENU_OPTION_COLOR),

		// DataBase colors
		COLOR_ENTRY(DATABASE_MENU_COLOR),
		COLOR_ENTRY(DATABASE_MENU_FOCUS_COLOR),
		COLOR_ENTRY(DATABASE_MENU_TITLE_COLOR),
		COLOR_ENTRY(DATABASE_MENU_ITEM_COLOR),
		COLOR_ENTRY(DATABASE_MENU_OPTION_COLOR),
		
		// PluginsManager colors
		COLOR_ENTRY(PLUGINSMANAGER_MENU_COLOR),
		COLOR_ENTRY(PLUGINSMANAGER_MENU_FOCUS_COLOR),
		COLOR_ENTRY(PLUGINSMANAGER_MENU_TITLE_COLOR),
		COLOR_ENTRY(PLUGINSMANAGER_NAME_COLOR),
		COLOR_ENTRY(PLUGINSMANAGER_TYPE_COLOR),
		COLOR_ENTRY(PLUGINSMANAGER_STATUS_COLOR),
		
		// ThemeManager colors
		COLOR_ENTRY(THEMEMANAGER_SELECT_COLOR),
		COLOR_ENTRY(THEMEMANAGER_CANCEL_COLOR),
		COLOR_ENTRY(THEMEMANAGER_ITEM_COLOR),
		
		// Vitaident colors
		COLOR_ENTRY(VITAIDENT_KERNEL_NAME_COLOR),
		COLOR_ENTRY(VITAIDENT_KERNEL_ITEM_COLOR),
		COLOR_ENTRY(VITAIDENT_SYSTEM_NAME_COLOR),
		COLOR_ENTRY(VITAIDENT_SYSTEM_ITEM_COLOR),
		COLOR_ENTRY(VITAIDENT_BATTERY_NAME_COLOR),
		COLOR_ENTRY(VITAIDENT_BATTERY_ITEM_COLOR),
		COLOR_ENTRY(VITAIDENT_MISC_NAME_COLOR),
		COLOR_ENTRY(VITAIDENT_MISC_ITEM_COLOR),
		
		// File browser colors
		COLOR_ENTRY(FOCUS_COLOR),
		COLOR_ENTRY(FILE_COLOR),
		COLOR_ENTRY(SFO_COLOR),
		COLOR_ENTRY(TXT_COLOR),
		COLOR_ENTRY(FOLDER_COLOR),
		COLOR_ENTRY(IMAGE_COLOR),
		COLOR_ENTRY(ARCHIVE_COLOR),
		COLOR_ENTRY(SCROLL_BAR_COLOR),
		COLOR_ENTRY(SCROLL_BAR_BG_COLOR),
		COLOR_ENTRY(MARKED_COLOR),

		// Context menu colors
		COLOR_ENTRY(CONTEXT_MENU_TEXT_COLOR),
		COLOR_ENTRY(CONTEXT_MENU_FOCUS_COLOR),
		COLOR_ENTRY(CONTEXT_MENU_COLOR),
		COLOR_ENTRY(CONTEXT_MENU_MORE_COLOR),
		COLOR_ENTRY(INVISIBLE_COLOR),

		// Dialog colors
		COLOR_ENTRY(DIALOG_COLOR),
		COLOR_ENTRY(DIALOG_BG_COLOR),
		COLOR_ENTRY(PROGRESS_BAR_COLOR),
		COLOR_ENTRY(PROGRESS_BAR_BG_COLOR),

		// Hex editor colors
		COLOR_ENTRY(HEX_COLOR),
		COLOR_ENTRY(HEX_OFFSET_COLOR),
		COLOR_ENTRY(HEX_NIBBLE_COLOR),

		// Text editor colors
		COLOR_ENTRY(TEXT_COLOR),
		COLOR_ENTRY(TEXT_FOCUS_COLOR),
		COLOR_ENTRY(TEXT_LINE_NUMBER_COLOR),
		COLOR_ENTRY(TEXT_LINE_NUMBER_COLOR_FOCUS),
		COLOR_ENTRY(TEXT_HIGHLIGHT_COLOR),

		// Photo viewer colors
		COLOR_ENTRY(PHOTO_ZOOM_COLOR),

		// Audio player colors
		COLOR_ENTRY(AUDIO_INFO_ASSIGN),
		COLOR_ENTRY(AUDIO_INFO),
		COLOR_ENTRY(AUDIO_SPEED),
		COLOR_ENTRY(AUDIO_TIME_CURRENT),
		COLOR_ENTRY(AUDIO_TIME_SLASH),
		COLOR_ENTRY(AUDIO_TIME_TOTAL),
		COLOR_ENTRY(AUDIO_TIME_BAR),
		COLOR_ENTRY(AUDIO_TIME_BAR_BG),
	};

	// Load default config file
	readConfigBuffer(&_binary_resources_colors_ini_start, (int)&_binary_resources_colors_ini_size, colors_entries, sizeof(colors_entries) / sizeof(ConfigEntry));

	#define POSITION_ENTRY(name) { #name, CONFIG_TYPE_DECIMAL, (void *)&name }
	ConfigEntry position_entries[] = {
		// THEME POSITION
		POSITION_ENTRY(SQUARE_x_POSITION),
		POSITION_ENTRY(SQUARE_y_POSITION),
		POSITION_ENTRY(TRIANGLE_x_POSITION),
		POSITION_ENTRY(TRIANGLE_y_POSITION),
		POSITION_ENTRY(SELECT_x_POSITION),
		POSITION_ENTRY(SELECT_y_POSITION),
		POSITION_ENTRY(CANCEL_x_POSITION),
		POSITION_ENTRY(CANCEL_y_POSITION),
		POSITION_ENTRY(NAME_x_POSITION),
		POSITION_ENTRY(NAME_y_POSITION),
		POSITION_ENTRY(NAME_CENTER_DIM),
		POSITION_ENTRY(PREVIEW_x_POSITION),
		POSITION_ENTRY(PREVIEW_y_POSITION),
		POSITION_ENTRY(FILTER_R_COLOR),
		POSITION_ENTRY(FILTER_G_COLOR),
		POSITION_ENTRY(FILTER_B_COLOR),
		POSITION_ENTRY(FILTER_A_COLOR),
		POSITION_ENTRY(FILTER_rW),
		POSITION_ENTRY(FILTER_rH),
		POSITION_ENTRY(FILTER_ACTIVE),
	//#define POSITION2_ENTRY(name) { #name, CONFIG_TYPE2_DECIMAL, (void *)&name }	
		POSITION_ENTRY(SCALE_x_POSITION),
		POSITION_ENTRY(SCALE_y_POSITION),
		POSITION_ENTRY(NAME_Size),
		POSITION_ENTRY(FONT_SIZE_CUSTOM),
	};

	int i;
	
	// Load default config file
	readConfigBuffer(&_binary_resources_param_ini_start, (int)&_binary_resources_param_ini_size, position_entries, sizeof(position_entries) / sizeof(ConfigEntry));

	
	// Load custom config file
	if (use_custom_config) {
		char path[MAX_PATH_LENGTH];
		
		char *theme_name = NULL;
		ConfigEntry theme_entries[] = {
			{ "THEME_NAME", CONFIG_TYPE_STRING, (void *)&theme_name },
		};
		
		// Load theme config
		readConfig("ux0:VitaToolBox/settings.ini", theme_entries, sizeof(theme_entries) / sizeof(ConfigEntry));
		
		if (theme_name) {
			// Load colors config
			snprintf(path, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/colors.ini", theme_name);
			readConfig(path, colors_entries, sizeof(colors_entries) / sizeof(ConfigEntry));
			
			// Load position config
			snprintf(path, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/param.ini", theme_name);
			readConfig(path, position_entries, sizeof(position_entries) / sizeof(ConfigEntry));
			
			// Font
			snprintf(path, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/font.pgf", theme_name);
 			font = vita2d_load_custom_pgf(path);
			
			// Wallpapers
			snprintf(path, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/wallpaper.png", theme_name);
			vita2d_texture *image = vita2d_load_PNG_file(path);
			if (image)
				wallpaper_image[wallpaper_count++] = image;
			
			int z;
			for (z = 1; z < MAX_WALLPAPERS; z++) {
				snprintf(path, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/wallpaper%d.png", theme_name, z + 1);
				vita2d_texture *image = vita2d_load_PNG_file(path);
				if (image)
					wallpaper_image[wallpaper_count++] = image;
			}

			// Set random wallpaper
			if (wallpaper_count > 0) {
				int random_num = randomNumber(0, wallpaper_count - 1);
				current_wallpaper_image = wallpaper_image[random_num];
			}
			
			// Load theme
			for (i = 0; i < N_THEME_IMAGES; i++) {
				snprintf(path, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/%s", theme_name, theme_images[i].name);
				if (theme_images[i].texture && *(theme_images[i].texture) == NULL)
					*(theme_images[i].texture) = vita2d_load_PNG_file(path);
			}
		}
	}

	// Load default theme
	for (i = 0; i < N_THEME_IMAGES; i++) {
		if (theme_images[i].texture && *(theme_images[i].texture) == NULL && theme_images[i].default_buf)
			*(theme_images[i].texture) = vita2d_load_PNG_buffer(theme_images[i].default_buf);
	}

	// Load default pngs
	if (!dialog_image) {
		dialog_image = vita2d_create_empty_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
		void *data = vita2d_texture_get_datap(dialog_image);

		int y;
		for (y = 0; y < SCREEN_HEIGHT; y++) {
			int x;
			for (x = 0; x < SCREEN_WIDTH; x++) {
				((uint32_t *)data)[x + SCREEN_WIDTH * y] = DIALOG_BG_COLOR;
			}
		}
	}

	if (!context_image) {
		context_image = vita2d_create_empty_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
		void *data = vita2d_texture_get_datap(context_image);

		int y;
		for (y = 0; y < SCREEN_HEIGHT; y++) {
			int x;
			for (x = 0; x < SCREEN_WIDTH; x++) {
				((uint32_t *)data)[x + SCREEN_WIDTH * y] = CONTEXT_MENU_COLOR;
			}
		}
	}

	if (!context_more_image) {
		context_more_image = vita2d_create_empty_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
		void *data = vita2d_texture_get_datap(context_more_image);

		int y;
		for (y = 0; y < SCREEN_HEIGHT; y++) {
			int x;
			for (x = 0; x < SCREEN_WIDTH; x++) {
				((uint32_t *)data)[x + SCREEN_WIDTH * y] = CONTEXT_MENU_MORE_COLOR;
			}
		}
	}

	if (!settings_image) {
		settings_image = vita2d_create_empty_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
		void *data = vita2d_texture_get_datap(settings_image);

		int y;
		for (y = 0; y < SCREEN_HEIGHT; y++) {
			int x;
			for (x = 0; x < SCREEN_WIDTH; x++) {
				((uint32_t *)data)[x + SCREEN_WIDTH * y] = SETTINGS_MENU_COLOR;
			}
		}
	}

	if (!database_image) {
		database_image = vita2d_create_empty_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
		void *data = vita2d_texture_get_datap(database_image);

		int y;
		for (y = 0; y < SCREEN_HEIGHT; y++) {
			int x;
			for (x = 0; x < SCREEN_WIDTH; x++) {
				((uint32_t *)data)[x + SCREEN_WIDTH * y] = DATABASE_MENU_COLOR;
			}
		}
	}
	
	if (!p_manager_image) {
		p_manager_image = vita2d_create_empty_texture(SCREEN_WIDTH, SCREEN_HEIGHT);
		void *data = vita2d_texture_get_datap(p_manager_image);

		int y;
		for (y = 0; y < SCREEN_HEIGHT; y++) {
			int x;
			for (x = 0; x < SCREEN_WIDTH; x++) {
				((uint32_t *)data)[x + SCREEN_WIDTH * y] = PLUGINSMANAGER_MENU_COLOR;
			}
		}
	}
	
	default_wallpaper = vita2d_load_PNG_buffer(&_binary_resources_bg_wallpaper_png_start);
	
	// Load system fonts
	if (!font)
		font = loadSystemFonts();

	char _param[MAX_PATH_LENGTH];
	snprintf(_param, MAX_PATH_LENGTH, "1.%d", FONT_SIZE_CUSTOM);
	SIZE_FONT_CUSTOM = atof(_param);
	
	// Font size cache
	for (i = 0; i < 256; i++) {
		char character[2];
		character[0] = i;
		character[1] = '\0';

		font_size_cache[i] = vita2d_pgf_text_width(font, SIZE_FONT_CUSTOM, character);
	}
}