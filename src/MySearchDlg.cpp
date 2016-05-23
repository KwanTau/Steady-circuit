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
   
// MySearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "�Ⱥ��·.h"
#include "MyEditCtrl.h"
#include "MySearchDlg.h"

#define IDC_EDIT_KEYWORD 3000
/////////////////////////////////////////////////////////////////////////////
// MySearchDlg dialog


MySearchDlg::MySearchDlg(	SEARCH_BY &searchBy,
							BODY_TYPE &seachRange,
							bool &isWholeWord,
							bool &isMatchCase,
							char * keyWord,
							bool &isSearchPre,
							CWnd * pParent /*=NULL*/)
	: CDialog(MySearchDlg::IDD, pParent),
	m_searchBy(searchBy),
	m_searchRange(seachRange),
	m_isWholeWord(isWholeWord),
	m_isMatchCase(isMatchCase),
	m_keyWord(keyWord),
	m_isSearchPre(isSearchPre)
{
	//{{AFX_DATA_INIT(MySearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(MySearchDlg, CDialog)
	//{{AFX_MSG_MAP(MySearchDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCH_RANGE, OnSearchRangeChange)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCH_BY, OnSearchByChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MySearchDlg message handlers

BOOL MySearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//��ѧԪ�������б�ؼ�
	CComboBox * pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_CTRL_TYPE);
	for(int i=0; i<CTRL_TYPE_NUM; ++i)
		pWnd->AddString(CTRL_STYLE_NAME[i]);
	pWnd->SetCurSel(0);

	//�ؼ��������
	RECT rect;
	rect.left = 108;
	rect.top = 80;
	rect.right = 114;
	rect.bottom = 24;
	new MyEditCtrl(	m_hWnd, 
					rect, 
					m_keyWord, 
					IDC_EDIT_KEYWORD, 
					DATA_STYLE_LPCTSTR, 
					false);

	//��������ѡ��
	pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_SEARCH_BY);
	pWnd->SetCurSel(m_searchBy);

	//������Χ
	pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_SEARCH_RANGE);
	switch(m_searchRange)
	{
	case BODY_ALL:	//��������
		pWnd->SetCurSel(0);
		break;

	case BODY_LEAD:	//����
		pWnd->SetCurSel(1);
		break;

	case BODY_CRUN:	//���
		pWnd->SetCurSel(2);
		break;

	case BODY_ALLCTRL:	//���е�ѧԪ��
		pWnd->SetCurSel(3);
		break;

	default:	//ĳ�ֵ�ѧԪ��
		pWnd->SetCurSel(4);
		pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_CTRL_TYPE);
		pWnd->SetCurSel(m_searchRange);
		break;
	}
	OnSearchRangeChange();

	//ȫ��ƥ��
	((CButton *)GetDlgItem(IDC_ISWHOLEWORD))->SetCheck(m_isWholeWord);

	//���ִ�Сд
	((CButton *)GetDlgItem(IDC_ISMATCHCASE))->SetCheck(m_isMatchCase);

	//��������
	((CButton *)GetDlgItem(IDC_DIRNEXT - m_isSearchPre))->SetCheck(TRUE);

	return TRUE;
}

void MySearchDlg::OnSearchRangeChange() 
{
	CComboBox * pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_SEARCH_RANGE);
	int searchRange = pWnd->GetCurSel();

	RECT rect;
	GetWindowRect(&rect);
	pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_CTRL_TYPE);
	if(searchRange == 4)	//����ĳ�ֵ�ѧԪ��
	{
		rect.right = rect.left + 440;
		pWnd->EnableWindow(true);
	}
	else
	{
		rect.right = rect.left + 240;
		pWnd->EnableWindow(false);
	}
	MoveWindow(&rect);

	pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_SEARCH_BY);
	if(searchRange == 1)	//��������
	{
		pWnd->EnableWindow(false);
		pWnd->SetCurSel(1);
		OnSearchByChange();
	}
	else
	{
		pWnd->EnableWindow(true);
	}
}

void MySearchDlg::OnSearchByChange() 
{
	CComboBox * pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_SEARCH_BY);
	MyEditCtrl * edit = (MyEditCtrl *)GetDlgItem(IDC_EDIT_KEYWORD);

	m_searchBy = (enum SEARCH_BY)pWnd->GetCurSel();

	if(SEARCH_BY_NAME == m_searchBy)
	{
		edit->ChangeDataStyle(DATA_STYLE_LPCTSTR);
	}
	else
	{
		edit->ChangeDataStyle(DATA_STYLE_UINT);
	}
}

BOOL MySearchDlg::DestroyWindow() 
{
	//��ȡ��������
	CComboBox * pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_SEARCH_BY);
	m_searchBy = (enum SEARCH_BY)pWnd->GetCurSel();

	//��ȡ������Χ
	pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_SEARCH_RANGE);
	switch(pWnd->GetCurSel())
	{
	case 0:	//��������
		m_searchRange = BODY_ALL;
		break;

	case 1:	//����
		m_searchRange = BODY_LEAD;
		break;

	case 2:	//���
		m_searchRange = BODY_CRUN;
		break;

	case 3:	//���е�ѧԪ��
		m_searchRange = BODY_ALLCTRL;
		break;

	case 4:	//ĳ�ֵ�ѧԪ��
		pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_CTRL_TYPE);
		m_searchRange = (enum BODY_TYPE)pWnd->GetCurSel();
		break;
	}

	//��ȡ�Ƿ�ȫ��ƥ��
	m_isWholeWord = ((CButton *)GetDlgItem(IDC_ISWHOLEWORD))->GetCheck() != 0;

	//��ȡ�Ƿ����ִ�Сд
	m_isMatchCase = ((CButton *)GetDlgItem(IDC_ISMATCHCASE))->GetCheck() != 0;

	//��ȡ�����ؼ���
	GetDlgItemText(IDC_EDIT_KEYWORD, m_keyWord, NAME_LEN);

	//��ȡ����ǰһ�����Ǻ�һ��
	m_isSearchPre = ((CButton *)GetDlgItem(IDC_DIRPRE))->GetCheck() != 0;

	//ɾ���ؼ��ֶԻ���
	CWnd * t = GetDlgItem(IDC_EDIT_KEYWORD);
	t->DestroyWindow();
	t->Detach();
	delete t;

	return CDialog::DestroyWindow();
}
