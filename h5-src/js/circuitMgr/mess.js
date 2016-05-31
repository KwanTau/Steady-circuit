
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
