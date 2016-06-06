
var CTRL_BITMAP_COUNT = CTRL_TYPE_COUNT*8;	//�ؼ�λͼ�ĸ���(������ת֮���)

//1��ʼ����������------------------------------------------------------------
var Manager = {

	// ��ʼ�����ӵ�λͼ
	CreateShowConnectImageData: function() {
		var len = 16*9;
		var imgData = Manager.ctx.createImageData(6,6);
		
		for (var i=8; i<len; i+=4) {
			imgData.data[i+0]=0;
			imgData.data[i+1]=0;
			imgData.data[i+2]=0;
			imgData.data[i+3]=255;
		}
		imgData.data[3]=imgData.data[7]=imgData.data[19]=imgData.data[23]=imgData.data[27]=imgData.data[47]=0;
		imgData.data[len-1]=imgData.data[len-5]=imgData.data[len-17]=imgData.data[len-21]=imgData.data[len-25]=imgData.data[len-45]=0;
	},
	
	// ��ָ����ɫ��ʼ��һ���ڵ�
	CreateCrunImageWithColor: function(colorHex) {
		var r = PaintCommonFunc.RedOfHexRGB(colorHex);
		var g = PaintCommonFunc.GreenOfHexRGB(colorHex);
		var b = PaintCommonFunc.BlueOfHexRGB(colorHex);
		
		var len = 16*DD*DD;
		var imgData = Manager.ctx.createImageData(2*DD,2*DD);
		
		for (var i=8; i<len; i+=4) {
			imgData.data[i+0]=r;
			imgData.data[i+1]=g;
			imgData.data[i+2]=b;
			imgData.data[i+3]=255;
		}
		imgData.data[3]=imgData.data[7]=imgData.data[DD*8-5]=imgData.data[DD*8-1]=imgData.data[DD*8+3]=imgData.data[DD*16-1]=0;
		imgData.data[len-1]=imgData.data[len-5]=imgData.data[len-DD*8+7]=imgData.data[len-DD*8+3]=imgData.data[len-DD*8-1]=imgData.data[len-DD*16+3]=0;
	},
	// ��ʼ�����нڵ�λͼ
	CreateAllCrunImageData: function() {
		var crunImageData = new Array(PAINT_CRUN_STYLE_COUNT);
		crunImageData[PAINT_CRUN_STYLE_NORMAL] = Manager.CreateCrunImageWithColor(COLOR_NORMAL);
		crunImageData[PAINT_CRUN_STYLE_FOCUS] = Manager.CreateCrunImageWithColor(COLOR_FOCUS);
		crunImageData[PAINT_CRUN_STYLE_SPECIAL] = Manager.CreateCrunImageWithColor(COLOR_SPECIAL);
		
		Manager.crunImageData = crunImageData;
	},
	
	//��ʼ��λͼ���
	InitBitmap: function() {
		//�����λͼ
		Manager.showConnectImageData = Manager.CreateShowConnectImageData();

		//�ڵ�λͼ
		Manager.CreateAllCrunImageData();

		//�ؼ�λͼ,����õ���ת�ؼ�
		Manager.ctrlImageList = new Array(CTRL_BITMAP_COUNT);
		for (var i=0; i<CTRL_TYPE_COUNT; ++i) {
			Manager.ctrlImageList[i*4] = document.getElementById("N-"+(i+1)+"-0");
			Manager.ctrlImageList[i*4+1] = document.getElementById("N-"+(i+2)+"-1");
			Manager.ctrlImageList[i*4+2] = document.getElementById("N-"+(i+3)+"-2");
			Manager.ctrlImageList[i*4+3] = document.getElementById("N-"+(i+4)+"-3");
		}
		for (var i=0; i<CTRL_TYPE_COUNT; ++i) {
			Manager.ctrlImageList[CTRL_TYPE_COUNT*4 + i*4] = document.getElementById("S-"+(i+1)+"-0");
			Manager.ctrlImageList[CTRL_TYPE_COUNT*4 + i*4+1] = document.getElementById("S-"+(i+2)+"-1");
			Manager.ctrlImageList[CTRL_TYPE_COUNT*4 + i*4+2] = document.getElementById("S-"+(i+3)+"-2");
			Manager.ctrlImageList[CTRL_TYPE_COUNT*4 + i*4+3] = document.getElementById("S-"+(i+4)+"-3");
		}
		for (var i=0; i<CTRL_BITMAP_COUNT; ++i) {
			if (Manager.ctrlImageList[i]) {
				Manager.ctx.drawImage(Manager.ctrlImageList[i], 0,0);
				Manager.ctrlImageList[i] = Manager.ctx.getImageData(0,0, CTRL_SIZE.cx,CTRL_SIZE.cy);
			}
		}
		Manager.ctx.fillStyle = "#FFFFFF";
		Manager.ctx.fillRect(0,0, CTRL_SIZE.cx,CTRL_SIZE.cy);
	},
	
	Init: function(canvas) {
		//������ʾ-------------------------------------------------------
		Manager.canvas = canvas;
		Manager.ctx = Manager.canvas.getContext("2d");

		//Manager.bitmapForRefresh.CreateBitmap(1, 1, 1, 32, null);	//ʹˢ�²�����ʹ�õ�bitmap
		//Manager.dcForRefresh.CreateCompatibleDC(ctx);				//ʹˢ�²�����ʹ�õ�DC
		//Manager.dcForRefresh.SelectObject(bitmapForRefresh);


		//��Ա���-------------------------------------------------------
		Manager.viewOrig = {x:0, y:0};				//�ӽǳ�ʼ����
		Manager.mouseWheelSense = {cx:32, cy:32};	//mouseWheel������
		Manager.moveBodySense = 3;					//���������Ҽ�����һ���ƶ��ľ���
		Manager.maxLeaveOutDis = 7;					//���ߺϲ�������


		//��·Ԫ������---------------------------------------------------
		Manager.crun = new Array();
		Manager.ctrl = new Array();
		Manager.lead = new Array();


		//�������Ϣ��¼-----------------------------------------------
		Manager.motiCount = 0;
		Manager.addState = BODY_NO;
		Manager.lButtonDownPos = {x:-100, y:-100};
		Manager.lButtonDownState = false;
		Manager.isUpRecvAfterDown = true;
		//Manager.FocusBodyClear(null);


		//��ͼ����-------------------------------------------------------
		Manager.textColor = COLOR_NORMAL;				//Ĭ��������ɫ
		Manager.focusLeadStyle = SOLID_SPECIAL_COLOR;	//Ĭ�Ͻ��㵼����ʽ
		Manager.focusCrunColor = COLOR_FOCUS;			//Ĭ�Ͻ�������ɫ
		Manager.focusCtrlColor = COLOR_FOCUS;			//Ĭ�Ͻ���ؼ���ɫ
		Manager.InitBitmap();							//��ʼ��λͼ

		//���ͼ��
		/*HINSTANCE hinst = AfxGetInstanceHandle();
		Manager.hcSizeNS		= LoadCursor(null,	IDC_SIZENS);
		Manager.hcSizeWE		= LoadCursor(null,	IDC_SIZEWE);
		Manager.hcShowConnect	= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_SHOWCONNECT));
		Manager.hcHand			= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_HAND));
		Manager.hcMoveHorz		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_HORZ_LEAD));
		Manager.hcMoveVert		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_VERT_LEAD));
		Manager.hcAddCrun		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_ADDCRUN));*/


		//��ȡ�ļ�-------------------------------------------------------
		Manager.fileName = "";
	}

};
