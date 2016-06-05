
var CTRL_BITMAP_TYPE_COUNT = CTRL_TYPE_COUNT*2;					//�ؼ�λͼ����ĸ���
var CTRL_BITMAP_COUNT = CTRL_BITMAP_TYPE_COUNT*4;	//�ؼ�λͼ�ĸ���(������ת֮���)

var FILE_VERSION		= 13;					//�ļ��汾,��ͬ�汾�ļ������ȡ
var MAX_MOVE_BODY_DIS	= 50;					//ʹ�÷����һ���ƶ�������뷶Χ1~MAX_MOVEBODYDIS
var MAX_LEAVE_OUT_DIS	= 15;					//���ڵ��ߺϲ����뷶Χ1~MAX_LEAVEOUTDIS



//1��ʼ����������------------------------------------------------------------
var Manager = {
	/*
	//��ͼ����---------------------------------------------------------------------
	CBitmap Manager.ctrlImageList[CTRL_BITMAP_COUNT];	//CTRL_BITMAP_NUM���ؼ�λͼ
	CDC showConnectImageData;
	CBitmap showConnectImageData;		//�����λͼ
	CDC crunImageData;
	CBitmap crunImageData;				//���λͼ

	CPen hp[COLOR_TYPE_NUM];		//COLOR_TYPE_NUM����ɫ����

	HCURSOR hcSizeNS;				//�ϱ�˫��ͷ
	HCURSOR hcSizeWE;				//����˫��ͷ
	HCURSOR hcShowConnect;			//���ӵ���ʱ���
	HCURSOR hcHand;					//��
	HCURSOR hcMoveHorz;				//�ƶ���ֱ����ʱ��ʾ
	HCURSOR hcMoveVert;				//�ƶ�ˮƽ����ʱ��ʾ
	HCURSOR hcAddCrun;				//�������ʱ��ʾ

	enum COLOR textColor;			//������ɫ
	enum LEADSTYLE focusLeadStyle;	//���㵼����ʽ
	enum COLOR focusCrunColor;		//��������ɫ
	enum COLOR focusCtrlColor;		//����ؼ���ɫ

	//������ʾ---------------------------------------------------------------------
	CWnd * canvas;			//��ǰ����ָ��
	CDC  * ctx;					//��ǰ�����豸������
	CDC dcForRefresh;			//ʹˢ�²�����ʹ�õ�DC
	CBitmap bitmapForRefresh;	//ʹˢ�²�����ʹ�õ�bitmap

	//��·Ԫ������-----------------------------------------------------------------
	CRUN	* crun[MAX_CRUN_COUNT];	//�洢���
	CTRL	* ctrl[MAX_CTRL_COUNT];	//�洢�ؼ�
	LEAD	* lead[MAX_LEAD_COUNT];	//�洢����
	unsigned short crunCount;		//���ĸ���
	unsigned short leadCount;		//�ؼ��ĸ���
	unsigned short ctrlCount;		//���ߵĸ���

	//��Ա���---------------------------------------------------------------------
	POINT viewOrig;			//�ӽǳ�ʼ����
	SIZE mouseWheelSense;	//mouseWheel������
	UINT moveBodySense;		//���������Ҽ�һ�������ƶ��ľ���
	UINT maxLeaveOutDis;	//һ�ε���,�������ںϲ��ٽ����

	//�������Ϣ��¼-------------------------------------------------------------
	BODY_TYPE addState;		//��¼Ҫ��ӵ���������
	unsigned short motiCount;	//��¼��꼤��Ĳ����ĸ���,LBUTTONDOWN��Ϣ�����
	Pointer motiBody[2];	//��¼��꼤��Ĳ���
	POINT lButtonDownPos;	//��¼��һ�����������µ�����
	//�ϴ�����Ƿ���������,��LBUTTONDOWNʱ��¼,��LBUTTONUP�����ж�
	bool lButtonDownState;
	//��¼WM_LBUTTONDOWN������һ�����WM_LBUTTONDOWN����ǰ,��û�н��ܵ�WM_LBUTTONUP
	bool isUpRecvAfterDown;

	//����ƶ���Ϣ��¼-------------------------------------------------------------
	Pointer lastMoveOnBody;	//��һ����꼤��Ĳ���,MOUSEMOVE��Ϣ�����
	POINT lastMoveOnPos;	//��¼�ϴ�����ƶ���������,�����ƶ�����

	//����������������---------------------------------------------------------
	Pointer focusBody;		//��������

	//���а����-------------------------------------------------------------------
	Pointer clipBody;		//ָ����а�������,�����ڵ�ǰ��·,����һ���ڴ�,��Ҫ�ͷ�

	//�ļ�����·������-------------------------------------------------------------
	char fileName[256];		//���浱ǰ���в������ļ�·��

	//��ʾ���Ʋ����---------------------------------------------------------------
	Pointer pressStart;			//������Ʋ����ʼλ��,ֻ��ָ���߻��߽ڵ�
	Pointer pressEnd;			//������Ʋ�Ľ���λ��,ֻ��ָ���߻��߽ڵ�
	double startEndPressure;	//��¼pressStart��pressEnd֮��ĵ��Ʋ�
*/
	
	// ��ʼ�����ӵ�λͼ
	CreateShowConnectImageData: function() {
		var len = 16*9;
		var imgData = Manager.ctx.createImageData(6,6);
		
		for (var i=8; i<len; i+=4) {
			imgData.data[i+0]=r;
			imgData.data[i+1]=g;
			imgData.data[i+2]=b;
			imgData.data[i+3]=255;
		}
		imgData.data[3]=imgData.data[7]=imgData.data[19]=imgData.data[23]=imgData.data[27]=imgData.data[47]=0;
		imgData.data[len-1]=imgData.data[len-5]=imgData.data[len-17]=imgData.data[len-21]=imgData.data[len-25]=imgData.data[len-45]=0;
	},
	
	// ��ָ����ɫ��ʼ��һ���ڵ�
	CreateCrunImageWithColor: function(r,g,b) {
		var len = 16*DD*DD;
		var imgData = Manager.ctx.createImageData(2*DD,2*DD);
		
		for (var i=8; i<len; i+=4) {
			imgData.data[i+0]=r;
			imgData.data[i+1]=g;
			imgData.data[i+2]=b;
			imgData.data[i+3]=255;
		}
		imgData.data[3]=imgData.data[7]=imgData.data[D*8-5]=imgData.data[D*8-1]=imgData.data[D*8+3]=imgData.data[D*16-1]=0;
		imgData.data[len-1]=imgData.data[len-5]=imgData.data[len-D*8+7]=imgData.data[len-D*8+3]=imgData.data[len-D*8-1]=imgData.data[len-D*16+3]=0;
	},
	// ��ʼ��һ����ת��ɫ�ڵ�
	/*CreateInverseCrunImage: function() {
		var len = 16*DD*DD;
		var imgData = Manager.ctx.createImageData(2*DD,2*DD);
		
		imgData.data[0]=imgData.data[1]=imgData.data[2]=imgData.data[3]=255;
		imgData.data[4]=imgData.data[5]=imgData.data[6]=imgData.data[7]=255;
		imgData.data[D*8-8]=imgData.data[D*8-7]=imgData.data[D*8-6]=imgData.data[D*8-5]=255;
		imgData.data[D*8-4]=imgData.data[D*8-3]=imgData.data[D*8-2]=imgData.data[D*8-1]=255;
		imgData.data[D*8]=imgData.data[D*8+1]=imgData.data[D*8+2]=imgData.data[D*8+3]=255;
		imgData.data[D*16-4]=imgData.data[D*16-3]=imgData.data[D*16-2]=imgData.data[D*16-1]=255;
		
		imgData.data[len-4]=imgData.data[len-3]=imgData.data[len-2]=imgData.data[len-1]=255;
		imgData.data[len-8]=imgData.data[len-7]=imgData.data[len-6]=imgData.data[len-5]=255;
		imgData.data[len-D*8+4]=imgData.data[len-D*8+5]=imgData.data[len-D*8+6]=imgData.data[len-D*8+7]=255;
		imgData.data[len-D*8]=imgData.data[len-D*8+1]=imgData.data[len-D*8+2]=imgData.data[len-D*8+3]=255;
		imgData.data[len-D*8-4]=imgData.data[len-D*8-3]=imgData.data[len-D*8-2]=imgData.data[len-D*8-1]=255;
		imgData.data[len-D*16]=imgData.data[len-D*16+1]=imgData.data[len-D*16+2]=imgData.data[len-D*16+3]=255;
	},*/
	// ��ʼ�����нڵ�λͼ
	CreateAllCrunImageData: function() {
		var crunImageData = new Array(PAINT_CRUN_STYLE_COUNT);
		crunImageData[PAINT_CRUN_STYLE_NORMAL] = Manager.CreateCrunImageWithColor(0,0,0);
		crunImageData[PAINT_CRUN_STYLE_FOCUS] = Manager.CreateCrunImageWithColor(30,250,30);
		crunImageData[PAINT_CRUN_STYLE_SPECIAL] = Manager.CreateCrunImageWithColor(190,30,100);
		
		Manager.crunImageData = crunImageData;
	},
	
	//��ʼ��λͼ���
	InitBitmap: function() {
		int i, j, k, l;
		UINT * buf1, * buf2, * p;

		//�����λͼ------------------------------------
		Manager.showConnectImageData = Manager.CreateShowConnectImageData();

		//�ڵ�λͼ--------------------------------------
		Manager.CreateAllCrunImageData();

		//�ؼ�λͼ,����õ���ת�ؼ�---------------------
		Manager.ctrlImageList = new Array(CTRL_BITMAP_COUNT);
	},
	
	Init: function(canvas) {
		//������ʾ-------------------------------------------------------
		Manager.canvas = canvas;
		Manager.ctx = Manager.canvas.getContext("2d");

		Manager.bitmapForRefresh.CreateBitmap(1, 1, 1, 32, null);	//ʹˢ�²�����ʹ�õ�bitmap
		Manager.dcForRefresh.CreateCompatibleDC(ctx);				//ʹˢ�²�����ʹ�õ�DC
		Manager.dcForRefresh.SelectObject(&bitmapForRefresh);


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
		Manager.FocusBodyClear(null);


		//��ͼ����-------------------------------------------------------
		Manager.textColor = BLACK;						//Ĭ��������ɫ
		Manager.focusLeadStyle = SOLID_RESERVE_COLOR;	//Ĭ�Ͻ��㵼����ʽ
		Manager.focusCrunColor = GREEN;					//Ĭ�Ͻ�������ɫ
		Manager.focusCtrlColor = RED;					//Ĭ�Ͻ���ؼ���ɫ
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
