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
#include "resource.h"
#include "StaticClass.h"	//����static��������
#include "MyPropertyDlg.h"	//ʹ��Property�Ի���
#include "Equation.h"		//ʹ�ü���NԪһ�η��̵���
#include "CountStruct.h"	//���ڽ���Ľṹ��
#include "Lead.h"			//������
#include "Ctrl.h"			//��ѧԪ����
#include "Crun.h"			//�����
#include "DataList.h"		//LISTDATA,ENUM_STYLE
#include "KMP.h"			//KMP�㷨��
#include "Manager.h"		//��ǰ��


//1��ʼ����������------------------------------------------------------------��
Manager::Manager(CWnd * outWnd)
{
	int i;
	HINSTANCE hinst = AfxGetInstanceHandle();


	//������ʾ-------------------------------------------------------
	wndPointer = outWnd;		//��ǰ����ָ��
	dc = wndPointer->GetDC();	//��ǰ�����豸������

	bitmapForRefresh.CreateBitmap(1, 1, 1, 32, NULL);	//ʹˢ�²�����ʹ�õ�bitmap
	dcForRefresh.CreateCompatibleDC(dc);				//ʹˢ�²�����ʹ�õ�DC
	dcForRefresh.SelectObject(&bitmapForRefresh);


	//��Ա���-------------------------------------------------------
	viewOrig.x = viewOrig.y = 0;					//�ӽǳ�ʼ����
	mouseWheelSense.cx = mouseWheelSense.cy = 32;	//mouseWheel������
	moveBodySense = 3;								//���������Ҽ�����һ���ƶ��ľ���
	maxLeaveOutDis = 7;								//���ߺϲ�������


	//��·Ԫ������---------------------------------------------------
	ZeroMemory(crun, sizeof(void *) * MAXCRUNNUM);
	ZeroMemory(ctrl, sizeof(void *) * MAXCTRLNUM);
	ZeroMemory(lead, sizeof(void *) * MAXLEADNUM);
	crunNum = leadNum = ctrlNum = 0;	//����ĸ�������


	//�������-------------------------------------------------------
	circu = NULL;		//��·
	circuNum = 0;		//��·����,С�ڵ��� crun*2
	crun2 = NULL;		//��crun��Ϣ��ȡ�������ڼ���,����ͬcrun
	maps = NULL;		//�������е���·
	groupNum = 0;		//����,ͬһ�����һ����ͨͼ��,���齨������
	equation = NULL;	//���̴������


	//�������Ϣ��¼-----------------------------------------------
	motiNum = 0;
	addState = BODY_NO;
	lButtonDownPos.x = -100;
	lButtonDownState = false;
	isUpRecvAfterDown = true;
	FocusBodyClear(NULL);


	//��ͼ����-------------------------------------------------------
	textColor = BLACK;						//Ĭ��������ɫ
	focusLeadStyle = SOLID_RESERVE_COLOR;	//Ĭ�Ͻ��㵼����ʽ
	focusCrunColor = GREEN;					//Ĭ�Ͻ�������ɫ
	focusCtrlColor = RED;					//Ĭ�Ͻ���ؼ���ɫ
	InitBitmap();							//��ʼ��λͼ

	//����
	for(i=COLOR_TYPE_NUM-1; i>=0; --i) 
		hp[i].CreatePen(PS_SOLID, 1, LEADCOLOR[i]);

	//���ͼ��
	hcSizeNS		= LoadCursor(NULL,	IDC_SIZENS);
	hcSizeWE		= LoadCursor(NULL,	IDC_SIZEWE);
	hcShowConnect	= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_SHOWCONNECT));
	hcHand			= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_HAND));
	hcMoveHorz		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_HORZ_LEAD));
	hcMoveVert		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_VERT_LEAD));
	hcAddCrun		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_ADDCRUN));


	//��ȡ�ļ�-------------------------------------------------------
	vectorPos = NULL;
	fileName[0] = '\0';
	PutCircuitToVector();	//����ǰ�յ�·��Ϣ���浽����
}

Manager::~Manager()
{
	DeleteVector(circuitVector.begin(), circuitVector.end());	//�����������ĵ�·��Ϣ

	ClearClipboard();	//��ռ��а�

	//�����ͼ����
	DeleteObject(hcSizeNS);
	DeleteObject(hcSizeWE);
	DeleteObject(hcShowConnect);
	DeleteObject(hcHand);
	DeleteObject(hcMoveHorz);
	DeleteObject(hcMoveVert);			//������ͼ��
	for(int i=COLOR_TYPE_NUM-1; i>=0; --i) hp[i].DeleteObject();	//�������
	UninitBitmap();						//�ͷ�λͼ
	wndPointer->ReleaseDC(dc);			//�ͷŻ�ͼDC
	bitmapForRefresh.DeleteObject();	//ʹˢ�²�����ʹ�õ�bitmap
	dcForRefresh.DeleteDC();			//ʹˢ�²�����ʹ�õ�DC
}

void Manager::InitBitmap()
//��ʼ��λͼ���
{
	int i, j, k, l;
	UINT * buf1, * buf2, * p;

	//�����λͼ------------------------------------
	showConnectDcMem.CreateCompatibleDC(dc);
	showConnectBitmap.LoadBitmap(IDB_SMALLCRUN);
	showConnectDcMem.SelectObject(&showConnectBitmap);

	//�ڵ�λͼ--------------------------------------
	crunDcMem.CreateCompatibleDC(dc);
	crunBitmap.LoadBitmap(IDB_CRUN);
	crunDcMem.SelectObject(&crunBitmap);

	//�ؼ�λͼ,����õ���ת�ؼ�---------------------
	buf1 = (UINT *)malloc(BODYSIZE.cx * BODYSIZE.cy * 4);
	buf2 = (UINT *)malloc(BODYSIZE.cx * BODYSIZE.cy * 4);

	for(k=CTRL_BITMAP_TYPE_NUM-1; k>=0; --k)
	{
		//ԭλͼ
		ctrlDcMem[k].CreateCompatibleDC(dc);
		ctrlBitmap[k].LoadBitmap(IDB_SOURCE + k);
		ctrlDcMem[k].SelectObject(ctrlBitmap + k);
		ctrlBitmap[k].GetBitmapBits(BODYSIZE.cx*BODYSIZE.cy*4, buf1);	//���ԭλͼ����

		//�����תλͼ
		for(l=1; l<4; ++l)
		{
			p = buf1 + (BODYSIZE.cy - 1) * BODYSIZE.cx + BODYSIZE.cx - 1;
			for(i = BODYSIZE.cy - 1; i >= 0; --i) for(j = BODYSIZE.cx - 1; j >= 0; --j)
				* ( buf2 + j * BODYSIZE.cx + BODYSIZE.cx - 1 - i) = * p --;

			i = k + CTRL_BITMAP_TYPE_NUM*l;
			ctrlDcMem[i].CreateCompatibleDC(dc);
			ctrlBitmap[i].CreateBitmap(BODYSIZE.cx, BODYSIZE.cy, 1, 32, buf2);
			ctrlDcMem[i].SelectObject(ctrlBitmap + i);

			p = buf1;
			buf1 = buf2;
			buf2 = p;
		}
	}

	free(buf1);
	free(buf2);
}

void Manager::UninitBitmap()
//�ͷ�λͼռ�ÿռ�
{
	showConnectBitmap.DeleteObject();
	showConnectDcMem.DeleteDC();

	crunBitmap.DeleteObject();
	crunDcMem.DeleteDC();

	for(int i=CTRL_BITMAP_NUM-1; i>=0; --i)
	{
		DeleteObject(ctrlBitmap[i]);
		ctrlDcMem[i].DeleteDC();
	}
}


//3��ͼ����------------------------------------------------------------------------��
void Manager::PaintCtrl(CTRL * c, bool isPaintName)
//���ؼ�
{
	ASSERT(c != NULL);
	if(isPaintName) PaintCtrlText(c);	//���ؼ�����
	dc->BitBlt(c->coord.x, c->coord.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintHandle(c), 0, 0, SRCAND);
}

void Manager::PaintCtrlText(const CTRL * c)const 
//���ؼ�������
{
	ASSERT(c != NULL);
	if(!c->isPaintName) return;
	dc->TextOut(c->coord.x, c->coord.y-15, c->name, strlen(c->name));
}

void Manager::PaintCrun(const CRUN * c, bool isPaintName)
//�����
{
	ASSERT(c != NULL);
	if(isPaintName) PaintCrunText(c);	//���������
	dc->BitBlt(c->coord.x-DD, c->coord.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCAND);
}

void Manager::PaintCrunText(const CRUN * c)const 
//���������
{
	ASSERT(c != NULL);
	if(!c->isPaintName) return;
	dc->TextOut(c->coord.x, c->coord.y-20, c->name, strlen(c->name));
}

void Manager::PaintLead(LEAD * l)
//������
{
	ASSERT(l != NULL);
	dc->SelectObject(hp + l->color);
	l->PaintLead(dc);
}

void Manager::PaintAllLead()
//�����е���; Ϊ����߻����ߵ�Ч��,��ͬ��ɫһ��
{
	int num, color;
	for(color=COLOR_TYPE_NUM-1; color>=0; --color)	//����ɫѭ��
	{
		dc->SelectObject(hp + color);
		for(num=leadNum-1; num>=0; --num)
			if(color == lead[num]->color) lead[num]->PaintLead(dc);
	}
	dc->SelectObject(hp);	//�ָ�����ɫ�Ļ���
}

void Manager::PaintAll()
//�����е�����
{
	int i;
	CDC * save = dc;
	RECT rect;
	BITMAP bitmap;

	//1,�������״̬��Ϣ----------------------------------------------------
	motiNum = 0;
	addState = BODY_NO;
	lastMoveOnPos.x = -100;
	lastMoveOnBody.Clear();

	//2,��ͼ��ʼ��----------------------------------------------------------
	//��ô��ڳߴ�
	wndPointer->GetClientRect(&rect);

	//��ʼ��ˢ��λͼ��С
	bitmapForRefresh.GetBitmap(&bitmap);
	if(rect.bottom > bitmap.bmHeight || rect.right > bitmap.bmWidth)
	{
		bitmapForRefresh.DeleteObject();
		bitmapForRefresh.CreateBitmap(rect.right, rect.bottom, 1, 32, NULL);
		dcForRefresh.SelectObject(&bitmapForRefresh);
	}

	//��dcForRefresh��ͼ
	dc->DPtoLP(&rect);			//��ǰrect���豸����任Ϊ�߼�����
	dc = &dcForRefresh;			//dc��ʱ�滻ΪdcForRefresh,���ڴ滭ͼ

	//����������ɫ���ӽ����
	dc->SetTextColor(LEADCOLOR[textColor]);
	dc->SetViewportOrg(-viewOrig.x, -viewOrig.y);	//��ʼ���ӽ���ʼ����

	//3,�ڴ滭ͼ------------------------------------------------------------
	//�ð�ɫ���θ��������ͻ���
	dc->SelectStockObject(WHITE_PEN);
	dc->SelectStockObject(WHITE_BRUSH);
	dc->Rectangle(&rect);

	//���ؼ�����Լ����ǵ�����
	for(i=ctrlNum-1; i>=0; --i)
		PaintCtrl(ctrl[i], true);
	for(i=crunNum-1; i>=0; --i)
		PaintCrun(crun[i], true);

	//������
	PaintAllLead();

	//������
	FocusBodyPaint(NULL);

	//�ػ���ʾ���Ʋ������
	PaintWithSpecialColor(pressStart, false);
	PaintWithSpecialColor(pressEnd, true);

	//4,��ԭdc, һ���Ի�ͼ--------------------------------------------------
	dc = save;
	dc->BitBlt(0, 0, rect.right, rect.bottom, &dcForRefresh, 0, 0, SRCCOPY);
}

void Manager::PaintMouseMotivate(const Pointer &mouseMoti)
//����������ӵ㲿λ,�ı������״
{
	const Pointer * mouse = &mouseMoti;
	POINT tempPos;
	const int CR = 3; //���ӵ㻭ͼ�뾶

	if(mouse->IsOnLead())
	{
		if(motiNum && motiBody[motiNum-1].IsOnConnectPos())
		{//ѡ�������ӵ�,�������ӽ��ͼ��,��ʾʹ��ConnectBodyLead����
			SetCursor(hcShowConnect);
		}
		else
		{//û��ѡ�����ӵ�,�����"ָ��",��ʾ�ı䵼������
			if(mouse->IsOnHoriLead())SetCursor(hcSizeNS);	//�ں���,�����"����ָ��"
			else SetCursor(hcSizeWE);						//������,�����"����ָ��"
		}
	}
	else if(mouse->IsOnBody())	//��������,������ֵ���״,��ʾ�ƶ�����
	{
		SetCursor(hcHand);
	}

	if(!lastMoveOnBody.IsAllSame(mouse))	//lastMoveOnBody��mouseָ���Pointer�ṹ�岻һ��
	{
		if(lastMoveOnBody.IsOnConnectPos())	//��ԭ��һ�����ӵ�
		{
			lastMoveOnBody.GetPosFromBody(tempPos);	//�������
			dc->BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2,
				&showConnectDcMem, 0, 0, SRCINVERT);
		}

		lastMoveOnBody = *mouse;	//��¼��ǰ��꼤������

		if(mouse->IsOnConnectPos())	//����ǰ�����ӵ�
		{
			mouse->GetPosFromBody(tempPos);	//�������
			dc->BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2,
				&showConnectDcMem, 0, 0, SRCINVERT);
		}
	}
}

void Manager::PaintLeadWithStyle(LEAD * lead, int leadStyle, enum COLOR colorNum)
//��ָ����ɫ�����ߵ���
{
	ASSERT(lead != NULL);
	CPen tempPen;

	tempPen.CreatePen(leadStyle, 1, LEADCOLOR[colorNum]);	//�½����⻭��
	dc->SelectObject(tempPen.m_hObject);					//ѡ�񻭱�
	lead->PaintLead(dc);									//������
	tempPen.DeleteObject();									//�ͷŻ���
	dc->SelectObject(hp);									//�ָ�����
}

void Manager::PaintCrunWithColor(CRUN * c, enum COLOR colorNum)
//��ָ����ɫ��ָ�����
{
	ASSERT(c != NULL);
	CBrush hb;

	//1,��ָ����ɫ���� -------------------------------------------------------------
	//����ָ����ɫ��ˢ
	hb.CreateSolidBrush(LEADCOLOR[colorNum]);
	dc->SelectObject(&hb);
	//���ÿջ���
	dc->SelectStockObject(NULL_PEN);
	//��ָ����ɫԲ��
	dc->Rectangle(c->coord.x-DD, c->coord.y-DD, c->coord.x+DD+1, c->coord.y+DD+1);

	//2,�ͷŻ�ˢ,��ԭ��ˢ ----------------------------------------------------------
	hb.DeleteObject();
	dc->SelectStockObject(NULL_BRUSH);

	//3,����ɫ���,ʹ�� "��" ���߼���ͼ,�õ�ָ����ɫ��� ---------------------------
	dc->BitBlt(c->coord.x-DD, c->coord.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCPAINT);
}

void Manager::PaintCtrlWithColor(CTRL * c, enum COLOR colorNum)
//��ָ����ɫ��ָ���ؼ�
{
	ASSERT(c != NULL);
	CBrush hb;

	//1,��ָ����ɫ���� -------------------------------------------------------------
	//����ָ����ɫ��ˢ
	hb.CreateSolidBrush(LEADCOLOR[colorNum]);
	dc->SelectObject(&hb);
	//���ÿջ���
	dc->SelectStockObject(NULL_PEN);
	//��ָ����ɫ����
	dc->Rectangle(c->coord.x, c->coord.y, c->coord.x+BODYSIZE.cx+1, c->coord.y+BODYSIZE.cy+1);

	//2,�ͷŻ�ˢ,��ԭ��ˢ ----------------------------------------------------------
	hb.DeleteObject();
	dc->SelectStockObject(NULL_BRUSH);

	//3,����ɫ�ؼ�,ʹ�� "��" ���߼���ͼ,�õ�ָ����ɫ�ؼ�
	dc->BitBlt(c->coord.x, c->coord.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintHandle(c), 0, 0, SRCPAINT);

	//4,���»������ǵ���Χ���� -----------------------------------------------------
	for(int num=0; num<2; ++num) if(c->lead[num] != NULL)
		PaintLead(c->lead[num]);
}

