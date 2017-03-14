/*
	VitaShell
	Copyright (C) 2015-2017, TheFloW

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
#include "file.h"
#include "message_dialog.h"
#include "uncommon_dialog.h"
#include "theme.h"
#include "language.h"
#include "utils.h"
#include "bm.h"

SceCtrlData pad;
uint32_t old_buttons, current_buttons, pressed_buttons, hold_buttons, hold2_buttons, released_buttons;

static int netdbg_sock = -1;
static void *net_memory = NULL;
static int net_init = -1;

static int lock_power = 0;

static uint64_t wallpaper_time_start = 0;
static int wallpaper_random_delay = 0;
static float wallpaper_alpha = 255.0f;

void startDrawing(vita2d_texture *bg) {
	vita2d_start_drawing();
	vita2d_set_clear_color(BACKGROUND_COLOR);
	vita2d_clear_screen();

	/*if (wallpaper_image) {
		vita2d_draw_texture(wallpaper_image, 0.0f, 0.0f);
	} else if (bg) {
		vita2d_draw_texture(bg, 0.0f, 0.0f);
	}*/

	// Wallpaper
	if (bg){
		vita2d_draw_texture(bg, 0.0f, 0.0f);
	}else if (current_wallpaper_image) {
		if (wallpaper_time_start == 0) {
			wallpaper_time_start = sceKernelGetProcessTimeWide();
			wallpaper_random_delay = randomNumber(15, 30);
		}

		if ((sceKernelGetProcessTimeWide() - wallpaper_time_start) >= (wallpaper_random_delay * 1000 * 1000)) {
			int random_num = randomNumber(0, wallpaper_count - 1);

			vita2d_texture *random_wallpaper_image = wallpaper_image[random_num];
			if (random_wallpaper_image != current_wallpaper_image) {
				previous_wallpaper_image = current_wallpaper_image;
				current_wallpaper_image = random_wallpaper_image;
				wallpaper_alpha = 0.0f;
			}

			wallpaper_time_start = 0;
		}

		if (previous_wallpaper_image) {
			vita2d_draw_texture(previous_wallpaper_image, 0.0f, 0.0f);
		}

		if (wallpaper_alpha < 255.0f) {
			vita2d_draw_texture_tint(current_wallpaper_image, 0.0f, 0.0f, RGBA8(255, 255, 255, (int)wallpaper_alpha));
			wallpaper_alpha += 1.5f;
		} else {
			vita2d_draw_texture(current_wallpaper_image, 0.0f, 0.0f);
			previous_wallpaper_image = NULL;
		}
	}
	
	if (filter_wallpaper_image)
	vita2d_draw_texture(filter_wallpaper_image, 0.0f, 0.0f);
}

void drawingWallpaperUI2(vita2d_texture *bg,float x, float y, float tex_x, float tex_y, float tex_w, float tex_h, int change) {
  	/*if (wallpaper_image) {
		vita2d_draw_texture_part(wallpaper_image, x, y, tex_x, tex_y, tex_w, tex_h);
	} else if (bg) {
		vita2d_draw_texture_part(bg, x, y, tex_x, tex_y, tex_w, tex_h);
	}*/
	
	// Wallpaper
	if (bg){
	    vita2d_draw_texture_part(bg, x, y, tex_x, tex_y, tex_w, tex_h);
	}else if (current_wallpaper_image) {
		if (wallpaper_time_start == 0) {
			wallpaper_time_start = sceKernelGetProcessTimeWide();
			wallpaper_random_delay = randomNumber(15, 30);
		}

		if ((sceKernelGetProcessTimeWide() - wallpaper_time_start) >= (wallpaper_random_delay * 1000 * 1000)) {
		  if (change == 1) {
			int random_num = randomNumber(0, wallpaper_count - 1);

			vita2d_texture *random_wallpaper_image = wallpaper_image[random_num];
			if (random_wallpaper_image != current_wallpaper_image) {
				previous_wallpaper_image = current_wallpaper_image;
				current_wallpaper_image = random_wallpaper_image;
				wallpaper_alpha = 0.0f;
			}
		  }
			wallpaper_time_start = 0;
		}

		if (previous_wallpaper_image) {
		  
		  vita2d_draw_texture_part(previous_wallpaper_image, x, y, tex_x, tex_y, tex_w, tex_h);
		    
		  
		}

		if (wallpaper_alpha < 255.0f) {
		  vita2d_draw_texture_tint_part(current_wallpaper_image, x, y, tex_x, tex_y, tex_w, tex_h, RGBA8(255, 255, 255, (int)wallpaper_alpha));	
		    
	      	wallpaper_alpha += 1.5f;
		} else {
		
		    vita2d_draw_texture_part(current_wallpaper_image,x, y, tex_x, tex_y, tex_w, tex_h);		    
	       	previous_wallpaper_image = NULL;
		}
	}
	
	if (filter_wallpaper_image)
	vita2d_draw_texture(filter_wallpaper_image, 0.0f, 0.0f);
}

