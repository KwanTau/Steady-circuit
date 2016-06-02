
var CTRL_BITMAP_TYPE_COUNT = 7;					//�ؼ�λͼ����ĸ���
var CTRL_BITMAP_COUNT = CTRL_BITMAP_TYPE_COUNT*4;	//�ؼ�λͼ�ĸ���(������ת֮���)

var FILE_VERSION		= 13;					//�ļ��汾,��ͬ�汾�ļ������ȡ
var FILE_RESERVE_SIZE	= 256;					//�ļ�������Ĵ�С
var MAXMOVEBODYDIS		= 50;					//ʹ�÷����һ���ƶ�������뷶Χ1~MAX_MOVEBODYDIS
var MAXLEAVEOUTDIS		= 15;					//���ڵ��ߺϲ����뷶Χ1~MAX_LEAVEOUTDIS



//1��ʼ����������------------------------------------------------------------
var Manager = {
	//��ͼ����---------------------------------------------------------------------
	CDC ctrlDcMem[CTRL_BITMAP_COUNT];
	CBitmap ctrlBitmap[CTRL_BITMAP_COUNT];	//CTRL_BITMAP_NUM���ؼ�λͼ
	CDC showConnectDcMem;
	CBitmap showConnectBitmap;		//�����λͼ
	CDC crunDcMem;
	CBitmap crunBitmap;				//���λͼ

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
	CWnd * wndPointer;			//��ǰ����ָ��
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


	CreateNew: function(outWnd) {
		var i;
		
		//������ʾ-------------------------------------------------------
		this.wndPointer = outWnd;		//��ǰ����ָ��
		ctx = wndPointer->GetDC();	//��ǰ�����豸������

		bitmapForRefresh.CreateBitmap(1, 1, 1, 32, NULL);	//ʹˢ�²�����ʹ�õ�bitmap
		dcForRefresh.CreateCompatibleDC(ctx);				//ʹˢ�²�����ʹ�õ�DC
		dcForRefresh.SelectObject(&bitmapForRefresh);


		//��Ա���-------------------------------------------------------
		viewOrig.x = viewOrig.y = 0;					//�ӽǳ�ʼ����
		mouseWheelSense.cx = mouseWheelSense.cy = 32;	//mouseWheel������
		moveBodySense = 3;								//���������Ҽ�����һ���ƶ��ľ���
		maxLeaveOutDis = 7;								//���ߺϲ�������


		//��·Ԫ������---------------------------------------------------
		ZeroMemory(crun, sizeof(void *) * MAX_CRUN_COUNT);
		ZeroMemory(ctrl, sizeof(void *) * MAX_CTRL_COUNT);
		ZeroMemory(lead, sizeof(void *) * MAX_LEAD_COUNT);
		crunCount = leadCount = ctrlCount = 0;	//����ĸ�������


		//�������Ϣ��¼-----------------------------------------------
		motiCount = 0;
		addState = BODY_NO;
		lButtonDownPos.x = -100;
		lButtonDownState = false;
		isUpRecvAfterDown = true;
		FocusBodyClear(NULL);


		//��ͼ����-------------------------------------------------------
		textColor = BLACK;						//Ĭ��������ɫ
		focusLeadStyle = SOLID_RESERVE_COLOR;	//Ĭ�Ͻ��㵼����ʽ
		focusCrunColor = GREEN;					//Ĭ�Ͻ�������ɫ
		focusCtrlColor = RED;					//Ĭ�Ͻ���ؼ���ɫ
		InitBitmap();							//��ʼ��λͼ

		//����
		for(i=COLOR_TYPE_NUM-1; i>=0; --i) 
			hp[i].CreatePen(PS_SOLID, 1, LEADCOLOR[i]);

		//���ͼ��
		/*HINSTANCE hinst = AfxGetInstanceHandle();
		hcSizeNS		= LoadCursor(NULL,	IDC_SIZENS);
		hcSizeWE		= LoadCursor(NULL,	IDC_SIZEWE);
		hcShowConnect	= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_SHOWCONNECT));
		hcHand			= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_HAND));
		hcMoveHorz		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_HORZ_LEAD));
		hcMoveVert		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_VERT_LEAD));
		hcAddCrun		= LoadCursor(hinst,	MAKEINTRESOURCE(IDC_CURSOR_ADDCRUN));*/


		//��ȡ�ļ�-------------------------------------------------------
		vectorPos = NULL;
		fileName[0] = '\0';
		PutCircuitToVector();	//����ǰ�յ�·��Ϣ���浽����
	},

	//��ʼ��λͼ���
	InitBitmap: function() {
		int i, j, k, l;
		UINT * buf1, * buf2, * p;

		//�����λͼ------------------------------------
		showConnectDcMem.CreateCompatibleDC(ctx);
		showConnectBitmap.LoadBitmap(IDB_SMALLCRUN);
		showConnectDcMem.SelectObject(&showConnectBitmap);

		//�ڵ�λͼ--------------------------------------
		crunDcMem.CreateCompatibleDC(ctx);
		crunBitmap.LoadBitmap(IDB_CRUN);
		crunDcMem.SelectObject(&crunBitmap);

		//�ؼ�λͼ,����õ���ת�ؼ�---------------------
		buf1 = (UINT *)malloc(BODYSIZE.cx * BODYSIZE.cy * 4);
		buf2 = (UINT *)malloc(BODYSIZE.cx * BODYSIZE.cy * 4);

		for(k=CTRL_BITMAP_TYPE_COUNT-1; k>=0; --k)
		{
			//ԭλͼ
			ctrlDcMem[k].CreateCompatibleDC(ctx);
			ctrlBitmap[k].LoadBitmap(IDB_SOURCE + k);
			ctrlDcMem[k].SelectObject(ctrlBitmap + k);
			ctrlBitmap[k].GetBitmapBits(BODYSIZE.cx*BODYSIZE.cy*4, buf1);	//���ԭλͼ����

			//�����תλͼ
			for(l=1; l<4; ++l)
			{
				p = buf1 + (BODYSIZE.cy - 1) * BODYSIZE.cx + BODYSIZE.cx - 1;
				for(i = BODYSIZE.cy - 1; i >= 0; --i) for(j = BODYSIZE.cx - 1; j >= 0; --j)
					* ( buf2 + j * BODYSIZE.cx + BODYSIZE.cx - 1 - i) = * p --;

				i = k + CTRL_BITMAP_TYPE_COUNT*l;
				ctrlDcMem[i].CreateCompatibleDC(ctx);
				ctrlBitmap[i].CreateBitmap(BODYSIZE.cx, BODYSIZE.cy, 1, 32, buf2);
				ctrlDcMem[i].SelectObject(ctrlBitmap + i);

				p = buf1;
				buf1 = buf2;
				buf2 = p;
			}
		}

		free(buf1);
		free(buf2);
	}

};