void Manager::PaintWithSpecialColor(const Pointer &body, bool isPaintNum)
//�ñ�����ɫ(��ɫ)��ʾ
{
	const COLOR colorNum = (enum COLOR)COLOR_TYPE_NUM;	//ѡ�ñ�����ɫ(��ɫ)

	if(body.IsOnLead())
	{
		if(isPaintNum)
		{
			//������
			PaintLeadWithStyle(body.p1, PS_SOLID, colorNum);

			//�ڵ�����ʼ�ͽ�β���ֱ���ʾ����'1'��'2'
			char text[8] = "0";
			POINT pos[2];
			body.p1->GetStartEndPos(pos[0], pos[1]);
			for(int i=0; i<2; ++i)
			{
				++(text[0]);
				dc->TextOut(pos[i].x, pos[i].y, text, 1);
			}
		}
		else
		{
			PaintLeadWithStyle(body.p1, PS_DOT, colorNum);	//������
		}
	}
	else if(body.IsOnCrun())
	{
		PaintCrunWithColor(body.p2, colorNum);	//�����
		if(isPaintNum)							//�ڽ���������ҷֱ���ʾ1,2,3,4
		{
			POINT pos = body.p2->coord;
			pos.x -= 5; pos.y -= 8;
			dc->TextOut(pos.x, pos.y-15, "1", 1);
			dc->TextOut(pos.x, pos.y+15, "2", 1);
			dc->TextOut(pos.x-15, pos.y, "3", 1);
			dc->TextOut(pos.x+15, pos.y, "4", 1);
		}
	}
	else if(body.IsOnCtrl())
	{
		PaintCtrlWithColor(body.p3, colorNum);	//���ؼ�
	}
}

void Manager::PaintInvertBodyAtPos(const Pointer &body, POINT pos)
//��ָ��λ����ʾ����ķ���
{
	ASSERT(body.IsOnBody(false));
	if(body.IsOnCrun())
	{
		dc->BitBlt(pos.x-DD, pos.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCINVERT);
	}
	else //if(body.IsOnCtrl())
	{
		dc->BitBlt(pos.x, pos.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintHandle(body.p3), 0, 0, SRCINVERT);
	}
}


//4��������------------------------------------------------------------------------��
void Manager::SetAddState(BODY_TYPE type)
//������Ӻ�������
{
	ASSERT(type>=BODY_NO && type<CTRL_TYPE_NUM);
	addState = type;
}

CDC * Manager::GetCtrlPaintHandle(const CTRL * c)
//��ÿؼ���ͼ���
{
	CDC * paintMenDc = ctrlDcMem + c->GetStyle();	//Ĭ�ϵĻ�ͼ���

	//�������⻭ͼЧ���ؼ��Ļ�ͼDC
	if(c->IsBulbOn())				//С���ݴﵽ�����
			paintMenDc = ctrlDcMem + IDB_BULB_SHINE - IDB_SOURCE;
	else if(c->SwitchOnOff(false))	//���رպ�
			paintMenDc = ctrlDcMem + IDB_SWITCH_CLOSE - IDB_SOURCE;

	return paintMenDc + c->dir * CTRL_BITMAP_TYPE_NUM;	//λͼ����ת�Ƕ��й�ϵ
}

void Manager::GetName(const Pointer &pointer, char * str)const
//�������,str����Ӧ�ô��ڵ���NAME_LEN*2
{
	ASSERT(pointer.IsOnAny());
	if(pointer.IsOnLead())
	{
		sprintf(str, "����[%d]", pointer.p1->GetInitOrder());
	}
	else if(pointer.IsOnCrun())
	{
		sprintf(str, "���[���(%d), ��ǰ����(%s)]", pointer.p2->GetInitOrder(), pointer.p2->name);
	}
	else //if(pointer.IsOnCtrl())
	{
		sprintf(str, "�ؼ�[���(%d), ��ǰ����(%s)]", pointer.p3->GetInitOrder(), pointer.p3->name);
	}
}

bool Manager::DeleteNote(const Pointer &body)
//ɾ����ʾ,����ֵΪfalse�û�ȡ��ɾ��
{
	int conNum;				//���ӵ�����
	char name[NAME_LEN*2];	//��������
	char note[NAME_LEN*4];	//��ʾ�ַ���

	//������ӵ�����
	if(body.IsOnLead())
		conNum = 0;
	else if(body.IsOnCrun())
		conNum = body.p2->GetConnectNum();
	else if(body.IsOnCtrl())
		conNum = body.p3->GetConnectNum();
	else
		return false;

	//�������ӵ�������ʾɾ����Ϣ
	GetName(body, name);
	if(conNum > 0)
		sprintf(note, "Ҫɾ�� %s �� ?\n�����ӵ� %d �ε���Ҳ��ɾ��!", name, conNum);
	else
		sprintf(note, "Ҫɾ�� %s �� ?", name);

	PaintWithSpecialColor(body, false);	//�ñ�����ɫ(��ɫ)��ʾ����
	return IDYES == wndPointer->MessageBox(note, "ɾ��������ʾ", MB_YESNO|MB_ICONWARNING);
}

void Manager::ClearCircuitState()
//�����·״̬
{
	FocusBodyClear(NULL);	//����
	ClearPressBody();		//��ʾ���Ʋ�
	motiNum = 0;			//������������
	addState = BODY_NO;		//�����������
	lastMoveOnBody.Clear();	//����ϴ��ƶ���������
	lButtonDownState = 0;	//������״̬
}

Pointer Manager::GetBodyPointer(FOCUS_OR_POS &body)
//�������ָ��
{
	Pointer pointer;

	if(body.isFocusBody)
	{
		pointer = focusBody;
	}
	else
	{
		motiNum = 0;
		MotivateAll(body.pos);
		motiNum = 0;
		pointer = motiBody[0];
	}

	return pointer;
}

void Manager::SaveAsPicture(const char * path)
//�����·��ͼƬ
{
	PaintAll();	//����·, bitmapForRefresh������λͼ
	StaticClass::SaveBitmapToFile(HBITMAP(bitmapForRefresh), path);
}


//5�༭����------------------------------------------------------------------------��
void Manager::AddCtrl(POINT pos, BODY_TYPE style)
//��ӿؼ�
{
	ASSERT(ctrlNum < MAXCTRLNUM);

	ctrl[ctrlNum] = new CTRL(ctrlNum, pos, style);
	++ ctrlNum;

	PaintCtrlText(ctrl[ctrlNum-1]);
	Pointer newFocus;
	newFocus.SetOnCtrl(ctrl[ctrlNum-1], 1);
	FocusBodyPaint(&newFocus);
}

void Manager::AddCrun(POINT pos)
//��ӽ��
{
	ASSERT(crunNum < MAXCRUNNUM);

	crun[crunNum] = new CRUN(crunNum, pos);
	++ crunNum;

	PaintCrunText(crun[crunNum-1]);
	Pointer newFocus;
	newFocus.SetOnCrun(crun[crunNum-1], 1);
	FocusBodyPaint(&newFocus);
}

void Manager::AddLead(Pointer a, Pointer b)
//�õ�������2������
{
	ASSERT(leadNum < MAXLEADNUM);						//���߹���
	ASSERT(a.IsOnConnectPos() && b.IsOnConnectPos());	//���ӵ�
	ASSERT(!a.IsBodySame(&b));							//����ͬһ������

	//��ӵ���
	lead[leadNum] = new LEAD(leadNum, a, b);
	++leadNum;

	//��������ָ����
	if(a.IsOnCrun())
		a.p2->lead[a.GetLeadNum()] = lead[leadNum-1];
	else 
		a.p3->lead[a.GetLeadNum()] = lead[leadNum-1];
	if(b.IsOnCrun())
		b.p2->lead[b.GetLeadNum()] = lead[leadNum-1];
	else 
		b.p3->lead[b.GetLeadNum()] = lead[leadNum-1];

	//��ʾ��ӵĵ���
	PaintLead(lead[leadNum-1]);
}

void Manager::DeleteLead(LEAD * l)
//ɾ������2�����������
//ʹ�ú���: Delete(Pointer), ConnectBodyLead
{
	ASSERT(l != NULL);
	Pointer * a = l->conBody, * b = l->conBody + 1;
	int dira = a->GetLeadNum(), dirb = b->GetLeadNum();
	int num = l->num;

	//���ɾ�������ǽ���,�������
	Pointer pointer;
	pointer.SetOnLead(l);
	FocusBodyClear(&pointer);

	//������ӵ�ָ��
	if(a->IsOnCrun()) a->p2->lead[dira] = NULL;
	else if(a->IsOnCtrl()) a->p3->lead[dira] = NULL;
	if(b->IsOnCrun()) b->p2->lead[dirb] = NULL;
	else if(b->IsOnCtrl()) b->p3->lead[dirb] = NULL;

	//ɾ������
	delete l;
	if(num != leadNum-1)
	{
		lead[num] = lead[leadNum-1];
		lead[num]->num = num;
	}
	lead[leadNum-1] = NULL;
	--leadNum;
}

void Manager::DeleteSingleBody(Pointer pointer)
//������ɾ��һ�������߿ؼ�,��Ӱ����Χ����
{
	ASSERT(pointer.IsOnBody());
	int num;

	FocusBodyClear(&pointer);	//���ɾ�������ǽ���,�������

	if(pointer.IsOnCrun())
	{
		num = pointer.p2->num;
		delete pointer.p2;
		if(num != crunNum-1)
		{
			crun[num] = crun[crunNum-1];
			crun[num]->num = num;
		}
		crun[crunNum-1] = NULL;
		--crunNum;
	}
	else //if(pointer.IsOnCtrl())
	{
		num = pointer.p3->num;
		delete pointer.p3;
		if(num != ctrlNum-1)
		{
			ctrl[num] = ctrl[ctrlNum-1];
			ctrl[num]->num = num;
		}
		ctrl[ctrlNum-1] = NULL;
		--ctrlNum;
	}
}

void Manager::Delete(Pointer pointer)
//ɾ��
{
	ASSERT(pointer.IsOnAny() && !pointer.IsOnConnectPos());
	CloneCircuitBeforeChange();	//�༭ǰ���Ƶ�·

	if(pointer.IsOnLead())
	{
		DeleteLead(pointer.p1);
	}
	else if(pointer.IsOnCrun())
	{
		for(int i=0; i<4; ++i) if(pointer.p2->lead[i] != NULL)
			DeleteLead(pointer.p2->lead[i]);
		DeleteSingleBody(pointer);
	}
	else //if(pointer.IsOnCtrl())
	{
		for(int i=0; i<2; ++i) if(pointer.p3->lead[i] != NULL)
			DeleteLead(pointer.p3->lead[i]);
		DeleteSingleBody(pointer);
	}

	PutCircuitToVector();	//���µĵ�·��Ϣ���浽����
}

bool Manager::ConnectBodyLead(POINT posb)
//����һ�����ӵ�͵���
{
	Pointer a;				//�ȵ����������ӵ�
	Pointer x, y;			//��������(����)��2����������
	Pointer newCrun;		//����ӵĽ��
	POINT posa;				//�ȵ�����������
	char dir1, dir2, dir3;	//�������x,y,a�����ӵ�λ��
	LEADSTEP newLeadPosx, newLeadPosy;

	//1,��麯����������
	ASSERT(motiNum == 2 && motiBody[0].IsOnConnectPos() && motiBody[1].IsOnLead());
	motiNum = 0;
	if(crunNum >= MAXCRUNNUM)	//ֻҪ���������,����һ����
	{
		wndPointer->MessageBox("��㳬���������!", "��㲻�����", MB_ICONWARNING);
		return false;
	}

	//2,�༭ǰ���Ƶ�·
	CloneCircuitBeforeChange();

	//3,������������
	a = motiBody[0];
	x = motiBody[1].p1->conBody[0];
	y = motiBody[1].p1->conBody[1];
	if(a.IsOnCrun())posa = a.p2->coord;
	else posa = a.p3->coord;	//����ȵ�����������

	//4,��ʼ����������ӽ��ķ���
	if(motiBody[1].IsOnHoriLead())	//-3,-5,-7....����
	{
		if(motiBody[1].p1->GetBodyPos() & 2)
		{
			dir1 = 4;
			dir2 = 3;
		}
		else
		{
			dir1 = 3;
			dir2 = 4;
		}

		if(posa.y > posb.y)dir3 = 2;	//�ȵ�������ں���λ�õ�����
		else dir3 = 1;	//�ȵ�������ں���λ�õ�����
	}
	else	//-2,-4,-6....����
	{
		if(motiBody[1].p1->GetBodyPos() & 1)
		{
			dir1 = 2;
			dir2 = 1;
		}
		else
		{
			dir1 = 1;
			dir2 = 2;
		}

		if(posa.x > posb.x)dir3 = 4;	//�ȵ�������ں���λ�õ�����
		else dir3 = 3;	//�ȵ�������ں���λ�õ�����
	}

	//5,���ɾ������
	motiBody[1].p1->Divide(motiBody[1].GetAtState(), posb, newLeadPosx, newLeadPosy);	//����ԭ����������
	DeleteLead(motiBody[1].p1);	//ɾ��ԭ������
	AddCrun(posb);	//��ӽ��

	newCrun.SetOnCrun(crun[crunNum-1]);	//newCrunָ������ӽ��

	newCrun.SetAtState(dir1);
	AddLead(x, newCrun);	//x�ͽڵ�����,x�����,�½ڵ����յ�
	lead[leadNum-1]->ReplacePos(newLeadPosx);	//���껹ԭ

	newCrun.SetAtState(dir2);
	AddLead(newCrun, y);	//y�ͽڵ�����,y���յ�,�½ڵ������
	lead[leadNum-1]->ReplacePos(newLeadPosy);	//���껹ԭ

	newCrun.SetAtState(dir3);
	AddLead(a, newCrun);	//a�ͽڵ�����

	//6,���µĵ�·��Ϣ���浽����
	PutCircuitToVector();

	return true;
}

bool Manager::Delete(FOCUS_OR_POS &body)
//ɾ������
{
	Pointer pointer = GetBodyPointer(body);
	if(!pointer.IsOnAny()) return false;

	if(DeleteNote(pointer))
	{
		Delete(pointer);
		return true;
	}
	else 
	{
		return false;
	}
}


//6�����Ϣ������----------------------------------------------------------------��
bool Manager::AddBody(POINT pos)
//�������
{
	BODY_TYPE temp = addState;

	addState = BODY_NO;	//�����������
	dc->DPtoLP(&pos);

	if(BODY_CRUN == temp)
	{
		if(crunNum >= MAXCRUNNUM)
		{
			wndPointer->MessageBox("��㳬���������!", "��㲻�����", MB_ICONWARNING);
			return false;
		}

		CloneCircuitBeforeChange();	//�༭ǰ���Ƶ�·
		AddCrun(pos);				//�༭����
		PutCircuitToVector();		//���µĵ�·��Ϣ���浽����
		return true;
	}
	else if(Pointer::IsCtrl(temp))
	{
		if(ctrlNum >= MAXCTRLNUM)
		{
			wndPointer->MessageBox("��ѧԪ�������������!", "��ѧԪ���������", MB_ICONWARNING);
			return false;
		}

		CloneCircuitBeforeChange();	//�༭ǰ���Ƶ�·
		AddCtrl(pos, temp);			//�༭����
		PutCircuitToVector();		//���µĵ�·��Ϣ���浽����
		return true;
	}
	else
	{
		return false;
	}
}

void Manager::Property(FOCUS_OR_POS &body, bool isReadOnly)
//��ʾ�͸ı���������
{
	char tempStr[NAME_LEN*3];
	LISTDATA list;
	CDC * model = NULL;
	Pointer pointer = GetBodyPointer(body);

	if(pointer.IsOnLead())
	{
		GetName(pointer, tempStr);
		strcat(tempStr, " ����ɫ");					//���ڱ���
		pointer.p1->GetDataList(tempStr, &list);	//����
	}
	else if(pointer.IsOnCrun())
	{
		GetName(pointer, tempStr);
		strcat(tempStr, " �ı�ǩ");					//���ڱ���
		pointer.p2->GetDataList(&list);				//����
		model = &crunDcMem;							//ʾ��
	}
	else if(pointer.IsOnCtrl())
	{
		GetName(pointer, tempStr);
		strcat(tempStr, " �ı�ǩ�͵�ѧ����");		//���ڱ���
		pointer.p3->GetDataList(&list);				//����
		model = GetCtrlPaintHandle(pointer.p3);		//ʾ��
	}
	else
	{
		return;
	}

	PaintWithSpecialColor(pointer, false);	//�ñ�����ɫ(��ɫ)��ʾ����
	MyPropertyDlg dlg(&list, isReadOnly, model, tempStr, wndPointer);
	dlg.DoModal();
}

