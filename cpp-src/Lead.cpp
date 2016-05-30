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
#include "Crun.h"			//�����
#include "Ctrl.h"			//��ѧԪ����
#include "Pointer.h"		//����ָ����
#include "DataList.h"		//LISTDATA, ENUM_STYLE��
#include "Lead.h"			//��ǰ��


unsigned long LEAD::s_initNum = 1;


void LEAD::Uninit()
//�ͷŵ���ռ�õĿռ�
{
	LEADSTEP * now = coord.next;
	LEADSTEP * next;
	while(now != NULL)
	{
		next = now->next;
		delete now;
		now = next;
	}
	coord.next = NULL;
}

LEAD::LEAD(int memNum, const Pointer &p1, const Pointer &p2, bool isInit, COLOR color)
{
	num = memNum;			//��ַ���
	initNum = s_initNum++;	//��ʼ������
	this->color = color;	//��ɫ
	elecDir  = UNKNOWNELEC;	//��������
	coord.next = NULL;		//��������
	if(isInit)
	{
		conBody[0] = p1;	//��������
		conBody[1] = p2;	//��������
		RefreshPos();		//������������
	}
}

LEAD::~LEAD()
{
	Uninit();
}

LEAD * LEAD::Clone(CLONE_PURPOSE cp)
//����
{
	LEAD * newLead = new LEAD(num, conBody[0], conBody[1], false, color);

	//��������
	LEADSTEP * p1 = &(this->coord);
	LEADSTEP * p2 = &(newLead->coord);
	while(p1->next != NULL)
	{
		p2->pos = p1->pos;
		p2->next = new LEADSTEP;

		p1 = p1->next;
		p2 = p2->next;
	}
	p2->pos = p1->pos;
	p2->next = NULL;

	if(CLONE_FOR_USE != cp)
	{
		newLead->initNum = this->initNum;
		--s_initNum;
	}
	return newLead;
}

unsigned long LEAD::GetInitOrder()const
//��ó�ʼ�����
{
	return initNum;
}

void LEAD::ResetInitNum()
//���ó�ʼ������
{
	LEAD::s_initNum = 1;
}

int LEAD::GetBodyPos()const 
//��õ�������������������λ��
//�ֽ����һλ:
//			0 ������� �� �յ����� ����
//			1 ������� �� �յ����� ����
//�ֽڵ����ڶ�λ:
//			0 ������� �� �յ����� ����
//			1 ������� �� �յ����� ����
{
	const Pointer * a = conBody;
	const Pointer * b = conBody + 1;
	POINT posa, posb;

	//�������
	ASSERT(a->IsOnBody(false));
	if(a->IsOnCrun())
		posa = a->p2->coord;
	else //if(a->IsOnCtrl())
		posa = a->p3->coord;

	ASSERT(b->IsOnBody(false));
	if(b->IsOnCrun())
		posb = b->p2->coord;
	else //if(b->IsOnCtrl())
		posb = b->p3->coord;

	return ((posa.x > posb.x) << 1) + (posa.y > posb.y);
}

void LEAD::GetDataList(const char * name, LISTDATA * list)
//��CProperty������Ϣ
{
	list->Init(1);
	list->SetAEnumMember(name, &color, ENUM_COLOR);
}

void LEAD::EasyInitPos(POINT from, POINT to)
//�ֲڵĳ�ʼ����������
{
	LEADSTEP * now;

	//�ͷ�����ռ�ÿռ�
	Uninit();

	//��ʼ����
	now = &coord;
	now->pos = from;
	now->next = new LEADSTEP;
	now = now->next;

	//�м������
	if(from.x != to.x && from.y != to.y)
	{
		now->pos.x = from.x;
		now->pos.y = to.y;
		now->next = new LEADSTEP;
		now = now->next;
	}

	//�յ�����
	now->pos = to;
	now->next = NULL;
}

