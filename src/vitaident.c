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
#include "vitaident.h"
#include "message_dialog.h"
#include "ime_dialog.h"
#include "utils.h"

static int n_Vitaident_entries = 0;

static VitaidentMenu Vitaident_menu;

static float easeOut(float x0, float x1, float a) {
	float dx = (x1 - x0);
	return ((dx * a) > 0.01f) ? (dx * a) : dx;
}

void initVitaidentMenu() {

	memset(&Vitaident_menu, 0, sizeof(VitaidentMenu));
	Vitaident_menu.status = VITAIDENT_MENU_CLOSED;
	n_Vitaident_entries = 1;
}

void openVitaidentMenu() {
	Vitaident_menu.status = VITAIDENT_MENU_OPENING;
}

void closeVitaidentMenu() {
	Vitaident_menu.status = VITAIDENT_MENU_CLOSING;
}

int getVitaidentMenuStatus() {
	return Vitaident_menu.status;
}

void launchVitaident() 
{

	float y2 = (SCREEN_HEIGHT - Vitaident_menu.cur_pos) + 25;
	
	// Kernel Info
	pgf_draw_text(20, y2 + 10, VITAIDENT_KERNEL_NAME_COLOR, 1, "Firmware version: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 10, VITAIDENT_KERNEL_ITEM_COLOR, 1, getFwVersion(false));
	
	pgf_draw_text(20, y2 + 35, VITAIDENT_KERNEL_NAME_COLOR, 1, "System version: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 35, VITAIDENT_KERNEL_ITEM_COLOR, 1, getFwVersion(true));
	
	char modelVersion[MAX_PATH_LENGTH];
	snprintf(modelVersion, MAX_PATH_LENGTH, "%s  %s  (0x%08X)", getDeviceModel(), vshSysconHasWWAN()? "3G" : "WiFi", getModel());
	pgf_draw_text(20, y2 + 60, VITAIDENT_KERNEL_NAME_COLOR, 1, "Model version: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 60, VITAIDENT_KERNEL_ITEM_COLOR, 1, modelVersion);
	
	pgf_draw_text(20, y2 + 85, VITAIDENT_KERNEL_NAME_COLOR, 1, "PS Vita unit:");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 85, VITAIDENT_KERNEL_ITEM_COLOR, 1, getUnit());
	
	pgf_draw_text(20, y2 + 110, VITAIDENT_KERNEL_NAME_COLOR, 1, "Motherboard: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 110, VITAIDENT_KERNEL_ITEM_COLOR, 1, getBoard());
	
	pgf_draw_text(20, y2 + 135, VITAIDENT_KERNEL_NAME_COLOR, 1, "PS Vita CID: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 135, VITAIDENT_KERNEL_ITEM_COLOR, 1, getCID()); //Thanks Major_Tom
	
	char pSID[MAX_PATH_LENGTH];
	snprintf(pSID, MAX_PATH_LENGTH, "%02X", getPSID());
	pgf_draw_text(20, y2 + 160, VITAIDENT_KERNEL_NAME_COLOR, 1, "PSID: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 160, VITAIDENT_KERNEL_ITEM_COLOR, 1, pSID); //Thanks SMOKE
  
	//System Info
	pgf_draw_text(20, y2 + 210, VITAIDENT_SYSTEM_NAME_COLOR, 1, "Language: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 210, VITAIDENT_SYSTEM_ITEM_COLOR, 1, getLang());
	
	pgf_draw_text(20, y2 + 235, VITAIDENT_SYSTEM_NAME_COLOR, 1, "MAC address:");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 235, VITAIDENT_SYSTEM_ITEM_COLOR, 1, getMacAddress());
	
	pgf_draw_text(20, y2 + 260, VITAIDENT_SYSTEM_NAME_COLOR, 1, "IP address: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 260, VITAIDENT_SYSTEM_ITEM_COLOR, 1, getIP());
	
	char armClock[MAX_PATH_LENGTH];
	snprintf(armClock, MAX_PATH_LENGTH, "%d MHz", getClockFrequency(ClockFrequencyType_Cpu));
	pgf_draw_text(20, y2 + 285, VITAIDENT_SYSTEM_NAME_COLOR, 1, "ARM clock frequency: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 285, VITAIDENT_SYSTEM_ITEM_COLOR, 1, armClock);
	
	char busClock[MAX_PATH_LENGTH];
	snprintf(busClock, MAX_PATH_LENGTH, "%d MHz", getClockFrequency(ClockFrequencyType_Bus));
	pgf_draw_text(20, y2 + 310, VITAIDENT_SYSTEM_NAME_COLOR, 1, "BUS clock frequency: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 310, VITAIDENT_SYSTEM_ITEM_COLOR, 1, busClock);
	
	char gpuClock[MAX_PATH_LENGTH];
	snprintf(gpuClock, MAX_PATH_LENGTH, "%d MHz", getClockFrequency(ClockFrequencyType_Gpu));
	pgf_draw_text(20, y2 + 335, VITAIDENT_SYSTEM_NAME_COLOR, 1, "GPU clock frequency: ");
	pgf_draw_text(SCREEN_WIDTH - (700), y2 + 335, VITAIDENT_SYSTEM_ITEM_COLOR, 1, gpuClock);
	
	
	//Battery Info	
	if (!(getModel() == 0x00020000))//if its a PS Vita TV this info is uselsss.
	{
	  
		char batteriePercentage[MAX_PATH_LENGTH];
		snprintf(batteriePercentage, MAX_PATH_LENGTH, "%s (%s)", getBatteryPercentage(), getBatteryStatus());
		pgf_draw_text(20, y2 + 385, VITAIDENT_BATTERY_NAME_COLOR, 1, "Battery percentage: ");
		pgf_draw_text(SCREEN_WIDTH - (700), y2 + 385, VITAIDENT_BATTERY_ITEM_COLOR, 1, batteriePercentage);
		
		pgf_draw_text(20, y2 + 410, VITAIDENT_BATTERY_NAME_COLOR, 1, "Battery capacity: ");
		pgf_draw_text(SCREEN_WIDTH - (700), y2 + 410, VITAIDENT_BATTERY_ITEM_COLOR, 1, getBatteryCapacity());
		pgf_draw_text(SCREEN_WIDTH - (580), y2 + 410, VITAIDENT_BATTERY_NAME_COLOR, 1, "(remaining: ");
		pgf_draw_text(SCREEN_WIDTH - (470), y2 + 410, VITAIDENT_BATTERY_ITEM_COLOR, 1, getBatteryRemainCapacity());
		pgf_draw_text(SCREEN_WIDTH - (365), y2 + 410, VITAIDENT_BATTERY_NAME_COLOR, 1, ")");
		
		int batteryLifeTime = scePowerGetBatteryLifeTime();
		char batterieLifeTime[MAX_PATH_LENGTH];
		snprintf(batterieLifeTime, MAX_PATH_LENGTH, "%02dh %02dm ", batteryLifeTime/60, batteryLifeTime-(batteryLifeTime/60*60));
		pgf_draw_text(20, y2 + 435, VITAIDENT_BATTERY_NAME_COLOR, 1, "Battery life time: ");
		pgf_draw_text(SCREEN_WIDTH - (700), y2 + 435, VITAIDENT_BATTERY_ITEM_COLOR, 1, batterieLifeTime);
		pgf_draw_text(SCREEN_WIDTH - (595), y2 + 435, VITAIDENT_BATTERY_NAME_COLOR, 1, "(SOH: ");
		pgf_draw_text(SCREEN_WIDTH - (530), y2 + 435, VITAIDENT_BATTERY_ITEM_COLOR, 1, getBatterySOH());
		pgf_draw_text(SCREEN_WIDTH - (485), y2 + 435, VITAIDENT_BATTERY_NAME_COLOR, 1, ")");
		
		char batterieVoltage[MAX_PATH_LENGTH];
		snprintf(batterieVoltage, MAX_PATH_LENGTH, "%s V ", getBatteryVoltage());
		pgf_draw_text(20, y2 + 460, VITAIDENT_BATTERY_NAME_COLOR, 1, "Battery voltage: ");
		pgf_draw_text(SCREEN_WIDTH - (700), y2 + 460, VITAIDENT_BATTERY_ITEM_COLOR, 1, batterieVoltage);
		pgf_draw_text(SCREEN_WIDTH - (630), y2 + 460, VITAIDENT_BATTERY_NAME_COLOR, 1, "(cycle count: ");
		pgf_draw_text(SCREEN_WIDTH - (495), y2 + 460, VITAIDENT_BATTERY_ITEM_COLOR, 1, getBatteryCycleCount());
		pgf_draw_text(SCREEN_WIDTH - (465), y2 + 460, VITAIDENT_BATTERY_NAME_COLOR, 1, ")");
		
		//char busClock[MAX_PATH_LENGTH];
		//snprintf(busClock, MAX_PATH_LENGTH, "%d MHz", getClockFrequency(ClockFrequencyType_Bus));
		/*pgf_draw_text(20, y2 + 485, VITAIDENT_BATTERY_NAME_COLOR, 1, "Udcd state: ");
		pgf_draw_text(SCREEN_WIDTH - (700), y2 + 485, VITAIDENT_BATTERY_ITEM_COLOR, 1, getUdcdCableState());
		pgf_draw_text(SCREEN_WIDTH - (500), y2 + 485, VITAIDENT_BATTERY_NAME_COLOR, 1, "(");
		pgf_draw_text(SCREEN_WIDTH - (450), y2 + 485, VITAIDENT_BATTERY_ITEM_COLOR, 1, getUdcdChargingState());
		pgf_draw_text(SCREEN_WIDTH - (445), y2 + 485, VITAIDENT_BATTERY_NAME_COLOR, 1, ")");*/
	}
	
	
	// Misc info 	
	pgf_draw_text(530, y2 + 210, VITAIDENT_MISC_NAME_COLOR, 1, "Username: ");
	pgf_draw_text(SCREEN_WIDTH - (150), y2 + 210, VITAIDENT_MISC_ITEM_COLOR, 1, getUser());
	
	pgf_draw_text(530, y2 + 235, VITAIDENT_MISC_NAME_COLOR, 1, "Enter button: ");
	pgf_draw_text(SCREEN_WIDTH - (150), y2 + 235, VITAIDENT_MISC_ITEM_COLOR, 1, getEnterButton());
	
	if (vshRemovableMemoryGetCardInsertState() == 1) //Memory card is inserted
	{
	  
		pgf_draw_text(530, y2 + 260, VITAIDENT_MISC_NAME_COLOR, 1, "Memory card storage: ");
		pgf_draw_text(SCREEN_WIDTH - (150), y2 + 260, VITAIDENT_MISC_ITEM_COLOR, 1, getStorageInfo(0));
		
		pgf_draw_text(530, y2 + 285, VITAIDENT_MISC_NAME_COLOR, 1, "Memory card storage free: ");
		pgf_draw_text(SCREEN_WIDTH - (150), y2 + 285, VITAIDENT_MISC_ITEM_COLOR, 1, getStorageInfo(1));
	  
	}	
	
	char free_size_string[16], max_size_string[16];
	SceOff freeSize = getPartitionInfo(0, "ur0:");
	SceOff maxSize = getPartitionInfo(1, "ur0:");
	
	getSizeString(free_size_string, freeSize);
	getSizeString(max_size_string, maxSize);
	
	pgf_draw_text(530, y2 + 310, VITAIDENT_MISC_NAME_COLOR, 1, "Internal storage: ");
	pgf_draw_text(SCREEN_WIDTH - (150), y2 + 310, VITAIDENT_MISC_ITEM_COLOR, 1, max_size_string);
	
	pgf_draw_text(530, y2 + 335, VITAIDENT_MISC_NAME_COLOR, 1, "Internal storage free: ");
	pgf_draw_text(SCREEN_WIDTH - (150), y2 + 335, VITAIDENT_MISC_ITEM_COLOR, 1, free_size_string);
}


void drawVitaidentMenu() {
  
	if (Vitaident_menu.status == VITAIDENT_MENU_CLOSED)
		return;

	// Closing settings menu
	if (Vitaident_menu.status == VITAIDENT_MENU_CLOSING) {
		if (Vitaident_menu.cur_pos > 0.0f) {
			Vitaident_menu.cur_pos -= easeOut(0.0f, Vitaident_menu.cur_pos, 0.25f);
		} else {
			Vitaident_menu.status = VITAIDENT_MENU_CLOSED;
		}
	}

	// Opening settings menu
	if (Vitaident_menu.status == VITAIDENT_MENU_OPENING) {
		if (Vitaident_menu.cur_pos < SCREEN_HEIGHT) {
			Vitaident_menu.cur_pos += easeOut(Vitaident_menu.cur_pos, SCREEN_HEIGHT, 0.25f);
		} else {
			Vitaident_menu.status = VITAIDENT_MENU_OPENED;
		}
	}

	// Draw settings menu
	vita2d_draw_texture(settings_image, 0.0f, SCREEN_HEIGHT - Vitaident_menu.cur_pos);

	float y = SCREEN_HEIGHT - Vitaident_menu.cur_pos + START_Y;
	float y2 = SCREEN_HEIGHT - Vitaident_menu.cur_pos;
	
	if (Vitaident_menu.cur_pos > 0.0f) {
		launchVitaident();
	}
}

static int agreement = VITAIDENT_AGREEMENT_NONE;

void VitaidentAgree() {
	agreement = VITAIDENT_AGREEMENT_AGREE;
}

void VitaidentDisagree() {
	agreement = VITAIDENT_AGREEMENT_DISAGREE;
}

void VitaidentMenuCtrl() {

	// Agreement
	if (agreement != VITAIDENT_AGREEMENT_NONE) {
		agreement = VITAIDENT_AGREEMENT_NONE;
	}

	// Close
	if (pressed_buttons & (SCE_CTRL_CANCEL | SCE_CTRL_RIGHT_ANALOG_RIGHT)) {
		closeVitaidentMenu();
	}
}
 
