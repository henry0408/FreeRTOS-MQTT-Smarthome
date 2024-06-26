#ifndef __FONT_SYSTEM_H
#define __FONT_SYSTEM_H

#include "font_manager.h"

void AddFontLibs(void);

PFontLib GetFontLib(char *name);
int SetDefaultFontLib(char *name);
int InitDefaultFontLib(void);
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap);

#endif

