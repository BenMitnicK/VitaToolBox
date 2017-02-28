/*
 * 
 * Made by NeoLTK with xerpi draw
 * 
 */
#ifndef __PLUGINS_MANAGER_H__
#define __PLUGINS_MANAGER_H__

enum PluginsManagerAgreement {
	PLUGINSMANAGER_AGREEMENT_NONE,
	PLUGINSMANAGER_AGREEMENT_AGREE,
	PLUGINSMANAGER_AGREEMENT_DISAGREE,
};

enum PluginsManagerOptionType {
	PLUGINSMANAGER_OPTION_TYPE_BOOLEAN,
	PLUGINSMANAGER_OPTION_TYPE_INTEGER,
	PLUGINSMANAGER_OPTION_TYPE_STRING,
	PLUGINSMANAGER_OPTION_TYPE_CALLBACK,
	PLUGINSMANAGER_OPTION_TYPE_NOTHING,
};

enum PluginsManagerMenuStatus {
	PLUGINSMANAGER_MENU_CLOSED,
	PLUGINSMANAGER_MENU_CLOSING,
	PLUGINSMANAGER_MENU_OPENED,
	PLUGINSMANAGER_MENU_OPENING,
};

typedef struct {
	int status;
	float cur_pos;
	int entry_sel;
	int option_sel;
	int n_options;
} PluginsManagerMenu;

typedef struct {
	int name;
	int type;
	int (* callback)();
	char *string;
	int size_string;
	int *value;
} PluginsManagerMenuOption;

typedef struct {
	int name;
	PluginsManagerMenuOption *options;
	int n_options;
} PluginsManagerMenuEntry;

typedef struct manager Manager;

typedef struct plugins {
	Manager *pluginsManager;
	char *name;
	char *path;
	bool active;
	//bool safe;
} Plugins;

typedef struct menu {
	char *title;
	void (*function)(Plugins *plugins);
} Menu;

typedef struct rightpanel {
	bool active;
	int navOffset;
	Menu *panelMenu;
} RightPanel;

struct manager {
	//char *version;
	//char *message;
	int navOffset;
	bool active;
	Plugins *plugins;
	RightPanel *rightPanel;
	//SceCtrlData pad;
};

#define defaultPath     ((char *) "ux0:/plugins/")
#define disablePath     ((char *) "ux0:/plugins_disabled/")
#define configsPath     ((char *) "ux0:/plugins/game.txt")

//void loadPluginsManagerConfig();
//void savePluginsManagerConfig();

void initPluginsManagerMenu();
void openPluginsManagerMenu();
void closePluginsManagerMenu();
int getPluginsManagerMenuStatus();
void drawPluginsManagerMenu();
void pluginsmanagerMenuCtrl();

void pluginsmanagerAgree();
void pluginsmanagerDisagree();

void uiDraw(Manager *pluginsManager);
//void rightPanelDraw(RightPanel *panel, int size)
void getPlugins(Manager *pluginsManager);
void getConfigs(Plugins plugins[]);
void dirPlugins(Manager *pluginsManager);
void putConfigs(Plugins plugins[]);
void enablePlugins(Plugins *plugins);
void disablePlugins(Plugins *plugins);
void controle(Manager *pluginsManager);

#endif