bool Manager::ShowBodyElec(FOCUS_OR_POS &body)
//���������,��ʾ��������ĵ���
{
	Pointer pointer = GetBodyPointer(body);
	if(!pointer.IsOnLead() && !pointer.IsOnCtrl()) return false;	//ֻ��ʾ���ߺͿؼ�

	char tempStr1[NAME_LEN*2];	//�ַ���
	char tempStr2[NAME_LEN*2];	//�ַ���
	char title[NAME_LEN*3];		//���ڱ���
	double elec;				//������С
	ELEC_STATE elecDir;			//��������
	CDC * model = NULL;			//property��ʾ�����ʾ��
	LISTDATA list;				//property��ʾ������

	//1,��õ�����Ϣ
	if(pointer.IsOnLead())
	{
		elec = pointer.p1->elec;
		elecDir  = pointer.p1->elecDir;
	}
	else //if(pointer.IsOnCtrl())
	{
		elec = pointer.p3->elec;
		elecDir  = pointer.p3->elecDir;

		model = GetCtrlPaintHandle(pointer.p3);	//ʾ��
	}

	//2,����LISTDATA
	switch(elecDir)
	{
	case UNKNOWNELEC:
		list.Init(1);
		list.SetAMember(DATA_STYLE_LPCTSTR, "������� :", "����û�м����!");
		break;

	case OPENELEC:
		list.Init(1);
		list.SetAMember(DATA_STYLE_LPCTSTR, "������� :", "û�е�������, ��·!");
		break;

	case SHORTELEC:
		list.Init(1);
		list.SetAMember(DATA_STYLE_LPCTSTR, "������� :", "��·��·!!!");
		break;

	case UNCOUNTABLEELEC:
		list.Init(1);
		list.SetAMember(DATA_STYLE_LPCTSTR, "������� :", "�����޵�����·��һ�ε���,�����޷�ȷ��!");
		break;

	case LEFTELEC:
	case RIGHTELEC:
		ASSERT(elec >= 0);	//������ָ�����

		if(StaticClass::IsZero(elec))
		{
			list.Init(1);
			list.SetAMember(DATA_STYLE_LPCTSTR, "������� :", "����Ϊ0");
			break;
		}

		if(pointer.IsOnLead())
		{
			GetName(pointer.p1->conBody[LEFTELEC != elecDir], tempStr1);
			GetName(pointer.p1->conBody[LEFTELEC == elecDir], tempStr2);

			list.Init(3);
			list.SetAMember(DATA_STYLE_double, DATA_NOTE[DATA_NOTE_CURRENT], &elec);
			list.SetAMember(DATA_STYLE_LPCTSTR, "������� :", tempStr1);
			list.SetAMember(DATA_STYLE_LPCTSTR, "�����յ� :", tempStr2);
		}
		else //if(pointer.IsOnCtrl())
		{
			switch(pointer.p3->dir ^ ((RIGHTELEC == elecDir)<<1))
			{
			case 0:
				strcpy(tempStr1, "������");
				break;
			case 1:
				strcpy(tempStr1, "���ϵ���");
				break;
			case 2:
				strcpy(tempStr1, "���ҵ���");
				break;
			case 3:
				strcpy(tempStr1, "���µ���");
				break;
			}

			list.Init(2);
			list.SetAMember(DATA_STYLE_double, DATA_NOTE[DATA_NOTE_CURRENT], &elec);
			list.SetAMember(DATA_STYLE_LPCTSTR, "���� :", tempStr1);
		}
		break;
	}	//switch(elecDir)

	//3,���ɴ��ڱ���
	strcpy(title, "����");
	GetName(pointer, title+strlen(title));
	strcat(title, "�ĵ���");

	//4,��ʾ�Ի���
	PaintWithSpecialColor(pointer, false);	//�ñ�����ɫ(��ɫ)��ʾ����
	MyPropertyDlg dlg(&list, true, model, title, wndPointer);
	dlg.DoModal();

	return true;
}

void Manager::ChangeCtrlStyle(FOCUS_OR_POS &body)
//�ı��ѧԪ������
{
	BODY_TYPE preStyle, newStyle;
	char tempStr[NAME_LEN*3];

	Pointer pointer = GetBodyPointer(body);
	if(!pointer.IsOnCtrl()) return;

	//���ԭ������
	preStyle = newStyle = pointer.p3->GetStyle();

	//��ʼ��list����
	LISTDATA list;
	list.Init(1);
	list.SetAEnumMember("��ѧԪ��������", &newStyle, ENUM_CTRL);

	//��ô��ڱ���
	GetName(pointer, tempStr);
	strcat(tempStr, " ������");

	//��ʾ�Ի���
	PaintWithSpecialColor(pointer, false);	//�ñ�����ɫ(��ɫ)��ʾ����
	MyPropertyDlg dlg(&list, false, GetCtrlPaintHandle(pointer.p3), tempStr, wndPointer);
	dlg.DoModal();

	//�ı�����
	if(preStyle != newStyle)
	{
		if(IDYES != AfxMessageBox("�ı����ͻᶪʧԭ�е�ѧԪ��������!\n������?", MB_YESNO)) return;
		pointer.p3->ChangeStyle(newStyle);
	}
}

