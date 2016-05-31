
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
