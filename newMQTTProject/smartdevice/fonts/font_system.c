#include "font_system.h"
#include "ascii_font.h"
#include "font_manager.h"

void AddFontLibs(void)
{
	AddFontLibASCII();
}

PFontLib GetFontLib(char *name)
{
	return __GetFontLib(name);
}

static PFontLib g_ptDefaultFontLib;
int SetDefaultFontLib(char *name)
{
	g_ptDefaultFontLib = GetFontLib(name);
	if (g_ptDefaultFontLib)
		return 0;
	else
		return -1;
}

/**********************************************************************
 * 函数名称： InitDefaultFontLib
 * 功能描述： 初始化默认的字库
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 成功-0, 失败-(-1)
 ***********************************************************************/
int InitDefaultFontLib(void)
{
	if (g_ptDefaultFontLib->FontInit)	
		return g_ptDefaultFontLib->FontInit(g_ptDefaultFontLib);
	else
		return 0;
}


/**********************************************************************
 * 函数名称： GetFontBitMap
 * 功能描述： 在默认的字库里, 获取某个字符的点阵
 * 输入参数： dwCode-哪个字符,字符的编码
 * 输出参数： ptFontBitMap-用来保存字库点阵
 * 返 回 值： 0-成功, -1-失败
 ***********************************************************************/
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	return g_ptDefaultFontLib->GetFontBitMap(dwCode, ptFontBitMap);
}