bool LEAD::Divide(int atstate, POINT pos, LEADSTEP &a, LEADSTEP &b)
//��������һ��Ϊ��
{
	LEADSTEP * now;
	LEADSTEP * newPoint;

	ASSERT(atstate <= -2);
	atstate = (-atstate - 2) >> 1;

	//�ҵ�ָ��
	a = coord;
	now = &a;	//���ܸ�Ϊnow = &coord;
	while(now!=NULL && atstate!=0)
	{
		now = now->next;
		-- atstate;
	}
	if(now == NULL || now->next == NULL) return false;

	b.next = now->next;
	b.pos = pos;
	if(now->pos.x == now->next->pos.x)b.pos.x = now->pos.x;	//��������
	else b.pos.y = now->pos.y;	//�ں�����

	//�ֿ�����
	newPoint = new LEADSTEP;
	newPoint->pos = b.pos;
	newPoint->next = NULL;
	now->next = newPoint;

	//���ٵ�����������	
	coord.next = NULL;

	return true;
}

void LEAD::ReplacePos(LEADSTEP newPos)
//�滻ԭ��������
{
	Uninit();
	coord = newPos;
}

int LEAD::At(POINT p)const 
//�������ڵ��ߵ�λ�� : -3,-5,-7,...���߲���; -2,-4,-6,...���߲���; 0����.
{
	const LEADSTEP * pre = &coord, * now = pre->next;
	long min, max;
	int i = -2;

	while(now != NULL)
	{
		if(pre->pos.x == now->pos.x)	//��������
		{
			if(now->pos.y > pre->pos.y)
			{
				min = pre->pos.y; 
				max = now->pos.y;
			}
			else 
			{
				min = now->pos.y; 
				max = pre->pos.y;
			}
			if( p.y > min 
				&& p.y < max 
				&& p.x > now->pos.x - DD 
				&& p.x < now->pos.x + DD)
				return i;
		}
		else	//�ں�����
		{
			if(now->pos.x > pre->pos.x)
			{
				min = pre->pos.x; 
				max = now->pos.x;
			}
			else 
			{
				min = now->pos.x; 
				max = pre->pos.x;
			}
			if( p.x > min 
				&& p.x < max 
				&& p.y > now->pos.y - DD
				&& p.y < now->pos.y + DD)
				return i-1;
		}

		pre = now;
		now = now->next;
		i -= 2;
	}

	return 0;
}

void LEAD::CleanLead()
//ɾ������ͬ����ĵ��߽��
{
	LEADSTEP * p1 = &coord;
	LEADSTEP * p2 = p1->next;
	LEADSTEP * p3 = p2->next;

	if(p3 == NULL) return;	//ֻ��2���ڵ�ĵ��߲�����
	if(p1->pos.x == p2->pos.x && p1->pos.y == p2->pos.y)
	{
		delete p2;
		p1->next = p3;
		return;
	}
	while(p3 != NULL)
	{
		if(p2->pos.x == p3->pos.x && p2->pos.y == p3->pos.y)
		{
			if(p3->next != NULL)
			{
				p1->next = p3->next;
				delete p2;
				delete p3;
			}
			else
			{
				p2->next = NULL;
				delete p3;
			}
			break;
		}

		p1 = p2;
		p2 = p3;
		p3 = p3->next;
	}
}

int LEAD::GetPosFit(int pos1, int pos2, int dis, bool isEnd)
//��2��ƽ�е���֮��������ҵ����ʵ���һ��ƽ�е��ߵ�λ��
{
	int dis2 = -2;
	if(isEnd) dis2 = 2;

	if(pos2 - pos1 > dis || pos1 - pos2 > dis)
	{
		return (pos2 + pos1)/2 + dis2;
	}
	else if(pos1 < 300)
	{
		if(pos2 >= pos1)
			return pos2 + dis + dis2;
		else
			return pos1 + dis + dis2;
	}
	else
	{
		if(pos2 <= pos1)
			return pos2 - dis - dis2;
		else
			return pos1 - dis - dis2;
	}
}

