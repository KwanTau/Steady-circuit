
//6�����Ϣ������----------------------------------------------------------------��

bool Manager::MotivateAll(POINT &pos)
//�����������,�����Ƿ���������,����������ӵ���
{
	Pointer * mouse = motiBody + motiCount;
	int i;

	//1,��ʼ��-------------------------------------------
	ASSERT(motiCount >= 0 && motiCount < 2);
	ctx->DPtoLP(&pos);
	mouse->Clear();

	//2,������ʲô������---------------------------------
	for(i = crunCount-1; i >= 0; --i)	//�������н��
	{
		mouse->SetAtState(crun[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnCrun(crun[i]);
			++motiCount;
			goto testPlace;
		}
	}
	for(i = leadCount-1; i >= 0; --i)	//�������е���
	{
		mouse->SetAtState(lead[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnLead(lead[i], false);
			++motiCount;
			goto testPlace;
		}
	}
	for(i = ctrlCount-1; i >= 0; --i)	//�������пؼ�
	{
		mouse->SetAtState(ctrl[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnCtrl(ctrl[i]);
			++motiCount;
			goto testPlace;
		}
	}

	return false;	//���е�����һ��û�м�������

testPlace:

	//3,ȥ������Ҫ��ʾ���ӵĲ���-------------------------
	if( 2 == motiCount		//ͬһ������������ӵ㲻����ʾ����
		&& motiBody[0].IsOnConnectPos() 
		&& motiBody[1].IsOnConnectPos()
		&& motiBody[0].IsBodySame(motiBody+1))	
	{
		--motiCount;
		return false;
	}
	else if(2 == motiCount	//���������
		&& motiBody[0].IsOnConnectPos()
		&& !motiBody[1].IsOnConnectPos() 
		&& !motiBody[1].IsOnLead())
	{
		--motiCount;
		return false;
	}

	return true;
}

bool Manager::LButtonDown(POINT pos)
//����WM_LBUTTONDOWN��Ϣ
{
	if(!isUpRecvAfterDown) motiCount = 0;		//���ϴ����������º�û�н��ܵ���갴����Ϣ
	lButtonDownState = MotivateAll(pos);	//��¼�������Ƿ���������
	lButtonDownPos = pos;					//��¼���������µ�����
	isUpRecvAfterDown = false;				//�յ���갴����Ϣ������Ϊtrue
	lastMoveOnPos.x = -100;					//��ԭ��������,����ƶ���������

	if(!lButtonDownState) //δ�����Ч��λ,����������,�������ӵ���
	{
		if(motiCount > 0 && motiBody[motiCount-1].IsOnConnectPos())
			PaintAll();	//����ShowAddLead���ĵ���

		motiCount = 0; return false;
	}
	else if(!motiBody[motiCount-1].IsOnConnectPos())	//����������ӵ�
	{
		FocusBodyPaint(motiBody+motiCount-1);	//�ػ潹������
	}

	if(2 == motiCount && motiBody[0].IsOnConnectPos())	//�жϵ�һ��ѡ�����Ƿ������ӵ�
	{
		if(motiBody[1].IsOnConnectPos())
		{
			CloneCircuitBeforeChange();			//�༭ǰ���Ƶ�·
			AddLead(motiBody[0], motiBody[1]);	//�༭����
			PutCircuitToVector();				//���µĵ�·��Ϣ���浽����
		}
		else if(motiBody[1].IsOnLead())
		{
			ConnectBodyLead(pos);
		}

		motiCount = 0; return true;
	}

	//AddLead������dira=1~4,dirb=1~4����Ϣ;ConnectBodyLead(pos)������dira=1~4,dirb=-2~-3,...����Ϣ.
	//dira=1~4,dirb=-1����Ϣ��������,����ˢ��
	//dira=-1,-2,-3,...����Ϣ���ε�(��Ϊ�����ֵ��������)
	//Ҳ����LButtonUpֻ�ܴ���1==motiCount,dira=-1,-2,-3,...����Ϣ;

	if(2 == motiCount) motiCount = 0;
	return false;
}

bool Manager::LButtonUp(POINT pos)
//�����������������Ϣ
{
	isUpRecvAfterDown = true;						//��갴�º��յ���갴����Ϣ
	if(!lButtonDownState || !motiCount) return false;	//û�е������
	ctx->DPtoLP(&pos);
	Pointer * body = motiBody + motiCount - 1;

	//������ºͰ����������ͬ,���ҵ���Ĳ������ӵ�
	if( lButtonDownPos.x == pos.x && lButtonDownPos.y == pos.y 
		&& !body->IsOnConnectPos())
	{
		if(body->IsOnCtrl())
			body->p3->SwitchOnOff();	//���ؿ�������ı�
		FocusBodyPaint(NULL);			//�ػ潹��

		motiCount = 0;
		return false;
	}

	if(body->IsOnLead())	//�ƶ�����
	{
		body->p1->Move(body->GetAtState(), pos, maxLeaveOutDis);
		motiCount = 0;
		return true;
	}
	else if(body->IsOnBody())	//�ƶ������������
	{
		if(StaticClass::IsCtrlDown())	//�����Ctrl�����¸�������
			PosBodyClone(body, lButtonDownPos, pos);
		else
			PosBodyMove(body, lButtonDownPos, pos);
		motiCount = 0;
		return true;
	}
	else if(!body->IsOnConnectPos())
	{
		motiCount = 0;
		return false;
	}

	return false;
}

void Manager::MouseMove(POINT pos, bool isLButtonDown)
//����ƶ���Ϣ����
{
	if(ShowAddBody(pos)) return;					//������������ʾ
	if(ShowMoveBody(pos, isLButtonDown)) return;	//�ƶ����������ʾ
	if(ShowMoveLead(isLButtonDown)) return;			//�ƶ����߹�����ʾ
	ShowAddLead(pos);								//���ӵ��߹�����ʾ

	//��꼤��������ʾ
	if(MotivateAll(pos))	//��꼤��������
	{
		--motiCount;
		PaintMouseMotivate(motiBody[motiCount]);
	}
	else					//���û�м�������
	{
		motiBody[1].Clear();
		PaintMouseMotivate(motiBody[1]);
	}
}
