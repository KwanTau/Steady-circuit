
//5�༭����------------------------------------------------------------------------��
void Manager::AddCtrl(POINT pos, BODY_TYPE style)
//��ӿؼ�
{
	ASSERT(ctrlCount < MAX_CTRL_COUNT);

	ctrl[ctrlCount] = new CTRL(ctrlCount, pos, style);
	++ ctrlCount;

	PaintCtrlText(ctrl[ctrlCount-1]);
	Pointer newFocus;
	newFocus.SetOnCtrl(ctrl[ctrlCount-1], 1);
	FocusBodyPaint(&newFocus);
}

void Manager::AddCrun(POINT pos)
//��ӽ��
{
	ASSERT(crunCount < MAX_CRUN_COUNT);

	crun[crunCount] = new CRUN(crunCount, pos);
	++ crunCount;

	PaintCrunText(crun[crunCount-1]);
	Pointer newFocus;
	newFocus.SetOnCrun(crun[crunCount-1], 1);
	FocusBodyPaint(&newFocus);
}

void Manager::AddLead(Pointer a, Pointer b)
//�õ�������2������
{
	ASSERT(leadCount < MAX_LEAD_COUNT);						//���߹���
	ASSERT(a.IsOnConnectPos() && b.IsOnConnectPos());	//���ӵ�
	ASSERT(!a.IsBodySame(&b));							//����ͬһ������

	//��ӵ���
	lead[leadCount] = new LEAD(leadCount, a, b);
	++leadCount;

	//��������ָ����
	if(a.IsOnCrun())
		a.p2->lead[a.GetLeadNum()] = lead[leadCount-1];
	else 
		a.p3->lead[a.GetLeadNum()] = lead[leadCount-1];
	if(b.IsOnCrun())
		b.p2->lead[b.GetLeadNum()] = lead[leadCount-1];
	else 
		b.p3->lead[b.GetLeadNum()] = lead[leadCount-1];

	//��ʾ��ӵĵ���
	PaintLead(lead[leadCount-1]);
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
	if(num != leadCount-1)
	{
		lead[num] = lead[leadCount-1];
		lead[num]->num = num;
	}
	lead[leadCount-1] = NULL;
	--leadCount;
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
		if(num != crunCount-1)
		{
			crun[num] = crun[crunCount-1];
			crun[num]->num = num;
		}
		crun[crunCount-1] = NULL;
		--crunCount;
	}
	else //if(pointer.IsOnCtrl())
	{
		num = pointer.p3->num;
		delete pointer.p3;
		if(num != ctrlCount-1)
		{
			ctrl[num] = ctrl[ctrlCount-1];
			ctrl[num]->num = num;
		}
		ctrl[ctrlCount-1] = NULL;
		--ctrlCount;
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
	ASSERT(motiCount == 2 && motiBody[0].IsOnConnectPos() && motiBody[1].IsOnLead());
	motiCount = 0;
	if(crunCount >= MAX_CRUN_COUNT)	//ֻҪ���������,����һ����
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

	newCrun.SetOnCrun(crun[crunCount-1]);	//newCrunָ������ӽ��

	newCrun.SetAtState(dir1);
	AddLead(x, newCrun);	//x�ͽڵ�����,x�����,�½ڵ����յ�
	lead[leadCount-1]->ReplacePos(newLeadPosx);	//���껹ԭ

	newCrun.SetAtState(dir2);
	AddLead(newCrun, y);	//y�ͽڵ�����,y���յ�,�½ڵ������
	lead[leadCount-1]->ReplacePos(newLeadPosy);	//���껹ԭ

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
