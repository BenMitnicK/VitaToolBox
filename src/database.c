/*
	VitaShell
	Copyright (C) 2015-2017, TheFloW and BenMitnicK

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
#include "database.h"
#include "message_dialog.h"
#include "ime_dialog.h"
#include "utils.h"

void updateDataBase();
void rebuildDataBase();

static int changed = 0;

static DataBaseMenuEntry *database_menu_entries = NULL;
static int n_database_entries = 0;

DataBaseMenuOption database_settings[] = {
	{ VITASHELL_SETTINGS_DATABASE_UPDATE,		DATABASE_OPTION_TYPE_CALLBACK, (void *)updateDataBase, NULL, 0, NULL },
	{ VITASHELL_SETTINGS_DATABASE_REBUILD,		DATABASE_OPTION_TYPE_CALLBACK, (void *)rebuildDataBase, NULL, 0, NULL },
};

DataBaseMenuEntry vitashell_database_menu_entries[] = {
	{ VITASHELL_SETTINGS_DATABASE, database_settings, sizeof(database_settings) / sizeof(DataBaseMenuOption) },
};

static DatabaseMenu database_menu;

static float easeOut(float x0, float x1, float a) {
	float dx = (x1 - x0);
	return ((dx * a) > 0.01f) ? (dx * a) : dx;
}

void updateDataBase() {
	initMessageDialog(SCE_MSG_DIALOG_BUTTON_TYPE_YESNO, language_container[DATABASEUPDATEQUESTION]);
	dialog_step = DIALOG_STEP_DATABASE_UPDATE_QUESTION;
}

void rebuildDataBase() {
	initMessageDialog(SCE_MSG_DIALOG_BUTTON_TYPE_YESNO, language_container[DATABASEREBUILDQUESTION]);
	dialog_step = DIALOG_STEP_DATABASE_REBUILD_QUESTION;			
}

void DataBaserebootDevice() {
	closeDataBaseMenu();
	scePowerRequestColdReset();
}

void initDataBaseMenu() {
	memset(&database_menu, 0, sizeof(DatabaseMenu));
	database_menu.status = DATABASE_MENU_CLOSED;

	n_database_entries = sizeof(vitashell_database_menu_entries) / sizeof(DataBaseMenuEntry);
	database_menu_entries = vitashell_database_menu_entries;

	int i;
	for (i = 0; i < n_database_entries; i++)
		database_menu.n_options += database_menu_entries[i].n_options;
}

void openDataBaseMenu() {
	database_menu.status = DATABASE_MENU_OPENING;
	database_menu.entry_sel = 0;
	database_menu.option_sel = 0;
}

void closeDataBaseMenu() {
	database_menu.status = DATABASE_MENU_CLOSING;
}

int getDataBaseMenuStatus() {
	return database_menu.status;
}

void drawDataBaseMenu() {
	if (database_menu.status == DATABASE_MENU_CLOSED)
		return;

	// Closing settings menu
	if (database_menu.status == DATABASE_MENU_CLOSING) {
		if (database_menu.cur_pos > 0.0f) {
			database_menu.cur_pos -= easeOut(0.0f, database_menu.cur_pos, 0.25f);
		} else {
			database_menu.status = DATABASE_MENU_CLOSED;
		}
	}

	// Opening settings menu
	if (database_menu.status == DATABASE_MENU_OPENING) {
		if (database_menu.cur_pos < SCREEN_HEIGHT) {
			database_menu.cur_pos += easeOut(database_menu.cur_pos, SCREEN_HEIGHT, 0.25f);
		} else {
			database_menu.status = DATABASE_MENU_OPENED;
		}
	}

	// Draw settings menu
	vita2d_draw_texture(database_image, 0.0f, SCREEN_HEIGHT - database_menu.cur_pos);

	float y = SCREEN_HEIGHT - database_menu.cur_pos + START_Y;

	int i;
	for (i = 0; i < n_database_entries; i++) {
		// Title
		float x = vita2d_pgf_text_width(font, 1.5, language_container[database_menu_entries[i].name]);
		pgf_draw_text(ALIGN_CENTER(SCREEN_WIDTH, x), y, DATABASE_MENU_TITLE_COLOR, 1.5, language_container[database_menu_entries[i].name]);

		y += FONT_Y_SPACE;
		
		DataBaseMenuOption *options = database_menu_entries[i].options;

		int j;
		for (j = 0; j < database_menu_entries[i].n_options; j++) {
			// Focus
			if (database_menu.entry_sel == i && database_menu.option_sel == j)
				vita2d_draw_rectangle(SHELL_MARGIN_X, y + 3.0f, MARK_WIDTH, FONT_Y_SPACE, DATABASE_MENU_FOCUS_COLOR);

			if (options[j].type == DATABASE_OPTION_TYPE_CALLBACK) {
				// Item
				float x = vita2d_pgf_text_width(font, FONT_SIZE, language_container[options[j].name]);
				pgf_draw_text(ALIGN_CENTER(SCREEN_WIDTH, x), y, DATABASE_MENU_ITEM_COLOR, FONT_SIZE, language_container[options[j].name]);
			} else {
				// Item
				float x = vita2d_pgf_text_width(font, FONT_SIZE, language_container[options[j].name]);
				pgf_draw_text(ALIGN_RIGHT(SCREEN_HALF_WIDTH - 10.0f, x), y, DATABASE_MENU_ITEM_COLOR, FONT_SIZE, language_container[options[j].name]);

				// Option
				if (options[j].type == DATABASE_OPTION_TYPE_BOOLEAN) {
					pgf_draw_text(SCREEN_HALF_WIDTH + 10.0f, y, DATABASE_MENU_OPTION_COLOR, FONT_SIZE, *(options[j].value) ? language_container[ON] : language_container[OFF]);
				} else if (options[j].type == DATABASE_OPTION_TYPE_STRING) {
					pgf_draw_text(SCREEN_HALF_WIDTH + 10.0f, y, DATABASE_MENU_OPTION_COLOR, FONT_SIZE, options[j].string);
				}
			}

			y += FONT_Y_SPACE;
		}

		y += FONT_Y_SPACE;
	}
}

static int agreement = DATABASE_AGREEMENT_NONE;

void databaseAgree() {
	agreement = DATABASE_AGREEMENT_AGREE;
}

void databaseDisagree() {
	agreement = DATABASE_AGREEMENT_DISAGREE;
}

void databaseMenuCtrl() {
	DataBaseMenuOption *option = &database_menu_entries[database_menu.entry_sel].options[database_menu.option_sel];

	// Agreement
	if (agreement != DATABASE_AGREEMENT_NONE) {
		agreement = DATABASE_AGREEMENT_NONE;
	}

	// Change options
	if (pressed_buttons & (SCE_CTRL_ENTER)) {
		changed = 1;

			if (option->type == DATABASE_OPTION_TYPE_BOOLEAN) {
				*(option->value) = !*(option->value);
			} else if (option->type == DATABASE_OPTION_TYPE_STRING) {
				initImeDialog(language_container[option->name], option->string, option->size_string, SCE_IME_TYPE_EXTENDED_NUMBER, 0);
				dialog_step = DIALOG_STEP_SETTINGS_STRING;
			} else if (option->type == DATABASE_OPTION_TYPE_CALLBACK) {
				option->callback(&option);
			}
	}

	// Move
	if (hold_buttons & SCE_CTRL_UP || hold2_buttons & SCE_CTRL_LEFT_ANALOG_UP) {
		if (database_menu.option_sel > 0) {
			database_menu.option_sel--;
		} else if (database_menu.entry_sel > 0) {
			database_menu.entry_sel--;
			database_menu.option_sel = database_menu_entries[database_menu.entry_sel].n_options - 1;
		}
	} else if (hold_buttons & SCE_CTRL_DOWN || hold2_buttons & SCE_CTRL_LEFT_ANALOG_DOWN) {
		if (database_menu.option_sel < database_menu_entries[database_menu.entry_sel].n_options - 1) {
			database_menu.option_sel++;
		} else if (database_menu.entry_sel < n_database_entries - 1) {
			database_menu.entry_sel++;
			database_menu.option_sel = 0;
		}
	}

	// Close
	if (pressed_buttons & (SCE_CTRL_CANCEL | SCE_CTRL_RIGHT_ANALOG_LEFT)) {
		closeDataBaseMenu();
	}
}
