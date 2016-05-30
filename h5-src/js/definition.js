
var NAME_LEN = 32;	//���Ƶ��ַ���ռ�ڴ��С

var FILE_EXTENT = "wan";						//�ļ���׺, ����.
var FILE_EXTENT_DOT = ".wan";					//�ļ���׺, ��.
var DEFAULT_FILE_NAME = "data.wan";				//Ĭ���ļ���
var FILE_LIST = "��·�ļ�(*.wan)|*.wan||";		//֧���ļ��б�


//ָ�����������
var BODY_ALL		= -5;
var BODY_ALLCTRL	= -4;
var BODY_LEAD		= -3;
var BODY_NO			= -2;
var BODY_CRUN		= -1;
var SOURCE			= 0;
var RESIST			= 1;
var BULB			= 2;
var CAPA			= 3;
var SWITCH			= 4;


//��ENUM_STYLEʹ��
var CTRL_TYPE_ENUM	= 0;	//�ؼ�����
var LEAD_STYLE_ENUM	= 1;	//������ʽ



var CTRL_TYPE_COUNT = 5;	//�ؼ����͸���
var CTRL_TYPE_NAMES = new Array(	//�ؼ����Ͷ�Ӧ������
	"��Դ",
	"����",
	"С����",
	"������",
	"����"
);


//LISTDATA���������Ͷ���
var DATA_TYPE_float		= 0;
var DATA_TYPE_uint		= 1;
var DATA_TYPE_bool		= 2;
var DATA_TYPE_string	= 3;
var DATA_TYPE_enum		= 4;



//������ʽ
var SOLID_RESERVE_COLOR		= 0;	//ʵ�߱���ɫ
var SOLID_ORIGINAL_COLOR	= 1;	//ʵ��ԭ����ɫ
var DOT_ORIGINAL_COLOR		= 2;	//����ԭ����ɫ
var DOT_RESERVE_COLOR		= 3;	//���߱���ɫ
var LEAD_STYLE_COUNT = 4;	//������ʽ����
//������ʽ��Ӧ������
var LEAD_STYLE_NAMES = new Array(
	"ʵ����ɫ",
	"ʵ��ԭ����ɫ",
	"����ԭ����ɫ",
	"������ɫ",
);

var TITLE_NOTE[]     = "��ǩ         (����Ϊ��)";	//��ǩ��Ӧ����ʾ
var TITLESHOW_NOTE[] = "��ʾ��ǩ";					//��ʾ��ǩ��Ӧ����ʾ


//����״̬ö��
var UNKNOWNELEC		= -2;	//����δ����
var ERRORELEC		= -1;	//�������
var NORMALELEC		= 0;	//��������
var LEFTELEC		= 0;	//����������
var RIGHTELEC		= 1;	//�������ҵ���
var OPENELEC		= 6;	//��·
var SHORTELEC		= 7;	//��·
var UNCOUNTABLEELEC	= 8;	//�����޷�����ķ�֧


var MAX_CTRL_COUNT	= 128;	//�ؼ��������
var MAX_CRUN_COUNT	= 64;	//����������
var MAX_LEAD_COUNT	= MAX_CRUN_COUNT*2 + MAX_CTRL_COUNT;	//�����������

var BODYSIZE = {29, 29};	//�ؼ��Ĵ�С
var DD = 4;	//����ĳ���������<=DDʱ,��Ϊ�ƶ�����������

//��������Ŀ��ö��
var CLONE_FOR_USE		= 0;	//����Ϊ�˵�ǰʹ��
var CLONE_FOR_SAVE		= 1;	//����Ϊ�˱���
var CLONE_FOR_CLIPBOARD	= 2;	//����Ϊ�˼��а�ʹ��


//����������������ϵ�����
var FOCUS_OR_POS = {
	CreateNew: {
		return {isFocusBody:false, x:0, y:0};
	}
};

// �����ؼ���
var SEARCH_BY_NAME	= 0;	//������������
var SEARCH_BY_ID	= 1;	//�����������


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
