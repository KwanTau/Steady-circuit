// �Ⱥ��·Dlg.h : header file
//

#if !defined(AFX_DLG_FDEF)
#define AFX_DLG_FDEF

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CMyDlg : public CDialog
{
// Construction
public:
	CMyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnHelpInfo(HELPINFO * pHelpInfo);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();


	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus();
	afx_msg void OnSetFocus();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);


	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnSaveAsPicture();
	afx_msg void OnExit();
	afx_msg void OnDropFiles(HDROP hDropInfo);


	afx_msg void OnFocusBodyCut();
	afx_msg void OnFocusBodyCopy();
	afx_msg void OnFocusBodyDelete();
	afx_msg void OnSetAddState(WORD nID);
	afx_msg void OnUnDo();
	afx_msg void OnReDo();
	afx_msg void OnFocusBodyProperty();
	afx_msg void OnFocusBodyChangeCtrlStyle();
	afx_msg void OnFocusBodyRotateCtrl(WORD nID);
	afx_msg void OnFocusBodyShowElec();
	afx_msg void OnSearch();


	afx_msg void OnSetMoveBodySense();
	afx_msg void OnSetLeaveOutDis();
	afx_msg void OnSetTextColor();
	afx_msg void OnSetFocusLeadStyle();
	afx_msg void OnSetFocusCrunColor();
	afx_msg void OnSetFocusCtrlColor();


	afx_msg void OnSaveTextFile();
	afx_msg void OnMakeMap();


	afx_msg void OnCountElec();
	afx_msg void OnShowPressure();
	afx_msg void OnPosBodyShowElec();
	afx_msg void OnUnLock();
	
	
	afx_msg void OnPosBodyCopy();
	afx_msg void OnPosBodyCut();
	afx_msg void OnPosBodyDelete();
	afx_msg void OnPaste();
	afx_msg void OnDeleteLead();
	afx_msg void OnPosBodyRotateCtrl(WORD nID);
	afx_msg void OnPosBodyProperty();
	afx_msg void OnPosBodyChangeCtrlStyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	class Manager * m_c;	//��·������
	CMenu * m_hm;			//�˵�ָ��

	bool m_inputLock;		//����Ƿ�����
	POINT m_mousePos;		//����һ�����
	bool m_focusFlag;		//�����Ƿ��ý���


	void LockInput();		//��������
	int  GetPageSize(int);	//��õ�ǰ��Ļ��С��һҳ
	void PasteByHotKey();	//ʹ�ÿ�ݼ�ճ��
	void SetWindowText();	//���ô��ڱ���
	bool SaveFileBeforeClose(const char *, bool);	//�ر��ļ�ǰ�û�ѡ�񱣴浱ǰ�ļ�

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_FDEF)
