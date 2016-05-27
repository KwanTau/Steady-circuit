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
#include "Crun.h"			//��ǰ��

unsigned long CRUN::s_initNum = 1;


CRUN::CRUN(int memNum, POINT p)
{
	num = memNum;						//��ַ���
	initNum = s_initNum++;				//��ʼ�����
	isPaintName = false;				//Ĭ�ϲ���ʾ����ǩ
	sprintf(name, "Crun%d", initNum);	//��ʼ��Ĭ������
	coord = p;							//��ʼ������
	lead[0] = lead[1] = lead[2] = lead[3] = NULL;	//��ʼ��ָ��
}

unsigned long CRUN::GetInitOrder()const
//��ȡ��ʼ�����
{
	return initNum;
}

void CRUN::ResetInitNum()
//���ó�ʼ������
{
	CRUN::s_initNum = 1;
}

void CRUN::SaveToFile(FILE * fp)const
//��������Ϣ���ļ�
{
	int j, t;
	ASSERT(fp != NULL);

	fwrite(&coord, sizeof(POINT), 1, fp);
	fwrite(&isPaintName, sizeof(bool), 1, fp);
	fwrite(name, 1, NAME_LEN, fp);

	for(j=0; j<4; ++ j)
	{
		if(lead[j])
			t = lead[j]->num;
		else
			t = -1;
		fwrite(&t, sizeof(int), 1, fp);
	}
}

void CRUN::ReadFromFile(FILE * fp, LEAD ** allLead)
//���ļ���ȡ�����Ϣ
{
	int j, t;
	ASSERT(fp != NULL);

	fread(&coord, sizeof(POINT), 1, fp);
	fread(&isPaintName, sizeof(bool), 1, fp);
	fread(name, 1, NAME_LEN, fp);

	for(j = 0; j < 4; ++ j)
	{
		fread(&t, sizeof(t), 1, fp);
		if(t >= 0 && t < MAXLEADNUM)
			lead[j] = allLead[t];
		else 
			lead[j] = NULL;
	}
}

int CRUN::At(POINT p)const
//�������ڽ���λ��
{
	int dis, disBetweenCenter;

	disBetweenCenter = (p.x-coord.x)*(p.x-coord.x)+(p.y-coord.y)*(p.y-coord.y);
	if(disBetweenCenter > 4 * DD * DD) return 0;	//�����Զ,���������Աߵ����ӵ�

	dis = (p.x-coord.x)*(p.x-coord.x)+(p.y-coord.y+DD)*(p.y-coord.y+DD);
	if(dis <= DD)	//�������ӵ�
	{
		if(lead[0] != NULL) return -1;
		else return 1;
	}

	dis = (p.x-coord.x)*(p.x-coord.x)+(p.y-coord.y-DD)*(p.y-coord.y-DD);
	if(dis <= DD)	//�������ӵ�
	{
		if(lead[1] != NULL) return -1;
		else return 2;
	}

	dis = (p.x-coord.x+DD)*(p.x-coord.x+DD)+(p.y-coord.y)*(p.y-coord.y);
	if(dis <= DD)	//�������ӵ�
	{
		if(lead[2] != NULL) return -1;
		else return 3;
	}

	dis = (p.x-coord.x-DD)*(p.x-coord.x-DD)+(p.y-coord.y)*(p.y-coord.y);
	if(dis <= DD)	//�������ӵ�
	{
		if(lead[3] != NULL) return -1;
		else return 4;
	}

	if(disBetweenCenter <= DD * DD) return -1;//�ڵ���

	return 0;
}

CRUN * CRUN::Clone(CLONE_PURPOSE cp)const
//�����ؼ������Ϣ���µĽ��
{
	CRUN * newCrun = new CRUN(num, coord);
	newCrun->isPaintName = newCrun->isPaintName;
	strcpy(newCrun->name, name);

	if(CLONE_FOR_USE != cp)
	{
		newCrun->initNum = this->initNum;
		--s_initNum;
	}
	return newCrun;
}

void CRUN::GetDataList(LISTDATA * list)const
//��CProperty����
{
	list->Init(2);
	list->SetAMember(DATA_STYLE_LPCTSTR, TITLE_NOTE, (void *)name);
	list->SetAMember(DATA_STYLE_bool, TITLESHOW_NOTE, (void *)(&isPaintName));
}

int CRUN::GetDirect(const LEAD * l)const
//Ѱ�ҵ������ĸ�����
{
	for(int i=0; i<4; ++i) if(lead[i] == l) return i;
	return -1;	//û���ҵ�
}

int CRUN::GetConnectNum()const
//��������˼�������
{
	return  (lead[0] != NULL) + 
			(lead[1] != NULL) + 
			(lead[2] != NULL) + 
			(lead[3] != NULL);
}