void endDrawing() {
	drawUncommonDialog();
	vita2d_end_drawing();
	vita2d_common_dialog_update();
	vita2d_swap_buffers();
	sceDisplayWaitVblankStart();
}

void closeWaitDialog() {
	sceMsgDialogClose();

	while (updateMessageDialog() != MESSAGE_DIALOG_RESULT_NONE) {
		sceKernelDelayThread(1000);
	}
}

void errorDialog(int error) {
	if (error < 0) {
		initMessageDialog(SCE_MSG_DIALOG_BUTTON_TYPE_OK, language_container[ERROR], error);
		dialog_step = DIALOG_STEP_ERROR;
	}
}

void infoDialog(char *msg, ...) {
	va_list list;
	char string[512];

	va_start(list, msg);
	vsprintf(string, msg, list);
	va_end(list);

	initMessageDialog(SCE_MSG_DIALOG_BUTTON_TYPE_OK, string);
	dialog_step = DIALOG_STEP_INFO;
}

int checkMemoryCardFreeSpace(uint64_t size) {
	uint64_t free_size = 0, max_size = 0;
	sceAppMgrGetDevInfo("ux0:", &max_size, &free_size);

	if (size >= (free_size + (40 * 1024 * 1024))) {
		closeWaitDialog();

		char size_string[16];
		getSizeString(size_string, size - (free_size + (40 * 1024 * 1024)));
		infoDialog(language_container[NO_SPACE_ERROR], size_string);

		return 1;
	}

	return 0;
}

int power_tick_thread(SceSize args, void *argp) {
	while (1) {
		if (lock_power > 0) {
			sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND);
		}

		sceKernelDelayThread(10 * 1000 * 1000);
	}

	return 0;
}

void initPowerTickThread() {
	SceUID thid = sceKernelCreateThread("power_tick_thread", power_tick_thread, 0x10000100, 0x40000, 0, 0, NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);
}

void powerLock() {
	if (!lock_power)
		sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN);

	lock_power++;
}

void powerUnlock() {
	if (lock_power)
		sceShellUtilUnlock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN);

	lock_power--;
	if (lock_power < 0)
		lock_power = 0;
}

void readTouchPad(){
  
	memset(&touch, 0, sizeof(SceTouchData));
	//sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
	//sceTouchPeek(SCE_TOUCH_PORT_BACK, &touch, 1);
  
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_BACK);
	
	SceTouchData touch_old[SCE_TOUCH_PORT_MAX_NUM];
	SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];
	
	memcpy(touch_old, touch, sizeof(touch_old));
	int port,i;
	
	/* sample both back and front surfaces */
	for(port = 0; port < SCE_TOUCH_PORT_MAX_NUM; port++){
		sceTouchPeek(port, &touch[port], 1);
		printf("%s",((const char*[]){"FRONT","BACK "})[port]);
		/* print every touch coordinates on that surface */
		for(i = 0; i < SCE_TOUCH_MAX_REPORT; i++)
			printf("\e[9%im%4i:%-4i ", (i < touch[port].reportNum)? 7:0,
		        touch[port].report[i].x,touch[port].report[i].y);
		printf("\n");
	}
  
	/*if ( (touch[SCE_TOUCH_PORT_FRONT].reportNum == 1)
		  && (touch_old[SCE_TOUCH_PORT_FRONT].reportNum == 1)
		  && (touch[SCE_TOUCH_PORT_FRONT].report[0].y >= 1000)
		  && (touch_old[SCE_TOUCH_PORT_FRONT].report[0].y < 1000))*/
  
}