bool Manager::MotivateAll(POINT &pos)
//�����������,�����Ƿ���������,����������ӵ���
{
	Pointer * mouse = motiBody + motiNum;
	int i;

	//1,��ʼ��-------------------------------------------
	ASSERT(motiNum >= 0 && motiNum < 2);
	dc->DPtoLP(&pos);
	mouse->Clear();

	//2,������ʲô������---------------------------------
	for(i = crunNum-1; i >= 0; --i)	//�������н��
	{
		mouse->SetAtState(crun[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnCrun(crun[i]);
			++motiNum;
			goto testPlace;
		}
	}
	for(i = leadNum-1; i >= 0; --i)	//�������е���
	{
		mouse->SetAtState(lead[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnLead(lead[i], false);
			++motiNum;
			goto testPlace;
		}
	}
	for(i = ctrlNum-1; i >= 0; --i)	//�������пؼ�
	{
		mouse->SetAtState(ctrl[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnCtrl(ctrl[i]);
			++motiNum;
			goto testPlace;
		}
	}

	return false;	//���е�����һ��û�м�������

testPlace:

	//3,ȥ������Ҫ��ʾ���ӵĲ���-------------------------
	if( 2 == motiNum		//ͬһ������������ӵ㲻����ʾ����
		&& motiBody[0].IsOnConnectPos() 
		&& motiBody[1].IsOnConnectPos()
		&& motiBody[0].IsBodySame(motiBody+1))	
	{
		--motiNum;
		return false;
	}
	else if(2 == motiNum	//���������
		&& motiBody[0].IsOnConnectPos()
		&& !motiBody[1].IsOnConnectPos() 
		&& !motiBody[1].IsOnLead())
	{
		--motiNum;
		return false;
	}

	return true;
}

bool Manager::LButtonDown(POINT pos)
//����WM_LBUTTONDOWN��Ϣ
{
	if(!isUpRecvAfterDown) motiNum = 0;		//���ϴ����������º�û�н��ܵ���갴����Ϣ
	lButtonDownState = MotivateAll(pos);	//��¼�������Ƿ���������
	lButtonDownPos = pos;					//��¼���������µ�����
	isUpRecvAfterDown = false;				//�յ���갴����Ϣ������Ϊtrue
	lastMoveOnPos.x = -100;					//��ԭ��������,����ƶ���������

	if(!lButtonDownState) //δ�����Ч��λ,����������,�������ӵ���
	{
		if(motiNum > 0 && motiBody[motiNum-1].IsOnConnectPos())
			PaintAll();	//����ShowAddLead���ĵ���

		motiNum = 0; return false;
	}
	else if(!motiBody[motiNum-1].IsOnConnectPos())	//����������ӵ�
	{
		FocusBodyPaint(motiBody+motiNum-1);	//�ػ潹������
	}

	if(2 == motiNum && motiBody[0].IsOnConnectPos())	//�жϵ�һ��ѡ�����Ƿ������ӵ�
	{
		if(motiBody[1].IsOnConnectPos())
		{
			CloneCircuitBeforeChange();			//�༭ǰ���Ƶ�·
			AddLead(motiBody[0], motiBody[1]);	//�༭����
			PutCircuitToVector();				//���µĵ�·��Ϣ���浽����
		}
		else if(motiBody[1].IsOnLead())
		{
			ConnectBodyLead(pos);
		}

		motiNum = 0; return true;
	}

	//AddLead������dira=1~4,dirb=1~4����Ϣ;ConnectBodyLead(pos)������dira=1~4,dirb=-2~-3,...����Ϣ.
	//dira=1~4,dirb=-1����Ϣ��������,����ˢ��
	//dira=-1,-2,-3,...����Ϣ���ε�(��Ϊ�����ֵ��������)
	//Ҳ����LButtonUpֻ�ܴ���1==motiNum,dira=-1,-2,-3,...����Ϣ;

	if(2 == motiNum) motiNum = 0;
	return false;
}

bool Manager::LButtonUp(POINT pos)
//�����������������Ϣ
{
	isUpRecvAfterDown = true;						//��갴�º��յ���갴����Ϣ
	if(!lButtonDownState || !motiNum) return false;	//û�е������
	dc->DPtoLP(&pos);
	Pointer * body = motiBody + motiNum - 1;

	//������ºͰ����������ͬ,���ҵ���Ĳ������ӵ�
	if( lButtonDownPos.x == pos.x && lButtonDownPos.y == pos.y 
		&& !body->IsOnConnectPos())
	{
		if(body->IsOnCtrl())
			body->p3->SwitchOnOff();	//���ؿ�������ı�
		FocusBodyPaint(NULL);			//�ػ潹��

		motiNum = 0;
		return false;
	}

	if(body->IsOnLead())	//�ƶ�����
	{
		body->p1->Move(body->GetAtState(), pos, maxLeaveOutDis);
		motiNum = 0;
		return true;
	}
	else if(body->IsOnBody())	//�ƶ������������
	{
		if(StaticClass::IsCtrlDown())	//�����Ctrl�����¸�������
			PosBodyClone(body, lButtonDownPos, pos);
		else
			PosBodyMove(body, lButtonDownPos, pos);
		motiNum = 0;
		return true;
	}
	else if(!body->IsOnConnectPos())
	{
		motiNum = 0;
		return false;
	}

	return false;
}

void Manager::PosBodyMove(Pointer * body, POINT firstPos, POINT lastPos)
//�ƶ�����
{
	int i;
	POINT inter;

	//����������
	inter.x = lastPos.x - firstPos.x;
	inter.y = lastPos.y - firstPos.y;
	if(inter.x==0 && inter.y==0) return;

	ASSERT(body->IsOnBody());
	if(body->IsOnCrun())
	{
		body->p2->coord.x += inter.x;
		body->p2->coord.y += inter.y;
		for(i=0; i<4; ++i) if(body->p2->lead[i])
			body->p2->lead[i]->RefreshPos();
	}
	else //if(body->IsOnCtrl())
	{
		body->p3->coord.x += inter.x;
		body->p3->coord.y += inter.y;
		for(i=0; i<2; ++i) if(body->p3->lead[i])
			body->p3->lead[i]->RefreshPos();
	}
}

bool Manager::PosBodyClone(const Pointer * body, POINT firstPos, POINT lastPos)
//���ƶ�λ�ø�������
{
	//����������
	POINT inter;
	inter.x = lastPos.x - firstPos.x;
	inter.y = lastPos.y - firstPos.y;

	//����
	if(body->IsOnCrun())
	{
		//��֤
		if(crunNum >= MAXCRUNNUM)
		{
			wndPointer->MessageBox("��㳬���������!", "��㲻�����", MB_ICONWARNING);
			return false;
		}

		//�༭ǰ���Ƶ�·
		CloneCircuitBeforeChange();

		//�༭��·
		crun[crunNum] = body->p2->Clone(CLONE_FOR_USE);
		crun[crunNum]->coord.x += inter.x;
		crun[crunNum]->coord.y += inter.y;
		crun[crunNum]->num = crunNum;
		++crunNum;

		//���µĵ�·��Ϣ���浽����
		PutCircuitToVector();

		//�ػ��·
		PaintCrun(crun[crunNum-1], true);
	}
	else //if(body->IsOnCtrl())
	{
		//��֤
		if(ctrlNum >= MAXCTRLNUM)
		{
			wndPointer->MessageBox("��ѧԪ�������������!", "��ѧԪ���������", MB_ICONWARNING);
			return false;
		}

		//�༭ǰ���Ƶ�·
		CloneCircuitBeforeChange();

		//�༭����
		ctrl[ctrlNum] = body->p3->Clone(CLONE_FOR_USE);
		ctrl[ctrlNum]->coord.x += inter.x;
		ctrl[ctrlNum]->coord.y += inter.y;
		ctrl[ctrlNum]->num = ctrlNum;
		++ctrlNum;

		//���µĵ�·��Ϣ���浽����
		PutCircuitToVector();

		//�ػ��·
		PaintCtrl(ctrl[ctrlNum-1], true);
	}

	return true;
}

void Manager::RotateCtrl(FOCUS_OR_POS &body, int rotateAngle)
//��ת�ؼ�
{
	Pointer pointer = GetBodyPointer(body);
	if(!pointer.IsOnCtrl()) return;
	pointer.p3->Rotate(rotateAngle);
}

BODY_TYPE Manager::PosBodyPaintRect(POINT pos)
//ͻ���һ�����
{
	Pointer * body = motiBody; //&motiBody[0]

	motiNum = 0;
	MotivateAll(pos);
	motiNum = 0;

	if(!body->IsOnAny()) return BODY_NO;

	if(body->IsOnConnectPos()) body->SetAtState(-1);

	if(body->IsOnBody()) dc->SelectObject(hp + BLUE);

	if(body->IsOnCrun())
	{
		dc->Rectangle(body->p2->coord.x-DD-2, body->p2->coord.y-DD-2, 
			body->p2->coord.x+DD+2, body->p2->coord.y+DD+2);
	}
	else if(body->IsOnCtrl())
	{
		dc->Rectangle(body->p3->coord.x-2, body->p3->coord.y-2, 
			body->p3->coord.x+BODYSIZE.cx+2, body->p3->coord.y+BODYSIZE.cy+2);
	}

	PaintWithSpecialColor(*body, false);
	return body->GetStyle();
}

void Manager::MouseMove(POINT pos, bool isLButtonDown)
//����ƶ���Ϣ����
{
	if(ShowAddBody(pos)) return;					//������������ʾ
	if(ShowMoveBody(pos, isLButtonDown)) return;	//�ƶ����������ʾ
	if(ShowMoveLead(isLButtonDown)) return;			//�ƶ����߹�����ʾ
	ShowAddLead(pos);								//���ӵ��߹�����ʾ

	//��꼤��������ʾ
	if(MotivateAll(pos))	//��꼤��������
	{
		--motiNum;
		PaintMouseMotivate(motiBody[motiNum]);
	}
	else					//���û�м�������
	{
		motiBody[1].Clear();
		PaintMouseMotivate(motiBody[1]);
	}
}

bool Manager::ShowAddLead(POINT pos)
//���ӵ��߹�����ʾ
{
	if(1 != motiNum) return false;

	Pointer * body = motiBody;
	POINT firstPos;

	if(!body->IsOnConnectPos()) return false;

	PaintAll();		//��ˢ��
	motiNum = 1;	//��ԭ����

	//dc�ƶ������
	dc->DPtoLP(&pos);
	dc->MoveTo(pos);

	//���ú�ɫ����
	dc->SelectStockObject(BLACK_PEN);

	//��ֱ��
	body->GetPosFromBody(firstPos);
	dc->LineTo(firstPos);

	return true;
}

bool Manager::ShowAddBody(POINT point)
//������������ʾ
{
	if(addState == BODY_CRUN)
	{
		if(lastMoveOnPos.x > -100)
			dc->BitBlt(lastMoveOnPos.x-DD, lastMoveOnPos.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCINVERT);
		dc->DPtoLP(&point);
		lastMoveOnPos = point;
		dc->BitBlt(lastMoveOnPos.x-DD, lastMoveOnPos.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCINVERT);

		::SetCursor(hcAddCrun);
		return true;
	}
	else if(Pointer::IsCtrl(addState))
	{
		CDC * tempDc = ctrlDcMem + addState;
		if(lastMoveOnPos.x > -100)
			dc->BitBlt(lastMoveOnPos.x, lastMoveOnPos.y, BODYSIZE.cx, BODYSIZE.cy, tempDc, 0, 0, SRCINVERT);
		dc->DPtoLP(&point);
		lastMoveOnPos = point;
		dc->BitBlt(lastMoveOnPos.x, lastMoveOnPos.y, BODYSIZE.cx, BODYSIZE.cy, tempDc, 0, 0, SRCINVERT);

		::SetCursor(NULL);
		return true;
	}
	else
	{
		return false;
	}
}

bool Manager::ShowMoveBody(POINT pos, bool isLButtonDown)
//�ƶ����������ʾ,lastMoveOnPos.x��ʼֵ��Ϊ-100,��LButtonDown��PaintAll������
{
	ASSERT(motiNum >= 0 && motiNum <= 2);
	if(motiNum == 0) return false;

	Pointer * body = motiBody + motiNum - 1;
	POINT bodyPos = {0, 0};

	if(!body->IsOnBody()) return false;
	if(!isLButtonDown)	//���û�а���
	{
		PaintAll(); 
		return false;
	}

	//�����������
	dc->DPtoLP(&pos);
	if(body->IsOnCrun()) bodyPos = body->p2->coord;
	else if(body->IsOnCtrl()) bodyPos = body->p3->coord;

	//�����������㻭ͼ����
	pos.x += bodyPos.x - lButtonDownPos.x;
	pos.y += bodyPos.y - lButtonDownPos.y;

	//����ϴ����껭������
	if(lastMoveOnPos.x > -100)
		PaintInvertBodyAtPos(*body, lastMoveOnPos);

	//���µ���������
	lastMoveOnPos = pos;	//����µ�����
	PaintInvertBodyAtPos(*body, lastMoveOnPos);

	//�����ctrl���������൱�ڸ���
	if(StaticClass::IsCtrlDown()) SetCursor(hcAddCrun);

	return true;
}

bool Manager::ShowMoveLead(bool isLButtonDown)
//�ƶ����߹�����ʾ
{
	ASSERT(motiNum>=0 && motiNum<=2);

	if(motiNum == 0 || !motiBody[motiNum-1].IsOnLead())
	{
		return false;
	}
	if(!isLButtonDown)	//���û�а���
	{
		PaintAll();
		return true;
	}

	if(motiBody[motiNum-1].IsOnHoriLead())
		SetCursor(hcMoveHorz);	//�ں���,�����"����ָ��"
	else 
		SetCursor(hcMoveVert);	//������,�����"����ָ��"

	return true;
}

void Manager::Help(POINT pos)
//���û���posλ�ð�F1,Ѱ�����
{
	char note[128];

	motiNum = 0;
	MotivateAll(pos);
	motiNum = 0;

	if(!motiBody[0].IsOnAny())
	{
		wndPointer->MessageBox("���û���ƶ��������� !", "��ʾ��Ϣ", MB_ICONINFORMATION);
		return;
	}

	if(motiBody[0].IsOnConnectPos())
	{
		strcpy(note, "�����������ӵ㲿��,����������������");
	}
	else if(motiBody[0].IsBodySame(&focusBody))
	{
		strcpy(note, "����ѡ������,��ʾ��ͬ����������");
		strcat(note, "\n������������ʹ�ÿ�ݼ�");
	}
	else if(motiBody[0].IsOnLead())
	{
		strcpy(note, "����,��������2������");
	}
	else if(motiBody[0].IsOnCrun())
	{
		strcpy(note, "���,��������4�ε���");
	}
	else //if(motiBody[0].IsOnCtrl())
	{
		strcpy(note, "��ѧԪ����");
		strcat(note, CTRL_STYLE_NAME[motiBody[0].p3->GetStyle()]);
		strcat(note, "\n������ת�� ���� ��Ϊ�������͵ĵ�ѧԪ��");
	}

	PaintWithSpecialColor(motiBody[0], false);
	wndPointer->MessageBox(note, "��ʾ��Ϣ", MB_ICONINFORMATION);
	PaintAll();
}

bool Manager::SearchNext(SEARCH_BY searchBy, BODY_TYPE range, bool isWholeWord, bool isMatchCase, char * keyWord)
//������һ������
{
	bool isAfterFocus = false;
	char str[32];
	bool isMatch;
	int round, j;
	bool isSearchLead = (range == BODY_ALL || range == BODY_LEAD) && searchBy == SEARCH_BY_ID;
	bool isSearchCrun = (range == BODY_ALL || range == BODY_CRUN);
	bool isSearchCtrl = (range == BODY_ALL || range == BODY_ALLCTRL || Pointer::IsCtrl(range));
	KMP kmp(keyWord, isWholeWord, isMatchCase || searchBy == SEARCH_BY_ID);	//�������ʱ�������ִ�Сд, �ӿ��ٶ�
	Pointer newFocus;

	for(round=0; round<2; ++round)
	{
		//search lead ----------------------------------------------------------
		if(!isAfterFocus && focusBody.IsOnLead())
		{
			isAfterFocus = true;
			if(isSearchLead)
				j = focusBody.p1->num + 1;
			else
				j = leadNum;
		}
		else if(isAfterFocus && isSearchLead)
		{
			j = 0;
		}
		else if(isAfterFocus && !isSearchLead && focusBody.IsOnLead())
		{
			return false;
		}
		else
		{
			j = leadNum;
		}

		for(; j<leadNum; ++j)
		{
			itoa(lead[j]->GetInitOrder(), str, 10);
			isMatch = kmp.IsMatch(str);
			if(focusBody.IsLeadSame(lead[j]))
			{
				return isMatch;
			}
			else if(isMatch)
			{
				newFocus.SetOnLead(lead[j], true);
				FocusBodyPaint(&newFocus);
				return true;
			}
		}

		//search crun ----------------------------------------------------------
		if(!isAfterFocus && focusBody.IsOnCrun())
		{
			isAfterFocus = true;
			if(isSearchCrun)
				j = focusBody.p2->num + 1;
			else
				j = crunNum;
		}
		else if(isAfterFocus && isSearchCrun)
		{
			j = 0;
		}
		else if(isAfterFocus && !isSearchCrun && focusBody.IsOnCrun())
		{
			return false;
		}
		else
		{
			j = crunNum;
		}

		for(; j<crunNum; ++j)
		{
			if(searchBy == SEARCH_BY_NAME)
			{
				isMatch = kmp.IsMatch(crun[j]->name);
			}
			else
			{
				itoa(crun[j]->GetInitOrder(), str, 10);
				isMatch = kmp.IsMatch(str);
			}
			if(focusBody.IsCrunSame(crun[j]))
			{
				return isMatch;
			}
			else if(isMatch)
			{
				newFocus.SetOnCrun(crun[j], true);
				FocusBodyPaint(&newFocus);
				return true;
			}
		}

		//search ctrl ----------------------------------------------------------
		if(!isAfterFocus && focusBody.IsOnCtrl())
		{
			isAfterFocus = true;
			if(isSearchCtrl)
				j = focusBody.p3->num + 1;
			else
				j = ctrlNum;
		}
		else if(isAfterFocus && isSearchCtrl)
		{
			j = 0;
		}
		else if(isAfterFocus && !isSearchCtrl && focusBody.IsOnCtrl())
		{
			return false;
		}
		else
		{
			j = ctrlNum;
		}

		for(; j<ctrlNum; ++j)
		{
			if(range == BODY_ALL || range == BODY_ALLCTRL || ctrl[j]->GetStyle() == range)
			{
				if(searchBy == SEARCH_BY_NAME)
				{
					isMatch = kmp.IsMatch(ctrl[j]->name);
				}
				else
				{
					itoa(ctrl[j]->GetInitOrder(), str, 10);
					isMatch = kmp.IsMatch(str);
				}
				if(focusBody.IsCtrlSame(ctrl[j]))
				{
					return isMatch;
				}
				else if(isMatch)
				{
					newFocus.SetOnCtrl(ctrl[j], true);
					FocusBodyPaint(&newFocus);
					return true;
				}
			}
		}
	}

	return false;
}

bool Manager::SearchPre(SEARCH_BY searchBy, BODY_TYPE range, bool isWholeWord, bool isMatchCase, char * keyWord)
//������һ������
{
	bool isAfterFocus = false;
	char str[32];
	bool isMatch;
	int round, j;
	bool isSearchLead = (range == BODY_ALL || range == BODY_LEAD) && searchBy == SEARCH_BY_ID;
	bool isSearchCrun = (range == BODY_ALL || range == BODY_CRUN);
	bool isSearchCtrl = (range == BODY_ALL || range == BODY_ALLCTRL || Pointer::IsCtrl(range));
	KMP kmp(keyWord, isWholeWord, isMatchCase || searchBy == SEARCH_BY_ID);	//�������ʱ�������ִ�Сд, �ӿ��ٶ�
	Pointer newFocus;

	for(round=0; round<2; ++round)
	{
		//search ctrl ----------------------------------------------------------
		if(!isAfterFocus && focusBody.IsOnCtrl())
		{
			isAfterFocus = true;
			if(isSearchCtrl)
				j = focusBody.p3->num - 1;
			else
				j = -1;
		}
		else if(isAfterFocus && isSearchCtrl)
		{
			j = ctrlNum-1;
		}
		else if(isAfterFocus && !isSearchCtrl && focusBody.IsOnCtrl())
		{
			return false;
		}
		else
		{
			j = -1;
		}

		for(; j>=0; --j)
		{
			if(range == BODY_ALL || range == BODY_ALLCTRL || ctrl[j]->GetStyle() == range)
			{
				if(searchBy == SEARCH_BY_NAME)
				{
					isMatch = kmp.IsMatch(ctrl[j]->name);
				}
				else
				{
					itoa(ctrl[j]->GetInitOrder(), str, 10);
					isMatch = kmp.IsMatch(str);
				}
				if(focusBody.IsCtrlSame(ctrl[j]))
				{
					return isMatch;
				}
				else if(isMatch)
				{
					newFocus.SetOnCtrl(ctrl[j], true);
					FocusBodyPaint(&newFocus);
					return true;
				}
			}
		}

		//search crun ----------------------------------------------------------
		if(!isAfterFocus && focusBody.IsOnCrun())
		{
			isAfterFocus = true;
			if(isSearchCrun)
				j = focusBody.p2->num - 1;
			else
				j = -1;
		}
		else if(isAfterFocus && isSearchCrun)
		{
			j = crunNum - 1;
		}
		else if(isAfterFocus && !isSearchCrun && focusBody.IsOnCrun())
		{
			return false;
		}
		else
		{
			j = -1;
		}

		for(; j>=0; --j)
		{
			if(searchBy == SEARCH_BY_NAME)
			{
				isMatch = kmp.IsMatch(crun[j]->name);
			}
			else
			{
				itoa(crun[j]->GetInitOrder(), str, 10);
				isMatch = kmp.IsMatch(str);
			}
			if(focusBody.IsCrunSame(crun[j]))
			{
				return isMatch;
			}
			else if(isMatch)
			{
				newFocus.SetOnCrun(crun[j], true);
				FocusBodyPaint(&newFocus);
				return true;
			}
		}

		//search lead ----------------------------------------------------------
		if(!isAfterFocus && focusBody.IsOnLead())
		{
			isAfterFocus = true;
			if(isSearchLead)
				j = focusBody.p1->num - 1;
			else
				j = -1;
		}
		else if(isAfterFocus && isSearchLead)
		{
			j = leadNum - 1;
		}
		else if(isAfterFocus && !isSearchLead && focusBody.IsOnLead())
		{
			return false;
		}
		else
		{
			j = -1;
		}

		for(; j>=0; --j)
		{
			itoa(lead[j]->GetInitOrder(), str, 10);
			isMatch = kmp.IsMatch(str);
			if(focusBody.IsLeadSame(lead[j]))
			{
				return isMatch;
			}
			else if(isMatch)
			{
				newFocus.SetOnLead(lead[j], true);
				FocusBodyPaint(&newFocus);
				return true;
			}
		}
	}

	return false;
}


//7�ļ�����--------------------------------------------------------------------��
const char * Manager::GetFilePath()
//��ȡ�ļ�·��
{
	return fileName;
}

bool Manager::SaveFile(const char * newFile)
//�����·
{
	ASSERT(newFile != NULL && newFile[0] != '\0');
	long i;
	FILE * fp;

	strcpy(fileName, newFile);	//�滻ԭ���ļ�·��
	fp = fopen(fileName, "wb");
	if(fp == NULL)	//�ļ����ܴ�
	{
		wndPointer->MessageBox("�ļ�����д !", "�����ļ�����", MB_ICONERROR);
		return false;
	}

	//1�����ļ��汾
	i = FILE_VERSION;
	fwrite(&i, sizeof(long), 1, fp);

	//2������������
	fwrite(&crunNum, sizeof(short), 1, fp);
	fwrite(&ctrlNum, sizeof(short), 1, fp);
	fwrite(&leadNum, sizeof(short), 1, fp);

	//3������
	for(i = crunNum-1; i >= 0; --i)
		crun[i]->SaveToFile(fp);

	//4����ؼ�
	for(i = ctrlNum-1; i >= 0; --i)
		ctrl[i]->SaveToFile(fp);

	//5���浼��
	for(i = leadNum-1; i >= 0; --i)
		lead[i]->SaveToFile(fp);

	//6������������
	fwrite(&moveBodySense, sizeof(int), 1, fp);		//�������һ�������ƶ��ľ���
	fwrite(&maxLeaveOutDis, sizeof(int), 1, fp);	//���ߺϲ�������
	fwrite(&textColor, sizeof(enum), 1, fp);		//������ɫ
	fwrite(&focusLeadStyle, sizeof(enum), 1, fp);	//���㵼����ʽ
	fwrite(&focusCrunColor, sizeof(enum), 1, fp);	//��������ɫ
	fwrite(&focusCtrlColor, sizeof(enum), 1, fp);	//����ؼ���ɫ
	focusBody.SaveToFile(fp);						//��������
	fwrite(&viewOrig, sizeof(POINT), 1, fp);		//�ӽǳ�ʼ����

	//7�ļ�������,�����ļ�����
	char tmpForReserve[FILE_RESERVE_SIZE] = {0};
	fwrite(tmpForReserve, FILE_RESERVE_SIZE, 1, fp);

	fclose(fp);
	return true;
}

bool Manager::ReadFile(const char * newFile)
//��ȡ��·
{
	ASSERT(newFile != NULL && newFile[0] != '\0');
	FILE * fp;
	int i;
	POINT pos1 = {NULL};
	Pointer body;

	fp = fopen(newFile, "rb");
	if(fp == NULL)
	{
		wndPointer->MessageBox("�ļ����ܲ����ڻ��ܶ�ȡ !", "��ȡ�ļ�����", MB_ICONERROR);
		return false;
	}

	//1��ȡ�ļ��汾
	fread(&i, sizeof(int), 1, fp);
	if(i != FILE_VERSION)	//�ļ��汾��ͬ,�����ȡ
	{
		fclose(fp);
		wndPointer->MessageBox("�ļ��汾���� !", "��ȡ�ļ�����", MB_ICONERROR);
		return false;
	}

	DeleteVector(circuitVector.begin(), circuitVector.end());	//�����������ĵ�·��Ϣ
	strcpy(fileName, newFile);	//�滻ԭ��·��

	try	//������Ϊ�ļ��������������
	{
		//2��ȡ��������
		fread(&crunNum, sizeof(short), 1, fp);
		fread(&ctrlNum, sizeof(short), 1, fp);
		fread(&leadNum, sizeof(short), 1, fp);

		//����ȡ�����������Ƿ�������ķ�Χ֮��
		if(crunNum < 0 || leadNum < 0 || ctrlNum < 0)
			goto READFILEERROR;
		if(crunNum>MAXCRUNNUM || leadNum>MAXLEADNUM || ctrlNum>MAXCTRLNUM)
			goto READFILEERROR;

		//Ϊÿ�����������ڴ�ռ�
		for(i = crunNum-1; i >= 0; --i)
			crun[i] = new CRUN(i, pos1);
		for(i = ctrlNum-1; i >= 0; --i)
			ctrl[i] = new CTRL(i, pos1, SOURCE, false);
		for(i = leadNum-1; i >= 0; --i)
			lead[i] = new LEAD(i, motiBody[0],motiBody[1], false);

		//3��ȡ���
		CRUN::ResetInitNum();
		for(i = crunNum-1; i >= 0; --i)
			crun[i]->ReadFromFile(fp, lead);

		//4��ȡ�ؼ�
		CTRL::ResetInitNum();
		for(i = ctrlNum-1; i >= 0; --i)
			ctrl[i]->ReadFromFile(fp, lead);

		//5��ȡ����
		LEAD::ResetInitNum();
		for(i = leadNum-1; i >= 0; --i)
			lead[i]->ReadFromFile(fp, lead, crun, ctrl);

		//6��ȡ��������
		fread(&moveBodySense, sizeof(UINT), 1, fp);		//�������һ�������ƶ��ľ���
		fread(&maxLeaveOutDis, sizeof(UINT), 1, fp);	//���ߺϲ�������
		fread(&textColor, sizeof(enum), 1, fp);			//������ɫ
		fread(&focusLeadStyle, sizeof(enum), 1, fp);	//���㵼����ʽ
		fread(&focusCrunColor, sizeof(enum), 1, fp);	//��������ɫ
		fread(&focusCtrlColor, sizeof(enum), 1, fp);	//����ؼ���ɫ
		body.ReadFromFile(fp, lead, crun, ctrl);		//��ȡ��������
		FocusBodySet(body);								//���ý�������
		fread(&viewOrig, sizeof(POINT), 1, fp);			//�ӽǳ�ʼ����

		dc->SetTextColor(LEADCOLOR[textColor]);								//��ʼ��������ɫ
		dc->SetViewportOrg(-viewOrig.x, -viewOrig.y);						//��ʼ���ӽǳ�ʼ����
		wndPointer->SetScrollPos(SB_HORZ, viewOrig.x/mouseWheelSense.cx);	//��ʼ��ˮƽ������
		wndPointer->SetScrollPos(SB_VERT, viewOrig.y/mouseWheelSense.cy);	//��ʼ����ֱ������

	}	//try

	catch(...)
	{
	READFILEERROR:
		fclose(fp);
		wndPointer->MessageBox("�ļ��������� !", "��ȡ�ļ�����", MB_ICONERROR);
		exit(0);
	}

	fclose(fp);				//�ر��ļ����
	PutCircuitToVector();	//����ǰ��·��Ϣ���浽����
	return true;			//�����˳�
}

void Manager::CreateFile()
//�������ļ�(�յ�)
{
	fileName[0] = '\0';											//·�����
	ClearCircuitState();										//�����·״̬��Ϣ
	DeleteVector(circuitVector.begin(), circuitVector.end());	//�����������ĵ�·��Ϣ
	leadNum = crunNum = ctrlNum = 0;							//����������Ϊ0
	PutCircuitToVector();										//����ǰ�յ�·��Ϣ���浽����
}


//8���㺯��------------------------------------------------------------------------��
void Manager::CombineGroup(
							int from,
							int to,
							int * group,
							int groupSize)
//��2�������� ����� �ϲ�
{
	ASSERT(group!=NULL && groupSize>0 && from>=0 && to>=0);
	for(int i=groupSize-1; i>=0; --i) if(group[i]==from)
		group[i] = to;
}

char Manager::GetCrun2ConnectNum(int a, int b)
//���2��crun2���ֱ�����ӵ���·����
{
	int i;
	char connect = 0;
	CIRCU * temp;
	for(i=0; i<4; ++i)
	{
		temp = crun2[a].c[i];
		if(!temp)continue;
		if(temp->to == crun2+b || temp->from == crun2+b) ++connect;
	}
	return connect;
}

CIRCU * Manager::GetCrun2FirstCircu(int a, int b, int &num)
//�ɽ���Ż�õ�һ���������ǵ���·
//�������ص�����·ָ��,ͬʱ����·��crun2[a].c[i]��i���ظ�num
{
	CIRCU * temp;

	for(int i=0; i<4; ++i)
	{
		temp = crun2[a].c[i];
		if(!temp) continue;
		if(temp->to == crun2+b || temp->from == crun2+b)
		{
			num = i;
			return temp;
		}
	}

	return NULL;
}

void Manager::PutIntoBuf(	int fromGroup,
							int toGroup,
							CRUNMAP * map,
							double * buf)
//��from,to����һ��������·�ĵ���������뻺��
{
	int i;
	CIRCU * c;
	ASSERT(map != NULL && buf != NULL);

	if(fromGroup < toGroup)
		i = CONVERT(fromGroup, toGroup, map->size);
	else
		i = CONVERT(toGroup, fromGroup, map->size);

	c = map->firstcircu[i];

	if(c->from - crun2 == map->crunTOorder[fromGroup])
	{
		buf[ c->numInGroup ]  =  c->resistance;
		buf[ map->circuNum ] += c->pressure;
	}
	else 
	{
		buf[ c->numInGroup ]  = -c->resistance;
		buf[ map->circuNum ] -= c->pressure;
	}
}

int Manager::CreateCrunEquation(CRUN2 * inputCrun2, double * buf)
//������㷽��,�����������
{
	CIRCU ** tempCircu = inputCrun2->c;
	int connectNum = 0;
	int i;

	for(i=0; i<4; ++i) if(tempCircu[i])
	{
		++ connectNum;

		if(inputCrun2 == tempCircu[i]->from && i == tempCircu[i]->dirFrom)
			buf[tempCircu[i]->numInGroup] += 1;
		else
			buf[tempCircu[i]->numInGroup] -= 1;
	}

	return connectNum;
}

void Manager::CollectCircuitInfo()
//����һ�ε�·,���ÿ��Ⱥ�����·��ѧ��Ϣ
{
	Pointer now, pre;	//��ǰ������
	int dir;			//��һ�������ڵ�ǰ����ķ���
	int i, j, tempVar;	//ѭ������
	int endCrunNum;		//��·���������
	int * group;		//�������
	int groupSize = 0;	//��ǰʹ�õ�group����Ĵ�С

	//1,��ʼ��----------------------------------------------------
	groupNum = 0;	//����,ͬһ�����һ����ͨͼ��,���齨������
	circuNum = 0;	//��·��
	group = new int[crunNum];		//�������ᳬ��crunNum
	crun2 = new CRUN2[crunNum];		//���ڼ���Ľ��
	circu = new CIRCU[crunNum*2];	//��·�����ᳬ��crunNum*2
	for(i=crunNum-1; i>=0; --i)group[i] = i;

	//2��������·,�Խ��Ϊͷ��β-----------------------------------
	for(i=crunNum-1; i>=0; --i) if(crun[i]->GetConnectNum() >= 3)
	//���������ӵ��߸���������3,�����㲻��Ҫ����
	//0--����ǹ�����;1--����·;2--����൱�ڵ���
		for(j=0; j<4; ++j) if(crun[i]->lead[j] && crun2[i].c[j] == NULL)
	//���㵱ǰ�����е������� ���� û�м�����(crun2[i].c[j] == NULL)
	{
		now.SetOnLead(crun[i]->lead[j]);
		dir = now.p1->conBody[0].IsCrunSame(crun[i]);

		circu[circuNum].resistance = 0;	//������0
		circu[circuNum].pressure   = 0;	//��ѹ��0

		while(true)	//������һ���������岻��2���Ľ�����
		{
			pre = now;

			if(now.IsOnCtrl())	//�ؼ�
			{
				//����ؼ�
				if(now.p3->GetResist() < 0 || now.p3->lead[dir] == NULL)	//��·��
				{
					circu[circuNum].resistance = -1;
					break;
				}
				circu[circuNum].resistance += now.p3->GetResist();
				circu[circuNum].pressure   += now.p3->GetPress(!dir);	//�����෴

				//����һ������
				now.SetOnLead(pre.p3->lead[dir]);
				dir = now.p1->conBody[0].IsCtrlSame(pre.p3);
			}
			else	//����,����һ������
			{
				now = pre.p1->conBody[dir];
				if(now.IsOnCrun())	//�����������岻��2���Ľ�����
				{
					tempVar = now.p2->GetConnectNum();
					if(tempVar >= 3)	//ͨ·
					{
						dir = now.p2->GetDirect(pre.p1);	//��¼�½����Ľ��ͷ���
						break;
					}
					else if(tempVar == 2)	//����,�൱�ڵ���
					{
						//�ҵ�������ӵ���һ������
						for(dir=0; dir<4; ++dir)
							if(now.p2->lead[dir]!=NULL && pre.p1 != now.p2->lead[dir]) break;
						//ת��������ӵ���һ������
						pre = now;
						now.SetOnLead(pre.p2->lead[dir]);
						dir = now.p1->conBody[0].IsCrunSame(pre.p2);
					}
					else if(tempVar == 1)	//��·
					{
						circu[circuNum].resistance = -1;
						break;	//��·����
					}
					else
					{
						throw "��·�ļ��д� !�������ӽ��,���ǽ�㲻���ӵ��� !";
						break;
					}
				}
				else if(now.IsOnCtrl())
				{
					dir = now.p3->lead[0] == pre.p1;
				}
				else if(now.IsOnLead())
				{
					throw "2������ֱ�����ӳ��ֻ�������� !";
					break;
				}
				else
				{
					throw "��·�ļ��д� !����ֻ����һ������ !";
					break;
				}
				
			}
		}//while(true)

		if(circu[circuNum].resistance >= 0)
		{
			endCrunNum = now.p2->num;

			circu[circuNum].eleNum = circuNum;
			circu[circuNum].from = crun2 + i;
			circu[circuNum].dirFrom = (char)j;
			circu[circuNum].to = crun2 + endCrunNum;
			circu[circuNum].dirTo = (char)dir;
			crun2[endCrunNum].c[dir] = crun2[i].c[j] = circu+circuNum;
			++ circuNum;	//���Ƕ�·,������Ч��·

			if(crun2[i].group >= 0)
			{
				if(crun2[endCrunNum].group >= 0 && group[crun2[i].group] == group[crun2[endCrunNum].group])
				{
					continue;
				}
				if(crun2[endCrunNum].group >= 0)	//group�ϲ�
				{
					CombineGroup(
									group[crun2[endCrunNum].group],
									group[crun2[i].group],
									group,
									groupSize);
					--groupNum;	//�ϲ�
				}
				else	//�̳����������group
				{
					crun2[endCrunNum].group = crun2[i].group;
				}
			}
			else
			{
				if(crun2[endCrunNum].group>=0)	//�̳����������group
				{
					crun2[i].group=crun2[endCrunNum].group;
				}
				else	//�����µ�group
				{
					crun2[i].group = crun2[endCrunNum].group = groupSize;
					++groupSize;
					++groupNum;	//�����µ�
				}
			}

		}//if( circu[i].resistance >= 0 )

	}//for

	//3,��group���гɴ�0��ʼ����������-----------------------------------
	dir = 0;
	for(i=groupNum-1; i>=0; --i)
	{
		for(; dir<groupSize; ++dir) if(group[dir] >= 0) break;
		for(j=dir+1; j<groupSize; ++j) if(group[j]==group[dir]) group[j] = -i - 1;
		group[dir] = -i - 1;
		++dir;
	}
	for(j=groupSize-1; j>=0; --j) group[j] = -group[j] - 1;

	//4,����crun2��group������������ӱ�־,����ָ��group�����ָ��-------
	//����ת��Ϊ��������ӱ�־
	for(i=crunNum-1; i>=0; --i) if(crun2[i].group >= 0) crun2[i].group = group[crun2[i].group];

	delete [] group;
}

bool Manager::FindRoad(const CRUNMAP * map, ROAD * roads, int j, int k)
//�γ�һ�����j �� ��������·��,����j k֮���ֱ������
{
	const int size = map->size;
	bool state;			//��¼�Ƿ�ı���
	int i, next;		//ѭ������
	bool * interFlag;	//j �Ƿ��ҵ���ĳ�����ߵı��

	interFlag = new bool[size];
	for(i=size-1; i>j; --i) interFlag[i] = map->direct[CONVERT(j,i,size)] > 0;
	for(i=j-1; i>=0; --i) interFlag[i] = map->direct[CONVERT(i,j,size)] > 0;
	interFlag[j] = interFlag[k] = false;

	do
	{
		state = false;	//����ϴ�״̬

		for(i=size-1; i>=0; --i) if(i!=j && i!=k && interFlag[i])
		{
			for(next=size-1; next>i; --next)
				if(next-j && !interFlag[next] && map->direct[CONVERT(i,next,size)]>0)
			{
				state = true;	//�ı���
				interFlag[next] = true;
				roads[i].Clone(roads[next]);
				roads[next].InsertPointAtTail(i);
			}

			if(interFlag[k]) goto end;	//�˳�ѭ��

			for(next=i-1; next>=0; --next)
			{
				if(next != j && !interFlag[next] && map->direct[CONVERT(next,i,size)] > 0)
				{
					state = true;	//�ı���
					interFlag[next] = true;
					roads[i].Clone(roads[next]);
					roads[next].InsertPointAtTail(i);
				}
			}

			if(interFlag[k]) goto end;	//�˳�ѭ��
		}

	}
	while(state);

end:

	state = interFlag[k];
	delete [] interFlag;
	return state;	//�����Ƿ��ҵ� j->k ����·
}

void Manager::CreateEquation()
//������·��Ϣ,��Ⱥ�齨������
{
	int group, i, j, k, size;
	CRUNMAP * nowMap;
	CRUNMAP * maps;

	//1 ��ʼ��maps---------------------------------------------------------
	//1.1 ��ʼ��ÿ��group��crun��Ա����
	this->maps = maps = new CRUNMAP[groupNum];
	for(i=groupNum-1; i>=0; --i)maps[i].size=0;
	for(i=crunNum-1; i>=0; --i) if(crun2[i].group >= 0)
		++ maps[crun2[i].group].size;
	for(i=groupNum-1; i>=0; --i)
	{
		maps[i].Init(maps[i].size);	//��ʼ��map�ڴ�
		maps[i].size = 0;
	}
	for(i=crunNum-1; i>=0; --i) if(crun2[i].group >= 0)
	{
		nowMap = maps + crun2[i].group;
		nowMap->crunTOorder[nowMap->size] = i;
		++nowMap->size;
	}

	//1.2 ��ʼ��ÿ��group��circu��Ա����
	for(i=groupNum-1; i>=0; --i) maps[i].circuNum = 0;
	for(i=circuNum-1; i>=0; --i)
	{
		nowMap = maps + circu[i].from->group;
		circu[i].numInGroup = nowMap->circuNum;
		++nowMap->circuNum;
	}

	//1.3 ��ʼ��ÿ��group��direct��Ա����
	for(group=groupNum-1; group>=0; --group)
	{
		nowMap = maps+group;
		size = nowMap->size;

		for(j=size-2; j>=0; --j) for(k=size-1; k>j; --k)
		{
			i = CONVERT(j, k, size);
			nowMap->direct[i] =
				GetCrun2ConnectNum(nowMap->crunTOorder[j], nowMap->crunTOorder[k]);
		}
	}//for(group

	//1.4  ��ʼ������2�����ĵ�һ����·
	for(group=groupNum-1; group>=0; --group)
	{
		nowMap = maps+group;
		size = nowMap->size;

		for(j=size-2; j>=0; --j) for(k=size-1; k>j; --k)
		{
			i = CONVERT(j, k, size);
			nowMap->firstcircu[i] = 
				GetCrun2FirstCircu(nowMap->crunTOorder[j], nowMap->crunTOorder[k], nowMap->dir[i]);
		}
	}


	//2	2�����֮����>=2��ֱ�����ӵ���·,	-----------------------------
	//	����֮���γɻ�·,�õ����ַ���		-----------------------------
	double * outPutBuf;	//��������̵Ļ�������
	double saveForBuf;	//���沿������
	CRUN2 * crunNum1, * crunNum2;
	int connect, firstConnect, nextConnect;

	equation = new Equation * [groupNum];	//����ָ��
	for(group=groupNum-1; group>=0; --group)
	{
		nowMap = maps + group;
		size = nowMap->size;

		outPutBuf = new double[nowMap->circuNum+1];	//��ʼ����������̵�����
		equation[group] = new Equation(size, nowMap->circuNum);	//��ʼ��������

		for(j=size-2; j>=0; --j) for(k=size-1; k>j; --k)
		{
			ZeroMemory(outPutBuf, (nowMap->circuNum+1) * sizeof(double));	//��������
			i = CONVERT(j, k, size);
			if(nowMap->direct[i] < 2) continue;

			crunNum1 = crun2 + nowMap->crunTOorder[j];
			crunNum2 = crun2 + nowMap->crunTOorder[k];
			firstConnect = nowMap->dir[i];	//��һ������

			//��ȡ�����沿����·����
			if(crunNum1->c[firstConnect]->from == crunNum2)
			{
				outPutBuf[crunNum1->c[firstConnect]->numInGroup] =
					-crunNum1->c[firstConnect]->resistance;
				saveForBuf = -crunNum1->c[firstConnect]->pressure;
			}
			else
			{
				outPutBuf[crunNum1->c[firstConnect]->numInGroup] =
					crunNum1->c[firstConnect]->resistance;
				saveForBuf = crunNum1->c[firstConnect]->pressure;
			}
			nextConnect = firstConnect + 1;

			//2�����֮����>=2��ֱ�����ӵ���·,����֮���γɻ�·,�õ����ַ���
			for(connect=nowMap->direct[i]-2; connect>=0; --connect)
			{
				//1,Ѱ����һ�����ӵ���·
				while(	crunNum1->c[nextConnect] == NULL
						|| 
						(	crunNum1->c[nextConnect]->to != crunNum2 
							&& 
							crunNum1->c[nextConnect]->from != crunNum2
						)
					 )
					++ nextConnect;

				//2,������,��ѹ����
				outPutBuf[nowMap->circuNum] = saveForBuf;	//д�뱣�������
				if(crunNum1->c[nextConnect]->from == crunNum2)
				{
					outPutBuf[crunNum1->c[nextConnect]->numInGroup] =
						crunNum1->c[nextConnect]->resistance;
					outPutBuf[nowMap->circuNum] += 
						crunNum1->c[nextConnect]->pressure;
				}
				else
				{
					outPutBuf[crunNum1->c[nextConnect]->numInGroup] =
						- crunNum1->c[nextConnect]->resistance;
					outPutBuf[nowMap->circuNum] -= 
						crunNum1->c[nextConnect]->pressure;
				}

				//3,���뷽��
				equation[group]->InputARow(outPutBuf);

				//4,�ָ�
				outPutBuf[crunNum1->c[nextConnect]->numInGroup] = 0;

				//5,��һ��
				++ nextConnect;
			}
		}

		delete [] outPutBuf;	//�ͷŻ���
	}//for(group


	//3 ������һ�����Ļ�·,ֱ�Ӽ��������뷽��  ----------------------
	for(i=circuNum-1; i>=0; --i) if(circu[i].from == circu[i].to)
	{
		//��ʼ������
		group  = circu[i].from->group;
		nowMap = maps + group;
		outPutBuf = new double[nowMap->circuNum+1];	//��������̵Ļ���
		ZeroMemory(outPutBuf, (nowMap->circuNum+1) * sizeof(double));	//��������

		if(StaticClass::IsZero(circu[i].resistance) && StaticClass::IsZero(circu[i].pressure))
		{//�����ѹ����0;�����Ϊ1,��ѹΪ0
			outPutBuf[circu[i].numInGroup]	= 1;
			outPutBuf[nowMap->circuNum]		= 0;
		}
		else
		{//����������·���
			outPutBuf[circu[i].numInGroup]	= circu[i].resistance;
			outPutBuf[nowMap->circuNum]		= circu[i].pressure;
		}

		equation[group]->InputARow(outPutBuf);	//���뷽�̵�һ��

		delete [] outPutBuf;	//�ͷŻ���
	}


	//4	��ֱ����·���ӵ�2�����, �γ�һ����·��		---------------------
	//	�û�·����һ������������ǵ���·,			---------------------
	//	�ɻ�·��Ϣ�ó����� .						---------------------
	for(group=groupNum-1; group>=0; --group)
	{
		nowMap = maps + group;
		size = nowMap->size;
		outPutBuf = new double[nowMap->circuNum+1];	//��������̵Ļ���
		ROAD * roads ;

		for(j=size-2; j>=0; --j) for(k=size-1; k>j; --k)
		{
			//��ʼ��
			i = CONVERT(j, k, size);
			if(nowMap->direct[i] <= 0) continue;
			
			roads = new ROAD[size] ;
			ZeroMemory(outPutBuf, (nowMap->circuNum+1) * sizeof(double));	//��������

			//���·��,��������
			if(FindRoad(nowMap, roads, j, k))	//��·���õ����̵�һ��
			{
				ROADSTEP * prep = roads[k].first;
				ROADSTEP * nowp;

				if(prep == NULL) continue;	//����

				nowp = prep->next;
				PutIntoBuf(j, prep->crunNum, nowMap, outPutBuf);

				while(nowp != NULL)
				{
					PutIntoBuf(prep->crunNum, nowp->crunNum, nowMap, outPutBuf);

					//��һ��
					prep = nowp;
					nowp = nowp->next;
				}

				PutIntoBuf(prep->crunNum, k, nowMap, outPutBuf);

				PutIntoBuf(k, j, nowMap, outPutBuf);	//������j��k�ĵ�һ����·

				equation[group]->InputARow(outPutBuf);	//���뷽��
			}
			else if(1 == nowMap->direct[i])	//û��·��,�õ��ߵ�����0
			{
				//���ߵ�����Ϊ0
				outPutBuf[nowMap->firstcircu[i]->numInGroup] = 1;
				outPutBuf[nowMap->circuNum] = 0;

				equation[group]->InputARow(outPutBuf);	//���뷽��
			}

			delete [] roads;
		}

		delete [] outPutBuf;	//�ͷŻ���
	}//for(group


	//5�γɽ�㷽��------------------------------------------------------
	for(group=groupNum-1; group>=0; --group)
	{
		nowMap = maps+group;
		size = nowMap->size;
		outPutBuf = new double[nowMap->circuNum+1];	//��������̵Ļ���

		for(k=size-2; k>=0; --k)	//ֻ������k-1����㷽��
		{
			ZeroMemory(outPutBuf, (nowMap->circuNum+1) * sizeof(double));	//��������
			if(CreateCrunEquation(crun2 + nowMap->crunTOorder[k], outPutBuf))	//��������
				equation[group]->InputARow(outPutBuf);	//���뷽��
		}

		delete [] outPutBuf;	//�ͷŻ���
	}
}

void Manager::TravelCircuitPutElec(	Pointer now,
									const CRUN * last,
									int dir,
									double elec,
									ELEC_STATE flag)
//��ָ���������,����·������ֵ��������
{
	Pointer pre;

	do
	{
		pre = now;

		if(now.IsOnCtrl())	//�ؼ�
		{
			if(NORMALELEC == flag) //������·
			{
				now.p3->elec	= elec;
				now.p3->elecDir	= (enum ELEC_STATE)(!dir);	//�����෴
			}
			else	//��������·
			{
				now.p3->elecDir = flag;
			}

			//����һ������
			now.SetOnLead(pre.p3->lead[dir]);
			dir = now.p1->conBody[0].IsCtrlSame(pre.p3);
		}
		else	//����,����һ������
		{
			if(NORMALELEC == flag) //������·
			{
				now.p1->elec	= elec;
				now.p1->elecDir	= (enum ELEC_STATE)(!dir);	//�����෴
			}
			else	//��������·
			{
				now.p1->elecDir = flag;
			}

			now = now.p1->conBody[dir];
			if(now.IsOnCrun())	//�����յ�(last���)����
			{
				if(now.p2 == last) break;	//�����յ�
				else //����,�൱�ڵ���
				{
					//�ҵ�������ӵ���һ������
					for(dir=0; dir<4; ++dir)
						if(now.p2->lead[dir]!=NULL && pre.p1 != now.p2->lead[dir]) break;
					//ָ��ָ�������ӵ���һ������
					pre = now;
					now.SetOnLead(pre.p2->lead[dir]);
					dir = now.p1->conBody[0].IsCrunSame(pre.p2);
				}
			}
			else if(now.IsOnLead())
			{
				throw "2������ֱ�����ӳ��ֻ�������� !";
			}
			else //now.IsOnBody
			{
				dir = now.p3->lead[0] == pre.p1;
			}
		}
	}//do
	while(!now.IsOnCrun() || now.p2 != last);	//�����յ�(last���)����
}

void Manager::TravelCircuitFindOpenBody(Pointer now, int dir)
//��ָ���������,����·�������õ�����·
//�յ�����:��·�ؼ�,������������2�Ľ��
{
	const Pointer first = now;
	Pointer pre;

	do
	{
		pre = now;

		if(now.IsOnCtrl())	//�ؼ�
		{
			//�������
			now.p3->elecDir = OPENELEC;
			now.p3->elec = 0;

			//����һ������
			now.SetOnLead(pre.p3->lead[dir]);
			if(now.p1 == NULL) break;	//��������
			dir = now.p1->conBody[0].IsCtrlSame(pre.p3);
		}
		else	//����,����һ������
		{
			//�������
			now.p1->elecDir = OPENELEC;
			now.p1->elec = 0;

			now = now.p1->conBody[dir];
			if(now.IsOnCrun())	//�����յ�(last���)����
			{
				if(2 != now.p2->GetConnectNum())	//�����յ�
				{
					break;
				}
				else //����,�൱�ڵ���
				{
					//�ҵ�������ӵ���һ������
					for(dir=0; dir<4; ++dir)
						if(now.p2->lead[dir]!=NULL && pre.p1 != now.p2->lead[dir]) break;
					//ָ��ָ�������ӵ���һ������
					pre = now;
					now.SetOnLead(pre.p2->lead[dir]);
					dir = now.p1->conBody[0].IsCrunSame(pre.p2);
				}
			}
			else if(now.IsOnLead())
			{
				throw "2������ֱ�����ӳ��ֻ��������";
			}
			else //now.IsOnBody()
			{
				dir = now.p3->lead[0] == pre.p1;
			}
		}
	}//do
	while(!now.IsBodySame(&first));	//�������յ�
}

ELEC_STATE Manager::TravelCircuitGetOrSetInfo(Pointer now, int dir, double &elec, ELEC_STATE flag)
//��ָ���������,��õ�ѹ�͵�����Ϣ,�������յ�
//ָ�����岻������·�а����Ľ��,������������ѭ��
{
	double press = 0;
	double resist = 0;
	const Pointer self = now;	//��¼�����
	Pointer pre;
	if(now.IsOnCrun()) return ERRORELEC;	//ָ�����岻������·�а����Ľ��

	do
	{
		pre = now;

		if(now.IsOnCtrl())	//�ؼ�
		{
			if(UNKNOWNELEC == flag)	//��õ�ѹ����
			{
				resist	+= now.p3->GetResist();
				press	+= now.p3->GetPress(!dir);	//�����෴
			}
			else if(NORMALELEC == flag)	//��������������Ϣ
			{
				now.p3->elecDir	= (enum ELEC_STATE)(!dir);	//�����෴
				now.p3->elec	= elec;
			}
			else	//���벻����������Ϣ
			{
				now.p3->elecDir = flag;
			}

			//����һ������
			now.SetOnLead(pre.p3->lead[dir]);
			if(now.p1 == NULL) return ERRORELEC;	//��������,��������Ǵ���
			dir = now.p1->conBody[0].IsCtrlSame(pre.p3);
		}
		else	//����,����һ������
		{
			if(NORMALELEC == flag)	//��������������Ϣ
			{
				now.p1->elecDir	= (enum ELEC_STATE)(!dir);	//�����෴
				now.p1->elec	= elec;
			}
			else if(UNKNOWNELEC != flag)	//���벻����������Ϣ
			{
				now.p1->elecDir = flag;
			}

			now = now.p1->conBody[dir];
			if(now.IsOnCrun())	//��ʱ���һ������2������,����,�൱�ڵ���
			{
				//�ҵ�������ӵ���һ������
				for(dir=0; dir<4; ++dir)
					if(now.p2->lead[dir]!=NULL && pre.p1 != now.p2->lead[dir]) break;
				//ָ��ָ�������ӵ���һ������
				pre = now;
				now.SetOnLead(pre.p2->lead[dir]);
				dir = now.p1->conBody[0].IsCrunSame(pre.p2);
			}
			else if(now.IsOnLead())
			{
				throw "2������ֱ�����ӳ��ֻ��������";
			}
			else	//now.IsOnBody
			{
				dir = now.p3->lead[0] == pre.p1;
			}
		}
	}//do
	while(!now.IsBodySame(&self));	//�������յ�

	if(UNKNOWNELEC == flag)	//��õ�ѹ����
	{
		if(!StaticClass::IsZero(resist))	//����--���費��0
		{
			elec = press/resist;
			return NORMALELEC;
		}
		else if(StaticClass::IsZero(press))	//����--�����ѹ����0
		{
			elec = 0;
			return NORMALELEC;
		}
		else	//��·
		{
			elec = 0;
			return SHORTELEC;
		}
	}

	return NORMALELEC;
}

void Manager::DistributeAnswer()
//������ĵ�������ֲ���ÿ������,�ؼ�
{
	int i;			//ѭ������
	int dir;		//��һ�������ڵ�ǰ����ķ���
	Pointer now;	//��ǰ���ʵ���·�ؼ�
	CRUN * end;		//��·���յ�
	double elec;

	//1,��ʼ��ÿ�����ߺ͵�ѧԪ����elecDir,�������ʹ��
	for(i=leadNum-1; i>=0; --i) lead[i]->elecDir = UNKNOWNELEC;
	for(i=ctrlNum-1; i>=0; --i) ctrl[i]->elecDir = UNKNOWNELEC;

	//2,��circu�Ľ���ֲ���ÿ����·�е�����
	for(i=circuNum-1; i>=0; --i)
	{
		//1,�ҵ���·�����,end����ʱ����
		end = crun[circu[i].from - crun2];
		now.SetOnLead(end->lead[circu[i].dirFrom]);

		//2,ȷ�����ҷ���,end����ʱ����
		dir = now.p1->conBody[0].IsCrunSame(end);

		//3,�ҵ���·���յ�,end����յ���ָ��
		end = crun[circu[i].to - crun2];

		//4,������·
		TravelCircuitPutElec(now, end, dir, circu[i].elec, circu[i].elecDir);
	}

	//���circu,crun2���ڴ�
	delete [] circu;
	delete [] crun2;
	circu = NULL;
	circuNum = 0;

	//3,�ҵ������ؼ�,��������Ϣ����Ϊ��·
	for(i=ctrlNum-1; i>=0; --i) if(!ctrl[i]->lead[0] && !ctrl[i]->lead[1])
	{
		ctrl[i]->elecDir = OPENELEC;
		ctrl[i]->elec = 0;
	}

	//4,�ҵ�һ�������ӵĿؼ�,�����ӵ����ж�·���������Ϣ���ú�
	//˼·:(ÿһ������������·��Ϣд��)
	//		1,�ҵ���·����
	//		2,�������յ�(�յ�����:��·�ؼ�,������������2�Ľ��)
	for(i=ctrlNum-1; i>=0; --i) if(1 == ctrl[i]->GetConnectNum())
	{
		//1,�ҵ���·�����
		now.SetOnCtrl(ctrl[i]);

		//2,ȷ�����ҷ���
		dir = now.p3->lead[1] != NULL;

		//3,������·
		TravelCircuitFindOpenBody(now, dir);
	}
	for(i=crunNum-1; i>=0; --i) if(1 == crun[i]->GetConnectNum())
	{
		//1,�ҵ���·�����
		for(dir=0;dir<4;dir++) if(crun[i]->lead[dir]) break;
		now.SetOnLead(crun[i]->lead[dir]);

		//2,ȷ�����ҷ���
		dir = now.p1->conBody[0].IsCrunSame(crun[i]);

		//3,������·
		TravelCircuitFindOpenBody(now, dir);
	}

	//5,����UNKNOWNELEC == elecDir,���ҿؼ������߶�������,����resist<0
	//	�������������ԭ��:��·���ж�·Ԫ��(���������),������·�������
	//	������·��·,�Ѷ�·��Ϣд������
	for(i=ctrlNum-1; i>=0; --i)
	{
		if(UNKNOWNELEC != ctrl[i]->elecDir || ctrl[i]->GetResist() >= 0) continue;

		//1,������·�����
		now.SetOnCtrl(ctrl[i]);

		//2,��2���������
		TravelCircuitFindOpenBody(now, 0);
		TravelCircuitFindOpenBody(now, 1);
	}

	//6,���� UNKNOWNELEC == elecDir������,��������ͷ���
	//�������������ԭ��:��·��û�нڵ�,���ɵ��ߺ͵�ѧԪ�����ӵĻ�·
	//˼· :(ÿһ����Ҫ��¼���߹��ĵ���͵�ѹ,��Ϊ��Ҫ����)
	// 1 ��������߿�ʼ��,һֱ�ҵ��Լ�ֹͣ
	// 2 ���������С,���±���һ��,����Ϣ���뵽������
	for(i=ctrlNum-1; i>=0; --i)
	{
		if(UNKNOWNELEC != ctrl[i]->elecDir) continue;

		//1,������·�����
		now.SetOnCtrl(ctrl[i]);

		//2,����߱���,��õ���͵�ѹ
		dir = TravelCircuitGetOrSetInfo(now, 0, elec, UNKNOWNELEC);

		//3,�ѽ����������
		if(ERRORELEC == dir)
		{
			throw "����������ִ���!!!";
		}
		else
		{
			if(NORMALELEC == dir && elec < 0)
			{
				//������Ϊ����,��ת��������
				elec = -elec;
				TravelCircuitGetOrSetInfo(now, 1, elec, (ELEC_STATE)dir);
			}
			else 
			{
				TravelCircuitGetOrSetInfo(now, 0, elec, (ELEC_STATE)dir);
			}
		}
	}

	//7,������·��ֻ����: ����(����) �� ����(2�����ӵ��ߵĽ��), ����������Ϊ0
	//	�������������ԭ��:��·û�г���3�����ߵĽڵ�,������뵽��·�б���
	//	��·��û�ж�·,ǰ��Ҳ�����鵽;������ֻ���ؼ�Ҳ�����鵽
	for(i=leadNum-1; i>=0; --i) if(UNKNOWNELEC == lead[i]->elecDir)
	{
		lead[i]->elecDir = LEFTELEC;
		lead[i]->elec = 0;
	}
}

void Manager::CountElec()
//���γɵ�nԪ1�η��̼��������·����ֵ
{
	int group;
	int i;
	ELEC_STATE flag;
	const double * ans;

	ClearCircuitState();	//�����·״̬��Ϣ
	CollectCircuitInfo();	//����һ�ε�·,���ÿ��Ⱥ�����·��ѧ��Ϣ
	CreateEquation();		//������·��Ϣ,��Ⱥ�齨������

	for(group=0; group<groupNum; ++group)	//��Ⱥ�����
	{
		flag = equation[group]->Count();	//���㷽��

		if(NORMALELEC == flag)	//��·����
		{
			ans = equation[group]->GetAnswer();	//��ý������ָ��
			for(i=circuNum-1; i>=0; --i) if(group == circu[i].from->group)
			{
				circu[i].elecDir = NORMALELEC;
				circu[i].elec = ans[circu[i].numInGroup];
				circu[i].ConvertWhenElecLessZero();	//����������ʱ��Ϊ����,����ת��������
			}
		}
		else	//��·���޷�ȷ������
		{
			for(i=circuNum-1; i>=0; --i) if(group == circu[i].from->group)
			{
				circu[i].elecDir = flag;
			}
		}

		maps[group].Uninit();	//ɾ��һ����·ͼ
		delete equation[group];	//ɾ��һ������
	}

	delete [] maps;		//ɾ����·ͼ����
	delete [] equation;	//ɾ����������ָ��
	DistributeAnswer();	//������ֲ���ÿ������,�������ͷ���circu��crun2
}


//10������а庯��-----------------------------------------------------------------��
void Manager::ClearClipboard()
//��ռ��а�
{
	if(clipBody.IsOnCrun())
		delete clipBody.p2;
	else if(clipBody.IsOnCtrl())
		delete clipBody.p3;
	clipBody.Clear();
}

bool Manager::GetClipboardState()
//��ȡ���а��Ƿ����
{
	return clipBody.IsOnBody();
}

void Manager::CopyToClipboard(const Pointer &body)
//����bodyָ������嵽���а�
{
	ASSERT(body.IsOnBody());
	motiNum = 0;
	ClearClipboard();	//��ռ��а�

	if(body.IsOnCrun())
		clipBody.SetOnCrun(body.p2->Clone(CLONE_FOR_CLIPBOARD), true);
	else //if(body.IsOnCtrl())
		clipBody.SetOnCtrl(body.p3->Clone(CLONE_FOR_CLIPBOARD), true);
}

Pointer Manager::CopyBody(FOCUS_OR_POS &body)
//�������嵽���а�
{
	Pointer pointer = GetBodyPointer(body);
	if(!pointer.IsOnBody()) return pointer;
	CopyToClipboard(pointer);
	return pointer;
}

void Manager::CutBody(FOCUS_OR_POS &body)
//�������嵽���а�
{
	Pointer pointer = CopyBody(body);	//��������
	if(!pointer.IsOnBody()) return;
	Delete(pointer);					//ɾ������
	PaintAll();							//�ػ��·
}

bool Manager::PasteBody(POINT pos)
//ճ������
{
	if(!clipBody.IsOnBody())
	{
		MessageBeep(0);
		return false;
	}
	dc->DPtoLP(&pos);

	if(clipBody.IsOnCrun())
	{
		if(crunNum >= MAXCRUNNUM)
		{
			wndPointer->MessageBox("��㳬���������!", "��㲻�����", MB_ICONWARNING);
			return false;
		}

		CloneCircuitBeforeChange();	//�༭ǰ���Ƶ�·
		//�༭����
		crun[crunNum] = clipBody.p2->Clone(CLONE_FOR_USE);
		crun[crunNum]->coord = pos;
		crun[crunNum]->num = crunNum;
		++ crunNum;

		PutCircuitToVector();	//���µĵ�·��Ϣ���浽����
		PaintCrun(crun[crunNum-1]);
	}
	else if(clipBody.IsOnCtrl())
	{
		if(ctrlNum >= MAXCTRLNUM)
		{
			wndPointer->MessageBox("��ѧԪ�������������!", "��ѧԪ���������", MB_ICONWARNING);
			return false;
		}

		CloneCircuitBeforeChange();	//�༭ǰ���Ƶ�·
		//�༭����
		ctrl[ctrlNum] = clipBody.p3->Clone(CLONE_FOR_USE);
		ctrl[ctrlNum]->coord = pos;
		ctrl[ctrlNum]->num = ctrlNum;
		++ ctrlNum;

		PutCircuitToVector();	//���µĵ�·��Ϣ���浽����
		PaintCtrl(ctrl[ctrlNum-1]);
	}

	return true;
}


//11��꽹�����庯��---------------------------------------------------------------��
void Manager::UpdateEditMenuState()
//���±༭�˵�״̬(MF_ENABLED or MF_GRAYED)
{
	CMenu * cm = wndPointer->GetMenu();
	UINT menuState;

	if(!focusBody.IsOnAny())
	{
		cm->EnableMenuItem(IDM_FOCUSBODY_COPY, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_CUT, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_DELETE, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_PROPERTY, MF_GRAYED);

		cm->EnableMenuItem(IDM_FOCUSBODY_CHANGECTRLSTYLE, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE1, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE2, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE3, MF_GRAYED);

		cm->EnableMenuItem(IDM_FOCUSBODY_SHOWELEC, MF_GRAYED);
	}
	else if(focusBody.IsOnLead())
	{
		cm->EnableMenuItem(IDM_FOCUSBODY_COPY, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_CUT, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_DELETE, MF_ENABLED);
		cm->EnableMenuItem(IDM_FOCUSBODY_PROPERTY, MF_ENABLED);

		cm->EnableMenuItem(IDM_FOCUSBODY_CHANGECTRLSTYLE, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE1, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE2, MF_GRAYED);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE3, MF_GRAYED);

		cm->EnableMenuItem(IDM_FOCUSBODY_SHOWELEC, MF_ENABLED);
	}
	else
	{
		cm->EnableMenuItem(IDM_FOCUSBODY_COPY, MF_ENABLED);
		cm->EnableMenuItem(IDM_FOCUSBODY_CUT, MF_ENABLED);
		cm->EnableMenuItem(IDM_FOCUSBODY_DELETE, MF_ENABLED);
		cm->EnableMenuItem(IDM_FOCUSBODY_PROPERTY, MF_ENABLED);

		if(focusBody.IsOnCtrl())
			menuState = MF_ENABLED;
		else
			menuState = MF_GRAYED;

		cm->EnableMenuItem(IDM_FOCUSBODY_CHANGECTRLSTYLE, menuState);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE1, menuState);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE2, menuState);
		cm->EnableMenuItem(IDM_FOCUSBODY_ROTATE3, menuState);
		cm->EnableMenuItem(IDM_FOCUSBODY_SHOWELEC, menuState);
	}
}

