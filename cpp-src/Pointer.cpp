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
#include "Ctrl.h"		//��ѧԪ����
#include "Crun.h"		//�����
#include "Lead.h"		//������
#include "Pointer.h"	//��ǰ��


void Pointer::SetOnCtrl(CTRL * ctrl, bool isSetAt)
//ָ��ؼ�
{
	p3 = ctrl;
	style = ctrl->GetStyle();	//����ؼ������ʼ�����
	if(isSetAt) atState = -1;
}

void Pointer::GetPosFromBody(POINT & pos)const
//����������ӵ�λ�û�õ��߶˵�����
{
	int leadNum = GetLeadNum();

	if(IsOnCrun())	//���ӽ��
	{
		pos = p2->coord;
		switch(leadNum)
		{
		case 0:	//���϶�
			pos.y -= DD;
			break;
		case 1:	//���¶�
			pos.y += DD - 1;	//��ʾ���������(-1)
			break;
		case 2:	//�����
			pos.x -= DD;
			break;
		case 3:	//���Ҷ�
			pos.x += DD - 1;	//��ʾ���������(-1)
			break;
		}
	}
	else if(IsOnCtrl())	//���ӿؼ�
	{
		pos = p3->coord;
		if(! (p3->dir & 1))	//����
		{
			pos.y += (BODYSIZE.cy>>1);
			if((p3->dir!=0) ^ (leadNum!=0))	//���Ҷ�
			{
				pos.x += BODYSIZE.cx - 1;	//��ʾ���������(-1)
			}
		}
		else//����
		{
			pos.x += (BODYSIZE.cx>>1);
			if(((p3->dir-1)!=0) ^ (leadNum!=0))	//���¶�
			{
				pos.y += BODYSIZE.cy - 1;	//��ʾ���������(-1)
			}
		}
	}
}

void Pointer::SaveToFile(FILE * fp)const 
//���浽�ļ�
{
	int num;
	ASSERT(fp != NULL);

	if(IsOnLead())
		num = p1->num;
	else if(IsOnCrun())
		num = p2->num;
	else if(IsOnCtrl())
		num = p3->num;

	fwrite(&style, sizeof(int), 1, fp);
	fwrite(&num, sizeof(int), 1, fp);
	fwrite(&atState, sizeof(atState), 1, fp);
}

bool Pointer::ReadFromFile(
						   FILE * fp,
						   LEAD ** allLead,
						   CRUN ** allCrun,
						   CTRL ** allCtrl)
//���ļ���ȡ
{
	int num;
	ASSERT(fp != NULL);

	Clear();
	fread(&style, sizeof(int), 1, fp);
	fread(&num, sizeof(int), 1, fp);

	if(IsOnLead())
	{
		if(num >= 0 && num < MAXLEADNUM)
			SetOnLead(allLead[num]);
		else
			return false;
	}
	else if(IsOnCrun())
	{
		if(num >= 0 && num < MAXCRUNNUM)
			SetOnCrun(allCrun[num]);
		else
			return false;
	}
	else if(IsOnCtrl())
	{
		if(num >= 0 && num < MAXCTRLNUM)
			SetOnCtrl(allCtrl[num]);
		else
			return false;
	}
	else
		return false;

	fread(&atState, sizeof(atState), 1, fp);

	return true;
}

int Pointer::GetConnectPosDir()const
//������ӵ�λ��
{
	ASSERT(IsOnConnectPos());
	
	if(IsOnCrun())
	{
		return atState;
	}
	else //if(IsOnCtrl())
	{
		ASSERT(p3->dir >=0 && p3->dir <= 3);

		if(atState == 1)
		{
			switch(p3->dir)
			{
			case 0:
				return 3;
				break;
			case 1: 
				return 1;
				break;
			case 2:
				return 4;
				break;
			case 3: 
				return 2;
				break;
			}
		}
		else //if(atState == 2)
		{
			switch(p3->dir)
			{
			case 0:
				return 4;
				break;
			case 1: 
				return 2;
				break;
			case 2:
				return 3;
				break;
			case 3: 
				return 1;
				break;
			}
		}

		return -1;
	}
}

void Pointer::SaveToTextFile(FILE * fp)
//���Ժ���
{
	ASSERT(fp != NULL);

	fprintf(fp, "{atState:%d,style:%d,", atState, style);

	if (IsOnLead())
	{
		fprintf(fp, "index:%d}", p1->num);
	}
	if (IsOnCrun())
	{
		fprintf(fp, "index:%d}", p2->num);
	}
	else if(IsOnCtrl()) 
	{
		fprintf(fp, "index:%d}", p3->num);
	}
	else
		fprintf(fp, "index:0}");
}
