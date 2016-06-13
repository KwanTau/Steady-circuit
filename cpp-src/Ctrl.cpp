/* �Ⱥ��·��ѧģ����
   ��Ȩ���У�C�� 2013 <����>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
   
#include "StdAfx.h"
#include "StaticClass.h"	//����static��������
#include "Lead.h"			//������
#include "DataList.h"		//LISTDATA, ENUM_STYLE��
#include "Equation.h"		//������
#include "Ctrl.h"			//��ǰ��

unsigned long CTRL::s_initNum = 1;


double CTRL::GetSpecialData()const
//@��ÿؼ�����������
{
	switch(style)
	{
	case SOURCE:
		return ((SOURCEDATA *)data)->pressure;
	case RESIST:
		return ((RESISTDATA *)data)->resist;
	case BULB:
		return ((BULBDATA *)data)->rating;
	case CAPA:
		return ((CAPACITYDATA *)data)->capa;
	case SWITCH:
		return ((SWITCHDATA *)data)->onOff;
	}

	return 0;
}

void CTRL::InitData(BODY_TYPE ctrlStyle)
//��ʼ���ؼ����ݲ���
{
	ASSERT(ctrlStyle >= 0 && ctrlStyle < CTRL_TYPE_NUM);

	this->style = ctrlStyle;
	data = malloc(CTRL_DATA_SIZE[style]);
	ZeroMemory(data, CTRL_DATA_SIZE[style]);
}

CTRL::CTRL(long memNum, POINT pos, BODY_TYPE ctrlStyle, bool isInit)
{
	lead[0] = lead[1] = NULL;			//��������
	num = memNum;						//���
	initNum = s_initNum++;				//��ʼ�����
	isPaintName = true;					//Ĭ����ʾ�ؼ���ǩ
	sprintf(name, "Ctrl%d", initNum);	//����Ĭ������
	dir = 0;							//�ؼ�Ĭ�Ϸ���
	coord = pos;						//��ʼ������
	elecDir = UNKNOWNELEC;				//��������

	if(isInit) InitData(ctrlStyle);		//��ʼ���ؼ����ݲ���
	else data = NULL;
}

CTRL::~CTRL()
{
	free(data); 
	data = NULL; 
}

CTRL * CTRL::Clone(CLONE_PURPOSE cp)const
//�����ؼ���Ϣ���µĿؼ�
{
	CTRL * newCtrl = new CTRL(num, coord, style);				//�½�ͬһ�ֿؼ�
	strcpy(newCtrl->name, this->name);							//ʹ������ͬ
	newCtrl->isPaintName = this->isPaintName;					//ʹ��ʾ��������һ��
	newCtrl->dir = this->dir;									//ʹ�ؼ�������ͬ
	memcpy(newCtrl->data, this->data, CTRL_DATA_SIZE[style]);	//ʹ������ͬ

	if(CLONE_FOR_USE != cp)
	{
		newCtrl->initNum = this->initNum;
		--s_initNum;
	}
	return newCtrl;
}

void CTRL::SaveToFile(FILE * fp)const
//���浽�ļ�
{
	int i, t;
	ASSERT(fp != NULL);

	fwrite(&coord, sizeof(POINT), 1, fp);
	fwrite(&dir, sizeof(dir), 1, fp);
	fwrite(&style, sizeof(int), 1, fp);
	for(i = 0; i < 2; ++i)
	{
		if(lead[i])
			t = lead[i]->num;
		else
			t = -1;
		fwrite(&t, sizeof(int), 1, fp);
	}
	fwrite(&isPaintName, sizeof(bool), 1, fp);
	fwrite(name, 1, NAME_LEN, fp);
	fwrite(data, CTRL_DATA_SIZE[style], 1, fp);
}

void CTRL::ReadFromFile(FILE * fp, LEAD ** allLead)
//���ļ���ȡ
{
	int i, t;
	ASSERT(fp!=NULL && allLead!=NULL);

	fread(&coord, sizeof(POINT), 1, fp);
	fread(&dir, sizeof(dir), 1, fp);
	fread(&style, sizeof(int), 1, fp);
	for(i = 0; i < 2; ++i)
	{
		fread(&t, sizeof(int), 1, fp);
		if(t >= 0 && t < MAXLEADNUM)
			lead[i] = allLead[t];
		else
			lead[i] = NULL;
	}
	fread(&isPaintName, sizeof(bool), 1, fp);
	fread(name, 1, NAME_LEN, fp);
	data = malloc(CTRL_DATA_SIZE[style]);	//����ռ�
	fread(data, CTRL_DATA_SIZE[style], 1, fp);
}

void CTRL::ChangeStyle(BODY_TYPE newStyle)
//�ı�ؼ�����
{
	ASSERT(newStyle != style);
	free(data);			//�ͷ�ԭ�����Ͷ�Ӧ�����ݿռ�
	InitData(newStyle);	//��ʼ���ؼ����ݲ���
}

unsigned long CTRL::GetInitOrder()const
//��ó�ʼ�����
{
	return initNum;
}

void CTRL::ResetInitNum()
//���ó�ʼ������
{
	CTRL::s_initNum = 1;
}

BODY_TYPE CTRL::GetStyle()const
//��ÿؼ�����
{
	return style;
}

int CTRL::GetConnectNum()const
//��ÿؼ����ӵĵ�����
{
	return (lead[0] != NULL) + (lead[1] != NULL) ; 
}

int CTRL::GetDirect(const LEAD * l)const
//Ѱ�ҵ������ĸ����� : 0��,1��,2��,3��
{
	int i;
	for(i=0; i<2; ++i) if(lead[i] == l) break;
	if(i >= 2) return -1;	//û���ҵ�

	ASSERT(dir>=0 && dir < 4);

	switch(dir)	//���ݿؼ������ж�
	{
	case 0: return 2 + i;	//0:2;1:3
	case 1: return i;		//0:0;1:1
	case 2: return 3 - i;	//0:3;1:2
	case 3: return 1 - i;	//0:1;1:0
	default: return 0;
	}
}

int CTRL::At(POINT p)const
//�������ڿؼ���λ��
{
	int ret = 0;
	POINT c;
	c.x = p.x - coord.x - (BODYSIZE.cx>>1);
	c.y = p.y - coord.y - (BODYSIZE.cy>>1);

	if(! (dir&1))	//����
	{
		if(c.x < 0)
		{
			c.x += (BODYSIZE.cx>>1);
			if(c.x*c.x + c.y*c.y <= DD*DD)
			{	//ѡ�������ӵ�
				if(! (dir&2)) ret = 1;
				else ret = 2;
			}
		}
		else
		{
			c.x -= (BODYSIZE.cx>>1);
			if(c.x*c.x + c.y*c.y <= DD*DD)
			{	//ѡ�������ӵ�
				if(! (dir&2)) ret = 2;
				else ret = 1;
			}
		}
	}

	else //����
	{
		if(c.y < 0)
		{
			c.y += (BODYSIZE.cy>>1);
			if(c.x*c.x + c.y*c.y <= DD*DD)
			{	//ѡ�������ӵ�
				if(! (dir&2)) ret = 1;
				else ret = 2;
			}
		}
		else
		{
			c.y -= (BODYSIZE.cy>>1);
			if(c.x*c.x + c.y*c.y <= DD*DD)
			{	//ѡ�������ӵ�
				if(! (dir&2)) ret = 2;
				else ret = 1;
			}
		}
	}

	if(ret != 0)
	{
		if(lead[ret-1] == NULL)
			return ret;
		else
			return -1;
	}

	if( p.x>=coord.x && p.x<coord.x+BODYSIZE.cx 
		&& p.y>=coord.y && p.y<coord.y+BODYSIZE.cy)
		return -1;	//�ڿؼ���

	return 0;
}

void CTRL::Rotate(int rotateAngle)
//��ת�ؼ�
{
	dir = (dir + rotateAngle) % 4;
	if(lead[0]) lead[0]->RefreshPos();
	if(lead[1]) lead[1]->RefreshPos();
}

double CTRL::GetResist()const
//@��ÿؼ��ĵ���
{
	if(RESISTANCE_TYPE[style] != 1)
		return RESISTANCE_TYPE[style];

	switch(style)
	{
	case SOURCE:
		if(((SOURCEDATA *)data)->haveResist)
			return ((SOURCEDATA*)data)->resist;
		else
			return 0;

	case RESIST:
		return ((RESISTDATA*)data)->resist;

	case BULB:
		return ((BULBDATA*)data)->resist;

	case SWITCH:
		if(((SWITCHDATA*)data)->onOff)
			return 0;	//���رպ�
		else
			return -1;	//���ضϿ�
	}

	return 0;
}

double CTRL::GetPress(int direction)const
//@��ÿؼ��ĵ�ѹ
{
	double pressure;	//���ص�ѹ
	if(!PRESSURE_TYPE[style]) return 0;	//���ṩ��ѹ

	switch(style)
	{
	case SOURCE:
		pressure = ((SOURCEDATA*)data)->pressure;
		if(direction)
			return - pressure;
		else
			return   pressure;
	}

	return 0;
}

bool CTRL::IsBulbOn()const
//@С�����Ƿ�ﵽ����ʶ�����
{
	double sData = GetSpecialData();

	if(BULB != style)
		return false;	//����С����
	if(elecDir != LEFTELEC && elecDir != RIGHTELEC)
		return false;	//����û�м�����߲���������

	double tempData = GetResist() * elec * elec;

	if(!StaticClass::IsZero(sData) && tempData >= sData)
		return true;	//�ﵽ����ʶ�����
	else
		return false;	//û�дﵽ�����
}

bool CTRL::SwitchOnOff(bool isSwitch)const
//@���رպϻ��߶Ͽ�
{
	if(SWITCH != style) return false;	//���ǿ���
	bool & tempData = ((SWITCHDATA *)data)->onOff ;
	if(isSwitch) tempData = !tempData;
	return tempData;
}

void CTRL::GetDataList(LISTDATA * list)const
//@��CProperty������Ϣ
{
	list->Init(2 + DATA_ITEM_NUM[style]);

	list->SetAMember(DATA_STYLE_LPCTSTR, TITLE_NOTE, (void *)name);
	list->SetAMember(DATA_STYLE_bool, TITLESHOW_NOTE, (void *)(&isPaintName));

	switch(style)
	{
	case SOURCE:
		list->SetAMember(
			DATA_STYLE_double,
			DATA_NOTE[DATA_NOTE_PRESS],
			(void *)(&((SOURCEDATA*)data)->pressure));
		list->SetAMember(
			DATA_STYLE_double,
			DATA_NOTE[DATA_NOTE_RESIST],
			(void *)(&((SOURCEDATA*)data)->resist));
		list->SetAMember(
			DATA_STYLE_bool,
			DATA_NOTE[DATA_NOTE_HAVERESIST],
			(void *)(&((SOURCEDATA*)data)->haveResist));
		break;

	case RESIST:
		list->SetAMember(
			DATA_STYLE_double,
			DATA_NOTE[DATA_NOTE_RESIST],
			(void *)(&((RESISTDATA*)data)->resist));
		break;

	case BULB:
		list->SetAMember(
			DATA_STYLE_double,
			DATA_NOTE[DATA_NOTE_RATING],
			(void *)(&((BULBDATA*)data)->rating));
		list->SetAMember(
			DATA_STYLE_double,
			DATA_NOTE[DATA_NOTE_RESIST],
			(void *)(&((BULBDATA*)data)->resist));
		break;

	case CAPA:
		list->SetAMember(
			DATA_STYLE_double,
			DATA_NOTE[DATA_NOTE_CAPA],
			(void *)(&((CAPACITYDATA*)data)->capa));
		break;

	case SWITCH:
		list->SetAMember(
			DATA_STYLE_bool,
			DATA_NOTE[DATA_NOTE_SWITCHONOFF],
			(void *)(&((SWITCHDATA*)data)->onOff));
		break;
	}
}

void CTRL::SaveToTextFile(FILE * fp)const
//@��������ʽ����,���Ժ���
{
	ASSERT(fp != NULL);

	//fprintf(fp, "name == %s\n", name);
	//fprintf(fp, "Init Order  == %d\n", GetInitOrder());

	switch(style)
	{
	case SOURCE:
		fprintf(fp, "pressure:%f,", ((SOURCEDATA*)data)->pressure);
		fprintf(fp, "resist:%f,", ((SOURCEDATA*)data)->resist);
		break;
	case RESIST:
		fprintf(fp, "resist:%f,", ((RESISTDATA*)data)->resist);
		break;
	case BULB:
		fprintf(fp, "rating:%f,", ((BULBDATA*)data)->rating);
		fprintf(fp, "resist:%f,", ((BULBDATA*)data)->resist);
		break;
	case CAPA:
		fprintf(fp, "capa:%f,", ((CAPACITYDATA*)data)->capa);
		break;
	case SWITCH:
		if(((SWITCHDATA*)data)->onOff)
			fputs("closed:true,", fp);
		else
			fputs("closed:false,", fp);
		break;
	}
}