void Manager::FocusBodyClear(const Pointer * deleteBody)
//�ж�ɾ�������Ƿ��ǵ�ǰ����,������������꽹������
//���deleteBody==NULL,ֱ��ɾ������
//����ִ����:Manager,DeleteSingleBody,ClearCircuitState
{
	if(deleteBody == NULL || focusBody.IsBodySame(deleteBody))
	{
		focusBody.Clear();
		UpdateEditMenuState();
	}
}

void Manager::FocusBodySet(const Pointer &newFocus)
//���ý�������
//����ִ����:FocusBodyPaint,ReadCircuitFromVector,ReadFile
{
	ASSERT(!newFocus.IsOnConnectPos());
	focusBody = newFocus;
	UpdateEditMenuState();
}

bool Manager::FocusBodyPaint(const Pointer * newFocus)
//�������꽹�������,������ԭ���Ľ���
//���newFocus==NULL�ػ�ԭ������;���򸲸�ԭ���Ľ���,�µĽ����ý���ɫ��
{
	if(newFocus != NULL)	//����ı�
	{
		if(focusBody.IsBodySame(newFocus))
			return false;

		//ԭ���Ľ����ú�ɫ��
		if(focusBody.IsOnLead())
			PaintLead(focusBody.p1);
		if(focusBody.IsOnCrun())
			PaintCrun(focusBody.p2, false);
		else if(focusBody.IsOnCtrl())
			PaintCtrl(focusBody.p3, false);

		//�����������
		FocusBodySet(*newFocus);
	}

	if(focusBody.IsOnLead())
	{
		switch(focusLeadStyle)
		{
		case SOLID_RESERVE_COLOR:
			PaintLeadWithStyle(focusBody.p1, PS_SOLID,  RESERVE_COLOR);
			break;
		case SOLID_ORIGINAL_COLOR:
			PaintLead(focusBody.p1);
			break;
		case DOT_ORIGINAL_COLOR:
			PaintLeadWithStyle(focusBody.p1, PS_DOT, focusBody.p1->color);
			break;
		case DOT_RESERVE_COLOR:
			PaintLeadWithStyle(focusBody.p1, PS_DOT, RESERVE_COLOR);
			break;
		}
	}
	else if(focusBody.IsOnCrun())
	{
		PaintCrunWithColor(focusBody.p2, focusCrunColor);
	}
	else if(focusBody.IsOnCtrl())
	{
		PaintCtrlWithColor(focusBody.p3, focusCtrlColor);
	}

	return true;
}

