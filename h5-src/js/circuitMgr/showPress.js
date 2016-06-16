
//13显示电势差函数-----------------------------------------------------------------↓
void Manager.ClearPressBody()
//清空显示电势差的成员变量
{
	Manager.pressStartBody.Clear();
	Manager.pressEndBody.Clear();
	startEndPressure = 0;
}

bool Manager.SetStartBody(POINT pos)
//设置计算电势差的起始位置
{
	Manager.motiCount = 0;
	if (!MotivateAll(pos)) return false;	//没有点击物体
	Manager.motiCount = 0;

	if (Manager.motiBody[0].IsOnLead())
	{
		if (StaticClass.IsElecError(Manager.motiBody[0].p1.elecDir))
		{
			Manager.canvas.MessageBox("当前选择的电路不正常", "无法计算电势差", MB_ICONWARNING);
			return false;
		}
	}
	else if (Manager.motiBody[0].IsOnCrun() && !Manager.motiBody[0].IsOnConnectPos())
	{
		CRUN * c = Manager.motiBody[0].p2;
		for (int i=0; i<4; ++i) if (c.lead[i] && StaticClass.IsElecError(c.lead[i].elecDir))
		{
			Manager.canvas.MessageBox("当前选择的电路不正常", "无法计算电势差", MB_ICONWARNING);
			return false;
		}
	}
	else 
	{
		return false;	//没有点击导线或者节点
	}

	Manager.pressStartBody = Manager.pressEndBody = Manager.motiBody[0];
	startEndPressure = 0;

	PaintAll();
	return true;
}

bool Manager.NextBodyByInputNum(UINT nChar)
//用户输入数字1,2,3,4来移动电势差结尾位置
{
	if (!Manager.pressStartBody.IsOnAny() || !Manager.pressEndBody.IsOnAny())
	{
		AfxMessageBox("请先鼠标点击导线或者连线选择电势差起始位置,\n然后输入数字移动电势差结尾位置.");
		return false;
	}

	int dir;
	switch(nChar)
	{
	case '#':
	case 'a':
		dir = 0; //小键盘'1'键
		break;

	case '(':
	case 'b':
		dir = 1; //小键盘'2'键
		break;

	case 34:
	case 'c':
		dir = 2; //小键盘'3'键
		break;

	case '%':
	case 'd':
		dir = 3; //小键盘'4'键
		break;

	default:
		if (nChar >= '1' && nChar <= '4')
			dir = nChar - '1';
		else
			return false;
	}

	if (Manager.pressEndBody.IsOnLead())	//结尾位置在导线上
	{
		if (dir < 0 || dir > 1) return false;
		
		Pointer temp = Manager.pressEndBody.p1.conBody[dir];
		temp.SetAtState(-1);

		if (temp.IsOnCrun())
		{
			Manager.pressEndBody = temp;
		}
		else //if (temp.IsOnCtrl())
		{
			if (temp.p3.GetResist() < 0)	//断路控件
			{
				Manager.canvas.MessageBox("这是一个断路电学元件 !", "电流无法流过 !", MB_ICONINFORMATION);
				return false;
			}
			if (temp.p3.GetConnectNum() < 2)	//控件没有连接2段导线
			{
				Manager.canvas.MessageBox("电学元件另一端没有连接导线 !", "电流无法流过 !", MB_ICONINFORMATION);
				return false;
			}
			dir = temp.p3.lead[0] == Manager.pressEndBody.p1;	//下一个导线索引(0或1)
			if (temp.p3.lead[dir] == Manager.pressEndBody.p1) return false;	//电路是一个控件2端都连接同一段导线
			if (temp.p3.elecDir == dir)
				startEndPressure -= temp.p3.GetResist() * temp.p3.elec;
			else
				startEndPressure += temp.p3.GetResist() * temp.p3.elec;
			startEndPressure += temp.p3.GetPress(dir);
			Manager.pressEndBody.SetOnLead(temp.p3.lead[dir]);
		}
	}
	else	//结尾位置在结点上
	{
		if (dir < 0 || dir > 3) return false;
		if (Manager.pressEndBody.p2.lead[dir] != null)
		{
			Manager.pressEndBody.SetOnLead(Manager.pressEndBody.p2.lead[dir]);
		}
		else 
		{
			Manager.canvas.MessageBox("结点这一端没有连接导线 !", "电流无法流过 !", MB_ICONINFORMATION);
			return false;
		}
	}

	PaintAll();
	return true;
}

bool Manager.ShowPressure()
//显示从起始位置到结尾位置的电势差(U0-U1)
{
	if (!Manager.pressStartBody.IsOnAny() || !Manager.pressEndBody.IsOnAny())
	{
		AfxMessageBox("请选择起始位置再查看电势差!\n起始位置可以用鼠标点击选择!");
		return false;
	}

	char note[] = "电势差";
	var name1 = Manager.GetBodyDefaultName(Manager.pressStartBody);
	var name2 = Manager.GetBodyDefaultName(Manager.pressEndBody);

	LISTDATA list;
	list.Init(3);

	if (IsFloatZero(startEndPressure)) startEndPressure = 0;
	list.SetAMember(DATA_STYLE_double, note, (void *)(&startEndPressure));
	list.SetAMember(DATA_STYLE_LPCTSTR, "起始位置", name1);
	list.SetAMember(DATA_STYLE_LPCTSTR, "结束位置", name2);

	MyPropertyDlg dlg(&list, true, null, note, Manager.canvas);
	dlg.DoModal();

	return true;
}
