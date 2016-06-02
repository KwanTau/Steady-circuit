
//3��ͼ����------------------------------------------------------------------------

//���ؼ�
Manager.PaintCtrl = function(c, isPaintName) {
	ASSERT(c != null);
	if (isPaintName) PaintCtrlText(c);	//���ؼ�����
	Manager.ctx.drawImage(GetCtrlPaintImage(c), c.x, c.y);
};

//���ؼ�������
Manager.PaintCtrlText = function(c) {
	ASSERT(c != null);
	if (!c.isPaintName) return;
	Manager.ctx.strokeText(c.name, c.x, c.y-15);
};

//�����
Manager.PaintCrun = function(c, isPaintName) {
	ASSERT(c != null);
	if (isPaintName) PaintCrunText(c);	//���������
	Manager.ctx.drawImage(crunDcMem, c.x-DD, c.y-DD);
};

//���������
Manager.PaintCrunText = function(c) {
	ASSERT(c != null);
	if (!c.isPaintName) return;
	Manager.ctx.strokeText(c.name, c.x, c.y-20);
};

//������
Manager.PaintLead = function(l) {
	ASSERT(l != null);
	Manager.ctx.strokeStyle = l.color;
	l.PaintLead(Manager.ctx);
};

//�����е���
Manager.PaintAllLead = function() {
	for (var index=leadCount-1; index>=0; --index) {
		Manager.PaintLead(lead[index]);
	}
	Manager.ctx.strokeStyle = "#000000";
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
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	//��ʼ��ˢ��λͼ��С
	bitmapForRefresh.GetBitmap(&bitmap);
	if (rect.bottom > bitmap.bmHeight || rect.right > bitmap.bmWidth) {
		bitmapForRefresh.DeleteObject();
		bitmapForRefresh.CreateBitmap(rect.right, rect.bottom, 1, 32, null);
		dcForRefresh.SelectObject(&bitmapForRefresh);
	}

	//��dcForRefresh��ͼ
	Manager.ctx.DPtoLP(&rect);			//��ǰrect���豸����任Ϊ�߼�����
	Manager.ctx = &dcForRefresh;		//dc��ʱ�滻ΪdcForRefresh,���ڴ滭ͼ

	//����������ɫ���ӽ����
	Manager.ctx.SetTextColor(LEADCOLOR[textColor]);
	Manager.ctx.SetViewportOrg(-viewOrig.x, -viewOrig.y);	//��ʼ���ӽ���ʼ����

	//3,�ڴ滭ͼ------------------------------------------------------------
	//�ð�ɫ���θ��������ͻ���
	Manager.ctx.fillStyle = "#FFFFFF";
	Manager.ctx.strokeStyle = "#FFFFFF";
	Manager.ctx.fillRect(&rect);

	//���ؼ�����Լ����ǵ�����
	for(i=ctrlCount-1; i>=0; --i)
		PaintCtrl(ctrl[i], true);
	for(i=crunCount-1; i>=0; --i)
		PaintCrun(crun[i], true);
	//������
	PaintAllLead();

	//������
	FocusBodyPaint(null);

	//�ػ���ʾ���Ʋ������
	PaintWithSpecialColor(pressStart, false);
	PaintWithSpecialColor(pressEnd, true);

	//4,��ԭdc, һ���Ի�ͼ--------------------------------------------------
	Manager.ctx = save;
	Manager.ctx.BitBlt(0, 0, rect.right, rect.bottom, &dcForRefresh, 0, 0, SRCCOPY);
};

//����������ӵ㲿λ,�ı������״
Manager.PaintMouseMotivate = function(mouseMoti) {
	var mm = mouseMoti;
	POINT tempPos;
	const int CR = 3; //���ӵ㻭ͼ�뾶

	if (mm.IsOnLead()) {
		//ѡ�������ӵ�,�������ӽ��ͼ��,��ʾʹ��ConnectBodyLead����
		if (motiCount && motiBody[motiCount-1].IsOnConnectPos()) {
			SetCursor(hcShowConnect);
		//û��ѡ�����ӵ�,�����"ָ��",��ʾ�ı䵼������
		} else {
			if (mm.IsOnHoriLead())
				SetCursor(hcSizeNS);	//�ں���,�����"����ָ��"
			else 
				SetCursor(hcSizeWE);	//������,�����"����ָ��"
		}
	} else if (mm.IsOnBody()) {	//��������,������ֵ���״,��ʾ�ƶ�����
		SetCursor(hcHand);
	}

	if (!lastMoveOnBody.IsAllSame(mm)) {	//lastMoveOnBody��mouseָ���Pointer�ṹ�岻һ��
		//��ԭ��һ�����ӵ�
		if (lastMoveOnBody.IsOnConnectPos()) {
			lastMoveOnBody.GetPosFromBody(tempPos);	//�������
			Manager.ctx.BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2,
				&showConnectDcMem, 0, 0, SRCINVERT);
		}

		lastMoveOnBody = *mm;	//��¼��ǰ��꼤������

		//����ǰ�����ӵ�
		if (mm.IsOnConnectPos()) {
			mm.GetPosFromBody(tempPos);	//�������
			Manager.ctx.BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2,
				&showConnectDcMem, 0, 0, SRCINVERT);
		}
	}
};

