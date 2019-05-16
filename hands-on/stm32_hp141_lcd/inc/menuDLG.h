#ifndef MENU_DLG
#define MENU_DLG

#include "DIALOG.h"
#include "options.h"
#include "WM.h"

WM_HWIN Createmenu(void);
void globalMenuRefresh();

extern option_t *option_setting_hook;
extern WM_HWIN menu_hwin;

#endif
