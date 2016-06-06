
//7�ļ�����--------------------------------------------------------------------��

//��ȡ�ļ�·��
Manager.GetFilePath = function() {
	return Manager.fileName;
};

//�����·
Manager.SaveFile = function(newFileName) {
	ASSERT(newFileName && newFileName.length > 0);
	Manager.fileName = newFileName;	//�滻ԭ���ļ�·��
	
	
	var data = {};

	//1�ļ��汾
	data.fileVersion = FILE_VERSION;

	//2���
	data.cruns = new Array();
	for (var i = Manager.crun.length-1; i >= 0; --i)
		data.cruns.push(Manager.crun[i].GenerateStoreJsonObj());
	//3�ؼ�
	data.ctrls = new Array();
	for (var i = Manager.ctrl.length-1; i >= 0; --i)
		data.ctrls.push(Manager.ctrl[i].GenerateStoreJsonObj());
	//4����
	data.leads = new Array();
	for (var i = Manager.lead.length-1; i >= 0; --i)
		data.leads.push(Manager.lead[i].GenerateStoreJsonObj());

	//5��������
	data.moveBodySense = Manager.moveBodySense;		//�������һ�������ƶ��ľ���
	data.maxLeaveOutDis = Manager.maxLeaveOutDis;	//���ߺϲ�������
	data.textColor = Manager.textColor;				//������ɫ
	data.focusLeadStyle = Manager.focusLeadStyle;	//���㵼����ʽ
	data.focusCrunColor = Manager.focusCrunColor;	//��������ɫ
	data.focusCtrlColor = Manager.focusCtrlColor;	//����ؼ���ɫ
	data.focusBody = Manager.focusBody.GenerateStoreJsonObj();	//��������
	data.viewOrig = Manager.viewOrig;				//�ӽǳ�ʼ����
	
	// ��������
	var callbackFunc = function(response) {};
	$.post("/saveCircuit", {"data":data}, callbackFunc)
	return true;
};

//��ȡ��·�ص�����
function readFileCallbackFunc(data) {
	var pos1 = {x:0, y:0};

	if (!data || data.length <= 0) {
		alert("�ļ����ܲ����ڻ��ܶ�ȡ !");
		return false;
	}

	//1�ļ��汾
	if (data.fileVersion != FILE_VERSION) {	//�ļ��汾��ͬ,�����ȡ
		alert("�ļ��汾���� ! ��ȡ�ļ�����");
		return false;
	}

	//Manager.fileName = newFileName;	//�滻ԭ��·��

	// ������Ϊ�ļ��������������
	try {
		//2��ȡ��������
		var crunCount = data.cruns.length;
		var ctrlCount = data.ctrls.length;
		var leadCount = data.leads.length;

		//����ȡ�����������Ƿ�������ķ�Χ֮��
		if (crunCount>MAX_CRUN_COUNT || leadCount>MAX_LEAD_COUNT || ctrlCount>MAX_CTRL_COUNT)
			throw new Error(10, "��·Ԫ��̫��");
		
		//3�½�ԭ��
		CRUN.ResetGlobalInitOrder();
		Manager.crun = new Array(crunCount);
		for (var i = crunCount-1; i >= 0; --i)
			Manager.crun[i] = CRUN.CreateNew(i, 0,0);
		
		CTRL.ResetGlobalInitOrder();
		Manager.ctrl = new Array(ctrlCount);
		for (var i = ctrlCount-1; i >= 0; --i)
			Manager.ctrl[i] = CTRL.CreateNew(i, 0,0, 0);
		
		LEAD.ResetGlobalInitOrder(leadCount);
		Manager.lead = new Array(leadCount);
		for (var i = leadCount-1; i >= 0; --i)
			Manager.lead[i] = LEAD.CreateNew(i, 0, null,null, false);
		
		//4��ȡ���
		for (var i = crunCount-1; i >= 0; --i)
			Manager.crun[i].ReadFromStoreJsonObj(data.cruns[i], Manager.lead);

		//5��ȡ�ؼ�
		for (var i = ctrlCount-1; i >= 0; --i)
			Manager.ctrl[i].ReadFromStoreJsonObj(data.ctrls[i], Manager.lead);

		//6��ȡ����
		for (var i = leadCount-1; i >= 0; --i)
			Manager.lead[i].ReadFromStoreJsonObj(data.leads[i], Manager.lead, Manager.crun, Manager.ctrl);

		//7��ȡ��������
		Manager.moveBodySense = data.moveBodySense;		//�������һ�������ƶ��ľ���
		Manager.maxLeaveOutDis = data.maxLeaveOutDis;	//���ߺϲ�������
		Manager.textColor = data.textColor;				//������ɫ
		Manager.focusLeadStyle = data.focusLeadStyle;	//���㵼����ʽ
		Manager.focusCrunColor = data.focusCrunColor;	//��������ɫ
		Manager.focusCtrlColor = data.focusCtrlColor;	//����ؼ���ɫ
		var focusBody = Pointer.CreateNew();
		focusBody.ReadFromStoreJsonObj(data.focusBody, lead, crun, ctrl);	//��ȡ��������
		Manager.FocusBodySet(focusBody);				//���ý�������
		Manager.viewOrig = data.viewOrig;				//�ӽǳ�ʼ����

		//ctx.strokeStyle = PaintCommonFunc.HexToRGBStr(Manager.textColor);	//��ʼ��������ɫ
		//ctx.SetViewportOrg(-Manager.viewOrig.x, -Manager.viewOrig.y);		//��ʼ���ӽǳ�ʼ����
		//Manager.canvas.SetScrollPos(SB_HORZ, viewOrig.x/mouseWheelSense.cx);	//��ʼ��ˮƽ������
		//Manager.canvas.SetScrollPos(SB_VERT, viewOrig.y/mouseWheelSense.cy);	//��ʼ����ֱ������
	} catch(e) {
		alert("�ļ��������� ! ��ȡ�ļ�����");
		exit(0);
	}

	return true;			//�����˳�
}
//��ȡ��·
Manager.ReadFile = function(newFileName) {
	ASSERT(newFileName && newFileName.length > 0);
	$.post("/testData.json", {}, readFileCallbackFunc)
	return true;
};

//�������ļ�(�յ�)
Manager.CreateFile = function() {
	Manager.fileName = '';													//·�����
	Manager.ClearCircuitState();											//�����·״̬��Ϣ
	Manager.crun.length = Manager.ctrl.length = Manager.lead.length = 0;	//����������Ϊ0
};