void Manager::FocusBodyChangeUseTab()
//�û���Tab���л����㴦��
{
	const int bodyNum = crunNum + ctrlNum;
	Pointer newFocus;
	int num;

	if(bodyNum == 0) return;	//û������

	if(focusBody.IsOnLead())	//��ǰ�����ǵ���
	{
		num = (focusBody.p1->num + 1) % leadNum;
		newFocus.SetOnLead(lead[num]);
	}
	else if(focusBody.IsOnCrun())	//��ǰ�����ǽ��
	{
		num = (focusBody.p2->num + 1) % crunNum;
		newFocus.SetOnCrun(crun[num], true);
	}
	else if(focusBody.IsOnCtrl())	//��ǰ�����ǿؼ�
	{
		num = (focusBody.p3->num + 1) % ctrlNum;
		newFocus.SetOnCtrl(ctrl[num], true);
	}
	else	//û���趨����
	{
		if(crunNum > 0)
			newFocus.SetOnCrun(crun[0], true);
		else
			newFocus.SetOnCtrl(ctrl[0], true);
	}

	FocusBodyPaint(&newFocus);
}

bool Manager::FocusBodyMove(int dir)
//�û����������Ҽ��ƶ���������
{
	motiNum = 0;
	if(!focusBody.IsOnBody()) return false;

	POINT fromPos, toPos;

	//�����������
	if(focusBody.IsOnCrun()) fromPos = focusBody.p2->coord;
	else fromPos = focusBody.p3->coord;
	toPos = fromPos;

	//�����ƶ��������
	switch(dir)
	{
	case VK_UP:		//�����ƶ�����
		toPos.y -= moveBodySense;
		break;
	case VK_DOWN:	//�����ƶ�����
		toPos.y += moveBodySense;
		break;
	case VK_LEFT:	//�����ƶ�����
		toPos.x -= moveBodySense;
		break;
	case VK_RIGHT:	//�����ƶ�����
		toPos.x += moveBodySense;
		break;
	default:
		return false;
	}

	//��������Ƿ�Խ��
	if(toPos.x < -BODYSIZE.cx/2 || toPos.y < -BODYSIZE.cy/2) return false;

	//�ƶ�����
	PosBodyMove(&focusBody, fromPos, toPos);
	return true;
}


