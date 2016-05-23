/* �Ⱥ��·��ѧģ����
   ��Ȩ���У�C�� 2013 <����>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
   
// �Ⱥ��·Dlg.cpp : implementation file
//

#include "StdAfx.h"
#include "�Ⱥ��·.h"
#include "StaticClass.h"	//����static��������
#include "Manager.h"		//�����·��
#include "MySearchDlg.h"	//�����Ի���
#include "�Ⱥ��·Dlg.h"	//��ǰ��

extern CMyApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	ON_COMMAND_RANGE(IDM_ADD_CRUNODE, IDM_ADD_SWITCH, OnSetAddState)
	ON_COMMAND_RANGE(IDM_POSBODY_ROTATE1, IDM_POSBODY_ROTATE3, OnPosBodyRotateCtrl)
	ON_COMMAND_RANGE(IDM_FOCUSBODY_ROTATE1, IDM_FOCUSBODY_ROTATE3, OnFocusBodyRotateCtrl)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_HELPINFO()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_ABOUT, OnAbout)


	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()


	ON_COMMAND(IDM_FILE_NEW, OnFileNew)
	ON_COMMAND(IDM_FILE_OPEN, OnFileOpen)
	ON_COMMAND(IDM_FILE_SAVE, OnFileSave)
	ON_COMMAND(IDM_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(IDM_SAVEASPIC, OnSaveAsPicture)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_WM_DROPFILES()


	ON_COMMAND(IDM_FOCUSBODY_CUT, OnFocusBodyCut)
	ON_COMMAND(IDM_FOCUSBODY_COPY, OnFocusBodyCopy)
	ON_COMMAND(IDM_FOCUSBODY_DELETE, OnFocusBodyDelete)
	ON_COMMAND(IDM_UNDO, OnUnDo)
	ON_COMMAND(IDM_REDO, OnReDo)
	ON_COMMAND(IDM_FOCUSBODY_PROPERTY, OnFocusBodyProperty)
	ON_COMMAND(IDM_FOCUSBODY_CHANGECTRLSTYLE, OnFocusBodyChangeCtrlStyle)
	ON_COMMAND(IDM_FOCUSBODY_SHOWELEC, OnFocusBodyShowElec)
	ON_COMMAND(IDM_SEARCH, OnSearch)


	ON_COMMAND(IDM_SETMOVEBODYSENSE, OnSetMoveBodySense)
	ON_COMMAND(IDM_SETLEAVEOUTDIS, OnSetLeaveOutDis)
	ON_COMMAND(IDM_SETTEXTCOLOR, OnSetTextColor)
	ON_COMMAND(IDM_SETFOCUSLEADSTYLE, OnSetFocusLeadStyle)
	ON_COMMAND(IDM_SETFOCUSCRUNCOLOR, OnSetFocusCrunColor)
	ON_COMMAND(IDM_SETFOCUSCTRLCOLOR, OnSetFocusCtrlColor)


	ON_COMMAND(IDM_SAVETOTEXTFILE, OnSaveTextFile)
	ON_COMMAND(IDM_MAKEMAP, OnMakeMap)


	ON_COMMAND(IDM_COUNTI, OnCountElec)
	ON_COMMAND(IDM_SHOWPRESSURE, OnShowPressure)
	ON_COMMAND(IDM_POSBODY_SHOWELEC, OnPosBodyShowElec)
	ON_COMMAND(IDM_RELEASE, OnUnLock)


	ON_COMMAND(IDM_POSBODY_COPY, OnPosBodyCopy)
	ON_COMMAND(IDM_POSBODY_CUT, OnPosBodyCut)
	ON_COMMAND(IDM_POSBODY_DELETE, OnPosBodyDelete)
	ON_COMMAND(IDM_PASTE, OnPaste)
	ON_COMMAND(IDM_DELETELEAD, OnDeleteLead)
	ON_COMMAND(IDM_POSBODY_PROPERTY, OnPosBodyProperty)
	ON_COMMAND(IDM_POSBODY_CHANGECTRLSTYLE, OnPosBodyChangeCtrlStyle)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// private function
void CMyDlg::LockInput() 
//��������
{
	m_inputLock = true;

	//��Ҫ���εĲ˵�
	m_hm->EnableMenuItem(0					, MF_GRAYED|MF_BYPOSITION);	//�ļ�����
	m_hm->EnableMenuItem(1					, MF_GRAYED|MF_BYPOSITION);	//�༭����
	m_hm->EnableMenuItem(3					, MF_GRAYED|MF_BYPOSITION);	//���Ժ���
	m_hm->EnableMenuItem(IDM_COUNTI			, MF_GRAYED);

	//��Ҫ����Ĳ˵�
	m_hm->EnableMenuItem(IDM_RELEASE		, MF_ENABLED);
	m_hm->EnableMenuItem(IDM_SHOWPRESSURE	, MF_ENABLED);

	DrawMenuBar();	//�ػ�˵���
}

int CMyDlg::GetPageSize(int nBar)
//��õ�ǰ��Ļ��С��һҳ
{
	RECT rect;
	int range;
	GetWindowRect(&rect);

	if(SB_HORZ == nBar)
	{
		range = rect.right - rect.left;
		range -= 5 + 20;
	}
	else //if(SB_VERT == nBar)
	{
		range = rect.bottom - rect.top;
		range -= 48 + 20;
	}
	range >>= 5;

	return range;
}

void CMyDlg::PasteByHotKey()
//ʹ�ÿ�ݼ�ճ��
{
	if(m_inputLock) return;

	RECT rect;
	POINT mousePos;

	//��ú�������
	GetWindowRect(&rect);
	rect.right -= rect.left + 50;	//ȥ���ұ߱߿�
	rect.bottom -= rect.top + 100;	//ȥ���±߱߿�
	GetCursorPos(&mousePos);
	ScreenToClient(&mousePos);
	if(mousePos.x < 0) mousePos.x = 0;
	if(mousePos.y < 0) mousePos.y = 0;
	if(mousePos.x > rect.right) mousePos.x = rect.right;
	if(mousePos.y > rect.bottom) mousePos.y = rect.bottom;

	m_c->PasteBody(mousePos);
}

void CMyDlg::SetWindowText()
//���ô��ڱ���
{
	char title[256];
	const char * filePath = m_c->GetFilePath();

	if(NULL == filePath || '\0' == filePath[0])
	{
		strcpy(title, "�µ�·�ļ�");
		strcat(title, FILE_EXTENT_DOT);
	}
	else
	{
		strcpy(title, filePath);
	}
	strcat(title, " - �Ⱥ��·");

	CDialog::SetWindowText(title);
}

bool CMyDlg::SaveFileBeforeClose(const char * caption, bool hasCancelButton)
//�ر��ļ�ǰ�û�ѡ�񱣴浱ǰ�ļ�
{
	const char * filePath = m_c->GetFilePath();
	char note[256];
	int ret;

	if(NULL == filePath || filePath[0] == '\0')
	{
		strcpy(note, "�����ļ��� ?");
	}
	else
	{
		strcpy(note, "��·���浽�ļ� :\n\t");
		strcat(note, filePath);
		strcat(note, "\n�� ?");
	}

	if(hasCancelButton)
		ret = MessageBox(note, caption, MB_YESNOCANCEL|MB_ICONASTERISK);
	else
		ret = MessageBox(note, caption, MB_YESNO|MB_ICONASTERISK);

	if(IDCANCEL == ret) return false;
	if(IDYES == ret) OnFileSave();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers
BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// ��ʼ�� /////////////////////////////////////////
	//��Ա������ֵ
	m_focusFlag = true;				//���ڻ�ý����־
	m_inputLock = false;			//��ʼ���벻����
	m_hm = GetMenu();				//��ȡ���˵����
	m_c = new class Manager(this);	//��ʼ����·������

	//���ù�������Χ
	SetScrollRange(SB_HORZ, 0, 50);	//ˮƽ
	SetScrollRange(SB_VERT, 0, 30);	//��ֱ

	//��ô��ļ�·��
	CCommandLineInfo cmdInfo;
	theApp.ParseCommandLine(cmdInfo);
	const char * filePath = cmdInfo.m_strFileName.GetBuffer(0);

	//���ļ�
	UINT length = strlen(filePath);
	if(length >= 5 && 0 == strcmp(FILE_EXTENT_DOT, filePath + length - 4))
	{
		m_c->ReadFile(filePath);
	}

	//���ô��ڱ���
	this->SetWindowText();

	return TRUE;
}

void CMyDlg::OnClose()
//�ر�
{
	ASSERT(m_c != NULL);

	if(!SaveFileBeforeClose("�ر�ǰ", true)) return;

	delete m_c;
	m_c = NULL;

	CDialog::OnClose();
}

void CMyDlg::OnDestroy() 
//ǿ���˳�
{
	CDialog::OnDestroy();

	if(m_c != NULL)
	{
		SaveFileBeforeClose("�Ⱥ��·", false);

		delete m_c;
		m_c = NULL;
	}
}

BOOL CMyDlg::PreTranslateMessage(MSG * pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{         
        switch(pMsg->wParam)
		{
		case VK_RETURN:
        case VK_ESCAPE:
			PostMessage(WM_CLOSE);
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		m_c->PaintAll();	//����·����
	}
}

BOOL CMyDlg::OnHelpInfo(HELPINFO * pHelpInfo) 
{
	if(pHelpInfo->iContextType == HELPINFO_MENUITEM)
	{
		char str[256];
		if(0 != LoadString(AfxGetInstanceHandle(), pHelpInfo->iCtrlId, str, 256))
			MessageBox(str, "�˵�������Ϣ");
	}
	else
	{
		RECT rect;
		POINT pos = pHelpInfo->MousePos;
		ScreenToClient(&pos);
		GetClientRect(&rect);

		if(pos.x>=0 && pos.x<rect.right && pos.y>=0 && pos.y<rect.bottom)
			m_c->Help(pos);
	}

	return true;
}

HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::OnAbout() 
//����
{
	PostMessage(WM_SYSCOMMAND, IDM_ABOUTBOX);
}


// ����������Ϣ�Ĵ�����-------------------------------------------------��
void CMyDlg::OnLButtonDown(UINT, CPoint point) 
//������������Ϣ����
{
	if(m_inputLock) return;
	m_c->AddBody(point);
	if(m_c->LButtonDown(point)) m_c->PaintAll();
}

void CMyDlg::OnMouseMove(UINT nFlags, CPoint point) 
//����ƶ�,ʧȥ���㲻�ж�
{
	if(m_inputLock || !m_focusFlag) return;
	m_c->MouseMove(point, nFlags&MK_LBUTTON);
	CDialog::OnMouseMove(nFlags, point);
}

void CMyDlg::OnLButtonUp(UINT, CPoint point) 
//������������Ϣ����
{
	if(m_inputLock)
		m_c->SetStartBody(point);
	else if(m_c->LButtonUp(point)) 
		m_c->PaintAll();
}

void CMyDlg::OnLButtonDblClk(UINT, CPoint point) 
//˫��������
{
	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = point;

	if(m_inputLock)	//��ʾ��������Ʋ�
	{
		if(m_c->ShowBodyElec(body))
			m_c->PaintAll();
		else
			m_c->ShowPressure();
	}
	else			//��ʾ��������
	{
		m_c->Property(body, false);
		m_c->PaintAll();
	}
}

void CMyDlg::OnRButtonUp(UINT, CPoint point) 
//����Ҽ�������Ϣ����
{
	HMENU hm;
	BODY_TYPE type;
	m_mousePos = point;	//���浱ǰ�������

	m_c->PaintAll();	//ˢ��
	type = m_c->PosBodyPaintRect(point);	//ͻ���һ�����

	if(m_inputLock)		//��������
	{
		hm = CreatePopupMenu();

		if(BODY_LEAD == type)				//�һ�����
		{
			AppendMenu(hm, 0, IDM_POSBODY_SHOWELEC, "�鿴����(&L)\tCtrl+L");
		}
		else if(BODY_CRUN == type)			//�һ����
		{
			AppendMenu(hm, 0, IDM_POSBODY_PROPERTY, "�鿴����(&P)\tCtrl+P");
		}
		else if(Pointer::IsCtrl(type))		//�һ��ؼ�
		{
			AppendMenu(hm, 0, IDM_POSBODY_SHOWELEC, "�鿴����(&L)\tCtrl+L");
			AppendMenu(hm, 0, IDM_POSBODY_PROPERTY, "�鿴����(&P)\tCtrl+P");
		}

		AppendMenu(hm, 0, IDM_RELEASE, "�����������(&R)\tCtrl+R");
		AppendMenu(hm, 0, IDM_SHOWPRESSURE, "��ʾ���Ʋ�(&U)\tCtrl+U");
		ClientToScreen(&point);
		TrackPopupMenu(hm, TPM_LEFTALIGN, point.x, point.y, 0, m_hWnd, NULL);
		DestroyMenu(hm);
	}

	else if(BODY_NO == type)	//�һ��հ״�
	{
		hm = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		HMENU temp_hm = GetSubMenu(hm, 1);
		HMENU subhm = GetSubMenu(temp_hm, 3);
		AppendMenu(subhm, 0, IDM_PASTE, "ճ��(&P)\tCtrl+V");
		if(!m_c->GetClipboardState())	//���а�û������
			EnableMenuItem(subhm, IDM_PASTE, MF_GRAYED);

		ClientToScreen(&point);
		TrackPopupMenu(subhm, TPM_LEFTALIGN, point.x, point.y, 0, m_hWnd, NULL);
		DestroyMenu(hm);
		DestroyMenu(temp_hm);
		DestroyMenu(subhm);
	}

	else
	{
		HMENU subhm;
		if(BODY_LEAD == type)	//����
			hm = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_LEADPRO));
		else
			hm = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_BODYPRO));
		subhm = GetSubMenu(hm, 0);

		if(Pointer::IsCtrl(type))
		{
			InsertMenu(subhm, IDM_POSBODY_PROPERTY, 0, IDM_POSBODY_ROTATE1, "˳ʱ����ת90��(&1)\tCtrl+1");
			InsertMenu(subhm, IDM_POSBODY_PROPERTY, 0, IDM_POSBODY_ROTATE2, "��ת180��(&2)\tCtrl+2");
			InsertMenu(subhm, IDM_POSBODY_PROPERTY, 0, IDM_POSBODY_ROTATE3, "��ʱ����ת90��(&3)\tCtrl+3");

			InsertMenu(subhm, IDM_POSBODY_PROPERTY, 0, IDM_POSBODY_CHANGECTRLSTYLE, "��ѧԪ������(&T)\tCtrl+T");
		}

		if(BODY_LEAD == type || Pointer::IsCtrl(type))	//���߻�ؼ���
			AppendMenu(subhm, 0, IDM_POSBODY_SHOWELEC, "�鿴����(&L)\tCtrl+L");

		ClientToScreen(&point);
		TrackPopupMenu(subhm, TPM_LEFTALIGN, point.x, point.y, 0, m_hWnd, NULL);
		DestroyMenu(hm);
		DestroyMenu(subhm);
	}

	m_c->PaintAll();	//ˢ��
}

void CMyDlg::OnKillFocus()
//����ʧȥ����
{
	m_focusFlag = false;
}

void CMyDlg::OnSetFocus()
//���ڻ�ý���
{
	m_focusFlag = true;
}

void CMyDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar) 
//��ֱ��������Ϣ����
{
	const int oldPos = GetScrollPos(SB_VERT);	//��¼��ʼ�Ĺ�����λ��
	int minPos, maxPos;	//��������Χ
	int newPos;			//�������µ�λ��
	GetScrollRange(SB_VERT, &minPos, &maxPos);

	switch(nSBCode)
	{
	case SB_TOP:
		newPos = minPos;
		break;
	case SB_BOTTOM:
		newPos = maxPos;
		break;
	case SB_LINEUP:
		newPos = oldPos - 1;
		break;
	case SB_LINEDOWN:
		newPos = oldPos + 1;
		break;
	case SB_PAGEUP:
		newPos = oldPos - GetPageSize(SB_VERT);
		break;
	case SB_PAGEDOWN:
		newPos = oldPos + GetPageSize(SB_VERT);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		newPos = nPos;
		break;
	default:
		return;
	}

	if(newPos < minPos)newPos = minPos;
	if(newPos > maxPos)newPos = maxPos;

	if(newPos != oldPos)	//�ı���
	{
		SetScrollPos(SB_VERT, newPos);
		m_c->SetViewOrig(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
		m_c->PaintAll();
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar) 
//ˮƽ��������Ϣ����
{
	const int oldPos = GetScrollPos(SB_HORZ);	//��¼��ʼ�Ĺ�����λ��
	int minPos, maxPos;	//��������Χ
	int newPos;			//�������µ�λ��
	GetScrollRange(SB_HORZ, &minPos, &maxPos);

	switch(nSBCode)
	{
	case SB_LEFT:
		newPos = minPos;
		break;
	case SB_RIGHT:
		newPos = maxPos;
		break;
	case SB_LINELEFT:
		newPos = oldPos - 1;
		break;
	case SB_LINERIGHT:
		newPos = oldPos + 1;
		break;
	case SB_PAGELEFT:
		newPos = oldPos - GetPageSize(SB_HORZ);
		break;
	case SB_PAGERIGHT:
		newPos = oldPos + GetPageSize(SB_HORZ);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		newPos = nPos;
		break;
	default:
		return;
	}

	if(newPos < minPos) newPos = minPos;
	if(newPos > maxPos) newPos = maxPos;

	if(newPos != oldPos)	//�ı���
	{
		SetScrollPos(SB_HORZ, newPos);
		m_c->SetViewOrig(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
		m_c->PaintAll();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CMyDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
//����������Ϣ����
{
	int vPos = GetScrollPos(SB_VERT);
	int hPos = GetScrollPos(SB_HORZ);	//��¼��ʼ�Ĺ�����λ��
	int temp;

	if(nFlags)	//�м�����,��ˮƽ����������
	{
		SetScrollPos(SB_HORZ, hPos-zDelta/WHEEL_DELTA);
		temp = GetScrollPos(SB_HORZ);
		if(hPos != temp)	//�ı���
		{
			m_c->SetViewOrig(temp, vPos);
			m_c->PaintAll();
		}
	}
	else	//�޼�����,����ֱ����������
	{
		SetScrollPos(SB_VERT, vPos-zDelta/WHEEL_DELTA);
		temp = GetScrollPos(SB_VERT);
		if(vPos != temp)	//�ı���
		{
			m_c->SetViewOrig(hPos, temp);
			m_c->PaintAll();
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//key down
{
	//�����ctrl��������
	if(StaticClass::IsCtrlDown())
	{
		switch(nChar)
		{
		//�ļ����ܿ�ݼ�
		case 'N':	//�½��ļ�
			OnFileNew();
			return;

		case 'O':	//���ļ�
			OnFileOpen();
			return;

		case 'S':	//�����ļ�
			OnFileSave();
			return;

		//�༭���ܿ�ݼ�
		case 'X':	//���н���
			OnFocusBodyCut();
			return;

		case 'C':	//���ƽ���
			OnFocusBodyCopy();
			return;

		case 'V':	//ʹ�ÿ�ݼ�ճ��
			PasteByHotKey();
			return;

		case 'Z':	//����
			OnUnDo();
			return;

		case 'Y':	//ǰ��
			OnReDo();
			return;

		case 'P':	//����
			OnFocusBodyProperty();
			return;

		case 'T':	//��ѧԪ������
			OnFocusBodyChangeCtrlStyle();
			return;

		case '1':
		case '2':
		case '3':	//��ת��ѧԪ��
			OnFocusBodyRotateCtrl(nChar - '1' + IDM_FOCUSBODY_ROTATE1);
			return;

		case 'F':
			OnSearch();
			return;

		//���㹦�ܿ�ݼ�
		case 'I':	//�������
			OnCountElec();
			return;

		case 'L':	//��ʾ�������
			OnFocusBodyShowElec();
			return;

		case 'U':	//��ʾ���Ʋ�
			OnShowPressure();
			return;

		case 'R':	//�����������
			OnUnLock();
			return;

		default:
			return;
		}
	}

	switch(nChar)
	{
	case VK_HOME:	//Home of a line
		OnHScroll(SB_LEFT, 0, NULL);
		return;

	case VK_END:	//End of a line
		OnHScroll(SB_RIGHT, 0, NULL);
		return;

	case 33:		//VK_PAGE_UP
		OnVScroll(SB_PAGEUP, 0, NULL);
		return;

	case 34:		//VK_PAGE_DOWN
		OnVScroll(SB_PAGEDOWN, 0, NULL);
		return;
	}

	if(m_inputLock)	//������˵���,�������ּ�ѡ����һ��λ��
			m_c->NextBodyByInputNum(nChar);

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
//key up
{
	switch(nChar)
	{
	case VK_UP:		//�����ƶ�����������Ϲ���
		if(!m_inputLock && m_c->FocusBodyMove(nChar))
			m_c->PaintAll();
		else
			OnVScroll(SB_LINEUP, 0, NULL);
		return;

	case VK_DOWN:	//�����ƶ�����������¹���
		if(!m_inputLock && m_c->FocusBodyMove(nChar))
			m_c->PaintAll();
		else
			OnVScroll(SB_LINEDOWN, 0, NULL);
		return;

	case VK_LEFT:	//�����ƶ���������������
		if(!m_inputLock && m_c->FocusBodyMove(nChar))
			m_c->PaintAll();
		else
			OnHScroll(SB_LINELEFT, 0, NULL);
		return;

	case VK_RIGHT:	//�����ƶ�����������ҹ���
		if(!m_inputLock && m_c->FocusBodyMove(nChar))
			m_c->PaintAll();
		else
			OnHScroll(SB_LINERIGHT, 0, NULL);
		return;

	case VK_SPACE:
	case VK_TAB:	//�л�����
		if(!m_inputLock) m_c->FocusBodyChangeUseTab();
		return;

	case 8:			//Backspace
	case VK_DELETE:	//ɾ������
		OnFocusBodyDelete();
		return;
	}

	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}


//�ļ�����----------------------------------------------------------------��
void CMyDlg::OnFileNew() 
//�½��ļ�
{
	if(m_inputLock) return;

	//�ر��ļ�ǰ�û�ѡ�񱣴浱ǰ�ļ�
	if(!SaveFileBeforeClose("�½��ļ�ǰ", true)) return;

	//�������ļ�
	m_c->CreateFile();
	m_c->PaintAll();
	this->SetWindowText();
}

void CMyDlg::OnFileOpen() 
//�Ӵ��̶�ȡָ���ļ�
{
	if(m_inputLock) return;

	//�ر��ļ�ǰ�û�ѡ�񱣴浱ǰ�ļ�
	if(!SaveFileBeforeClose("���ļ�ǰ", true)) return;

	//��ö�ȡ�ļ�·��
	CFileDialog * lpszOpenFile = new CFileDialog(	//���ɶԻ���
									TRUE, 
									FILE_EXTENT, 
									DEFAULT_FILE_NAME, 
									OFN_FILEMUSTEXIST,
									FILE_LIST);

	CString szGetName;
	if(lpszOpenFile->DoModal() == IDOK)	//����Ի���ȷ����ť
	{
		szGetName = lpszOpenFile->GetPathName();	//�õ��ļ���·��
		delete lpszOpenFile;						//�ͷŶԻ�����Դ
	}
	else
	{
		delete lpszOpenFile;	//�ͷŶԻ�����Դ
		return;
	}

	//��ȡ�ļ�
	if(m_c->ReadFile(szGetName.GetBuffer(0)))
	{
		this->SetWindowText();	//���´��ڱ���
		m_c->PaintAll();		//��ȡ�ļ���ˢ��
	}
}

void CMyDlg::OnFileSave()
//���浽�ļ�
{
	if(m_inputLock) return;

	const char * path = m_c->GetFilePath();

	if('\0' == path[0])	//·��Ϊ��
	{
		OnFileSaveAs();
	}
	else
	{
		m_c->SaveFile(path);
	}
}

void CMyDlg::OnFileSaveAs()
//���Ϊ�ļ�
{
	if(m_inputLock) return;

	//������·��
	CFileDialog * lpszOpenFile = new CFileDialog(	//���ɶԻ���
									FALSE, 
									FILE_EXTENT, 
									DEFAULT_FILE_NAME, 
									OFN_OVERWRITEPROMPT, 
									FILE_LIST);

	CString szGetName;
	if(lpszOpenFile->DoModal() == IDOK)	//����Ի���ȷ����ť
	{
		szGetName = lpszOpenFile->GetPathName();	//�õ��ļ���·��
		delete lpszOpenFile;						//�ͷŶԻ�����Դ
	}
	else
	{
		delete lpszOpenFile;	//�ͷŶԻ�����Դ
		return;
	}

	//����ļ�
	m_c->SaveFile(szGetName.GetBuffer(0));	//�����ļ�
	this->SetWindowText();					//���´��ڱ���
}

void CMyDlg::OnSaveAsPicture()
//�����·��ͼƬ
{
	//���ͼƬ����·��
	CFileDialog * lpszOpenFile = new CFileDialog(	//���ɶԻ���
									FALSE, 
									"bmp", 
									"shot.bmp", 
									OFN_OVERWRITEPROMPT, 
									"λͼ�ļ�(*.bmp)|*.bmp||");
	lpszOpenFile->m_ofn.lpstrTitle = "ѡ��ͼƬ·��";

	CString szGetName;
	if(lpszOpenFile->DoModal() == IDOK)	//����Ի���ȷ����ť
	{
		szGetName = lpszOpenFile->GetPathName();	//�õ��ļ���·��
		delete lpszOpenFile;						//�ͷŶԻ�����Դ
	}
	else
	{
		delete lpszOpenFile;	//�ͷŶԻ�����Դ
		return;
	}

	//����ͼƬ
	m_c->SaveAsPicture(szGetName.GetBuffer(0));
}

void CMyDlg::OnExit() 
//�˵��˳�,Ч��ͬOnClose()
{
	PostMessage(WM_CLOSE);
}

void CMyDlg::OnDropFiles(HDROP hDropInfo)
//�û�����ק��ʽ���ļ�
{
	if(m_inputLock) return;

	int count, filePathLen;
	char filePath[256];
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	if(count > 0)
	{
		//����ļ���׺
		for(--count; count>=0; --count)
		{
			DragQueryFile(hDropInfo, count, filePath, sizeof(filePath));
			filePathLen = strlen(filePath);
			if(filePathLen >= 5 && 0 == strcmp(FILE_EXTENT_DOT, filePath + filePathLen - 4))
				break;
		}
		DragFinish(hDropInfo);
		if(count < 0)
		{
			CString note = "�����ǵ�·�ļ�: ";
			note += FILE_EXTENT_DOT;
			MessageBox(note, "��ק�ļ���û�е�·�ļ�", MB_ICONASTERISK);
			return;
		}

		//�ر��ļ�ǰ�û�ѡ�񱣴浱ǰ�ļ�
		if(!SaveFileBeforeClose("���ļ�ǰ", true)) return;

		//��ȡ�ļ�
		if(m_c->ReadFile(filePath))
		{
			this->SetWindowText();	//���´��ڱ���
			m_c->PaintAll();		//��ȡ�ļ���ˢ��
		}
	}
	else
	{
		DragFinish(hDropInfo);
	}
}


//�༭����----------------------------------------------------------------��
void CMyDlg::OnFocusBodyCut()
//���н���
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = true;

	m_c->CutBody(body);
}

void CMyDlg::OnFocusBodyCopy()
//���ƽ���
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = true;

	m_c->CopyBody(body);
}

void CMyDlg::OnFocusBodyDelete()
//ɾ������
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = true;

	m_c->Delete(body);
	m_c->PaintAll();
}

void CMyDlg::OnSetAddState(WORD nID)
//������Ӻ�������,�������λ�����������λ��ȷ��
{
	if(m_inputLock) return;
	m_c->PaintAll();
	m_c->SetAddState(BODY_TYPE(nID-IDM_ADD_SOURCE));
}

void CMyDlg::OnUnDo()
//����
{
	if(m_inputLock) return;
	m_c->UnDo();
}

void CMyDlg::OnReDo()
//�ظ�
{
	if(m_inputLock) return;
	m_c->ReDo();
}

void CMyDlg::OnFocusBodyProperty()
//��������
{
	FOCUS_OR_POS body;
	body.isFocusBody = true;

	m_c->Property(body, m_inputLock);
	m_c->PaintAll();
}

void CMyDlg::OnFocusBodyChangeCtrlStyle()
//�ı佹���ѧԪ������
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = true;

	m_c->ChangeCtrlStyle(body);
	m_c->PaintAll();
}

void CMyDlg::OnFocusBodyRotateCtrl(WORD nID)
//��ת�����ѧԪ��
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = true;

	m_c->RotateCtrl(body, nID-IDM_FOCUSBODY_ROTATE1+1);
	m_c->PaintAll();
}

void CMyDlg::OnFocusBodyShowElec()
//��ʾ��������ĵ���
{
	FOCUS_OR_POS body;
	body.isFocusBody = true;

	m_c->ShowBodyElec(body);
	m_c->PaintAll();
}

void CMyDlg::OnSearch()
//��������
{
	if(m_inputLock) return;

	static SEARCH_BY searchBy = SEARCH_BY_NAME;
	static BODY_TYPE searchRange = BODY_ALL;
	static bool isWholeWord = false;
	static bool isMatchCase = false;
	static char keyWord[NAME_LEN] = {0};
	static bool isSearchPre = false;
	bool isMatch;

	MySearchDlg dlg(searchBy, searchRange, isWholeWord, isMatchCase, keyWord, isSearchPre, this);
	if(1 == dlg.DoModal())	//����û���Ϣ
	{
		if(isSearchPre)
			isMatch = m_c->SearchPre(searchBy, searchRange, isWholeWord, isMatchCase, keyWord);		//������һ��
		else
			isMatch = m_c->SearchNext(searchBy, searchRange, isWholeWord, isMatchCase, keyWord);	//������һ��

		if(!isMatch) MessageBox("δ�ҵ�ƥ�� !", "�������");
	}
}


//���ú���----------------------------------------------------------------��
void CMyDlg::OnSetMoveBodySense() 
//���÷�����ƶ�����ľ���
{
	m_c->SetMoveBodySense();
}

void CMyDlg::OnSetLeaveOutDis() 
//���õ��ߺϲ�������
{
	m_c->SetLeaveOutDis();
}

void CMyDlg::OnSetTextColor() 
//����������ɫ
{
	m_c->SetTextColor();
}

void CMyDlg::OnSetFocusLeadStyle()
//���ý��㵼����ʽ
{
	m_c->SetFocusLeadStyle();
}

void CMyDlg::OnSetFocusCrunColor() 
//���ý�������ɫ
{
	m_c->SetFocusCrunColor();
}

void CMyDlg::OnSetFocusCtrlColor() 
//���ý����ѧԪ����ɫ
{
	m_c->SetFocusCtrlColor();
}


//���Ժ���----------------------------------------------------------------��
void CMyDlg::OnSaveTextFile()
{
	if(m_inputLock) return;
	m_c->SaveCircuitInfoToTextFile();
}

void CMyDlg::OnMakeMap()
{
	if(m_inputLock) return;
	m_c->SaveCountInfoToTextFile();
}


//���㺯��----------------------------------------------------------------��
void CMyDlg::OnCountElec()
//�������
{
	if(m_inputLock) return;

	LockInput();		//����
	m_c->CountElec();	//�������
	m_c->PaintAll();	//��������������Ч����Ҫˢ��
}

void CMyDlg::OnShowPressure()
//��ʾ���Ʋ�
{
	if(!m_inputLock) return;
	m_c->ShowPressure();
}

void CMyDlg::OnPosBodyShowElec() 
//��ʾ�����һ�����ĵ���
{
	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->ShowBodyElec(body);
	m_c->PaintAll();
}

void CMyDlg::OnUnLock() 
//���������
{
	if(!m_inputLock) return;
	m_inputLock = false;	//���������

	//��Ҫ����Ĳ˵�
	m_hm->EnableMenuItem(0					, MF_ENABLED|MF_BYPOSITION);	//�ļ�����
	m_hm->EnableMenuItem(1					, MF_ENABLED|MF_BYPOSITION);	//�༭����
	m_hm->EnableMenuItem(3					, MF_ENABLED|MF_BYPOSITION);	//���Ժ���
	m_hm->EnableMenuItem(IDM_COUNTI			, MF_ENABLED);

	//��Ҫ���εĲ˵�
	m_hm->EnableMenuItem(IDM_RELEASE		, MF_GRAYED);
	m_hm->EnableMenuItem(IDM_SHOWPRESSURE	, MF_GRAYED);

	DrawMenuBar();			//�ػ�˵���
	m_c->ClearPressBody();	//�����ʾ���Ʋ�ĳ�Ա����
	m_c->PaintAll();		//ˢ��
}


//�һ��˵�����------------------------------------------------------------��
void CMyDlg::OnPosBodyCopy()
//�����һ�����
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->CopyBody(body);
}

void CMyDlg::OnPosBodyCut()
//�����һ�����
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->CutBody(body);
}

void CMyDlg::OnPosBodyDelete() 
//ɾ���һ�����
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->Delete(body);
	m_c->PaintAll();
}

void CMyDlg::OnPaste()
//ճ�����а����嵽�һ�λ��
{
	if(m_inputLock) return;
	m_c->PasteBody(m_mousePos);
}

void CMyDlg::OnDeleteLead() 
//ɾ���һ�����
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->Delete(body);
	m_c->PaintAll();
}

void CMyDlg::OnPosBodyRotateCtrl(WORD nID)
//��ת�һ���ѧԪ��
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->RotateCtrl(body, nID-IDM_POSBODY_ROTATE1+1);
	m_c->PaintAll();
}

void CMyDlg::OnPosBodyProperty()
//�һ���������
{
	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->Property(body, m_inputLock);
	m_c->PaintAll();
}

void CMyDlg::OnPosBodyChangeCtrlStyle() 
//�ı��һ���ѧԪ������
{
	if(m_inputLock) return;

	FOCUS_OR_POS body;
	body.isFocusBody = false;
	body.pos = m_mousePos;

	m_c->ChangeCtrlStyle(body);
	m_c->PaintAll();
}
