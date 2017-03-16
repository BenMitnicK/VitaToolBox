/*
	VitaShell
	Copyright (C) 2015-2017, TheFloW And BenMitnicK

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
#include "init.h"
#include "theme.h"
#include "language.h"
#include "settings.h"
#include "message_dialog.h"
#include "ime_dialog.h"
#include "utils.h"
#include "file.h"

#include "henkaku_config.h"

/*
	* HENkaku settings *
	- Enable PSN spoofing
	- Enable unsafe homebrew
	- Enable version spoofing
	- Spoofed version

	* Main *
	- Language
	- Theme
	- CPU
	- Disable auto-update

	* FTP *
	- Auto-start

	* Status bar *
	- Display battery percentage
*/

char array_buffer[256][30];
char *themeName;
int count = 0;
int NavOffset = 0;

extern unsigned char _binary_resources_no_preview_png_start;
extern unsigned char _binary_resources_square_png_start;
extern unsigned char _binary_resources_triangle_png_start;

int taiReloadConfig();
int henkaku_reload_config();

SceTouchData touch;

void taihenReloadConfig();
void henkakuRestoreDefaultSettings();
void rebootDevice();
void shutdownDevice();
void suspendDevice();

static int changed = 0;

static HENkakuConfig henkaku_config;

static char spoofed_version[6];

static SettingsMenuEntry *settings_menu_entries = NULL;
static int n_settings_entries = 0;

static char *select_button_options[2];
static char *select_lang_options[19];
static char select_theme_options[256] = "";

static ConfigEntry settings_entries[] = {
	{ "SELECT_BUTTON", CONFIG_TYPE_DECIMAL, (int *)&vitashell_config.select_button },
	{ "DISABLE_AUTOUPDATE", CONFIG_TYPE_BOOLEAN, (int *)&vitashell_config.disable_autoupdate },
	{ "LANGUAGE", CONFIG_TYPE_DECIMAL, (int *)&vitashell_config.select_lang },
	{ "THEME_NAME", CONFIG_TYPE_STRING, (char *)&vitashell_config.select_theme },
};

SettingsMenuOption henkaku_settings[] = {
	{ HENKAKU_ENABLE_PSN_SPOOFING,			SETTINGS_OPTION_TYPE_BOOLEAN, NULL, NULL, 0, NULL, 0, &henkaku_config.use_psn_spoofing },
	{ HENKAKU_ENABLE_UNSAFE_HOMEBREW,		SETTINGS_OPTION_TYPE_BOOLEAN, NULL, NULL, 0, NULL, 0, &henkaku_config.allow_unsafe_hb },
	{ HENKAKU_ENABLE_VERSION_SPOOFING,		SETTINGS_OPTION_TYPE_BOOLEAN, NULL, NULL, 0, NULL, 0, &henkaku_config.use_spoofed_version },
	{ HENKAKU_SPOOFED_VERSION,			SETTINGS_OPTION_TYPE_STRING, NULL, spoofed_version, sizeof(spoofed_version) - 1, NULL, 0, NULL },
	{ HENKAKU_RESTORE_DEFAULT_SETTINGS,		SETTINGS_OPTION_TYPE_CALLBACK, (void *)henkakuRestoreDefaultSettings, NULL, 0, NULL, 0, NULL },
	{ HENKAKU_RELOAD_CONFIG,			SETTINGS_OPTION_TYPE_CALLBACK, (void *)taihenReloadConfig, NULL, 0, NULL, 0, NULL },
};

