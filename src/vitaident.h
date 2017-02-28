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

#ifndef __VITAIDENT_H__
#define __VITAIDENT_H__

#include "vitaident/app.h"
#include "vitaident/utils.h"
#include "vitaident/fs.h"
#include "vitaident/kernel.h"
#include "vitaident/net.h"
#include "vitaident/power.h"

enum VitaidentAgreement {
	VITAIDENT_AGREEMENT_NONE,
	VITAIDENT_AGREEMENT_AGREE,
	VITAIDENT_AGREEMENT_DISAGREE,
};

enum VitaidentOptionType {
	VITAIDENT_OPTION_TYPE_BOOLEAN,
	VITAIDENT_OPTION_TYPE_INTEGER,
	VITAIDENT_OPTION_TYPE_STRING,
	VITAIDENT_OPTION_TYPE_CALLBACK,
	VITAIDENT_OPTION_TYPE_NOTHING,
};

enum VitaidentMenuStatus {
	VITAIDENT_MENU_CLOSED,
	VITAIDENT_MENU_CLOSING,
	VITAIDENT_MENU_OPENED,
	VITAIDENT_MENU_OPENING,
};

typedef struct {
	int status;
	float cur_pos;
} VitaidentMenu;

void initVitaidentMenu();
void openVitaidentMenu();
void closeVitaidentMenu();
int getVitaidentMenuStatus();
void drawVitaidentMenu();
void VitaidentMenuCtrl();

void VitaidentAgree();
void VitaidentDisagree();

#endif 
