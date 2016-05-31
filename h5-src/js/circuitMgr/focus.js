
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