SettingsMenuOption main_settings[] = {
	
	{ VITASHELL_SETTINGS_LANGUAGE,			SETTINGS_OPTION2_TYPE_OPTIONS, NULL, NULL, 0, 
											select_lang_options, sizeof(select_lang_options) / sizeof(char **), 
											&vitashell_config.select_lang },
											
	{ VITASHELL_SETTINGS_THEME,			SETTINGS_OPTION2_TYPE_STRING, NULL, select_theme_options, sizeof(select_theme_options), NULL },
	
	{ VITASHELL_SETTINGS_SELECT_BUTTON,		SETTINGS_OPTION_TYPE_OPTIONS, NULL, NULL, 0,
											select_button_options, sizeof(select_button_options) / sizeof(char **),
											&vitashell_config.select_button },
	{ VITASHELL_SETTINGS_NO_AUTO_UPDATE,		SETTINGS_OPTION_TYPE_BOOLEAN, NULL, NULL, 0, NULL, 0, &vitashell_config.disable_autoupdate },
};

SettingsMenuOption power_settings[] = {
	{ VITASHELL_SETTINGS_REBOOT,			SETTINGS_OPTION_TYPE_CALLBACK, (void *)rebootDevice, NULL, 0, NULL, 0, NULL },
	{ VITASHELL_SETTINGS_POWEROFF,			SETTINGS_OPTION_TYPE_CALLBACK, (void *)shutdownDevice, NULL, 0, NULL, 0, NULL },
	{ VITASHELL_SETTINGS_STANDBY,			SETTINGS_OPTION_TYPE_CALLBACK, (void *)suspendDevice, NULL, 0, NULL, 0, NULL },
};

SettingsMenuEntry molecularshell_settings_menu_entries[] = {
	{ HENKAKU_SETTINGS, henkaku_settings, sizeof(henkaku_settings) / sizeof(SettingsMenuOption) },
	{ VITASHELL_SETTINGS_MAIN, main_settings, sizeof(main_settings) / sizeof(SettingsMenuOption) },
	{ VITASHELL_SETTINGS_POWER, power_settings, sizeof(power_settings) / sizeof(SettingsMenuOption) },
};

SettingsMenuEntry vitashell_settings_menu_entries[] = {
	{ VITASHELL_SETTINGS_MAIN, main_settings, sizeof(main_settings) / sizeof(SettingsMenuOption) },
	{ VITASHELL_SETTINGS_POWER, power_settings, sizeof(power_settings) / sizeof(SettingsMenuOption) },
};

static SettingsMenu settings_menu;

static float easeOut(float x0, float x1, float a) {
	float dx = (x1 - x0);
	return ((dx * a) > 0.01f) ? (dx * a) : dx;
}

void loadSettingsConfig() {
	// Load settings config file
	memset(&vitashell_config, 0, sizeof(VitaShellConfig));
	readConfig("ux0:VitaToolBox/settings.ini", settings_entries, sizeof(settings_entries) / sizeof(ConfigEntry));
}

void saveSettingsConfig() {
	// Save settings config file
	writeConfig("ux0:VitaToolBox/settings.ini", settings_entries, sizeof(settings_entries) / sizeof(ConfigEntry));
}

void rebootDevice() {
	closeSettingsMenu();
	scePowerRequestColdReset();
}

void shutdownDevice() {
	closeSettingsMenu();
	scePowerRequestStandby();
}

void suspendDevice() {
	closeSettingsMenu();
	scePowerRequestSuspend();
}

void henkakuRestoreDefaultSettings() {
	memset(&henkaku_config, 0, sizeof(HENkakuConfig));
	henkaku_config.use_psn_spoofing = 1;
	henkaku_config.use_spoofed_version = 1;
	strcpy(spoofed_version, HENKAKU_DEFAULT_VERSION_STRING);
	changed = 1;

	infoDialog(language_container[HENKAKU_RESTORE_DEFAULT_MESSAGE]);
}

void taihenReloadConfig() {
	taiReloadConfig();
	infoDialog(language_container[HENKAKU_RELOAD_CONFIG_MESSAGE]);
}

int doesFileExist(const char* path) { 
	SceUID dir = sceIoOpen(path, SCE_O_RDONLY, 0777); 
 	if (dir >= 0) { 
 		sceIoClose(dir); 
 		return 1; 
 	} else { 
 		return 0; 
 	} 
} 

