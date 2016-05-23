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
   
// MyEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "StaticClass.h"	//����static��������
#include "DataList.h"		//LISTDATA, ENUM_STYLE��
#include "MyEditCtrl.h"		//��ǰ��

/////////////////////////////////////////////////////////////////////////////
// MyEditCtrl
MyEditCtrl::MyEditCtrl(HWND hWnd, RECT rect, char * text, UINT nID, DATA_STYLE style, bool readOnly)
//����edit�ؼ�
{
	this->CreateEx(
		WS_EX_CLIENTEDGE,
		_T("EDIT"),
		text,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL,
		rect.left, rect.top, rect.right, rect.bottom,
		hWnd,
		(HMENU)nID);

	m_readOnly = readOnly;
	ChangeDataStyle(style);
}

MyEditCtrl::~MyEditCtrl()
{
}

void MyEditCtrl::ChangeDataStyle(DATA_STYLE style)
{
	m_style = style;

	if(m_readOnly)	//ֻ��
	{
		SetReadOnly();
		return;
	}

	switch(m_style)
	{
	case DATA_STYLE_float:		//10λС��
		ModifyStyle(ES_NUMBER, 0);
		SetLimitText(10);
		break;

	case DATA_STYLE_double:		//17λС��
		ModifyStyle(ES_NUMBER, 0);
		SetLimitText(17);
		break;

	case DATA_STYLE_UINT:		//9λ����
		ModifyStyle(0, ES_NUMBER);
		SetLimitText(9);
		break;

	case DATA_STYLE_LPCTSTR:	//NAME_LEN-2���ַ�
		ModifyStyle(ES_NUMBER, 0);
		SetLimitText(NAME_LEN - 2);
		break;
	}

	//ȥ�������ַ�
	char text[128];
	GetWindowText(text, 128);
	if(strlen(text) > GetLimitText())
	{
		char * p = text;
		while(*p != '\0' && *p != '.') ++p;
		*p = '\0';
		SetWindowText(text);
	}

	OnUpdate();
}

bool MyEditCtrl::ValidateString()
//��֤�ַ����Ƿ�Ϸ�
{
	bool flag = true;
	char text[NAME_LEN*2];
	GetWindowText(text, NAME_LEN*2);

	switch(m_style)
	{
	case DATA_STYLE_float:
	case DATA_STYLE_double:
		flag = StaticClass::IsFloat(text);
		break;

	case DATA_STYLE_UINT:
		flag = StaticClass::IsUnsignedInteger(text);
		break;

	case DATA_STYLE_LPCTSTR:
		flag = StaticClass::IsNormalStr(text);
		break;
	}

	return flag;
}


BEGIN_MESSAGE_MAP(MyEditCtrl, CEdit)
	//{{AFX_MSG_MAP(MyEditCtrl)
	ON_WM_HELPINFO()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyEditCtrl message handlers

BOOL MyEditCtrl::OnHelpInfo(HELPINFO *) 
//�û����F1,�������
{
	char note[64];
	int limitTextNum = GetLimitText();

	if(m_readOnly)
	{
		switch(m_style)
		{
		case DATA_STYLE_float:
		case DATA_STYLE_double:
			sprintf(note, "����һ����ʾ�������ı༭��\n");
			break;

		case DATA_STYLE_UINT:
			sprintf(note, "����һ����ʾ�������ı༭��\n");
			break;

		case DATA_STYLE_LPCTSTR:
			sprintf(note, "����һ����ʾ�ַ��ı༭��\n");
			break;
		}
	}
	else
	{
		switch(m_style)
		{
		case DATA_STYLE_float:
			sprintf(note, "����һ���༭�������ı༭��\n��������%dλ���ڵĸ�����", limitTextNum);
			break;

		case DATA_STYLE_double:
			sprintf(note, "����һ���༭�������ı༭��\n��������%dλ���ڵĸ�����", limitTextNum);
			break;

		case DATA_STYLE_UINT:
			sprintf(note, "����һ���༭�������ı༭��\n��������%dλ���ڵ�������", limitTextNum);
			break;

		case DATA_STYLE_LPCTSTR:
			sprintf(note, "����һ���༭�ַ��ı༭��\n��������%dλ���ڵ��ַ�", limitTextNum);
			break;
		}
	}

	AfxMessageBox(note);
	return true;
}

void MyEditCtrl::OnUpdate() 
//�û��ı��ַ�,��������Ƿ�����.��OnChar()�������ܶ�ճ�����м��
{
	if(m_readOnly) return;

	if(!ValidateString())	//�ַ����Ƿ�
	{
		if(CanUndo())
		{
			Undo();
		}
		else
		{
			SetWindowText("");
		}
	}

	EmptyUndoBuffer();
}
