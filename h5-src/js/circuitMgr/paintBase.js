
//3��ͼ����------------------------------------------------------------------------

//���ؼ�
Manager.PaintCtrl = function(c, isPaintName) {
	ASSERT(c != NULL);
	if (isPaintName) PaintCtrlText(c);	//���ؼ�����
	Manager.ctx.BitBlt(c.x, c.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintHandle(c), 0, 0, SRCAND);
};

//���ؼ�������
Manager.PaintCtrlText = function(c) {
	ASSERT(c != NULL);
	if (!c.isPaintName) return;
	Manager.ctx.TextOut(c.x, c.y-15, c.name, strlen(c.name));
};

//�����
Manager.PaintCrun = function(c, isPaintName) {
	ASSERT(c != NULL);
	if (isPaintName) PaintCrunText(c);	//���������
	Manager.ctx.BitBlt(c.x-DD, c.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCAND);
};

//���������
Manager.PaintCrunText = function(c) {
	ASSERT(c != NULL);
	if (!c.isPaintName) return;
	Manager.ctx.TextOut(c.x, c.y-20, c.name, strlen(c.name));
};

//������
Manager.PaintLead = function(l) {
	ASSERT(l != NULL);
	Manager.ctx.fillStyle = l.color;
	l.PaintLead(Manager.ctx);
};

//�����е���
Manager.PaintAllLead = function() {
	for (var index=leadCount-1; index>=0; --index) {
		Manager.PaintLead(lead[index]);
	}
	Manager.ctx.fillStyle = "#000000";
};

//�����е�����
Manager.PaintAll = function() {
	int i;
	CDC * save = Manager.ctx;
	RECT rect;
	BITMAP bitmap;

	//1,�������״̬��Ϣ----------------------------------------------------
	motiCount = 0;
	addState = BODY_NO;
	lastMoveOnPos.x = -100;
	lastMoveOnBody.Clear();

	//2,��ͼ��ʼ��----------------------------------------------------------
	//��ô��ڳߴ�
	wndPointer.GetClientRect(&rect);

	//��ʼ��ˢ��λͼ��С
	bitmapForRefresh.GetBitmap(&bitmap);
	if (rect.bottom > bitmap.bmHeight || rect.right > bitmap.bmWidth)
	{
		bitmapForRefresh.DeleteObject();
		bitmapForRefresh.CreateBitmap(rect.right, rect.bottom, 1, 32, NULL);
		dcForRefresh.SelectObject(&bitmapForRefresh);
	}

	//��dcForRefresh��ͼ
	Manager.ctx.DPtoLP(&rect);			//��ǰrect���豸����任Ϊ�߼�����
	Manager.ctx = &dcForRefresh;			//dc��ʱ�滻ΪdcForRefresh,���ڴ滭ͼ

	//����������ɫ���ӽ����
	Manager.ctx.SetTextColor(LEADCOLOR[textColor]);
	Manager.ctx.SetViewportOrg(-viewOrig.x, -viewOrig.y);	//��ʼ���ӽ���ʼ����

	//3,�ڴ滭ͼ------------------------------------------------------------
	//�ð�ɫ���θ��������ͻ���
	Manager.ctx.SelectStockObject(WHITE_PEN);
	Manager.ctx.SelectStockObject(WHITE_BRUSH);
	Manager.ctx.Rectangle(&rect);

	//���ؼ�����Լ����ǵ�����
	for(i=ctrlCount-1; i>=0; --i)
		PaintCtrl(ctrl[i], true);
	for(i=crunCount-1; i>=0; --i)
		PaintCrun(crun[i], true);

	//������
	PaintAllLead();

	//������
	FocusBodyPaint(NULL);

	//�ػ���ʾ���Ʋ������
	PaintWithSpecialColor(pressStart, false);
	PaintWithSpecialColor(pressEnd, true);

	//4,��ԭdc, һ���Ի�ͼ--------------------------------------------------
	Manager.ctx = save;
	Manager.ctx.BitBlt(0, 0, rect.right, rect.bottom, &dcForRefresh, 0, 0, SRCCOPY);
};

//����������ӵ㲿λ,�ı������״
Manager.PaintMouseMotivate = function(const Pointer &mouseMoti) {
	const Pointer * mouse = &mouseMoti;
	POINT tempPos;
	const int CR = 3; //���ӵ㻭ͼ�뾶

	if (mouse.IsOnLead())
	{
		if (motiCount && motiBody[motiCount-1].IsOnConnectPos())
		{//ѡ�������ӵ�,�������ӽ��ͼ��,��ʾʹ��ConnectBodyLead����
			SetCursor(hcShowConnect);
		}
		else
		{//û��ѡ�����ӵ�,�����"ָ��",��ʾ�ı䵼������
			if (mouse.IsOnHoriLead())SetCursor(hcSizeNS);	//�ں���,�����"����ָ��"
			else SetCursor(hcSizeWE);						//������,�����"����ָ��"
		}
	}
	else if (mouse.IsOnBody())	//��������,������ֵ���״,��ʾ�ƶ�����
	{
		SetCursor(hcHand);
	}

	if (!lastMoveOnBody.IsAllSame(mouse))	//lastMoveOnBody��mouseָ���Pointer�ṹ�岻һ��
	{
		if (lastMoveOnBody.IsOnConnectPos())	//��ԭ��һ�����ӵ�
		{
			lastMoveOnBody.GetPosFromBody(tempPos);	//�������
			Manager.ctx.BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2,
				&showConnectDcMem, 0, 0, SRCINVERT);
		}

		lastMoveOnBody = *mouse;	//��¼��ǰ��꼤������

		if (mouse.IsOnConnectPos())	//����ǰ�����ӵ�
		{
			mouse.GetPosFromBody(tempPos);	//�������
			Manager.ctx.BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2,
				&showConnectDcMem, 0, 0, SRCINVERT);
		}
	}
};