//12���ú���-----------------------------------------------------------------------��
void Manager::SetViewOrig(int xPos, int yPos)
//���û�ͼ�ĳ�ʼ����
{
	viewOrig.x = xPos * mouseWheelSense.cx;
	viewOrig.y = yPos * mouseWheelSense.cy;
	dc->SetViewportOrg(-viewOrig.x, -viewOrig.y);
}

void Manager::SetMoveBodySense()
//���ð������һ���ƶ�����ľ���
{
	LISTDATA list;
	char title[NAME_LEN*2];

	list.Init(1);
	list.SetAMember(DATA_STYLE_UINT, "������ƶ�����ľ���", &moveBodySense, 1, MAXMOVEBODYDIS);

	sprintf(title, "�����ȷ�Χ : 1 ~ %d", MAXMOVEBODYDIS);
	MyPropertyDlg dlg(&list, false, NULL, title, wndPointer);
	dlg.DoModal();
}

void Manager::SetLeaveOutDis()
//��������ߺϲ�����
{
	LISTDATA list;
	char title[NAME_LEN*2];

	list.Init(1);
	list.SetAMember(DATA_STYLE_UINT, "�����������ںϲ��ٽ����", &maxLeaveOutDis, 1, MAXLEAVEOUTDIS);

	sprintf(title, "�ٽ���뷶Χ : 1 ~ %d", MAXLEAVEOUTDIS);
	MyPropertyDlg dlg(&list, false, NULL, title, wndPointer);
	dlg.DoModal();
}

void Manager::SetTextColor()
//����������ɫ
{
	const enum COLOR preColor = textColor;
	LISTDATA list;
	list.Init(1);
	list.SetAEnumMember("��ǩ��ɫ", &textColor, ENUM_COLOR);

	MyPropertyDlg dlg(&list, false, NULL, "���ñ�ǩ��ɫ", wndPointer);
	dlg.DoModal();

	if(preColor != textColor)
	{
		dc->SetTextColor(LEADCOLOR[textColor]);
		PaintAll();
	}
}

void Manager::SetFocusLeadStyle()
//���ý��㵼����ʽ
{
	const enum LEADSTYLE save = focusLeadStyle;
	LISTDATA list;
	list.Init(1);
	list.SetAEnumMember("ѡ��������ʽ", &focusLeadStyle, ENUM_LEADSTYLE);

	MyPropertyDlg dlg(&list, false, NULL, "����ѡ��������ʽ", wndPointer);
	dlg.DoModal();

	if(save != focusLeadStyle && focusBody.IsOnLead())
		FocusBodyPaint(NULL);
}

void Manager::SetFocusCrunColor()
//���ý�������ɫ
{
	const enum COLOR save = focusCrunColor;
	LISTDATA list;
	list.Init(1);
	list.SetAEnumMember("ѡ�������ɫ", &focusCrunColor, ENUM_COLOR, RED, BLUE);

	MyPropertyDlg dlg(&list, false, NULL, "����ѡ�������ɫ", wndPointer);
	dlg.DoModal();

	if(save != focusCrunColor && focusBody.IsOnCrun())
		FocusBodyPaint(NULL);
}

void Manager::SetFocusCtrlColor()
//���ý���ؼ���ɫ
{
	const enum COLOR save = focusCtrlColor;
	LISTDATA list;
	list.Init(1);
	list.SetAEnumMember("ѡ����ѧԪ����ɫ", &focusCtrlColor, ENUM_COLOR, RED, BLUE);

	MyPropertyDlg dlg(&list, false, NULL, "����ѡ����ѧԪ����ɫ", wndPointer);
	dlg.DoModal();

	if(save != focusCtrlColor && focusBody.IsOnCtrl())
		FocusBodyPaint(NULL);
}


//13��ʾ���Ʋ��-----------------------------------------------------------------��
void Manager::ClearPressBody()
//�����ʾ���Ʋ�ĳ�Ա����
{
	pressStart.Clear();
	pressEnd.Clear();
	startEndPressure = 0;
}