void readPad() {
	static int hold_n = 0, hold2_n = 0;

	memset(&pad, 0, sizeof(SceCtrlData));
	sceCtrlPeekBufferPositive(0, &pad, 1);

	if (pad.ly < ANALOG_CENTER - ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_LEFT_ANALOG_UP;
	} else if (pad.ly > ANALOG_CENTER + ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_LEFT_ANALOG_DOWN;
	}

	if (pad.lx < ANALOG_CENTER - ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_LEFT_ANALOG_LEFT;
	} else if (pad.lx > ANALOG_CENTER + ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_LEFT_ANALOG_RIGHT;
	}

	if (pad.ry < ANALOG_CENTER - ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_RIGHT_ANALOG_UP;
	} else if (pad.ry > ANALOG_CENTER + ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_RIGHT_ANALOG_DOWN;
	}

	if (pad.rx < ANALOG_CENTER - ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_RIGHT_ANALOG_LEFT;
	} else if (pad.rx > ANALOG_CENTER + ANALOG_THRESHOLD) {
		pad.buttons |= SCE_CTRL_RIGHT_ANALOG_RIGHT;
	}

	current_buttons = pad.buttons;
	pressed_buttons = current_buttons & ~old_buttons;
	hold_buttons = pressed_buttons;
	hold2_buttons = pressed_buttons;
	released_buttons = ~current_buttons & old_buttons;

	if (old_buttons == current_buttons) {
		hold_n++;
		if (hold_n >= 10) {
			hold_buttons = current_buttons;
			hold_n = 6;
		}

		hold2_n++;
		if (hold2_n >= 10) {
			hold2_buttons = current_buttons;
			hold2_n = 10;
		}
	} else {
		hold_n = 0;
		hold2_n = 0;
		old_buttons = current_buttons;
	}
}

int holdButtons(SceCtrlData *pad, uint32_t buttons, uint64_t time) {
	if ((pad->buttons & buttons) == buttons) {
		uint64_t time_start = sceKernelGetProcessTimeWide();

		while ((pad->buttons & buttons) == buttons) {
			sceKernelDelayThread(10 * 1000);
			sceCtrlPeekBufferPositive(0, pad, 1);

			if ((sceKernelGetProcessTimeWide() - time_start) >= time) {
				return 1;
			}
		}
	}

	return 0;
}

int hasEndSlash(char *path) {
	return path[strlen(path) - 1] == '/';
}

int removeEndSlash(char *path) {
	int len = strlen(path);

	if (path[len - 1] == '/') {
		path[len - 1] = '\0';
		return 1;
	}

	return 0;
}

int addEndSlash(char *path) {
	int len = strlen(path);
	if (len < MAX_PATH_LENGTH - 2) {
		if (path[len - 1] != '/') {
			strcat(path, "/");
			return 1;
		}
	}

	return 0;
}

void getSizeString(char *string, uint64_t size) {
	double double_size = (double)size;

	int i = 0;
	static char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
	while (double_size >= 1024.0f) {
		double_size /= 1024.0f;
		i++;
	}

	sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}

void convertUtcToLocalTime(SceDateTime *time_local, SceDateTime *time_utc) {
	SceRtcTick tick;
	sceRtcGetTick(time_utc, &tick);
	sceRtcConvertUtcToLocalTime(&tick, &tick);
	sceRtcSetTick(time_local, &tick);	
}

void convertLocalTimeToUtc(SceDateTime *time_utc, SceDateTime *time_local) {
	SceRtcTick tick;
	sceRtcGetTick(time_local, &tick);
	sceRtcConvertLocalTimeToUtc(&tick, &tick);
	sceRtcSetTick(time_utc, &tick);	
}

