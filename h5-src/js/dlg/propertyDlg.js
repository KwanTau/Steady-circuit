
var MyPropertyDlg = {
	CreateNew : function(list, readOnly, model, windowTitle) {
		var inter = {x: 20, y: 15};
		var firstNotePos = {x: 20, y: 20};
		var noteTextSize = {cx: 180, cy: 20};
		var firstCtrlPos = {x: firstNotePos.x + noteTextSize.cx + inter.x, y: firstNotePos.y};	//��һ���ؼ���ʼ����
		var ctrlSize = {cx: 300, cy: noteTextSize.cy};	//�ؼ���С
		var okButtonPos = {x: firstCtrlPos.x, y: firstCtrlPos.y + (ctrlSize.cy + inter.y) * list.GetListSize()};
		var cancelButtonPos = {x: okButtonPos.x + 150 + m_inter.x, y: okButtonPos.y};
		var wndSize = {cx: firstCtrlPos.x + ctrlSize.cx + inter.x + 10, cy: cancelButtonPos.y + 70};
			
		return {
			m_windowTitle: windowTitle,	//��������
			m_model: model,				//ʾ��
			m_readOnly: readOnly,		//�Ƿ�ֻ��
			m_list: list,				//�����б�

			m_inter: inter,	// x:�ؼ� �� ������ʾtext �ļ��; y:�ؼ�֮�� ���� ������ʾtext ֮�� �ļ��

			m_firstNotePos: firstNotePos,	//��һ��������ʾtext��ʼ����
			m_noteTextSize: noteTextSize,	//������ʾtext��С

			m_firstCtrlPos: firstCtrlPos,	//��һ���ؼ���ʼ����
			m_ctrlSize: ctrlSize,			//�ؼ���С
			
			m_okButtonPos: okButtonPos,	//ȷ����ť������
			m_cancelButtonPos: cancelButtonPos,	//ȡ����ť������
			
			m_wndSize: wndSize,	//���ڴ�С
			
			__proto__: MyPropertyDlg
		};
	},

	OnInitDialog : function() {
		//1,��ʼ������λ�úʹ�С
		RECT rect;
		GetParent().GetWindowRect(&rect);
		if (rect.bottom > m_wndSize.cy) {
			rect.left += (rect.right  - rect.left)/2 - m_wndSize.cx/2;
			rect.top  += (rect.bottom - rect.top) /2 - m_wndSize.cy/2;
		}
		MoveWindow(rect.left, rect.top, m_wndSize.cx, m_wndSize.cy);
		if (m_windowTitle) SetWindowText(m_windowTitle);

		//2,��ʼ��ȷ��,ȡ����ť,ʾ��Label
		GetDlgItem(IDOK).MoveWindow(m_okButtonPos.x, m_okButtonPos.y, 130, 25);
		GetDlgItem(IDCANCEL).MoveWindow(m_cancelButtonPos.x, m_cancelButtonPos.y, 130, 25);
		GetDlgItem(IDC_STATIC_MODELTEXT).MoveWindow(m_firstNotePos.x, m_okButtonPos.y, 40, 25);
		if (m_model != NULL) SetDlgItemText(IDC_STATIC_MODELTEXT, "ʾ��");

		//3,��ʼ��ÿ�����ݶ�Ӧ�Ŀؼ�
		char tempStr[NAME_LEN];
		class ENUM_STYLE * tempEnumStyle;

		for (int i = m_list.GetListSize()-1; i>=0; --i) {
			rect.left = m_firstNotePos.x;
			rect.right = m_noteTextSize.cx;
			rect.top = m_firstNotePos.y + i * (m_noteTextSize.cy + m_inter.y);
			rect.bottom = m_noteTextSize.cy;
			CreateLabel(rect, m_list.noteText[i], NOTEID(i));

			rect.left = m_firstCtrlPos.x;
			rect.right = m_ctrlSize.cx;
			rect.top = m_firstCtrlPos.y + i * (m_ctrlSize.cy + m_inter.y);
			rect.bottom = m_ctrlSize.cy;

			switch (m_list.listStyle[i]) {
			case DATA_STYLE_float:
				sprintf(tempStr, "%f", *((float *)m_list.dataPoint[i]));
				new MyEditCtrl(
					m_hWnd, 
					rect, 
					tempStr, 
					CTRLID(i), 
					m_list.listStyle[i], 
					m_readOnly);
				break;

			case DATA_STYLE_double:
				sprintf(tempStr, "%f", *((double *)m_list.dataPoint[i]));
				new MyEditCtrl(
					m_hWnd, 
					rect, 
					tempStr, 
					CTRLID(i), 
					m_list.listStyle[i], 
					m_readOnly);
				break;

			case DATA_STYLE_UINT:
				itoa(*((int *)m_list.dataPoint[i]), tempStr, 10);
				new MyEditCtrl(
					m_hWnd, 
					rect, 
					tempStr, 
					CTRLID(i), 
					m_list.listStyle[i], 
					m_readOnly);
				break;

			case DATA_STYLE_bool:
				CreateCheck(rect, *(bool *)m_list.dataPoint[i], CTRLID(i));
				break;

			case DATA_STYLE_LPCTSTR://char[NAME_LEN]
				new MyEditCtrl(
					m_hWnd, 
					rect, 
					(char *)m_list.dataPoint[i], 
					CTRLID(i), 
					m_list.listStyle[i], 
					m_readOnly);
				break;

			case DATA_STYLE_enum:
				tempEnumStyle = (ENUM_STYLE *)m_list.dataPoint[i];
				CreateCombo(
					rect,
					tempEnumStyle.GetStyleNum(),
					*(tempEnumStyle.GetDataPoint()),
					tempEnumStyle.GetNote(),
					CTRLID(i));
				break;
			}
		}

		return TRUE;  // return TRUE  unless you set the focus to a control
	},

	OnPaint : function() {
		if (IsIconic()) {
			CPaintDC dc(this);
		} else {
			CDialog::OnPaint();

			if (m_model != NULL) {
				CClientDC dc(this);
				BITMAP buf;
				m_model.GetCurrentBitmap().GetBitmap(&buf);
				dc.BitBlt(100, m_okButtonPos.y, buf.bmWidth, buf.bmHeight, m_model, 0, 0, SRCAND);
			}
		}
	},
	
	//�ؼ�����---------------------------------------------------------------------
	//����label�ؼ�
	CreateLabel : function(RECT rect, LPCTSTR lpszWindowName, UINT nID) {
		CWnd * pWnd = new CStatic;
		DWORD ctrlStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE;
		if(nID & 2)ctrlStyle ^= SS_SUNKEN;

		pWnd.CreateEx(
			0,
			_T("STATIC"), lpszWindowName,
			ctrlStyle ,
			rect.left, rect.top, rect.right, rect.bottom,
			m_hWnd, (HMENU)nID);
	},

	//����check�ؼ�
	CreateCheck : function(RECT rect, bool check, UINT nID) {
		CWnd * pWnd = new CButton;
		DWORD ctrlStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_AUTOCHECKBOX;

		pWnd.CreateEx(
			WS_EX_CLIENTEDGE,
			_T("BUTTON"), NULL,
			ctrlStyle,
			rect.left, rect.top, rect.right, rect.bottom,
			m_hWnd, (HMENU)nID);
		((CButton *)pWnd).SetCheck(check);

		if (m_readOnly)	//ֻ��
			pWnd.EnableWindow(false);
	},

	//����ComboBox�ؼ�
	CreateCombo : function(RECT rect, int num, int select, const char ** comboNotes, UINT nID) {
		CComboBox * pWnd = new CComboBox;
		DWORD ctrlStyle = CBS_DROPDOWNLIST | CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP;
		rect.right += rect.left;
		rect.bottom = rect.top + num * 30;

		pWnd.Create(ctrlStyle, rect, this, nID);

		for (int i=0; i<num; ++i)
			pWnd.AddString(comboNotes[i]);

		ASSERT(select>=0 && select<num);
		pWnd.SetCurSel(select);

		if (m_readOnly)	//ֻ��
			pWnd.EnableWindow(false);
	},


	//��Ϣ������---------------------------------------------------------------------
	DestroyWindow : function() {
		CWnd * t;

		for (int i = m_list.GetListSize()-1; i>=0; --i) {
			t = GetDlgItem(NOTEID(i));
			t.DestroyWindow();
			t.Detach();
			delete t;

			t = GetDlgItem(CTRLID(i));
			t.DestroyWindow();
			t.Detach();
			delete t;
		}

		return CDialog::DestroyWindow();
	},

	//��ȷ����ť
	OnOK : function() {
		if (m_readOnly) {	//ֻ��״̬��������Ч��Ϣ
			CDialog::OnOK();
			return;
		}

		ERROR_TYPE errorType = ERROR_NO;
		int i;

		//��������
		for (i = m_list.GetListSize()-1; i>=0; --i) {
			errorType = m_list.CheckAMember(i, GetDlgItem(CTRLID(i)));
			if(errorType != ERROR_NO) break;
		}

		//������ʾ
		if (errorType != ERROR_NO) {
			CString showText;
			char errorText[NAME_LEN*2];
			char nameText[NAME_LEN];

			switch (errorType) { //��������
			case ERROR_STRNULL:
				strcpy(errorText, "�������Ϊ��!");
				break;
			case ERROR_FLOATMIX:
				strcpy(errorText, "������������ֻ�������ֺ����һ��С����!");
				break;
			case ERROR_UINTMIX:
				strcpy(errorText, "������������������������ַ�!");
				break;
			case ERROR_UINTOVER:
				strcpy(errorText, "������������ķ�Χ��!");
				break;
			case ERROR_ENUMOVER:
				strcpy(errorText, "û��ѡ��ѡ���е�ĳһ��!");
				break;
			case ERROR_STRMIX:
				strcpy(errorText, "��ǩ�в��ܰ��� [ ] ( ) { }");
				break;
			case ERROR_ENUMNOTALLOWED:
				strcpy(errorText, "ѡ��������ѡ����ѧԪ����ɫ����Ϊ��ɫ !");
				break;
			}

			if (1 == m_list.GetListSize()) {
				showText.Format("%s\n����������!", errorText);
			} else {
				GetDlgItemText(NOTEID(i), nameText, NAME_LEN);	//�������ǩ
				showText.Format("��%d��������:\n\t%s\n%s\n����������!", i+1, nameText, errorText);
			}
			MessageBox(showText);
			GetDlgItem(CTRLID(i)).SetFocus();	//���ݲ��Ϸ��ؼ���ý���
			return;
		}

		//���Գɹ�д������
		for (i = m_list.GetListSize()-1; i>=0; --i)
			m_list.SaveAMember(i, GetDlgItem(CTRLID(i)));

		CDialog::OnOK();
	}
};
