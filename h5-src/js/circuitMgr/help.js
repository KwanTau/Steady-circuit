
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
