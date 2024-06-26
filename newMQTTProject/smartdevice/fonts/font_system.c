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
 * �������ƣ� InitDefaultFontLib
 * ���������� ��ʼ��Ĭ�ϵ��ֿ�
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� �ɹ�-0, ʧ��-(-1)
 ***********************************************************************/
int InitDefaultFontLib(void)
{
	if (g_ptDefaultFontLib->FontInit)	
		return g_ptDefaultFontLib->FontInit(g_ptDefaultFontLib);
	else
		return 0;
}


/**********************************************************************
 * �������ƣ� GetFontBitMap
 * ���������� ��Ĭ�ϵ��ֿ���, ��ȡĳ���ַ��ĵ���
 * ��������� dwCode-�ĸ��ַ�,�ַ��ı���
 * ��������� ptFontBitMap-���������ֿ����
 * �� �� ֵ�� 0-�ɹ�, -1-ʧ��
 ***********************************************************************/
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	return g_ptDefaultFontLib->GetFontBitMap(dwCode, ptFontBitMap);
}
