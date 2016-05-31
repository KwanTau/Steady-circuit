
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
//��ָ��λ�ø�������
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