void LEAD::FitStart(int dis)
//ʹ���߲��ڵ����ӵĵ�1������
{
	ASSERT(coord.next != NULL);
	//��ʼ������ -------------------------------------------------------
	LEADSTEP * next = coord.next;
	LEADSTEP * next2 = next->next;
	LEADSTEP * temp, * now;
	const int dir = conBody[0].GetConnectPosDir();
	const int dirSum = dir + conBody[1].GetConnectPosDir();
	const bool oppositeFlag = (dirSum == 3 || dirSum == 7);
	int dis2 = 15;
	if(dir & 1) dis2 = -15;

	//�ж�ִ������ -----------------------------------------------------
	switch(dir)
	{
	case 1:	//�����ӵ�
		if(coord.pos.x != next->pos.x || coord.pos.y >= next->pos.y)
			return;
		break;

	case 2:	//�����ӵ�
		if(coord.pos.x != next->pos.x || coord.pos.y <= next->pos.y)
			return;
		break;

	case 3:	//�����ӵ�
		if(coord.pos.y != next->pos.y || coord.pos.x >= next->pos.x)
			return;
		break;

	case 4:	//�����ӵ�
		if(coord.pos.y != next->pos.y || coord.pos.x <= next->pos.x)
			return;
		break;
	}

	//����ֻ��2���ڵ� ---------------------------------------------------
	if(next2 == NULL)
	{
		switch(dir)
		{
		case 1:	//�����ӵ�
		case 2:	//�����ӵ�
			coord.next = now = new LEADSTEP;
			now->pos.x = coord.pos.x;
			now->pos.y = coord.pos.y + dis2;

			now->next = temp = new LEADSTEP;
			temp->pos.y = now->pos.y;
			temp->pos.x = now->pos.x - dis;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.x = now->pos.x;
			temp->pos.y = now->pos.y - dis2*2 + next->pos.y - coord.pos.y;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.y = now->pos.y;
			temp->pos.x = now->pos.x + dis;

			temp->next = next;
			return;

		case 3:	//�����ӵ�
		case 4:	//�����ӵ�
			coord.next = now = new LEADSTEP;
			now->pos.y = coord.pos.y;
			now->pos.x = coord.pos.x + dis2;
			
			now->next = temp = new LEADSTEP;
			temp->pos.x = now->pos.x;
			temp->pos.y = now->pos.y - dis;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.y = now->pos.y;
			temp->pos.x = now->pos.x - dis2*2 + next->pos.x - coord.pos.x;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.x = now->pos.x;
			temp->pos.y = now->pos.y + dis;

			temp->next = next;
			return;
		}
	}
	
	//����ֻ��3���ڵ� ---------------------------------------------------
	else if(next2->next == NULL)
	{
		if(oppositeFlag)
		{
			switch(dir)
			{
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				//add point1
				coord.next = now = new LEADSTEP;
				now->pos.x = coord.pos.x;
				now->pos.y = coord.pos.y + dis2;

				//add point2
				now->next = temp = new LEADSTEP;
				temp->pos.y = now->pos.y;
				temp->pos.x = GetPosFit(coord.pos.x, next2->pos.x, dis, false);
				now = temp;

				//add point3
				now->next = temp = new LEADSTEP;
				temp->pos.x = now->pos.x;
				temp->pos.y = next2->pos.y - dis2;
				now = temp;

				//update point next
				now->next = next;

				next->pos.x = next2->pos.x;
				next->pos.y = now->pos.y;
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				//add point1
				coord.next = now = new LEADSTEP;
				now->pos.y = coord.pos.y;
				now->pos.x = coord.pos.x + dis2;

				//add point2
				now->next = temp = new LEADSTEP;
				temp->pos.x = now->pos.x;
				temp->pos.y = GetPosFit(coord.pos.y, next2->pos.y, dis, false);
				now = temp;

				//add point3
				now->next = temp = new LEADSTEP;
				temp->pos.y = now->pos.y;
				temp->pos.x = next2->pos.x - dis2;
				now = temp;

				//update point next
				now->next = next;

				next->pos.y = next2->pos.y;
				next->pos.x = now->pos.x;
				return;
			}
		}
		else
		{
			switch(dir)
			{
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				coord.next = now = new LEADSTEP;
				now->pos.x = coord.pos.x;
				now->pos.y = coord.pos.y + dis2;

				now->next = next;

				next->pos.x = next2->pos.x;
				next->pos.y = now->pos.y;
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				coord.next = now = new LEADSTEP;
				now->pos.y = coord.pos.y;
				now->pos.x = coord.pos.x + dis2;

				now->next = next;

				next->pos.y = next2->pos.y;
				next->pos.x = now->pos.x;
				return;
			}
		}
	}
	
	//����ֻ��4���ڵ������ӵ���� ---------------------------------------
	else if(oppositeFlag && next2->next->next == NULL)
	{
		switch(dir)
		{
		case 1:	//�����ӵ�
		case 2:	//�����ӵ�
			//new point
			coord.next = temp = new LEADSTEP;
			temp->pos.y = coord.pos.y;
			temp->pos.x = GetPosFit(coord.pos.x, next2->pos.x, dis, false);
			temp->next = next;
			
			//next point
			next->pos.x = temp->pos.x;
			return;

		case 3:	//�����ӵ�
		case 4:	//�����ӵ�
			//new point
			coord.next = temp = new LEADSTEP;
			temp->pos.x = coord.pos.x;
			temp->pos.y = GetPosFit(coord.pos.y, next2->pos.y, dis, false);
			temp->next = next;
			
			//next point
			next->pos.y = temp->pos.y;
			return;
		}
	}
	
	//����������5���ڵ�����ӵ㲻��� -----------------------------------
	else
	{
		switch(dir)
		{
		case 1:	//�����ӵ�
		case 2:	//�����ӵ�
			next2->pos.y = next->pos.y = coord.pos.y + dis2;
			return;

		case 3:	//�����ӵ�
		case 4:	//�����ӵ�
			next2->pos.x = next->pos.x = coord.pos.x + dis2;
			return;
		}
	}
}

