
//4��������------------------------------------------------------------------------

//������Ӻ�������
Manager.SetAddState = function(type) {
	ASSERT(type>=BODY_NO && type<CTRL_TYPE_NUM);
	Manager.addState = type;
};

//��ÿؼ���ͼ���
Manager.GetCtrlPaintImage = function(c) {
	if (c.IsBulbOn() || c.SwitchOnOff(false))	//С���ݴﵽ�����, ���رպ�
		return Manager.ctrlImageList[(CTRL_TYPE_COUNT + c.GetStyle())*4 + c.dir];
	else
		return Manager.ctrlImageList[c.GetStyle()*4 + c.dir];	//Ĭ�ϵĻ�ͼ���
};

//�������,str����Ӧ�ô��ڵ���NAME_LEN*2
Manager.GetBodyDefaultName = function(pointer) {
	ASSERT(pointer.IsOnAny());
	if (pointer.IsOnLead()) {
		return "����[" + pointer.p.GetInitOrder() + "]";
	} else if(pointer.IsOnCrun()) {
		return "���[���("+pointer.p.GetInitOrder()+"), ��ǰ����("+pointer.p.name+")]";
	} else { //if(pointer.IsOnCtrl())
		return "�ؼ�[���("+pointer.p.GetInitOrder()+"), ��ǰ����("+pointer.p.name+")]";
	}
};

//ɾ����ʾ,����ֵΪfalse�û�ȡ��ɾ��
Manager.DeleteNote = function(body) {
	var conCount;	//���ӵ�����
	var name;	//��������
	var note;	//��ʾ�ַ���

	//������ӵ�����
	if(body.IsOnLead())
		conCount = 0;
	else if(body.IsOnCrun())
		conCount = body.p.GetConnectNum();
	else if(body.IsOnCtrl())
		conCount = body.p.GetConnectNum();
	else
		return false;

	//�������ӵ�������ʾɾ����Ϣ
	name = GetBodyDefaultName(body);
	if (conCount > 0)
		note = "Ҫɾ�� "+name+" �� ?\n�����ӵ� "+conCount+" �ε���Ҳ��ɾ��!";
	else
		note = "Ҫɾ�� "+name+" �� ?";

	PaintWithSpecialColorAndRect(body, false);
	return IDYES == this.canvas.MessageBox(note, "ɾ��������ʾ", MB_YESNO|MB_ICONWARNING);
};

//�����·״̬
Manager.ClearCircuitState = function() {
	FocusBodyClear(null);	//����
	ClearPressBody();		//��ʾ���Ʋ�
	motiCount = 0;			//������������
	addState = BODY_NO;		//�����������
	lastMoveOnBody.Clear();	//����ϴ��ƶ���������
	lButtonDownState = 0;	//������״̬
};

//�������ָ��
Manager.GetBodyPointer = function(body) {
	var pointer;

	if (body.isFocusBody) {
		pointer = focusBody;
	} else {
		motiCount = 0;
		MotivateAll(body.pos);
		motiCount = 0;
		pointer = motiBody[0];
	}

	return pointer;
};

//�����·��ͼƬ
Manager.SaveAsPicture = function(path) {
	PaintAll();	//����·, bitmapForRefresh������λͼ
	SaveBitmapToFile(HBITMAP(bitmapForRefresh), path);
};