//��ָ����ɫ�����ߵ���
Manager.PaintLeadWithStyle = function(LEAD * lead, int leadStyle, enum COLOR colorNum) {
	ASSERT(lead != NULL);
	CPen tempPen;

	tempPen.CreatePen(leadStyle, 1, LEADCOLOR[colorNum]);	//�½����⻭��
	Manager.ctx.SelectObject(tempPen.m_hObject);			//ѡ�񻭱�
	lead.PaintLead(Manager.ctx);							//������
	tempPen.DeleteObject();									//�ͷŻ���
	Manager.ctx.SelectObject(hp);							//�ָ�����
};

//��ָ����ɫ��ָ�����
Manager.PaintCrunWithColor = function(CRUN * c, enum COLOR colorNum) {
	ASSERT(c != NULL);
	CBrush hb;

	//1,��ָ����ɫ���� -------------------------------------------------------------
	//����ָ����ɫ��ˢ
	hb.CreateSolidBrush(LEADCOLOR[colorNum]);
	Manager.ctx.SelectObject(&hb);
	//���ÿջ���
	Manager.ctx.SelectStockObject(NULL_PEN);
	//��ָ����ɫԲ��
	Manager.ctx.Rectangle(c.x-DD, c.y-DD, c.x+DD+1, c.y+DD+1);

	//2,�ͷŻ�ˢ,��ԭ��ˢ ----------------------------------------------------------
	hb.DeleteObject();
	Manager.ctx.SelectStockObject(NULL_BRUSH);

	//3,����ɫ���,ʹ�� "��" ���߼���ͼ,�õ�ָ����ɫ��� ---------------------------
	Manager.ctx.BitBlt(c.x-DD, c.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCPAINT);
};

//��ָ����ɫ��ָ���ؼ�
Manager.PaintCtrlWithColor = function(CTRL * c, enum COLOR colorNum) {
	ASSERT(c != NULL);
	CBrush hb;

	//1,��ָ����ɫ���� -------------------------------------------------------------
	//����ָ����ɫ��ˢ
	hb.CreateSolidBrush(LEADCOLOR[colorNum]);
	Manager.ctx.SelectObject(&hb);
	//���ÿջ���
	Manager.ctx.SelectStockObject(NULL_PEN);
	//��ָ����ɫ����
	Manager.ctx.Rectangle(c.x, c.y, c.x+BODYSIZE.cx+1, c.y+BODYSIZE.cy+1);

	//2,�ͷŻ�ˢ,��ԭ��ˢ ----------------------------------------------------------
	hb.DeleteObject();
	Manager.ctx.SelectStockObject(NULL_BRUSH);

	//3,����ɫ�ؼ�,ʹ�� "��" ���߼���ͼ,�õ�ָ����ɫ�ؼ�
	Manager.ctx.BitBlt(c.x, c.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintHandle(c), 0, 0, SRCPAINT);

	//4,���»������ǵ���Χ���� -----------------------------------------------------
	for(int num=0; num<2; ++num) if (c.lead[num] != NULL)
		PaintLead(c.lead[num]);
};

//�ñ�����ɫ(��ɫ)��ʾ
Manager.PaintWithSpecialColor = function(const Pointer &body, bool isPaintNum) {
	const COLOR colorNum = (enum COLOR)COLOR_TYPE_NUM;	//ѡ�ñ�����ɫ(��ɫ)

	if (body.IsOnLead())
	{
		if (isPaintNum)
		{
			//������
			PaintLeadWithStyle(body.p1, PS_SOLID, colorNum);

			//�ڵ�����ʼ�ͽ�β���ֱ���ʾ����'1'��'2'
			char text[8] = "0";
			POINT pos[2];
			body.p1.GetStartEndPos(pos[0], pos[1]);
			for(int i=0; i<2; ++i)
			{
				++(text[0]);
				Manager.ctx.TextOut(pos[i].x, pos[i].y, text, 1);
			}
		}
		else
		{
			PaintLeadWithStyle(body.p1, PS_DOT, colorNum);	//������
		}
	}
	else if (body.IsOnCrun())
	{
		PaintCrunWithColor(body.p2, colorNum);	//�����
		if (isPaintNum)							//�ڽ���������ҷֱ���ʾ1,2,3,4
		{
			POINT pos = body.p2.coord;
			pos.x -= 5; pos.y -= 8;
			Manager.ctx.TextOut(pos.x, pos.y-15, "1", 1);
			Manager.ctx.TextOut(pos.x, pos.y+15, "2", 1);
			Manager.ctx.TextOut(pos.x-15, pos.y, "3", 1);
			Manager.ctx.TextOut(pos.x+15, pos.y, "4", 1);
		}
	}
	else if (body.IsOnCtrl())
	{
		PaintCtrlWithColor(body.p3, colorNum);	//���ؼ�
	}
};

//��ָ��λ����ʾ����ķ���
Manager.PaintInvertBodyAtPos = function(const Pointer &body, POINT pos) {
	ASSERT(body.IsOnBody(false));
	if (body.IsOnCrun())
	{
		Manager.ctx.BitBlt(pos.x-DD, pos.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCINVERT);
	}
	else //if (body.IsOnCtrl())
	{
		Manager.ctx.BitBlt(pos.x, pos.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintHandle(body.p3), 0, 0, SRCINVERT);
	}
};
