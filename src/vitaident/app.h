#ifndef _VITAIDENT_APP_H_
#define _VITAIDENT_APP_H_

#include "../main.h"

void initAppUtil();
void termAppUtil();
SceChar8 * getUser();
const char * getLang();
char * getStorageInfo(int type);
const char * getEnterButton();

#endif