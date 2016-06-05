
//3��ͼ����------------------------------------------------------------------------

//���ؼ�
Manager.PaintCtrl = function(c, isPaintName) {
	ASSERT(c != null);
	if (isPaintName) PaintCtrlText(c);	//���ؼ�����
	Manager.ctx.putImageData(Manager.GetCtrlPaintImage(c), c.x, c.y);
};

//���ؼ�������
Manager.PaintCtrlText = function(c) {
	ASSERT(c != null);
	if (!c.isPaintName) return;
	ctx.strokeStyle = PaintCommonFunc.HexToRGBStr(Manager.textColor);
	Manager.ctx.strokeText(c.name, c.x, c.y-15);
};

//�����
Manager.PaintCrun = function(c, isPaintName) {
	ASSERT(c != null);
	if (isPaintName) PaintCrunText(c);	//���������
	Manager.PaintCrunWithStyle(c, PAINT_CRUN_STYLE_NORMAL);
};

//���������
Manager.PaintCrunText = function(c) {
	ASSERT(c != null);
	if (!c.isPaintName) return;
	ctx.strokeStyle = PaintCommonFunc.HexToRGBStr(Manager.textColor);
	Manager.ctx.strokeText(c.name, c.x, c.y-20);
};

//������
Manager.PaintLead = function(l) {
	ASSERT(l != null);
	Manager.ctx.strokeStyle = PaintCommonFunc.HexToRGBStr(l.color);
	Manager.ctx.lineWidth = 1;
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
	var rect = {};
	//var save = Manager.ctx;
	//var bitmap;

	//1,�������״̬��Ϣ----------------------------------------------------
	motiCount = 0;
	addState = BODY_NO;
	lastMoveOnPos.x = -100;
	lastMoveOnBody.Clear();

	//2,��ͼ��ʼ��----------------------------------------------------------
	//��ô��ڳߴ�
	rect.left = 0;
	rect.top = 0;
	rect.width = Manager.canvas.width;
	rect.height = Manager.canvas.height;

	//��ʼ��ˢ��λͼ��С
	/*bitmapForRefresh.GetBitmap(&bitmap);
	if (rect.height > bitmap.bmHeight || rect.width > bitmap.bmWidth) {
		bitmapForRefresh.DeleteObject();
		bitmapForRefresh.CreateBitmap(rect.width, rect.height, 1, 32, null);
		dcForRefresh.SelectObject(&bitmapForRefresh);
	}*/

	//��dcForRefresh��ͼ
	//Manager.ctx.DPtoLP(&rect);			//��ǰrect���豸����任Ϊ�߼�����
	//Manager.ctx = &dcForRefresh;		//dc��ʱ�滻ΪdcForRefresh,���ڴ滭ͼ

	//����������ӽ����
	ctx.font = "15px Georgia";
	//Manager.ctx.SetViewportOrg(-viewOrig.x, -viewOrig.y);	//��ʼ���ӽ���ʼ����

	//3,�ڴ滭ͼ------------------------------------------------------------
	//�ð�ɫ���θ��������ͻ���
	Manager.ctx.fillStyle = "#FFFFFF";
	Manager.ctx.strokeStyle = "#FFFFFF";
	Manager.ctx.fillRect(rect.left,rect.top, rect.width,rect.height);

	//���ؼ�����Լ����ǵ�����
	for(var i=ctrlCount-1; i>=0; --i)
		PaintCtrl(ctrl[i], true);
	for(var i=crunCount-1; i>=0; --i)
		PaintCrun(crun[i], true);
	//������
	PaintAllLead();

	//������
	//FocusBodyPaint(null);

	//�ػ���ʾ���Ʋ������
	//PaintWithSpecialColorAndRect(pressStart, false);
	//PaintWithSpecialColorAndRect(pressEnd, true);

	//4,��ԭdc, һ���Ի�ͼ--------------------------------------------------
	//Manager.ctx = save;
	//Manager.ctx.BitBlt(0, 0, rect.width, rect.height, &dcForRefresh, 0, 0, SRCCOPY);
};

//����������ӵ㲿λ,�ı������״
Manager.PaintMouseMotivate = function(mouseMoti) {
	var mm = mouseMoti;
	var CR = 3; //���ӵ㻭ͼ�뾶

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
			var tempPos = lastMoveOnBody.GetPosFromBody();	//�������
			Manager.ctx.BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2, showConnectImageData, 0, 0, SRCINVERT);
		}

		lastMoveOnBody = mm;	//��¼��ǰ��꼤������

		//����ǰ�����ӵ�
		if (mm.IsOnConnectPos()) {
			var tempPos = mm.GetPosFromBody();	//�������
			Manager.ctx.BitBlt(tempPos.x-CR, tempPos.y-CR, CR*2, CR*2, showConnectImageData, 0, 0, SRCINVERT);
		}
	}
};

