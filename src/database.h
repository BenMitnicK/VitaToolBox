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

#ifndef __DATABASE_H__
#define __DATABASE_H__

enum DataBaseAgreement {
	DATABASE_AGREEMENT_NONE,
	DATABASE_AGREEMENT_AGREE,
	DATABASE_AGREEMENT_DISAGREE,
};

enum DataBaseOptionType {
	DATABASE_OPTION_TYPE_BOOLEAN,
	DATABASE_OPTION_TYPE_INTEGER,
	DATABASE_OPTION_TYPE_STRING,
	DATABASE_OPTION_TYPE_CALLBACK,
};

enum DataBaseMenuStatus {
	DATABASE_MENU_CLOSED,
	DATABASE_MENU_CLOSING,
	DATABASE_MENU_OPENED,
	DATABASE_MENU_OPENING,
};

typedef struct {
	int status;
	float cur_pos;
	int entry_sel;
	int option_sel;
	int n_options;
} DatabaseMenu;

typedef struct {
	int name;
	int type;
	int (* callback)();
	char *string;
	int size_string;
	int *value;
} DataBaseMenuOption;

typedef struct {
	int name;
	DataBaseMenuOption *options;
	int n_options;
} DataBaseMenuEntry;

void DataBaserebootDevice();
void initDataBaseMenu();
void openDataBaseMenu();
void closeDataBaseMenu();
int getDataBaseMenuStatus();
void drawDataBaseMenu();
void databaseMenuCtrl();

void databaseAgree();
void databaseDisagree();

#endif