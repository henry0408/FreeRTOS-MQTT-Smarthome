#ifndef __FONT_MANAGER_H
#define __FONT_MANAGER_H

typedef struct FontBitMap {
	int iLeftUpX;     /* λͼ���Ͻ�X���� */
	int iLeftUpY;     /* λͼ���Ͻ�Y���� */
	int iWidth;       /* ������ */
	int iRows;        /* ����߶� */
	int iCurOriginX;  /* ԭ��X���� */
	int iCurOriginY;  /* ԭ��Y���� */
	int iNextOriginX; /* ��һ���ַ�X���� */ 
	int iNextOriginY; /* ��һ���ַ�Y���� */ 
	unsigned char *pucBuffer; /* �ַ����� */
}FontBitMap, *PFontBitMap;

typedef struct FontLib {
	char *name;
	int (*FontInit)(struct FontLib *pFontLib);  /* ��ʼ���ֿ� */
	void (*GetFontSize)(int *piWidth, int *piHeigh);                   /* ��ȡ�����С */
	int (*SetFontSize)(int iFontSize);          /* ���������С */
	int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);  /* ��ȡĳ���ַ����ֿ� */
	struct FontLib *pNext;
}FontLib, *PFontLib;


/**********************************************************************
 * �������ƣ� FontLibRegister
 * ���������� ע��һ���ֿ�(��������)
 * ��������� ptFontLib-�ֿ�
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
void FontLibRegister(PFontLib ptFontLib);


//����ֿ�
PFontLib __GetFontLib(char *name);

#endif