void LEAD::FitEnd(int dis)
//ʹ���߲��ڵ����ӵĵ�2������
{
	ASSERT(coord.next != NULL);
	//��ʼ������ -------------------------------------------------------
	LEADSTEP * temp, * now, * pre1, * pre2, * next, * next2;
	const int dir = conBody[1].GetConnectPosDir();
	const int dirOther = conBody[0].GetConnectPosDir();
	int dis2 = 15;
	if(dir & 1) dis2 = -15;

	pre2 = NULL;
	pre1 = &coord;
	now  = coord.next;
	while(now->next != NULL)
	{
		pre2 = pre1;
		pre1 = now;
		now = now->next;
	}
	next = coord.next;
	next2 = next->next;

	//�ж�ִ������ -----------------------------------------------------
	switch(dir)
	{
	case 1:	//�����ӵ�
		if(now->pos.x != pre1->pos.x || now->pos.y >= pre1->pos.y)
			return;
		break;

	case 2:	//�����ӵ�
		if(now->pos.x != pre1->pos.x || now->pos.y <= pre1->pos.y)
			return;
		break;

	case 3:	//�����ӵ�
		if(now->pos.y != pre1->pos.y || now->pos.x >= pre1->pos.x)
			return;
		break;

	case 4:	//�����ӵ�
		if(now->pos.y != pre1->pos.y || now->pos.x <= pre1->pos.x)
			return;
		break;
	}

	//����ֻ��2���ڵ� ---------------------------------------------------
	if(next2 == NULL)
	{
		switch(dir)
		{
		case 1:	//�����ӵ�
		case 2:	//�����ӵ�
			coord.next = temp = new LEADSTEP;
			temp->pos.y = coord.pos.y;
			if(dirOther == 4)
				temp->pos.x = coord.pos.x - dis;
			else
				temp->pos.x = coord.pos.x + dis;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.x = now->pos.x;
			temp->pos.y = next->pos.y + dis2;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.y = now->pos.y;
			temp->pos.x = next->pos.x;

			temp->next = next;
			return;

		case 3:	//�����ӵ�
		case 4:	//�����ӵ�
			coord.next = temp = new LEADSTEP;
			temp->pos.x = coord.pos.x;
			if(dirOther == 2)
				temp->pos.y = coord.pos.y - dis;
			else
				temp->pos.y = coord.pos.y + dis;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.y = now->pos.y;
			temp->pos.x = next->pos.x + dis2;
			now = temp;

			now->next = temp = new LEADSTEP;
			temp->pos.x = now->pos.x;
			temp->pos.y = next->pos.y;

			temp->next = next;
			return;
		}
	}

	//����ֻ��3���ڵ� ---------------------------------------------------
	else if(next2->next == NULL)
	{
		switch(dir)
		{
		case 1:	//�����ӵ�
		case 2:	//�����ӵ�
			//next->pos.x
			if(dirOther == 4)
				next->pos.x = next2->pos.x + dis;
			else if(dirOther == 3)
				next->pos.x = next2->pos.x - dis;
			else
				next->pos.x = GetPosFit(coord.pos.x, next2->pos.x, dis, true);

			//add point
			next->next = temp = new LEADSTEP;
			temp->pos.x = next->pos.x;
			temp->pos.y = next2->pos.y;
			temp->next = next2;
			return;

		case 3:	//�����ӵ�
		case 4:	//�����ӵ�
			//next->pos.y
			if(dirOther == 2)
				next->pos.y = next2->pos.y + dis;
			else if(dirOther == 1)
				next->pos.y = next2->pos.y - dis;
			else
				next->pos.y = GetPosFit(coord.pos.y, next2->pos.y, dis, true);

			//add point
			next->next = temp = new LEADSTEP;
			temp->pos.y = next->pos.y;
			temp->pos.x = next2->pos.x;
			temp->next = next2;
			return;
		}
	}
	
	//����ֻ��4���ڵ� ----------------------------------------------
	else if(next2->next->next == NULL)
	{
		switch(dir)
		{
		case 1:	//�����ӵ�
		case 2:	//�����ӵ�
			if(dirOther == 3 || dirOther == 4)
			{
				pre1->pos.y = pre2->pos.y = now->pos.y + dis2;
			}
			else //dir != dirOther
			{
				next2->next = temp = new LEADSTEP;
				temp->next = now;

				temp->pos.y = now->pos.y;
				next2->pos.x = temp->pos.x = GetPosFit(coord.pos.x, next2->pos.x, dis, true);
			}
			return;

		case 3:	//�����ӵ�
		case 4:	//�����ӵ�
			if(dirOther == 1 || dirOther == 2)
			{
				pre1->pos.x = pre2->pos.x = now->pos.x + dis2;
			}
			else //dir != dirOther
			{
				next2->next = temp = new LEADSTEP;
				temp->next = now;

				temp->pos.x = now->pos.x;
				next2->pos.y = temp->pos.y = GetPosFit(coord.pos.y, next2->pos.y, dis, true);
			}
			return;
		}
	}

	//����������5���ڵ� --------------------------------------------
	else
	{
		switch(dir)
		{
		case 1:	//�����ӵ�
		case 2:	//�����ӵ�
			pre1->pos.y = pre2->pos.y = now->pos.y + dis2;
			return;

		case 3:	//�����ӵ�
		case 4:	//�����ӵ�
			pre1->pos.x = pre2->pos.x = now->pos.x + dis2;
			return;
		}
	}
}

