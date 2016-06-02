
//12���ú���-----------------------------------------------------------------------��
void Manager::SetViewOrig(int xPos, int yPos)
//���û�ͼ�ĳ�ʼ����
{
	viewOrig.x = xPos * mouseWheelSense.cx;
	viewOrig.y = yPos * mouseWheelSense.cy;
	ctx->SetViewportOrg(-viewOrig.x, -viewOrig.y);
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
		ctx->SetTextColor(LEADCOLOR[textColor]);
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
