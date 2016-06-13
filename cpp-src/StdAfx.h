// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_FDEF)
#define AFX_STDAFX_FDEF

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

///////////////////////////////////////////////////////////////////////////
#define _ITERATOR_DEBUG_LEVEL 0


const long NAME_LEN = 32;									//���Ƶ��ַ���ռ�ڴ��С

const char FILE_EXTENT[8] = "wan";							//�ļ���׺, ����.
const char FILE_EXTENT_DOT[8] = ".wan";						//�ļ���׺, ��.
const char DEFAULT_FILE_NAME[16] = "data.wan";				//Ĭ���ļ���
const char FILE_LIST[32] = "��·�ļ�(*.wan)|*.wan||";		//֧���ļ��б�

enum BODY_TYPE												//ָ�����������
{
	BODY_ALL		= -5,
	BODY_ALLCTRL	= -4,
	BODY_LEAD		= -3,
	BODY_NO			= -2,
	BODY_CRUN		= -1,
	SOURCE			= 0,
	RESIST			= 1,
	BULB			= 2,
	CAPA			= 3,
	SWITCH			= 4
};

//��ENUM_STYLEʹ��
enum STYLE_LIST
{
	ENUM_CTRL		= 0,									//�ؼ�����
	ENUM_COLOR		= 1,									//��ɫ����
	ENUM_LEADSTYLE	= 2										//������ʽ
};

const long CTRL_TYPE_NUM = 5;								//�ؼ����͸���
const char CTRL_STYLE_NAME[CTRL_TYPE_NUM][NAME_LEN] =		//�ؼ����Ͷ�Ӧ������
{
	"��Դ",
	"����",
	"С����",
	"������",
	"����"
};

enum DATA_STYLE												//LISTDATA���������Ͷ���
{
	DATA_STYLE_float	= 0,
	DATA_STYLE_double	= 1,
	DATA_STYLE_UINT		= 2,
	DATA_STYLE_bool		= 3,
	DATA_STYLE_LPCTSTR	= 4,
	DATA_STYLE_enum		= 5
};

enum COLOR													//��ɫö��
{
	BLACK			= 0, 
	RED				= 1, 
	YELLOW			= 2, 
	GREEN			= 3, 
	BLUE			= 4,
	RESERVE_COLOR	= 5
};
const long COLOR_TYPE_NUM = 5;								//�û�����ѡ����ɫ����
const char COLORNAMES[COLOR_TYPE_NUM][8] =					//��ɫ��Ӧ������
{
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"��ɫ",
	"��ɫ"
};

enum LEADSTYLE													//������ʽ
{
	SOLID_RESERVE_COLOR		= 0,	//ʵ�߱���ɫ
	SOLID_ORIGINAL_COLOR	= 1,	//ʵ��ԭ����ɫ
	DOT_ORIGINAL_COLOR		= 2,	//����ԭ����ɫ
	DOT_RESERVE_COLOR		= 3		//���߱���ɫ
};
const long LEAD_STYLE_NUM = 4;									//������ʽ����
const char LEADSTYLENAMES[4][16] =								//������ʽ��Ӧ������
{
	"ʵ����ɫ",
	"ʵ��ԭ����ɫ",
	"����ԭ����ɫ",
	"������ɫ",
};

const char TITLE_NOTE[]     = "��ǩ         (����Ϊ��)";	//��ǩ��Ӧ����ʾ
const char TITLESHOW_NOTE[] = "��ʾ��ǩ";					//��ʾ��ǩ��Ӧ����ʾ


enum ELEC_STATE												//����״̬ö��
{
	UNKNOWNELEC		= -2,	//����δ����
	ERRORELEC		= -1,	//�������
	NORMALELEC		= 0,	//��������
	LEFTELEC		= 0,	//����������
	RIGHTELEC		= 1,	//�������ҵ���
	OPENELEC		= 6,	//��·
	SHORTELEC		= 7,	//��·
	UNCOUNTABLEELEC	= 8		//�����޷�����ķ�֧
};

const long MAXCTRLNUM	= 128;								//�ؼ��������
const long MAXCRUNNUM	= 64;								//����������
const long MAXLEADNUM	= MAXCRUNNUM*2 + MAXCTRLNUM;		//�����������

const SIZE BODYSIZE = {29, 29};								//�ؼ��Ĵ�С
const long DD = 4;											//����ĳ���������<=DDʱ,��Ϊ�ƶ�����������

enum CLONE_PURPOSE
{
	CLONE_FOR_USE		= 0,	//����Ϊ�˵�ǰʹ��
	CLONE_FOR_SAVE		= 1,	//����Ϊ�˱���
	CLONE_FOR_CLIPBOARD	= 2		//����Ϊ�˼��а�ʹ��
};

//����������������ϵ�����
struct FOCUS_OR_POS
{
	bool isFocusBody;
	POINT pos;
};

enum SEARCH_BY
{
	SEARCH_BY_NAME	= 0,	//������������
	SEARCH_BY_ID	= 1		//�����������
};

//������
class CTRL;
class CRUN;
class LEAD;
class Pointer;
class LISTDATA;
class ENUM_STYLE;


////////////////////////////////////////////////////////////////////////////
//������� "resource.h" ��Ҫ����:
/*
	IDB_SOURCE
	IDB_RESIST
	IDB_BULB
	IDB_CAPA
	IDB_SWITCH
	IDB_BULB_SHINE
	IDB_SWITCH_CLOSE

	IDM_ADD_NO
	IDM_ADD_CRUNODE
	IDM_ADD_SOURCE
	IDM_ADD_RESIST
	IDM_ADD_BULB
	IDM_ADD_CAPA
	IDM_ADD_SWITCH
*/


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_FDEF)
