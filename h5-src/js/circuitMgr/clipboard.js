
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
