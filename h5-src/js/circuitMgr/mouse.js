
//6�����Ϣ������----------------------------------------------------------------��

bool Manager::MotivateAll(POINT &pos)
//�����������,�����Ƿ���������,����������ӵ���
{
	Pointer * mouse = motiBody + motiNum;
	int i;

	//1,��ʼ��-------------------------------------------
	ASSERT(motiNum >= 0 && motiNum < 2);
	dc->DPtoLP(&pos);
	mouse->Clear();

	//2,������ʲô������---------------------------------
	for(i = crunNum-1; i >= 0; --i)	//�������н��
	{
		mouse->SetAtState(crun[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnCrun(crun[i]);
			++motiNum;
			goto testPlace;
		}
	}
	for(i = leadNum-1; i >= 0; --i)	//�������е���
	{
		mouse->SetAtState(lead[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnLead(lead[i], false);
			++motiNum;
			goto testPlace;
		}
	}
	for(i = ctrlNum-1; i >= 0; --i)	//�������пؼ�
	{
		mouse->SetAtState(ctrl[i]->At(pos));
		if(mouse->IsOnAny())
		{
			mouse->SetOnCtrl(ctrl[i]);
			++motiNum;
			goto testPlace;
		}
	}

	return false;	//���е�����һ��û�м�������

testPlace:

	//3,ȥ������Ҫ��ʾ���ӵĲ���-------------------------
	if( 2 == motiNum		//ͬһ������������ӵ㲻����ʾ����
		&& motiBody[0].IsOnConnectPos() 
		&& motiBody[1].IsOnConnectPos()
		&& motiBody[0].IsBodySame(motiBody+1))	
	{
		--motiNum;
		return false;
	}
	else if(2 == motiNum	//���������
		&& motiBody[0].IsOnConnectPos()
		&& !motiBody[1].IsOnConnectPos() 
		&& !motiBody[1].IsOnLead())
	{
		--motiNum;
		return false;
	}

	return true;
}

bool Manager::LButtonDown(POINT pos)
//����WM_LBUTTONDOWN��Ϣ
{
	if(!isUpRecvAfterDown) motiNum = 0;		//���ϴ����������º�û�н��ܵ���갴����Ϣ
	lButtonDownState = MotivateAll(pos);	//��¼�������Ƿ���������
	lButtonDownPos = pos;					//��¼���������µ�����
	isUpRecvAfterDown = false;				//�յ���갴����Ϣ������Ϊtrue
	lastMoveOnPos.x = -100;					//��ԭ��������,����ƶ���������

	if(!lButtonDownState) //δ�����Ч��λ,����������,�������ӵ���
	{
		if(motiNum > 0 && motiBody[motiNum-1].IsOnConnectPos())
			PaintAll();	//����ShowAddLead���ĵ���

		motiNum = 0; return false;
	}
	else if(!motiBody[motiNum-1].IsOnConnectPos())	//����������ӵ�
	{
		FocusBodyPaint(motiBody+motiNum-1);	//�ػ潹������
	}

	if(2 == motiNum && motiBody[0].IsOnConnectPos())	//�жϵ�һ��ѡ�����Ƿ������ӵ�
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

		motiNum = 0; return true;
	}

	//AddLead������dira=1~4,dirb=1~4����Ϣ;ConnectBodyLead(pos)������dira=1~4,dirb=-2~-3,...����Ϣ.
	//dira=1~4,dirb=-1����Ϣ��������,����ˢ��
	//dira=-1,-2,-3,...����Ϣ���ε�(��Ϊ�����ֵ��������)
	//Ҳ����LButtonUpֻ�ܴ���1==motiNum,dira=-1,-2,-3,...����Ϣ;

	if(2 == motiNum) motiNum = 0;
	return false;
}

bool Manager::LButtonUp(POINT pos)
//�����������������Ϣ
{
	isUpRecvAfterDown = true;						//��갴�º��յ���갴����Ϣ
	if(!lButtonDownState || !motiNum) return false;	//û�е������
	dc->DPtoLP(&pos);
	Pointer * body = motiBody + motiNum - 1;

	//������ºͰ����������ͬ,���ҵ���Ĳ������ӵ�
	if( lButtonDownPos.x == pos.x && lButtonDownPos.y == pos.y 
		&& !body->IsOnConnectPos())
	{
		if(body->IsOnCtrl())
			body->p3->SwitchOnOff();	//���ؿ�������ı�
		FocusBodyPaint(NULL);			//�ػ潹��

		motiNum = 0;
		return false;
	}

	if(body->IsOnLead())	//�ƶ�����
	{
		body->p1->Move(body->GetAtState(), pos, maxLeaveOutDis);
		motiNum = 0;
		return true;
	}
	else if(body->IsOnBody())	//�ƶ������������
	{
		if(StaticClass::IsCtrlDown())	//�����Ctrl�����¸�������
			PosBodyClone(body, lButtonDownPos, pos);
		else
			PosBodyMove(body, lButtonDownPos, pos);
		motiNum = 0;
		return true;
	}
	else if(!body->IsOnConnectPos())
	{
		motiNum = 0;
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
		--motiNum;
		PaintMouseMotivate(motiBody[motiNum]);
	}
	else					//���û�м�������
	{
		motiBody[1].Clear();
		PaintMouseMotivate(motiBody[1]);
	}
}