bool Manager::SetStartBody(POINT pos)
//���ü�����Ʋ����ʼλ��
{
	motiNum = 0;
	if(!MotivateAll(pos)) return false;	//û�е������
	motiNum = 0;

	if(motiBody[0].IsOnLead())
	{
		if(StaticClass::IsElecError(motiBody[0].p1->elecDir))
		{
			wndPointer->MessageBox("��ǰѡ��ĵ�·������", "�޷�������Ʋ�", MB_ICONWARNING);
			return false;
		}
	}
	else if(motiBody[0].IsOnCrun() && !motiBody[0].IsOnConnectPos())
	{
		CRUN * c = motiBody[0].p2;
		for(int i=0; i<4; ++i) if(c->lead[i] && StaticClass::IsElecError(c->lead[i]->elecDir))
		{
			wndPointer->MessageBox("��ǰѡ��ĵ�·������", "�޷�������Ʋ�", MB_ICONWARNING);
			return false;
		}
	}
	else 
	{
		return false;	//û�е�����߻��߽ڵ�
	}

	pressStart = pressEnd = motiBody[0];
	startEndPressure = 0;

	PaintAll();
	return true;
}

bool Manager::NextBodyByInputNum(UINT nChar)
//�û���������1,2,3,4���ƶ����Ʋ��βλ��
{
	if(!pressStart.IsOnAny() || !pressEnd.IsOnAny())
	{
		AfxMessageBox("������������߻�������ѡ����Ʋ���ʼλ��,\nȻ�����������ƶ����Ʋ��βλ��.");
		return false;
	}

	int dir;
	switch(nChar)
	{
	case '#':
	case 'a':
		dir = 0; //С����'1'��
		break;

	case '(':
	case 'b':
		dir = 1; //С����'2'��
		break;

	case 34:
	case 'c':
		dir = 2; //С����'3'��
		break;

	case '%':
	case 'd':
		dir = 3; //С����'4'��
		break;

	default:
		if(nChar >= '1' && nChar <= '4')
			dir = nChar - '1';
		else
			return false;
	}

	if(pressEnd.IsOnLead())	//��βλ���ڵ�����
	{
		if(dir < 0 || dir > 1) return false;
		
		Pointer temp = pressEnd.p1->conBody[dir];
		temp.SetAtState(-1);

		if(temp.IsOnCrun())
		{
			pressEnd = temp;
		}
		else //if(temp.IsOnCtrl())
		{
			if(temp.p3->GetResist() < 0)	//��·�ؼ�
			{
				wndPointer->MessageBox("����һ����·��ѧԪ�� !", "�����޷����� !", MB_ICONINFORMATION);
				return false;
			}
			if(temp.p3->GetConnectNum() < 2)	//�ؼ�û������2�ε���
			{
				wndPointer->MessageBox("��ѧԪ����һ��û�����ӵ��� !", "�����޷����� !", MB_ICONINFORMATION);
				return false;
			}
			dir = temp.p3->lead[0] == pressEnd.p1;	//��һ����������(0��1)
			if(temp.p3->lead[dir] == pressEnd.p1) return false;	//��·��һ���ؼ�2�˶�����ͬһ�ε���
			if(temp.p3->elecDir == dir)
				startEndPressure -= temp.p3->GetResist() * temp.p3->elec;
			else
				startEndPressure += temp.p3->GetResist() * temp.p3->elec;
			startEndPressure += temp.p3->GetPress(dir);
			pressEnd.SetOnLead(temp.p3->lead[dir]);
		}
	}
	else	//��βλ���ڽ����
	{
		if(dir < 0 || dir > 3) return false;
		if(pressEnd.p2->lead[dir] != NULL)
		{
			pressEnd.SetOnLead(pressEnd.p2->lead[dir]);
		}
		else 
		{
			wndPointer->MessageBox("�����һ��û�����ӵ��� !", "�����޷����� !", MB_ICONINFORMATION);
			return false;
		}
	}

	PaintAll();
	return true;
}

bool Manager::ShowPressure()
//��ʾ����ʼλ�õ���βλ�õĵ��Ʋ�(U0-U1)
{
	if(!pressStart.IsOnAny() || !pressEnd.IsOnAny())
	{
		AfxMessageBox("��ѡ����ʼλ���ٲ鿴���Ʋ�!\n��ʼλ�ÿ����������ѡ��!");
		return false;
	}

	char note[] = "���Ʋ�";
	char name1[NAME_LEN*2], name2[NAME_LEN*2];
	GetName(pressStart, name1);
	GetName(pressEnd, name2);

	LISTDATA list;
	list.Init(3);

	if(StaticClass::IsZero(startEndPressure)) startEndPressure = 0;
	list.SetAMember(DATA_STYLE_double, note, (void *)(&startEndPressure));
	list.SetAMember(DATA_STYLE_LPCTSTR, "��ʼλ��", name1);
	list.SetAMember(DATA_STYLE_LPCTSTR, "����λ��", name2);

	MyPropertyDlg dlg(&list, true, NULL, note, wndPointer);
	dlg.DoModal();

	return true;
}


//2��������------------------------------------------------------------------------��
void Manager::PutCircuitToVector()
//����ǰ��·��Ϣ���浽����
{
	CircuitInfo ci;
	DeleteVector(vectorPos+1, circuitVector.end());

	ci.leadNum = this->leadNum;
	if(leadNum != 0)
	{
		ci.lead = new LEAD * [leadNum];
		memcpy(ci.lead, this->lead, leadNum*sizeof(void *));
	}

	ci.crunNum = this->crunNum;
	if(crunNum != 0)
	{
		ci.crun = new CRUN * [crunNum];
		memcpy(ci.crun, this->crun, crunNum*sizeof(void *));
	}

	ci.ctrlNum = this->ctrlNum;
	if(ctrlNum != 0)
	{
		ci.ctrl = new CTRL * [ctrlNum];
		memcpy(ci.ctrl, this->ctrl, ctrlNum*sizeof(void *));
	}

	ci.focusBody = this->focusBody;

	circuitVector.push_back(ci);
	if(circuitVector.size() > 200)
		DeleteVector(circuitVector.begin(), circuitVector.begin()+50);
	vectorPos = circuitVector.end() - 1;
	UpdateUnReMenuState();
}

void Manager::ReadCircuitFromVector(MyIterator it)
//��������ȡ��·��Ϣ
{
	ClearCircuitState();	//�����·״̬��Ϣ

	this->leadNum = it->leadNum;
	memcpy(this->lead, it->lead, leadNum*sizeof(void *));

	this->crunNum = it->crunNum;
	memcpy(this->crun, it->crun, crunNum*sizeof(void *));

	this->ctrlNum = it->ctrlNum;
	memcpy(this->ctrl, it->ctrl, ctrlNum*sizeof(void *));

	this->FocusBodySet(it->focusBody);

	vectorPos = it;
	UpdateUnReMenuState();
	PaintAll();
}

void Manager::DeleteVector(MyIterator first, MyIterator last)
//ɾ��������һ����������
{
	MyIterator it;
	int i;
	if(first >= circuitVector.end() || first >= last) return;

	for(it=last-1; it>=first; --it)
	{
		for(i = it->leadNum-1; i>=0; --i)
			delete it->lead[i];
		if(it->leadNum != 0)
			delete [] it->lead;

		for(i = it->crunNum-1; i>=0; --i)
			delete it->crun[i];
		if(it->crunNum != 0)
			delete [] it->crun;

		for(i = it->ctrlNum-1; i>=0; --i)
			delete it->ctrl[i];
		if(it->ctrlNum != 0)
			delete [] it->ctrl;

		circuitVector.erase(it);
	}

	vectorPos = circuitVector.end() - 1;
}

void Manager::CloneCircuitBeforeChange()
//���Ƶ�ǰ��·������
{
	int i, j;
	MyIterator it = vectorPos;
	DeleteVector(it+1, circuitVector.end());

	//��������
	for(i=leadNum-1; i>=0; --i)
		it->lead[i] = this->lead[i]->Clone(CLONE_FOR_SAVE);

	for(i=crunNum-1; i>=0; --i)
		it->crun[i] = this->crun[i]->Clone(CLONE_FOR_SAVE);

	for(i=ctrlNum-1; i>=0; --i)
		it->ctrl[i] = this->ctrl[i]->Clone(CLONE_FOR_SAVE);

	//����ָ��
	for(i=leadNum-1; i>=0; --i)
	{
		for(j=0; j<2; ++j)
		{
			if(this->lead[i]->conBody[j].IsOnCrun())
			{
				it->lead[i]->conBody[j].SetOnCrun(it->crun[ this->lead[i]->conBody[j].p2->num ]);
			}
			else if(this->lead[i]->conBody[j].IsOnCtrl())
			{
				it->lead[i]->conBody[j].SetOnCtrl(it->ctrl[ this->lead[i]->conBody[j].p3->num ]);
			}
			else
			{
				it->lead[i]->conBody[j].Clear();
			}
			it->lead[i]->conBody[j].SetAtState(this->lead[i]->conBody[j].GetAtState());
		}
	}

	for(i=crunNum-1; i>=0; --i)
	{
		for(j=0; j<4; ++j)
		{
			if(this->crun[i]->lead[j] != NULL)
				it->crun[i]->lead[j] = it->lead[this->crun[i]->lead[j]->num];
			else
				it->crun[i]->lead[j] = NULL;
		}
	}

	for(i=ctrlNum-1; i>=0; --i)
	{
		for(j=0; j<2; ++j)
		{
			if(this->ctrl[i]->lead[j] != NULL)
				it->ctrl[i]->lead[j] = it->lead[this->ctrl[i]->lead[j]->num];
			else
				it->ctrl[i]->lead[j] = NULL;
		}
	}

	//���ƽ���
	if(this->focusBody.IsOnLead())
	{
		it->focusBody.SetOnLead(it->lead[this->focusBody.p1->num]);
		it->focusBody.SetAtState(this->focusBody.GetAtState());
	}
	else if(this->focusBody.IsOnCrun())
	{
		it->focusBody.SetOnCrun(it->crun[this->focusBody.p2->num]);
	}
	else if(this->focusBody.IsOnCtrl())
	{
		it->focusBody.SetOnCtrl(it->ctrl[this->focusBody.p3->num]);
	}
	else
	{
		it->focusBody.Clear();
	}

	vectorPos = circuitVector.end() - 1;
}

void Manager::UpdateUnReMenuState()
//���³������ظ��˵�״̬
{
	CMenu * cm = wndPointer->GetMenu();
	if(vectorPos > circuitVector.begin())
		cm->EnableMenuItem(IDM_UNDO, MF_ENABLED);
	else
		cm->EnableMenuItem(IDM_UNDO, MF_GRAYED);
	if(vectorPos < circuitVector.end()-1)
		cm->EnableMenuItem(IDM_REDO, MF_ENABLED);
	else
		cm->EnableMenuItem(IDM_REDO, MF_GRAYED);
}

void Manager::UnDo()
//����
{
	if(vectorPos <= circuitVector.begin())
	{
		MessageBeep(0);
		return;
	}
	ReadCircuitFromVector(--vectorPos);
}

void Manager::ReDo()
//�ظ�
{
	if(vectorPos >= circuitVector.end()-1)
	{
		MessageBeep(0);
		return;
	}
	ReadCircuitFromVector(++vectorPos);
}


//9���Ժ���------------------------------------------------------------------------��
void Manager::SaveCircuitInfoToTextFile()
//�����·��Ϣ���ı��ļ�,���Ժ���
{
	int i;
	FILE * fp = fopen("D:\\data.txt", "w");
	if(fp == NULL) return;

	fprintf(fp, "cruns:[\n");
	for(i=0; i<crunNum; i++)
	{
		fprintf(fp, "{id:%d,x:%d,y:%d,", crun[i]->GetInitOrder(), crun[i]->coord.x, crun[i]->coord.y);
		fprintf(fp, "name:\"%s\",lead:[", crun[i]->name);
		for(int j=0; j<4; j++)
		{
			if(crun[i]->lead[j]) fprintf(fp, "%d", crun[i]->lead[j]->GetInitOrder());
			else fprintf(fp, "null");
			if (j!=4-1) fprintf(fp, ",");
		}
		fprintf(fp, "]}\n");

		if (i != crunNum-1) fprintf(fp, ",");
	}
	fprintf(fp, "],\n");

	fprintf(fp, "leads:[\n");
	for(i=0; i<leadNum; ++i)
	{
		fprintf(fp, "{id:%d,", (int)lead[i]->GetInitOrder());
		lead[i]->SaveToTextFile(fp);
		fprintf(fp, "color:%d,", (int)lead[i]->color);
		fprintf(fp, "conBody[");	lead[i]->conBody[0].SaveToTextFile(fp);
		fprintf(fp, ",");	lead[i]->conBody[1].SaveToTextFile(fp);
		fprintf(fp, "]}\n");

		if (i != leadNum-1) fprintf(fp, ",");
	}
	fprintf(fp, "],\n");

	fprintf(fp, "ctrls:[\n", ctrlNum);
	const char * ctrlStyleStr[] = {"source","resistance","bulb","capa","switch"}; 
	for(i=0; i<ctrlNum; ++i)
	{
		fprintf(fp, "{id:%d,x:%d,y:%d,", ctrl[i]->GetInitOrder(), ctrl[i]->coord.x, ctrl[i]->coord.y);
		fprintf(fp, "name:\"%s\",lead:[", ctrl[i]->name);
		if(ctrl[i]->lead[0])fprintf(fp, "%d,", ctrl[i]->lead[0]->GetInitOrder());
		else fputs("-1,", fp);
		if(ctrl[i]->lead[1])fprintf(fp, "%d],", ctrl[i]->lead[1]->GetInitOrder());
		else fputs("-1],", fp);

		ctrl[i]->SaveToTextFile(fp);

		fprintf(fp, "style:\"%s\"}", ctrlStyleStr[ctrl[i]->GetStyle()]);
		
		if (i != leadNum-1) fprintf(fp, ",");
	}
	fprintf(fp, "]\n}\n");

	fclose(fp);
}

void Manager::SaveCountInfoToTextFile()
//���������̵��ı��ļ�,���Ժ���
{
	FILE * fp = fopen("D:\\Circuit.txt", "w");
	int i, j, group, ijPos, tempDir;

	if(fp == NULL) return;

	CollectCircuitInfo();

	for(i=0; i<crunNum; ++i)
	{
		fprintf(fp, "crun[%d]:\n", i);
		fprintf(fp, "\tgroup = %d\n", crun2[i].group);
		//fprintf(fp, "\tgroup = %f\n", crun2[i].potential);
		for(j=0;j<4;j++)
		{
			if(crun2[i].c[j])fprintf(fp, "\tcircuit[%d] = %d\n", j, crun2[i].c[j]->eleNum);
			else fprintf(fp, "\tcircuit[%d] = NULL\n", j);
		}
	}

	fputc('\n', fp);

	for(i=0; i<circuNum; ++i)
	{
		fprintf(fp, "circu[%d]:\n", i);
		fprintf(fp, "\tnum in group = %d\n", circu[i].numInGroup);
		fprintf(fp, "\tfromcrun = %d\n", circu[i].from-crun2);
		fprintf(fp, "\tfromdir = %d\n", circu[i].dirFrom);
		fprintf(fp, "\ttocrun = %d\n", circu[i].to-crun2);
		fprintf(fp, "\ttodir = %d\n", circu[i].dirTo);
		//fprintf(fp, "\telectic = %f\n", circu[i].elec);
		fprintf(fp, "\tpressure = %f\n", circu[i].pressure);
		fprintf(fp, "\tresistance = %f\n", circu[i].resistance);
	}

	fclose(fp);

	//////////////////////
	CreateEquation();
	CRUNMAP * maps = this->maps;
	fp = fopen("D:\\Map.txt", "w");
	if(fp == NULL) return;

	for(group=0; group<groupNum; ++group) for(i=maps[group].size-2; i>=0; --i) for(j=maps[group].size-1; j>i; --j)
	{
		ijPos = CONVERT(i, j, maps[group].size);
		tempDir = maps[group].direct[ijPos];
	
		fprintf(fp, "%d Direct Connections ", tempDir);

		fprintf(fp, " between %3d and %3d \n", maps[group].crunTOorder[i], maps[group].crunTOorder[j]);
	}

	delete [] crun2;
	delete [] circu;
	circu = NULL;
	circuNum = 0;
	fclose(fp);

	fp = fopen("D:\\Equation.txt", "w");
	if(fp == NULL) return;
	for(group=0; group<groupNum; ++group)
	{
		fprintf(fp, "\ngroup[%d]------------\n", group);
		//equation[group]->Simple_Equation();
		equation[group]->OutputToFile(fp);
	}
	fclose(fp);

	for(group=0; group<groupNum; ++group)
	{
		maps[group].Uninit();
		delete equation[group];
	}
	delete [] maps;
	delete [] equation;
}
