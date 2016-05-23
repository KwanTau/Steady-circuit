#if !defined(AFX_MYPROPERTYDLG_FDEF)
#define AFX_MYPROPERTYDLG_FDEF

#define NOTEID(i) 3000+i*2		//��ʾ�ַ�����text�ؼ�ID
#define CTRLID(i) 3000+i*2+1	//��Ӧ��ŵ����ݿؼ�ID
/////////////////////////////////////////////////////////////////////////////
// MyPropertyDlg dialog

class MyPropertyDlg : public CDialog
{
// Construction
public:
	MyPropertyDlg(
		LISTDATA * list, 
		bool readOnly, 
		CDC * model = NULL, 
		const char * windowTitle = NULL,  
		CWnd * pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MyPropertyDlg)
	enum { IDD = IDD_PROPERTY };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyPropertyDlg)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:

	void CreateLabel(RECT, LPCTSTR, UINT);										//����label�ؼ�
	void CreateCheck(RECT, bool, UINT nID);										//����check�ؼ�
	void CreateCombo(RECT, int, int, const char **, UINT);						//����Combo Box�ؼ�

	// Generated message map functions
	//{{AFX_MSG(MyPropertyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnHelpInfo(HELPINFO * pHelpInfo);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	const char * m_windowTitle;	//��������
	CDC * m_model;	//ʾ��

	bool m_readOnly;		//�Ƿ�ֻ��
	POINT m_firstCtrlPos;	//��һ���ؼ���ʼ����
	SIZE m_ctrlSize;		//�ؼ���С

	POINT m_firstNotePos;	//��һ��������ʾtext��ʼ����
	SIZE m_noteTextSize;	//������ʾtext��С

	//x��ʾ �ؼ� �� ������ʾtext �ļ��;
	//y��ʾ �ؼ�֮�� ���� ������ʾtext ֮�� �ļ��
	POINT m_inter;

	SIZE m_wndSize;	//���ڴ�С

	POINT m_okButtonPos;		//ȷ����ť������
	POINT m_cancelButtonPos;	//ȡ����ť������

	LISTDATA * m_list;	//�����б�
};

#endif // !defined(AFX_MYPROPERTYDLG_FDEF)