void getDateString(char *string, int date_format, SceDateTime *time) {
	SceDateTime time_local;
	convertUtcToLocalTime(&time_local, time);

	switch (date_format) {
		case SCE_SYSTEM_PARAM_DATE_FORMAT_YYYYMMDD:
			sprintf(string, "%04d/%02d/%02d", time_local.year, time_local.month, time_local.day);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY:
			sprintf(string, "%02d/%02d/%04d", time_local.day, time_local.month, time_local.year);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY:
			sprintf(string, "%02d/%02d/%04d", time_local.month, time_local.day, time_local.year);
			break;
	}
}

void getTimeString(char *string, int time_format, SceDateTime *time) {
	SceDateTime time_local;
	convertUtcToLocalTime(&time_local, time);

	switch(time_format) {
		case SCE_SYSTEM_PARAM_TIME_FORMAT_12HR:
			sprintf(string, "%02d:%02d %s", (time_local.hour > 12) ? (time_local.hour - 12) : ((time_local.hour == 0) ? 12 : time_local.hour), time_local.minute, time_local.hour >= 12 ? "PM" : "AM");
			break;

		case SCE_SYSTEM_PARAM_TIME_FORMAT_24HR:
			sprintf(string, "%02d:%02d", time_local.hour, time_local.minute);
			break;
	}
}

int randomNumber(int low, int high) {
   return rand() % (high - low + 1) + low;
}

int debugPrintf(char *text, ...) {
	va_list list;
	char string[512];

	va_start(list, text);
	vsprintf(string, text, list);
	va_end(list);

	SceUID fd = sceIoOpen("ux0:data/vitatoolbox_log.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777);
	if (fd >= 0) {
		sceIoWrite(fd, string, strlen(string));
		sceIoClose(fd);
	}

	return 0;
}

int launchAppByUriExit(char *titleid) {
	char uri[32];
	sprintf(uri, "psgm:play?titleid=%s", titleid);

	sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);
	sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);

	sceKernelExitProcess(0);

	return 0;
}

int DestroyAppByName(char *name){
	
	sceAppMgrDestroyAppByName(name);
	
	return 0;
}

int launchAppByExec(char *app){
	char Patchapp[32];
	sprintf(Patchapp, "app0:%s", app);
	sceAppMgrLoadExec(Patchapp, NULL, NULL);
	return 0;
}

char *strcasestr(const char *haystack, const char *needle) {
	return boyer_moore(haystack, needle); 
}

int vshIoUmount(int id, int a2, int a3, int a4);
int _vshIoMount(int id, int a2, int permission, void *buf);

int vshIoMount(int id, int a2, int permission, int a4, int a5, int a6) {
	uint32_t buf[6];

	buf[0] = a4;
	buf[1] = a5;
	buf[2] = a6;
	buf[3] = 0;
	buf[4] = 0;
	buf[5] = 0;

	return _vshIoMount(id, a2, permission, buf);
}

SceUID startUsb(const char *usbDevicePath, const char *imgFilePath, int type) {
	int res;

	// Load and start usbdevice module
	SceUID modid = taiLoadStartKernelModule(usbDevicePath, 0, NULL, 0);
	if (modid < 0)
		return modid;

	// Stop MTP driver
	res = sceMtpIfStopDriver(1);
	if (res < 0)
		return res;

	// Set device information
	res = sceUsbstorVStorSetDeviceInfo("\"PS Vita\" MC", "1.00");
	if (res < 0)
		return res;

	// Set image file path
	res = sceUsbstorVStorSetImgFilePath(imgFilePath);
	if (res < 0)
		return res;

	// Start USB storage
	res = sceUsbstorVStorStart(type);
	if (res < 0)
		return res;

	return modid;
}

int stopUsb(SceUID modid) {
	int res;

	// Stop USB storage
	res = sceUsbstorVStorStop();
	if (res < 0)
		return res;

	// Start MTP driver
	res = sceMtpIfStartDriver(1);
	if (res < 0)
		return res;

	// Stop and unload usbdevice module
	res = taiStopUnloadKernelModule(modid, 0, NULL, 0, NULL, NULL);
	if (res < 0)
		return res;

	// Remount
	vshIoUmount(0x800, 0, 0, 0);
	vshIoMount(0x800, 0, 0, 0, 0, 0);

	// Trick
	sceAppMgrLoadExec("app0:eboot.bin", NULL, 0);

	return 0;
}
