
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
	motiCount = 0;
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
	ctx->DPtoLP(&pos);

	if(clipBody.IsOnCrun())
	{
		if(crunCount >= MAX_CRUN_COUNT)
		{
			wndPointer->MessageBox("��㳬���������!", "��㲻�����", MB_ICONWARNING);
			return false;
		}

		CloneCircuitBeforeChange();	//�༭ǰ���Ƶ�·
		//�༭����
		crun[crunCount] = clipBody.p2->Clone(CLONE_FOR_USE);
		crun[crunCount]->coord = pos;
		crun[crunCount]->num = crunCount;
		++ crunCount;

		PutCircuitToVector();	//���µĵ�·��Ϣ���浽����
		PaintCrun(crun[crunCount-1]);
	}
	else if(clipBody.IsOnCtrl())
	{
		if(ctrlCount >= MAX_CTRL_COUNT)
		{
			wndPointer->MessageBox("��ѧԪ�������������!", "��ѧԪ���������", MB_ICONWARNING);
			return false;
		}

		CloneCircuitBeforeChange();	//�༭ǰ���Ƶ�·
		//�༭����
		ctrl[ctrlCount] = clipBody.p3->Clone(CLONE_FOR_USE);
		ctrl[ctrlCount]->coord = pos;
		ctrl[ctrlCount]->num = ctrlCount;
		++ ctrlCount;

		PutCircuitToVector();	//���µĵ�·��Ϣ���浽����
		PaintCtrl(ctrl[ctrlCount-1]);
	}

	return true;
}