//��ָ����ɫ�����ߵ���
Manager.PaintLeadWithStyle = function(lead, leadStyle, colorType) {
	ASSERT(lead != null);
	CPen tempPen;

	tempPen.CreatePen(leadStyle, 1, LEADCOLOR[colorType]);	//�½����⻭��
	Manager.ctx.SelectObject(tempPen.m_hObject);			//ѡ�񻭱�
	lead.PaintLead(Manager.ctx);							//������
	tempPen.DeleteObject();									//�ͷŻ���
	Manager.ctx.SelectObject(hp);							//�ָ�����
};

//��ָ����ɫ��ָ�����
Manager.PaintCrunWithColor = function(c, colorType) {
	ASSERT(c != null);
	CBrush hb;

	//1,��ָ����ɫ���� -------------------------------------------------------------
	//����ָ����ɫ��ˢ
	hb.CreateSolidBrush(LEADCOLOR[colorType]);
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
Manager.PaintCtrlWithColor = function(c, colorType) {
	ASSERT(c != null);
	CBrush hb;

	//1,��ָ����ɫ���� -------------------------------------------------------------
	//����ָ����ɫ��ˢ
	hb.CreateSolidBrush(LEADCOLOR[colorType]);
	Manager.ctx.SelectObject(&hb);
	//���ÿջ���
	Manager.ctx.SelectStockObject(NULL_PEN);
	//��ָ����ɫ����
	Manager.ctx.Rectangle(c.x, c.y, c.x+BODYSIZE.cx+1, c.y+BODYSIZE.cy+1);

	//2,�ͷŻ�ˢ,��ԭ��ˢ ----------------------------------------------------------
	hb.DeleteObject();
	Manager.ctx.SelectStockObject(NULL_BRUSH);

	//3,����ɫ�ؼ�,ʹ�� "��" ���߼���ͼ,�õ�ָ����ɫ�ؼ�
	Manager.ctx.BitBlt(c.x, c.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintImage(c), 0, 0, SRCPAINT);

	//4,���»������ǵ���Χ���� -----------------------------------------------------
	for(int num=0; num<2; ++num) if (c.lead[num] != null)
		PaintLead(c.lead[num]);
};

//�ñ�����ɫ(��ɫ)��ʾ
Manager.PaintWithSpecialColor = function(const Pointer &body, isPaintNum) {
	const COLOR colorType = (enum COLOR)COLOR_TYPE_COUNT;	//ѡ�ñ�����ɫ(��ɫ)

	if (body.IsOnLead()) {
		if (isPaintNum) {
			//������
			PaintLeadWithStyle(body.p1, PS_SOLID, colorType);

			//�ڵ�����ʼ�ͽ�β���ֱ���ʾ����'1'��'2'
			char text[8] = "0";
			POINT pos[2];
			body.p1.GetStartEndPos(pos[0], pos[1]);
			for(int i=0; i<2; ++i) {
				++(text[0]);
				Manager.ctx.TextOut(pos[i].x, pos[i].y, text, 1);
			}
		} else {
			PaintLeadWithStyle(body.p1, PS_DOT, colorType);	//������
		}
	} else if (body.IsOnCrun()) {
		PaintCrunWithColor(body.p2, colorType);	//�����
		if (isPaintNum)							//�ڽ���������ҷֱ���ʾ1,2,3,4
		{
			POINT pos = body.p2.coord;
			pos.x -= 5; pos.y -= 8;
			Manager.ctx.TextOut(pos.x, pos.y-15, "1", 1);
			Manager.ctx.TextOut(pos.x, pos.y+15, "2", 1);
			Manager.ctx.TextOut(pos.x-15, pos.y, "3", 1);
			Manager.ctx.TextOut(pos.x+15, pos.y, "4", 1);
		}
	} else if (body.IsOnCtrl()) {
		PaintCtrlWithColor(body.p3, colorType);	//���ؼ�
	}
};

//��ָ��λ����ʾ����ķ���
Manager.PaintInvertBodyAtPos = function(const Pointer &body, POINT pos) {
	ASSERT(body.IsOnBody(false));
	if (body.IsOnCrun()) {
		Manager.ctx.BitBlt(pos.x-DD, pos.y-DD, DD*2, DD*2, &crunDcMem, 0, 0, SRCINVERT);
	} else {//if (body.IsOnCtrl())
		Manager.ctx.BitBlt(pos.x, pos.y, BODYSIZE.cx, BODYSIZE.cy, GetCtrlPaintImage(body.p3), 0, 0, SRCINVERT);
	}
};
