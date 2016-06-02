
//13��ʾ���Ʋ��-----------------------------------------------------------------��
void Manager::ClearPressBody()
//�����ʾ���Ʋ�ĳ�Ա����
{
	pressStart.Clear();
	pressEnd.Clear();
	startEndPressure = 0;
}

bool Manager::SetStartBody(POINT pos)
//���ü�����Ʋ����ʼλ��
{
	motiCount = 0;
	if(!MotivateAll(pos)) return false;	//û�е������
	motiCount = 0;

	if(motiBody[0].IsOnLead())
	{
		if(StaticClass::IsElecError(motiBody[0].p1->elecDir))
		{
			wndPointer->MessageBox("��ǰѡ��ĵ�·������", "�޷�������Ʋ�", MB_ICONWARNING);
			return false;
		}
	}
	else if(motiBody[0].IsOnCrun() && !motiBody[0].IsOnConnectPos())
	{
		CRUN * c = motiBody[0].p2;
		for(int i=0; i<4; ++i) if(c->lead[i] && StaticClass::IsElecError(c->lead[i]->elecDir))
		{
			wndPointer->MessageBox("��ǰѡ��ĵ�·������", "�޷�������Ʋ�", MB_ICONWARNING);
			return false;
		}
	}
	else 
	{
		return false;	//û�е�����߻��߽ڵ�
	}

	pressStart = pressEnd = motiBody[0];
	startEndPressure = 0;

	PaintAll();
	return true;
}

bool Manager::NextBodyByInputNum(UINT nChar)
//�û���������1,2,3,4���ƶ����Ʋ��βλ��
{
	if(!pressStart.IsOnAny() || !pressEnd.IsOnAny())
	{
		AfxMessageBox("������������߻�������ѡ����Ʋ���ʼλ��,\nȻ�����������ƶ����Ʋ��βλ��.");
		return false;
	}

	int dir;
	switch(nChar)
	{
	case '#':
	case 'a':
		dir = 0; //С����'1'��
		break;

	case '(':
	case 'b':
		dir = 1; //С����'2'��
		break;

	case 34:
	case 'c':
		dir = 2; //С����'3'��
		break;

	case '%':
	case 'd':
		dir = 3; //С����'4'��
		break;

	default:
		if(nChar >= '1' && nChar <= '4')
			dir = nChar - '1';
		else
			return false;
	}

	if(pressEnd.IsOnLead())	//��βλ���ڵ�����
	{
		if(dir < 0 || dir > 1) return false;
		
		Pointer temp = pressEnd.p1->conBody[dir];
		temp.SetAtState(-1);

		if(temp.IsOnCrun())
		{
			pressEnd = temp;
		}
		else //if(temp.IsOnCtrl())
		{
			if(temp.p3->GetResist() < 0)	//��·�ؼ�
			{
				wndPointer->MessageBox("����һ����·��ѧԪ�� !", "�����޷����� !", MB_ICONINFORMATION);
				return false;
			}
			if(temp.p3->GetConnectNum() < 2)	//�ؼ�û������2�ε���
			{
				wndPointer->MessageBox("��ѧԪ����һ��û�����ӵ��� !", "�����޷����� !", MB_ICONINFORMATION);
				return false;
			}
			dir = temp.p3->lead[0] == pressEnd.p1;	//��һ����������(0��1)
			if(temp.p3->lead[dir] == pressEnd.p1) return false;	//��·��һ���ؼ�2�˶�����ͬһ�ε���
			if(temp.p3->elecDir == dir)
				startEndPressure -= temp.p3->GetResist() * temp.p3->elec;
			else
				startEndPressure += temp.p3->GetResist() * temp.p3->elec;
			startEndPressure += temp.p3->GetPress(dir);
			pressEnd.SetOnLead(temp.p3->lead[dir]);
		}
	}
	else	//��βλ���ڽ����
	{
		if(dir < 0 || dir > 3) return false;
		if(pressEnd.p2->lead[dir] != NULL)
		{
			pressEnd.SetOnLead(pressEnd.p2->lead[dir]);
		}
		else 
		{
			wndPointer->MessageBox("�����һ��û�����ӵ��� !", "�����޷����� !", MB_ICONINFORMATION);
			return false;
		}
	}

	PaintAll();
	return true;
}

bool Manager::ShowPressure()
//��ʾ����ʼλ�õ���βλ�õĵ��Ʋ�(U0-U1)
{
	if(!pressStart.IsOnAny() || !pressEnd.IsOnAny())
	{
		AfxMessageBox("��ѡ����ʼλ���ٲ鿴���Ʋ�!\n��ʼλ�ÿ����������ѡ��!");
		return false;
	}

	char note[] = "���Ʋ�";
	char name1[NAME_LEN*2], name2[NAME_LEN*2];
	GetName(pressStart, name1);
	GetName(pressEnd, name2);

	LISTDATA list;
	list.Init(3);

	if(StaticClass::IsZero(startEndPressure)) startEndPressure = 0;
	list.SetAMember(DATA_STYLE_double, note, (void *)(&startEndPressure));
	list.SetAMember(DATA_STYLE_LPCTSTR, "��ʼλ��", name1);
	list.SetAMember(DATA_STYLE_LPCTSTR, "����λ��", name2);

	MyPropertyDlg dlg(&list, true, NULL, note, wndPointer);
	dlg.DoModal();

	return true;
}