void LEAD::MakeFit()
//���µĵ���λ�ø������ӵ�����ʱ,��������
{
	ASSERT(coord.next != NULL);

	if(conBody[0].IsOnCrun())
	{
		FitStart(DD*3);
	}
	else //if(conBody[0].IsOnCtrl())
	{
		FitStart(BODYSIZE.cx);
	}

	if(conBody[1].IsOnCrun())
	{
		FitEnd(DD*3);
	}
	else //if(conBody[1].IsOnCtrl())
	{
		FitEnd(BODYSIZE.cx);
	}
}

bool LEAD::Move(int dir, POINT pos, const int dis)
//�ƶ�����
{
	LEADSTEP * pre2 = NULL;
	LEADSTEP * pre  = &coord;
	LEADSTEP * now  = pre->next;
	LEADSTEP * next = NULL;
	int i = -2;
	int inter = 0;

	//1,�ҵ�ָ��----------------------------------
	while(now != NULL)
	{
		if(pre->pos.x == now->pos.x)	//��������
		{
			if(i == dir) break;
		}
		else	//�ں�����
		{
			if(i-1 == dir) break;
		}
		pre2 = pre;
		pre = now;
		now = now->next;
		i -= 2;
	}
	if(now == NULL) return false;	//û���ҵ�
	else next = now->next;

	//2����������������--------------------------
	if(pre->pos.x == now->pos.x)	//��������
	{
		if(pos.x == pre->pos.x) return false;	//����ı�

		//2.1����pre��ͷ.........................
		if(pre2 == NULL)	//pre��ͷ
		{
			if(next != NULL)	//now���ǽ�β
			{
				inter = pos.x - next->pos.x;
				if(inter < 0)inter = -inter;
			}

			if(next != NULL && inter <= dis)	//now���ǽ�β
			{
				if(next->next != NULL)	//next���ǽ�β
				{
					delete now;
					pre->next = next;
					next->pos.y = pre->pos.y;
				}
				else	//next�ǽ�β
				{
					now->pos.x = next->pos.x;
					now->pos.y = pre->pos.y;
				}
			}
			else if(next != NULL)	//now���ǽ�β
			{
				pre2  = new LEADSTEP;
				pre2->pos.x = pos.x;
				pre2->pos.y = pre->pos.y;
				pre2->next = now;

				pre->next = pre2;

				now->pos.x = pos.x;
			}
			else	//now�ǽ�β
			{
				pre2 = new LEADSTEP;
				pre2->pos.x = pos.x;
				pre2->pos.y = pre->pos.y;

				next = new LEADSTEP;
				next->pos.x = pos.x;
				next->pos.y = now->pos.y;

				pre->next = pre2;
				pre2->next = next;
				next->next = now;
			}

			goto end;
		}//����pre����ͷ

		//2.2����now�ǽ�β.........................
		if(next == NULL)	//now�ǽ�β
		{
			inter = pos.x - pre2->pos.x;
			if(inter < 0)inter = -inter;

			if(inter <= dis)
			{
				if(pre2 != &coord)	//pre2����ͷ
				{
					delete pre;
					pre2->next = now;
					pre2->pos.y = now->pos.y;
				}
				else	//pre2��ͷ
				{
					pre->pos.x = pre2->pos.x;
					pre->pos.y = now->pos.y;
				}
			}
			else
			{
				next  = new LEADSTEP;
				next->pos.x = pos.x;
				next->pos.y = now->pos.y;

				pre->pos.x = pos.x;

				pre->next = next;
				next->next = now;
			}

			goto end;
		}//����now���ǽ�β

		//2.3������ǰ��ϲ�..........................
		inter = pos.x - pre2->pos.x;
		if(inter < 0)inter = -inter;

		if(inter <= dis)	//���ߺϲ�
		{
			if(pre2 != &coord)	//pre2����ͷ
			{
				delete pre;
				delete now;
				pre2->next = next;
				pre2->pos.y = next->pos.y;
			}
			else	//pre2��ͷ
			{
				delete pre;
				pre2->next = now;
				now->pos.x = pre2->pos.x;

				if(now->pos.x == next->pos.x && now->pos.y == next->pos.y)
				{
					delete now;
					pre2->next = next;
				}
			}
			
			goto end;
		}

		//2.4���������ϲ�..........................
		inter = pos.x - next->pos.x;
		if(inter < 0)inter = -inter;

		if(inter <= dis)	//���ߺϲ�
		{
			if(next->next != NULL)	//next���ǽ�β
			{
				delete pre;
				delete now;
				pre2->next = next;
				next->pos.y = pre2->pos.y;
			}
			else	//next�ǽ�β
			{
				delete now;
				pre->next = next;
				pre->pos.x = next->pos.x;
			}
			goto end;
		}

		//2.5�����������..........................
		now->pos.x = pos.x;
		pre->pos.x = pos.x;
		goto end;

	}//����������������

	//3�������ú�������--------------------------
	if(pre->pos.y == now->pos.y)	//�ں�����
	{
		if(pos.y == pre->pos.y) return false;	//����ı�

		//3.1����pre��ͷ.........................
		if(pre2 == NULL)	//pre��ͷ
		{
			if(next != NULL)	//now���ǽ�β
			{
				inter = pos.y - next->pos.y;
				if(inter < 0)inter = -inter;
			}

			if(next != NULL && inter <= dis)	//now���ǽ�β
			{
				if(next->next != NULL)	//next���ǽ�β
				{
					delete now;
					pre->next = next;
					next->pos.x = pre->pos.x;
				}
				else	//next�ǽ�β
				{
					now->pos.y = next->pos.y;
					now->pos.x = pre->pos.x;
				}
			}
			else if(next != NULL)	//now���ǽ�β
			{
				pre2  = new LEADSTEP;
				pre2->pos.y = pos.y;
				pre2->pos.x = pre->pos.x;
				pre2->next = now;

				pre->next = pre2;

				now->pos.y = pos.y;
			}
			else	//now�ǽ�β
			{
				pre2 = new LEADSTEP;
				pre2->pos.y = pos.y;
				pre2->pos.x = pre->pos.x;

				next = new LEADSTEP;
				next->pos.y = pos.y;
				next->pos.x = now->pos.x;

				pre->next = pre2;
				pre2->next = next;
				next->next = now;
			}

			goto end;
		}//����pre����ͷ

		//3.2����now�ǽ�β.........................
		if(next == NULL)	//now�ǽ�β
		{
			inter = pos.y - pre2->pos.y;
			if(inter < 0)inter = -inter;

			if(inter <= dis)
			{
				if(pre2 != &coord)	//pre2����ͷ
				{
					delete pre;
					pre2->next = now;
					pre2->pos.x = now->pos.x;
				}
				else	//pre2��ͷ
				{
					pre->pos.y = pre2->pos.y;
					pre->pos.x = now->pos.x;
				}
			}
			else
			{
				next  = new LEADSTEP;
				next->pos.y = pos.y;
				next->pos.x = now->pos.x;

				pre->pos.y = pos.y;

				pre->next = next;
				next->next = now;
			}

			goto end;
		}//����now���ǽ�β

		//3.3������ǰ��ϲ�..........................
		inter = pos.y - pre2->pos.y;
		if(inter < 0)inter = -inter;

		if(inter <= dis)	//���ߺϲ�
		{
			if(pre2 != &coord)	//pre2����ͷ
			{
				delete pre;
				delete now;
				pre2->next = next;
				pre2->pos.x = next->pos.x;
			}
			else	//pre2��ͷ
			{
				delete pre;
				pre2->next = now;
				now->pos.y = pre2->pos.y;

				if(now->pos.x == next->pos.x && now->pos.y == next->pos.y)
				{
					delete now;
					pre2->next = next;
				}
			}

			goto end;
		}

		//3.4���������ϲ�..........................
		inter = pos.y - next->pos.y;
		if(inter < 0)inter = -inter;

		if(inter <= dis)	//���ߺϲ�
		{
			if(next->next != NULL)	//next���ǽ�β
			{
				delete pre;
				delete now;
				pre2->next = next;
				next->pos.x = pre2->pos.x;
			}
			else	//next�ǽ�β
			{
				delete now;
				pre->next = next;
				pre->pos.y = next->pos.y;
			}
			goto end;
		}

		//3.5�����������..........................
		now->pos.y = pos.y;
		pre->pos.y = pos.y;
		goto end;

	}	//�������ú�������

end:

	CleanLead();	//ɾ������ͬ����ĵ��߽��
	//MakeFit();		//��������

	return true;
}

