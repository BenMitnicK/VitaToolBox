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
#include "plugins_manager.h"
#include "utils.h"

int n_pluginsmanager_entries = 0;

Manager pluginsManager;

PluginsManagerMenu pluginsmanager_menu;

static float easeOut(float x0, float x1, float a) {
	float dx = (x1 - x0);
	return ((dx * a) > 0.01f) ? (dx * a) : dx;
}

void menuDraw(Manager *pluginsManager) {
  	
	int size;
	char *type = "PSVita";
	char *type2 = "PSP";
	float y2 = (SCREEN_HEIGHT - pluginsmanager_menu.cur_pos) + 65;
	
	for (size = 0;  pluginsManager->plugins[size].name != '\0'; size++)
		continue;	
	
	if (size > 0) {
	  
		if (pluginsManager->navOffset > size-1) {
			pluginsManager->navOffset = 0;
			
		} else if (pluginsManager->navOffset < 0) {
			pluginsManager->navOffset = size-1;
			
		}
		
		for (int i = 0; pluginsManager->plugins[i].name != '\0'; i++) {
			
			if (pluginsManager->navOffset == i) {
			  
				vita2d_draw_rectangle(0, y2 + (i * 25), 960, FONT3_Y_SPACE, PLUGINSMANAGER_MENU_FOCUS_COLOR);
				
				pgf_draw_text(50, y2 + (i * 25), PLUGINSMANAGER_NAME_COLOR, 1, pluginsManager->plugins[i].name);
				pgf_draw_text(550, y2 + (i * 25), PLUGINSMANAGER_TYPE_COLOR, 1, "PSVita");
				pgf_draw_text(SCREEN_WIDTH - (160), y2 + (i * 25), PLUGINSMANAGER_STATUS_COLOR, 1, (pluginsManager->plugins[i].active ? "[ENABLED]" : "[DISABLED]"));

			} else {
				pgf_draw_text(50, y2 + (i * 25), PLUGINSMANAGER_NAME_COLOR,  1, pluginsManager->plugins[i].name);
				pgf_draw_text(550, y2 + (i * 25), PLUGINSMANAGER_TYPE_COLOR, 1, "PSVita");
				pgf_draw_text(SCREEN_WIDTH - (160), y2 + (i * 25), PLUGINSMANAGER_STATUS_COLOR, 1, (pluginsManager->plugins[i].active ? "[ENABLED]" : "[DISABLED]"));
			}

		}

	}
}

void init() {
	struct stat st = {0};

	if (stat(disablePath, &st) == -1) {
		sceIoMkdir(disablePath,  0777);
	}
}

void initPluginsManagerMenu() {

	memset(&pluginsmanager_menu, 0, sizeof(PluginsManagerMenu));
	pluginsmanager_menu.status = PLUGINSMANAGER_MENU_CLOSED;

	n_pluginsmanager_entries = 1;
}

void openPluginsManagerMenu() {
	pluginsmanager_menu.status = PLUGINSMANAGER_MENU_OPENING;
}

void closePluginsManagerMenu() {
	pluginsmanager_menu.status = PLUGINSMANAGER_MENU_CLOSING;
}

int getPluginsManagerMenuStatus() {
	return pluginsmanager_menu.status;
}

void uiDraw(Manager *pluginsManager){

	menuDraw(pluginsManager);
}

void openPluginsManager(){
  
      init();
	
	Plugins plugins[50];
		
	pluginsManager.plugins = plugins;

	getPlugins(&pluginsManager);
	getConfigs(plugins);
	
	mainControle(&pluginsManager);
	menuDraw(&pluginsManager);  
}

void drawPluginsManagerMenu() {
  
	
	if (pluginsmanager_menu.status == PLUGINSMANAGER_MENU_CLOSED)
		return;

	// Closing settings menu
	if (pluginsmanager_menu.status == PLUGINSMANAGER_MENU_CLOSING) {
		if (pluginsmanager_menu.cur_pos > 0.0f) {
			pluginsmanager_menu.cur_pos -= easeOut(0.0f, pluginsmanager_menu.cur_pos, 0.25f);
		} else {
			pluginsmanager_menu.status = PLUGINSMANAGER_MENU_CLOSED;
		}
	}

	// Opening settings menu
	if (pluginsmanager_menu.status == PLUGINSMANAGER_MENU_OPENING) {
		if (pluginsmanager_menu.cur_pos < SCREEN_HEIGHT) {
			pluginsmanager_menu.cur_pos += easeOut(pluginsmanager_menu.cur_pos, SCREEN_HEIGHT, 0.25f);
		} else {
			pluginsmanager_menu.status = PLUGINSMANAGER_MENU_OPENED;
		}
	}

	// Draw settings menu
	vita2d_draw_texture(p_manager_image, 0.0f, SCREEN_HEIGHT - pluginsmanager_menu.cur_pos);

	float y = SCREEN_HEIGHT - pluginsmanager_menu.cur_pos + START_Y;
	float y2 = SCREEN_HEIGHT - pluginsmanager_menu.cur_pos;

	int i;
	for (i = 0; i < n_pluginsmanager_entries; i++) {
	  
		// Drawing
		pgf_draw_text(20, y2 + (10), PLUGINSMANAGER_MENU_TITLE_COLOR, 1.2, "Plugins Name|-");
		pgf_draw_text(545, y2 + (10), PLUGINSMANAGER_MENU_TITLE_COLOR, 1.2, "-|Type|-");
		pgf_draw_text(SCREEN_WIDTH - (155), y2 + (10), PLUGINSMANAGER_MENU_TITLE_COLOR, 1.2, "-|Status|-");
		vita2d_draw_rectangle(0, y2 + (38), 960, 3, BLUE);
		pgf_draw_text(5, y2 + (520), PLUGINSMANAGER_MENU_TITLE_COLOR, 1.2, "PluginsManager|-");
		pgf_draw_text(SCREEN_WIDTH - (150), y2 + (520), PLUGINSMANAGER_MENU_TITLE_COLOR, 1.2, "-|BenMitnicK");
		vita2d_draw_rectangle(0, y2 + (515), 960, 3, BLUE);		
				
	}
	
	if (pluginsmanager_menu.cur_pos > 0.0f) {
		openPluginsManager();
	}
}

static int agreement = PLUGINSMANAGER_AGREEMENT_NONE;

void pluginsmanagerAgree() {
	agreement = PLUGINSMANAGER_AGREEMENT_AGREE;
}

void pluginsmanagerDisagree() {
	agreement = PLUGINSMANAGER_AGREEMENT_DISAGREE;
}

void mainControle(Manager *pluginsManager) {
  
	     if (hold_buttons & SCE_CTRL_UP || hold2_buttons & SCE_CTRL_LEFT_ANALOG_UP) {
		      pluginsManager->navOffset -= 1;      
	      } 	      
	      else if (hold_buttons & SCE_CTRL_DOWN || hold2_buttons & SCE_CTRL_LEFT_ANALOG_DOWN) {
		      pluginsManager->navOffset += 1;	     
	      } 
	      else if (hold_buttons & SCE_CTRL_ENTER) {
		      dirPlugins(pluginsManager);
		      putConfigs(pluginsManager->plugins);		
	      } 
}

void pluginsmanagerMenuCtrl() {

	// Agreement
	if (agreement != PLUGINSMANAGER_AGREEMENT_NONE) {
		agreement = PLUGINSMANAGER_AGREEMENT_NONE;
	}

	// Close
	if (pressed_buttons & (SCE_CTRL_CANCEL | SCE_CTRL_RIGHT_ANALOG_UP)) {
		closePluginsManagerMenu();
	}
}
 