//��ָ����ɫ�����ߵ���
Manager.PaintLeadWithStyle = function(lead, leadWidth, color) {
	ASSERT(lead != null);
	
	Manager.ctx.strokeStyle = PaintCommonFunc.HexToRGBStr(color);
	Manager.ctx.lineWidth = leadWidth;
	lead.PaintLead(Manager.ctx);
};

//��ָ����PAINT_CRUN_STYLE, ��ָ�����
Manager.PaintCrunWithStyle = function(c, style) {
	ASSERT(c != null);
	ASSERT(style >= 0 && style < PAINT_CRUN_STYLE_COUNT);

	Manager.ctx.fillRect(c.x-DD, c.y-DD, 2*DD, 2*DD);
	Manager.ctx.putImageData(Manager.crunImageData[style], c.x-DD, c.y-DD);
};

//��ָ����ɫ��ָ���ؼ�
Manager.PaintCtrlWithColor = function(c, color) {
	ASSERT(c != null);

	//��ָ����ɫ����
	//����ָ����ɫ��ˢ
	Manager.ctx.fillStyle = PaintCommonFunc.HexToRGBStr(color);
	//���ÿջ���
	Manager.ctx.lineWidth = 0;
	//��ָ����ɫ����
	Manager.ctx.fillRect(c.x, c.y, CTRL_SIZE.cx, CTRL_SIZE.cy);

	//����ɫ�ؼ�,ʹ�� "��" ���߼���ͼ,�õ�ָ����ɫ�ؼ�
	PaintCommonFunc.PaintImageDataOr(Manager.ctx, Manager.GetCtrlPaintImage(c), c.x,c.y);

	//���»������ǵ���Χ����
	for (var i=0; i<2; ++i) 
		if (c.lead[i] != null)
			Manager.PaintLead(c.lead[i]);
};

//�ñ�����ɫ(��ɫ)��ʾ����, �������ⲿ�þ��ΰ�Χ
Manager.PaintWithSpecialColorAndRect = function(body, isPaintNum) {
	var color = COLOR_SPECIAL;	//ѡ�ñ�����ɫ(��ɫ)

	if (body.IsOnLead()) {
		if (isPaintNum) {
			//������
			PaintLeadWithStyle(body.p, 1, color);

			//�ڵ�����ʼ�ͽ�β���ֱ���ʾ����'1'��'2'
			var startPos = {}, endPos = {};
			body.p.GetStartEndPos(startPos, endPos);
			Manager.ctx.strokeText("1", startPos.x, startPos.y);
			Manager.ctx.strokeText("2", endPos.x, endPos.y);
		} else {
			PaintLeadWithStyle(body.p, 2, color);	//������
		}
	} else if (body.IsOnCrun()) {
		PaintCrunWithStyle(body.p, PAINT_CRUN_STYLE_SPECIAL);	//�����
		PaintCommonFunc.PaintSurrendedRect(Manager.ctx, body.p.x-DD, body.p.y-DD, DD*2, DD*2, 0xF01010);
		
		if (isPaintNum) {	//�ڽ���������ҷֱ���ʾ1,2,3,4
			var pos = {x:body.p.x-5, y:body.p.y-8};
			Manager.ctx.strokeText("1", pos.x, pos.y-15);
			Manager.ctx.strokeText("2", pos.x, pos.y+15);
			Manager.ctx.strokeText("3", pos.x-15, pos.y);
			Manager.ctx.strokeText("4", pos.x+15, pos.y);
		}
	} else if (body.IsOnCtrl()) {
		PaintCtrlWithColor(body.p, color);	//���ؼ�
		PaintCommonFunc.PaintSurrendedRect(Manager.ctx, body.p.x, body.p.y, CTRL_SIZE.cx, CTRL_SIZE.cy, 0xF01010);
	}
};

//��ָ��λ����ʾ����ķ���
Manager.PaintInvertBodyAtPos = function(body, pos) {
	ASSERT(body.IsOnBody(false));
	if (body.IsOnCrun()) {
		PaintCommonFunc.PaintImageDataXor(Manager.ctx, crunImageData, pos.x-DD, pos.y-DD);
	} else {//if (body.IsOnCtrl())
		PaintCommonFunc.PaintImageDataXor(Manager.ctx, Manager.GetCtrlPaintImage(body.p3), pos.x, pos.y);
	}
};
