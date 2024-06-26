#include "font_manager.h"
#include <stdio.h>

#include <string.h>

//�����ֿ�����
static PFontLib g_ptFontLibs;

//ע���ֿ�
void FontLibRegister(PFontLib ptFontLib)
{
	ptFontLib->pNext = g_ptFontLibs;
	g_ptFontLibs = ptFontLib;
}

//����ֿ�
PFontLib __GetFontLib(char *name)
{
	PFontLib pTmp = g_ptFontLibs;
	while (pTmp)
	{
		if (strcmp(pTmp->name, name) == 0)
			return pTmp;
		else
			pTmp = pTmp->pNext;
	}
	return NULL;
}
