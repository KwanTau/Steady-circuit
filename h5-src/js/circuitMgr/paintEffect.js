
//���ӵ��߹�����ʾ
Manager.ShowAddLead = function(pos) {
	if(1 != motiCount) return false;

	Pointer * body = motiBody;
	POINT firstPos;

	if(!body.IsOnConnectPos()) return false;

	PaintAll();		//��ˢ��
	motiCount = 1;	//��ԭ����

	//dc�ƶ������
	Manager.ctx.DPtoLP(&pos);
	Manager.ctx.MoveTo(pos);

	//���ú�ɫ����
	Manager.ctx.SelectStockObject(BLACK_PEN);

	//��ֱ��
	body.GetPosFromBody(firstPos);
	Manager.ctx.LineTo(firstPos);

	return true;
};

bool Manager::ShowAddBody(POINT point)
//������������ʾ
{
	if(addState == BODY_CRUN)
	{
		if(lastMoveOnPos.x > -100)
			Manager.ctx.BitBlt(lastMoveOnPos.x-DD, lastMoveOnPos.y-DD, DD*2, DD*2, &crunImageData, 0, 0, SRCINVERT);
		Manager.ctx.DPtoLP(&point);
		lastMoveOnPos = point;
		Manager.ctx.BitBlt(lastMoveOnPos.x-DD, lastMoveOnPos.y-DD, DD*2, DD*2, &crunImageData, 0, 0, SRCINVERT);

		::SetCursor(hcAddCrun);
		return true;
	}
	else if(Pointer::IsCtrl(addState))
	{
		var tempImage = Manager.ctrlImageList[addState*4];
		if(lastMoveOnPos.x > -100)
			Manager.ctx.BitBlt(lastMoveOnPos.x, lastMoveOnPos.y, CTRL_SIZE.cx, CTRL_SIZE.cy, tempImage, 0, 0, SRCINVERT);
		Manager.ctx.DPtoLP(&point);
		lastMoveOnPos = point;
		Manager.ctx.BitBlt(lastMoveOnPos.x, lastMoveOnPos.y, CTRL_SIZE.cx, CTRL_SIZE.cy, tempImage, 0, 0, SRCINVERT);

		::SetCursor(null);
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
	ASSERT(motiCount >= 0 && motiCount <= 2);
	if(motiCount == 0) return false;

	Pointer * body = motiBody + motiCount - 1;
	POINT bodyPos = {0, 0};

	if(!body.IsOnBody()) return false;
	if(!isLButtonDown)	//���û�а���
	{
		PaintAll(); 
		return false;
	}

	//�����������
	Manager.ctx.DPtoLP(&pos);
	if(body.IsOnCrun()) bodyPos = body.p2.coord;
	else if(body.IsOnCtrl()) bodyPos = body.p3.coord;

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
	ASSERT(motiCount>=0 && motiCount<=2);

	if(motiCount == 0 || !motiBody[motiCount-1].IsOnLead())
	{
		return false;
	}
	if(!isLButtonDown)	//���û�а���
	{
		PaintAll();
		return true;
	}

	if(motiBody[motiCount-1].IsOnHoriLead())
		SetCursor(hcMoveHorz);	//�ں���,�����"����ָ��"
	else 
		SetCursor(hcMoveVert);	//������,�����"����ָ��"

	return true;
}


BODY_TYPE Manager::PosBodyPaintRect(POINT pos)
//ͻ���һ�����
{
	Pointer * body = motiBody; //&motiBody[0]

	motiCount = 0;
	MotivateAll(pos);
	motiCount = 0;

	if(!body.IsOnAny()) return BODY_NO;

	if(body.IsOnConnectPos()) body.SetAtState(-1);

	if(body.IsOnBody()) Manager.ctx.SelectObject(hp + BLUE);

	if(body.IsOnCrun())
	{
		Manager.ctx.Rectangle(body.p2.coord.x-DD-2, body.p2.coord.y-DD-2, 
			body.p2.coord.x+DD+2, body.p2.coord.y+DD+2);
	}
	else if(body.IsOnCtrl())
	{
		Manager.ctx.Rectangle(body.p3.coord.x-2, body.p3.coord.y-2, 
			body.p3.coord.x+CTRL_SIZE.cx+2, body.p3.coord.y+CTRL_SIZE.cy+2);
	}

	PaintWithSpecialColorAndRect(*body, false);
	return body.GetStyle();
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
	CDC * model = null;			//property��ʾ�����ʾ��
	LISTDATA list;				//property��ʾ������

	//1,��õ�����Ϣ
	if(pointer.IsOnLead())
	{
		elec = pointer.p1.elec;
		elecDir  = pointer.p1.elecDir;
	}
	else //if(pointer.IsOnCtrl())
	{
		elec = pointer.p3.elec;
		elecDir  = pointer.p3.elecDir;

		model = GetCtrlPaintImage(pointer.p3);	//ʾ��
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
			GetName(pointer.p1.conBody[LEFTELEC != elecDir], tempStr1);
			GetName(pointer.p1.conBody[LEFTELEC == elecDir], tempStr2);

			list.Init(3);
			list.SetAMember(DATA_STYLE_double, DATA_NOTE[DATA_NOTE_CURRENT], &elec);
			list.SetAMember(DATA_STYLE_LPCTSTR, "������� :", tempStr1);
			list.SetAMember(DATA_STYLE_LPCTSTR, "�����յ� :", tempStr2);
		}
		else //if(pointer.IsOnCtrl())
		{
			switch(pointer.p3.dir ^ ((RIGHTELEC == elecDir)<<1))
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
	PaintWithSpecialColorAndRect(pointer, false);
	MyPropertyDlg dlg(&list, true, model, title, this.canvas);
	dlg.DoModal();

	return true;
}