void LEAD::RefreshPos()
//������������ı�,���µ���λ��
{
	POINT from, to;
	LEADSTEP * now;
	LEADSTEP * p1, * p2;

	//���»�������˵�����
	conBody[0].GetPosFromBody(from);
	conBody[1].GetPosFromBody(to);

	//��ʼ��
	if(!coord.next || !coord.next->next)
	{
		EasyInitPos(from, to);
		MakeFit();	//��������
		return;
	}

	now = &coord;
	
	//�������ı�
	if(from.x != now->pos.x || from.y != now->pos.y)
	{
		p1 = now->next;
		p2 = p1->next;
		
		if(p1->pos.x != now->pos.x || p1->pos.y != now->pos.y)
		{//ǰ2�����겻ͬ
			if(p1->pos.x == now->pos.x)
				p1->pos.x = from.x;
			else
				p1->pos.y = from.y;
			now->pos = from;
		}
		else if(p1->pos.x != p2->pos.x || p1->pos.y != p2->pos.y)
		{//��2,3�����겻ͬ
			if(p1->pos.x == p2->pos.x)
				p1->pos.y = from.y;
			else
				p1->pos.x = from.x;
			now->pos = from;
		}
		else
		{
			EasyInitPos(from, to);	//��ʼ��
			MakeFit();	//��������
			return;
		}
	}
	
	//�õ��յ�����
	p1 = p2 = NULL;
	while (now->next != NULL)
	{
		p2 = p1;
		p1 = now;
		now = now->next;
	}
	
	//�յ�����ı�
	if(to.x != now->pos.x || to.y != now->pos.y)
	{
		if(p1->pos.x != now->pos.x || p1->pos.y != now->pos.y)
		{//��2�����겻ͬ
			if(p1->pos.x == now->pos.x)
				p1->pos.x = to.x;
			else
				p1->pos.y = to.y;
			now->pos = to;
		}
		else if(p1->pos.x != p2->pos.x || p1->pos.y != p2->pos.y)
		{//����2,3�����겻ͬ
			if(p1->pos.x == p2->pos.x)
				p1->pos.y = to.y;
			else
				p1->pos.x = to.x;
			now->pos = to;
		}
		else
		{
			EasyInitPos(from, to);	//��ʼ��
			MakeFit();	//��������
			return;
		}
	}

	CleanLead();	//ȥ����ͬ����ĵ��߽ڵ�
	MakeFit();		//��������
}