void addtextures(vita2d_texture** themes,vita2d_texture* previewNotFoundImage) {
  
	SceUID dfd = sceIoDopen("ux0:VitaToolBox/theme/");
	char* themePreviewPath;
	
	if (dfd >= 0) {
		int res = 0;
		int v = 0;		
		
		do {
			SceIoDirent dir;
			memset(&dir, 0, sizeof(SceIoDirent));
			
			res = sceIoDread(dfd, &dir);
			if (res > 0) {
			 
				if (strcmp(dir.d_name, ".") == 0 || strcmp(dir.d_name, "..") == 0)
					continue;		
				
		 themes[v]=previewNotFoundImage;
		 themePreviewPath = (char*)malloc(1024);
		 snprintf(themePreviewPath, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/preview.png",array_buffer[v]);
	         if (!(sceIoOpen(themePreviewPath, SCE_O_RDONLY, 0777) < 0))
	        {
				    themes[v]=vita2d_load_PNG_file(themePreviewPath); 
	        }
	        else
		{
			 free(themePreviewPath);
			 themePreviewPath = (char*)malloc(1024);
			 snprintf(themePreviewPath, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/Preview.png", array_buffer[v]);
			 if (!(sceIoOpen(themePreviewPath, SCE_O_RDONLY, 0777) < 0))
			 {
				 themes[v]=vita2d_load_PNG_file(themePreviewPath); 
			 }
			 else
			 {
				 free(themePreviewPath);
				 themePreviewPath = (char*)malloc(1024);
				 snprintf(themePreviewPath, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/preview.jpg", array_buffer[v]);
				 if (!(sceIoOpen(themePreviewPath, SCE_O_RDONLY, 0777) < 0))
				{
					themes[v]=vita2d_load_JPEG_file(themePreviewPath); 
				}
				else
				{
					free(themePreviewPath);
					themePreviewPath = (char*)malloc(1024);
					snprintf(themePreviewPath, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/Preview.jpg", array_buffer[v]);
					 if (!(sceIoOpen(themePreviewPath, SCE_O_RDONLY, 0777) < 0))
					{
					  themes[v]=vita2d_load_JPEG_file(themePreviewPath); 
					}
					else
					{
						themes[v]=previewNotFoundImage;
					}
				}
				 
			 }
		}
		 v++;
		 free(themePreviewPath);
		}
		} while (res > 0);
		
		sceIoDclose(dfd);
		
	}
}

int check_for_theme_content(char *path) {
	SceUID dfd = sceIoDopen(path);
	static char pathbuff[256];
	
	if (dfd >= 0) {
		int res = 0;
		int i = 0;		
		
		do {
			SceIoDirent dir;
			memset(&dir, 0, sizeof(SceIoDirent));
			res = sceIoDread(dfd, &dir);
			if (res > 0) {

				if (strcmp(dir.d_name, ".") == 0 || strcmp(dir.d_name, "..") == 0)
					continue;
				
				strcpy(array_buffer[i], dir.d_name);
				i++;				
				
			}
		} while (res > 0);
		count = i;	
		sceIoDclose(dfd);
		return count;
	}

	return 0;
}

int copyFiles(char *src_path, char *dst_path) {
	// The source and destination paths are identical
	if (strcmp(src_path, dst_path) == 0) return -1;

	// The destination is a subfolder of the source folder
	int len = strlen(src_path);
	if (strncmp(src_path, dst_path, len) == 0 && dst_path[len] == '/') return -1;

	SceUID fdsrc = sceIoOpen(src_path, SCE_O_RDONLY, 0);
	if (fdsrc < 0) return fdsrc;

	SceUID fddst = sceIoOpen(dst_path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
	if (fddst < 0) {
		sceIoClose(fdsrc);
		return fddst;
	}

	void *buf = malloc(TRANSFER_SIZE);

	int read;
	while ((read = sceIoRead(fdsrc, buf, TRANSFER_SIZE)) > 0) {
		sceIoWrite(fddst, buf, read);

			sceIoClose(fddst);
			sceIoClose(fdsrc);

			return 0;
	}

	free(buf);

	sceIoClose(fddst);
	sceIoClose(fdsrc);

	return 1;
}

void saveThemeName(){
  
	themeName = array_buffer[NavOffset];
	char buffer[50];
	char chaine[50];
	int ret;
	int i = 0;

	FILE* f1 = fopen("ux0:VitaToolBox/settings.ini", "r");
	FILE* f2 = fopen("ux0:temp/settings.ini", "w");
	
	if (f1 == NULL){
		if (f1 != NULL) fclose(f1);
	
	} else {
		while (fscanf(f1, "%s", buffer) != EOF) { 

			if( strstr(&buffer[0], "\"") ){

			  fprintf(f2, "\"%s\"", themeName);

			}else{

			  if(i == 3){

			    fprintf(f2, "%s = ", buffer);

			  }else if(i < 3){
			    
			      fgets(chaine, sizeof chaine, (FILE*)f1);
			      fprintf(f2, "%s%s", buffer, chaine);
			      
			  }
			i++;
			}
		}
		fflush(f1);
		fclose(f2);
		fclose(f1);
	}
	ret = copyFiles("ux0:temp/settings.ini", "ux0:VitaToolBox/settings.ini");
	ret = sceIoRemove("ux0:temp/settings.ini");  
	ret = launchAppByExec("eboot.bin");
}

void loadPreview(){
	
	int themefound = 0;	
	themefound = check_for_theme_content("ux0:VitaToolBox/theme/");
	
	float rW = 563;
	float rH = 323;
	int res =0;
	char param[MAX_PATH_LENGTH];
	snprintf(param, MAX_PATH_LENGTH, "0.%d", SCALE_x_POSITION);
	float Scale_x = atof(param);
	snprintf(param, MAX_PATH_LENGTH, "0.%d", SCALE_y_POSITION);
	float Scale_y = atof(param);
	snprintf(param, MAX_PATH_LENGTH, "1.%d", NAME_Size);
	float Size_text = atof(param);
	
	char path[MAX_PATH_LENGTH];
	char path2[MAX_PATH_LENGTH];
	char path3[MAX_PATH_LENGTH];
	
	vita2d_texture *previewNotFoundImage;
	vita2d_texture *square;
	vita2d_texture *triangle;
	
	snprintf(path, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/no_preview.png", select_theme_options);
	snprintf(path2, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/square.png", select_theme_options);
	snprintf(path3, MAX_PATH_LENGTH, "ux0:VitaToolBox/theme/%s/triangle.png", select_theme_options);

	if ( doesFileExist(path) ){
	  
	    previewNotFoundImage = vita2d_load_PNG_file(path);
	  
	}else{
	    previewNotFoundImage = vita2d_load_PNG_buffer(&_binary_resources_no_preview_png_start);
	}
	
	if ( doesFileExist(path2) ){
	  
	    square = vita2d_load_PNG_file(path2);
	  
	}else{
	    square = vita2d_load_PNG_buffer(&_binary_resources_square_png_start);
	}
	
	if ( doesFileExist(path3) ){
	  
	    triangle = vita2d_load_PNG_file(path3);
	  
	}else{
	    triangle = vita2d_load_PNG_buffer(&_binary_resources_triangle_png_start);
	}
	
	vita2d_texture** TextureArray;
	TextureArray=(vita2d_texture**)malloc(sizeof(vita2d_texture*)*count);
	addtextures(TextureArray,previewNotFoundImage);
	
		while (1) {
			
		  sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
		  readPad();
		  
		  if (pressed_buttons & SCE_CTRL_SQUARE){
		    
		    saveThemeName();

		    
		  }else if (pressed_buttons & SCE_CTRL_TRIANGLE){
			  
			  break;
		  }else if (pressed_buttons & SCE_CTRL_LEFT || pressed_buttons & SCE_CTRL_LTRIGGER || pressed_buttons & SCE_CTRL_LEFT_ANALOG_LEFT) {
			if(NavOffset>0)
				{
				NavOffset--;
				}
		  
		  }else if(pressed_buttons & SCE_CTRL_RIGHT || pressed_buttons & SCE_CTRL_RTRIGGER || pressed_buttons & SCE_CTRL_LEFT_ANALOG_RIGHT){
			if(NavOffset<count-1)
				{
				NavOffset++;
				}
		  
		  }		

		// Start drawing
		startDrawing(bg_select_theme_image);
		vita2d_draw_texture_scale_rotate_hotspot(TextureArray[NavOffset], PREVIEW_x_POSITION, PREVIEW_y_POSITION, Scale_x, Scale_y, 0, 0, 0);

		if(FILTER_ACTIVE == 1)
		vita2d_draw_rectangle(PREVIEW_x_POSITION, PREVIEW_y_POSITION, FILTER_rW, FILTER_rH, RGBA8(FILTER_R_COLOR,FILTER_G_COLOR,FILTER_B_COLOR,FILTER_A_COLOR));    

		vita2d_draw_texture(square, SQUARE_x_POSITION, SQUARE_y_POSITION);
		pgf_draw_text(SELECT_x_POSITION,  SELECT_y_POSITION, THEMEMANAGER_SELECT_COLOR, 1.5f, language_container[THEMEMANAGER_SELECT]);
		vita2d_draw_texture(triangle, SCREEN_WIDTH-TRIANGLE_x_POSITION, TRIANGLE_y_POSITION);
		pgf_draw_text(SCREEN_WIDTH-CANCEL_x_POSITION, CANCEL_y_POSITION, THEMEMANAGER_CANCEL_COLOR, 1.5f, language_container[THEMEMANAGER_CANCEL]);
		float _x = vita2d_pgf_text_width(font, FONT_SIZE, array_buffer[NavOffset]);
		pgf_draw_text(NAME_x_POSITION+ALIGN_CENTER(NAME_CENTER_DIM, _x), NAME_y_POSITION, THEMEMANAGER_ITEM_COLOR, Size_text, array_buffer[NavOffset]);
		
		if (touch.reportNum == 1){
		      vita2d_draw_texture_scale_rotate_hotspot(TextureArray[NavOffset], 0, 0, 1, 1, 0, 0, 0);
		}
		
		// End drawing
		endDrawing();
		
		}
		vita2d_free_texture(previewNotFoundImage);
		vita2d_free_texture(TextureArray[NavOffset]);
}

void initSettingsMenu() {
	memset(&settings_menu, 0, sizeof(SettingsMenu));
	settings_menu.status = SETTINGS_MENU_CLOSED;

	if (is_molecular_shell) {
		n_settings_entries = sizeof(molecularshell_settings_menu_entries) / sizeof(SettingsMenuEntry);
		settings_menu_entries = molecularshell_settings_menu_entries;
	} else {
		n_settings_entries = sizeof(vitashell_settings_menu_entries) / sizeof(SettingsMenuEntry);
		settings_menu_entries = vitashell_settings_menu_entries;
	}
	  
	int i;
	for (i = 0; i < n_settings_entries; i++){
		settings_menu.n_options += settings_menu_entries[i].n_options;
	}
	
	select_button_options[0] = language_container[VITASHELL_SETTINGS_SELECT_BUTTON_USB];
	select_button_options[1] = language_container[VITASHELL_SETTINGS_SELECT_BUTTON_FTP];

	select_lang_options[0]  = language_container[english_us];
	select_lang_options[1]  = language_container[japanese];
	select_lang_options[2]  = language_container[french];
	select_lang_options[3]  = language_container[spanish];
	select_lang_options[4]  = language_container[german];
	select_lang_options[5]  = language_container[italian];
	select_lang_options[6]  = language_container[dutch];
	select_lang_options[7]  = language_container[portuguese];
	select_lang_options[8]  = language_container[russian];
	select_lang_options[9]  = language_container[korean];
	select_lang_options[10] = language_container[chinese_t];
	select_lang_options[11] = language_container[chinese_s];
	select_lang_options[12] = language_container[finnish];
	select_lang_options[13] = language_container[swedish];
	select_lang_options[14] = language_container[danish];
	select_lang_options[15] = language_container[norwegian];
	select_lang_options[16] = language_container[polish];
	select_lang_options[17] = language_container[portuguese_br];
	select_lang_options[18] = language_container[turkish];
	
	strcpy(select_theme_options, theme_name);	
}

void openSettingsMenu() {
	settings_menu.status = SETTINGS_MENU_OPENING;
	settings_menu.entry_sel = 0;
	settings_menu.option_sel = 0;

	if (is_molecular_shell) {
		memset(&henkaku_config, 0, sizeof(HENkakuConfig));
		int res = ReadFile(henkaku_config_path, &henkaku_config, sizeof(HENkakuConfig));

		if (res != sizeof(HENkakuConfig) || henkaku_config.magic != HENKAKU_CONFIG_MAGIC || henkaku_config.version != HENKAKU_VERSION) {
			memset(&henkaku_config, 0, sizeof(HENkakuConfig));
			henkaku_config.use_psn_spoofing = 1;
			henkaku_config.use_spoofed_version = 1;
		}

		char a = (henkaku_config.spoofed_version >> 24) & 0xF;
		char b = (henkaku_config.spoofed_version >> 20) & 0xF;
		char c = (henkaku_config.spoofed_version >> 16) & 0xF;
		char d = (henkaku_config.spoofed_version >> 12) & 0xF;

		memset(spoofed_version, 0, sizeof(spoofed_version));

		if (a || b || c || d) {
			spoofed_version[0] = '0' + a;
			spoofed_version[1] = '.';
			spoofed_version[2] = '0' + b;
			spoofed_version[3] = '0' + c;
			spoofed_version[4] = '\0';

			if (d) {
				spoofed_version[4] = '0' + d;
				spoofed_version[5] = '\0';
			}
		} else {
			strcpy(spoofed_version, HENKAKU_DEFAULT_VERSION_STRING);
		}
	}

	changed = 0;
}

void closeSettingsMenu() {
	settings_menu.status = SETTINGS_MENU_CLOSING;

	if (changed) {
		if (is_molecular_shell) {
			if (IS_DIGIT(spoofed_version[0]) && spoofed_version[1] == '.' && IS_DIGIT(spoofed_version[2]) && IS_DIGIT(spoofed_version[3])) {
				char a = spoofed_version[0] - '0';
				char b = spoofed_version[2] - '0';
				char c = spoofed_version[3] - '0';
				char d = IS_DIGIT(spoofed_version[4]) ? spoofed_version[4] - '0' : '\0';

				henkaku_config.spoofed_version = ((a << 24) | (b << 20) | (c << 16) | (d << 12));
			} else {
				henkaku_config.spoofed_version = 0;
			}

			henkaku_config.magic = HENKAKU_CONFIG_MAGIC;
			henkaku_config.version = HENKAKU_VERSION;

			WriteFile(henkaku_config_path, &henkaku_config, sizeof(HENkakuConfig));

			henkaku_reload_config();
		}

		saveSettingsConfig();
	}
}

int getSettingsMenuStatus() {
	return settings_menu.status;
}

void drawSettingsMenu() {
	if (settings_menu.status == SETTINGS_MENU_CLOSED)
		return;

	// Closing settings menu
	if (settings_menu.status == SETTINGS_MENU_CLOSING) {
		if (settings_menu.cur_pos > 0.0f) {
			settings_menu.cur_pos -= easeOut(0.0f, settings_menu.cur_pos, 0.25f);
		} else {
			settings_menu.status = SETTINGS_MENU_CLOSED;
		}
	}

	// Opening settings menu
	if (settings_menu.status == SETTINGS_MENU_OPENING) {
		if (settings_menu.cur_pos < SCREEN_HEIGHT) {
			settings_menu.cur_pos += easeOut(settings_menu.cur_pos, SCREEN_HEIGHT, 0.25f);
		} else {
			settings_menu.status = SETTINGS_MENU_OPENED;
		}
	}

	// Draw settings menu
	vita2d_draw_texture(settings_image, 0.0f, SCREEN_HEIGHT - settings_menu.cur_pos);

	float y = SCREEN_HEIGHT - settings_menu.cur_pos + START_Y;

	int i;
	for (i = 0; i < n_settings_entries; i++) {
		// Title
		float x = vita2d_pgf_text_width(font, FONT_SIZE, language_container[settings_menu_entries[i].name]);
		pgf_draw_text(ALIGN_CENTER(SCREEN_WIDTH, x), y, SETTINGS_MENU_TITLE_COLOR, FONT_SIZE, language_container[settings_menu_entries[i].name]);

		y += FONT_Y_SPACE;

		SettingsMenuOption *options = settings_menu_entries[i].options;

		int j;
		for (j = 0; j < settings_menu_entries[i].n_options; j++) {
			// Focus
			if (settings_menu.entry_sel == i && settings_menu.option_sel == j)
				vita2d_draw_rectangle(SHELL_MARGIN_X, y + 3.0f, MARK_WIDTH, FONT_Y_SPACE, SETTINGS_MENU_FOCUS_COLOR);

			if (options[j].type == SETTINGS_OPTION_TYPE_CALLBACK) {
				// Item
				float x = vita2d_pgf_text_width(font, FONT_SIZE, language_container[options[j].name]);
				pgf_draw_text(ALIGN_CENTER(SCREEN_WIDTH, x), y, SETTINGS_MENU_ITEM_COLOR, FONT_SIZE, language_container[options[j].name]);
			} else {
				// Item
				float x = vita2d_pgf_text_width(font, FONT_SIZE, language_container[options[j].name]);
				pgf_draw_text(ALIGN_RIGHT(SCREEN_HALF_WIDTH - 10.0f, x), y, SETTINGS_MENU_ITEM_COLOR, FONT_SIZE, language_container[options[j].name]);

				// Option
				switch (options[j].type) {
					case SETTINGS_OPTION_TYPE_BOOLEAN:
						pgf_draw_text(SCREEN_HALF_WIDTH + 10.0f, y, SETTINGS_MENU_OPTION_COLOR, FONT_SIZE, *(options[j].value) ? language_container[ON] : language_container[OFF]);
						break;

					case SETTINGS_OPTION_TYPE_STRING:
						pgf_draw_text(SCREEN_HALF_WIDTH + 10.0f, y, SETTINGS_MENU_OPTION_COLOR, FONT_SIZE, options[j].string);
						break;

					case SETTINGS_OPTION_TYPE_OPTIONS:
					{
						int value = *(options[j].value);
						pgf_draw_text(SCREEN_HALF_WIDTH + 10.0f, y, SETTINGS_MENU_OPTION_COLOR, FONT_SIZE, options[j].options[value]);
						break;
					}
					case SETTINGS_OPTION2_TYPE_OPTIONS:
					{
						int value = *(options[j].value);
						pgf_draw_text(SCREEN_HALF_WIDTH + 10.0f, y, SETTINGS_MENU_OPTION_COLOR, FONT_SIZE, options[j].options[value]);
						break;
					}
					case SETTINGS_OPTION2_TYPE_STRING:
						pgf_draw_text(SCREEN_HALF_WIDTH + 10.0f, y, SETTINGS_MENU_OPTION_COLOR, FONT_SIZE, options[j].string);
						break;					
				}
			}

			y += FONT_Y_SPACE;
		}

		y += FONT_Y_SPACE;
	}
}

static int agreement = SETTINGS_AGREEMENT_NONE;

void settingsAgree() {
	agreement = SETTINGS_AGREEMENT_AGREE;
}

void settingsDisagree() {
	agreement = SETTINGS_AGREEMENT_DISAGREE;
}

void settingsMenuCtrl() {
	SettingsMenuOption *option = &settings_menu_entries[settings_menu.entry_sel].options[settings_menu.option_sel];
	
	// Agreement
	if (agreement != SETTINGS_AGREEMENT_NONE) {
		agreement = SETTINGS_AGREEMENT_NONE;

		if (option->name == HENKAKU_ENABLE_UNSAFE_HOMEBREW) {
			*(option->value) = !*(option->value);
		}
	}

	// Change options
	if (pressed_buttons & (SCE_CTRL_ENTER | SCE_CTRL_LEFT | SCE_CTRL_RIGHT)) {
		changed = 1;

		if (option->name == HENKAKU_ENABLE_UNSAFE_HOMEBREW) {
			if (*(option->value) == 0) {
				initMessageDialog(SCE_MSG_DIALOG_BUTTON_TYPE_OK, language_container[HENKAKU_UNSAFE_HOMEBREW_MESSAGE]);
				dialog_step = DIALOG_STEP_SETTINGS_AGREEMENT;
			} else {
				*(option->value) = !*(option->value);
			}
		} else {
			switch (option->type) {
				case SETTINGS_OPTION_TYPE_BOOLEAN:
					*(option->value) = !*(option->value);
					break;
				
				case SETTINGS_OPTION_TYPE_STRING:
					initImeDialog(language_container[option->name], option->string, option->size_string, SCE_IME_TYPE_EXTENDED_NUMBER, 0);
					dialog_step = DIALOG_STEP_SETTINGS_STRING;
					break;
					
				case SETTINGS_OPTION_TYPE_CALLBACK:
					option->callback(&option);
					break;
					
				case SETTINGS_OPTION2_TYPE_STRING:
				{
					
					if (pressed_buttons & SCE_CTRL_ENTER) {	
					  
						loadPreview();
						
					}
				  
					break;
				}
					
				case SETTINGS_OPTION_TYPE_OPTIONS:
				{			
				      if (pressed_buttons & SCE_CTRL_LEFT) {					
						if (*(option->value) > 0)
							(*(option->value))--;
						else
							*(option->value) = option->n_options-1;
					}else if (pressed_buttons & (SCE_CTRL_ENTER | SCE_CTRL_RIGHT)) {
						if (*(option->value) < option->n_options-1)
							(*(option->value))++;
						else
							*(option->value) = 0;
					}
					
					break;
				}
				
				case SETTINGS_OPTION2_TYPE_OPTIONS:
				{			
				      if (pressed_buttons & SCE_CTRL_LEFT) {					
						if (*(option->value) > 0)
							(*(option->value))--;
						else
							*(option->value) = option->n_options-1;
					}else if (pressed_buttons & SCE_CTRL_RIGHT) {
						if (*(option->value) < option->n_options-1)
							(*(option->value))++;
						else
							*(option->value) = 0;
					}else if (pressed_buttons & SCE_CTRL_ENTER){
						closeSettingsMenu();
						int ret;
						ret = launchAppByExec("eboot.bin");
					}
					
					break;
				}
			}
		}
	}

	// Move
	if (hold_buttons & SCE_CTRL_UP || hold2_buttons & SCE_CTRL_LEFT_ANALOG_UP) {
		if (settings_menu.option_sel > 0) {
			settings_menu.option_sel--;
		} else if (settings_menu.entry_sel > 0) {
			settings_menu.entry_sel--;
			settings_menu.option_sel = settings_menu_entries[settings_menu.entry_sel].n_options-1;
		}
	} else if (hold_buttons & SCE_CTRL_DOWN || hold2_buttons & SCE_CTRL_LEFT_ANALOG_DOWN) {
		if (settings_menu.option_sel < settings_menu_entries[settings_menu.entry_sel].n_options-1) {
			settings_menu.option_sel++;
		} else if (settings_menu.entry_sel < n_settings_entries-1) {
			settings_menu.entry_sel++;
			settings_menu.option_sel = 0;
		}
	}

	// Close
	if (pressed_buttons & (SCE_CTRL_CANCEL | SCE_CTRL_LTRIGGER)) {
		closeSettingsMenu();
	}
}