void LEAD::SaveToFile(FILE * fp)const 
//���浽�ļ�
{
	const LEADSTEP * temp = &coord;
	ASSERT(fp != NULL);

	while(temp != NULL)
	{
		fwrite(temp, sizeof(LEADSTEP), 1, fp);
		temp = temp->next;
	}

	fwrite(&color, sizeof(enum), 1, fp);

	conBody[0].SaveToFile(fp);
	conBody[1].SaveToFile(fp);
}

void LEAD::ReadFromFile(FILE * fp, LEAD ** allLead, CRUN ** allCrun, CTRL ** allCtrl)
//���ļ���ȡ
{
	LEADSTEP * temp = &coord;
	ASSERT(fp!=NULL && allLead!=NULL && allCrun!=NULL && allCtrl!=NULL);

	while(temp != NULL)
	{
		fread(temp, sizeof(LEADSTEP), 1, fp);
		if(temp->next)temp->next = new LEADSTEP;
		temp = temp->next;
	}

	fread(&color, sizeof(enum), 1, fp);

	conBody[0].ReadFromFile(fp, allLead, allCrun, allCtrl);
	conBody[1].ReadFromFile(fp, allLead, allCrun, allCtrl);
}

void LEAD::PaintLead(CDC * dc)const 
//������
{
	ASSERT(dc != NULL);

	const LEADSTEP * temp = &coord;
	dc->MoveTo(temp->pos);
	temp = temp->next;
	while(temp != NULL)
	{
		dc->LineTo(temp->pos);
		temp = temp->next;
	}
}

void LEAD::GetStartEndPos(POINT &pos1, POINT &pos2)const
//��õ��߿�ʼλ�úͽ�β����
{
	const LEADSTEP * temp = &coord;
	while(temp->next != NULL) temp = temp->next;
	pos1 = coord.pos;
	pos2 = temp->pos;
}

void LEAD::SaveToTextFile(FILE * fp)const 
//��������ʽ����,���Ժ���
{
	ASSERT(fp != NULL);

	const LEADSTEP * temp;

	//fprintf(fp, "���� Init Order = %d :\n", GetInitOrder());

	fprintf(fp, "x:[");
	temp = &coord;
	while(temp)
	{
		fprintf(fp, "%d", temp->pos.x);
		temp = temp->next;
		if (temp) fputc(',', fp);
	}
	fprintf(fp, "],\n");

	fprintf(fp, "y:[");
	temp = &coord;
	while(temp)
	{
		fprintf(fp, "%d", temp->pos.y);
		temp = temp->next;
		if (temp) fputc(',', fp);
	}
	fprintf(fp, "],\n");
}
