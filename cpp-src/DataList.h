#if !defined(AFX_DATALIST_FDEF)
#define AFX_DATALIST_FDEF


//��������:
enum ERROR_TYPE
{
	ERROR_NO				= 0,	//�޴�
	ERROR_STRNULL			= 1,	//�ַ���Ϊ��
	ERROR_FLOATMIX			= 2,	//���������зǷ��ַ�
	ERROR_UINTMIX			= 3,	//���������зǷ��ַ�
	ERROR_UINTOVER			= 4,	//���������ڷ�Χ
	ERROR_ENUMOVER			= 5,	//ö�ٲ���ѡ���ĳһ��
	ERROR_STRMIX			= 6,	//���Ʊ�ǩ���зǷ��ַ� [](){}
	ERROR_ENUMNOTALLOWED	= 7,	//ö����һЩ����²�����һЩֵ(��:����������ɫ����Ϊ��ɫ)
};

class ENUM_STYLE	//����enum���ԵĽṹ��
{
	int * data;			//ָ���������ݵ�ָ��,�������ݶ���int�͵�
	int num;			//������
	const char ** note;	//�����Ӧ���ַ�����ʾ

	ENUM_STYLE(const ENUM_STYLE &);			//������ֱ�Ӹ��ƶ���
	void operator =(const ENUM_STYLE &);	//������ֱ�Ӹ�ֵ����

public:

	ENUM_STYLE(STYLE_LIST bufStyle, int * dataPoint)
	{
		int i;

		//1,����б��С
		data = dataPoint;
		switch(bufStyle)
		{
		case ENUM_CTRL:			//�ؼ�����
			num = CTRL_TYPE_NUM;
			break;

		case ENUM_COLOR:		//��ɫ����
			num = COLOR_TYPE_NUM;
			break;

		case ENUM_LEADSTYLE:	//������ʽ
			num = LEAD_STYLE_NUM;
			break;
		default: ASSERT(false);
		}

		//2,��ʼ��ָ��
		note = new const char * [num];

		//3,��ʼ����ʾ�ַ���
		switch(bufStyle)
		{
		case ENUM_CTRL:		//�ؼ�����
			for(i=num-1; i>=0; --i)
				note[i] = CTRL_STYLE_NAME[i];
			break;

		case ENUM_COLOR:	//��ɫ����
			for(i=num-1; i>=0; --i)
				note[i] = COLORNAMES[i];
			break;

		case ENUM_LEADSTYLE:	//������ʽ
			for(i=num-1; i>=0; --i)
				note[i] = LEADSTYLENAMES[i];
			break;
		}
	}

	~ENUM_STYLE()
	{
		delete [] note;
	}

	int * GetDataPoint()
	{
		return data;
	}

	int GetStyleNum()
	{
		return num;
	}

	const char ** GetNote()
	{
		return note;
	}
};

class LISTDATA	//�����б���Ϣ��
{
	int gotoRow;	//���뵽����
	int listSize;	//�б��С

	LISTDATA(const LISTDATA &);			//������ֱ�Ӹ��ƶ���
	void operator =(const LISTDATA &);	//������ֱ�Ӹ�ֵ����

public:

	DATA_STYLE * listStyle;	//ÿ������������
	long * minData;			//�����short,int,long����������,����Сֵ
	long * maxData;			//�����short,int,long����������,�����ֵ
	char ** noteText;		//ÿ��������������ʾ��Ϣ
	void ** dataPoint;		//����

	LISTDATA()
	{
		gotoRow   = 0;
		listSize  = 0;
		minData  = NULL;
		maxData  = NULL;
		listStyle = NULL;
		dataPoint = NULL;
		noteText  = NULL;
	}

	~LISTDATA()
	{
		for(int i=listSize-1; i>=0; --i)
			delete [] noteText[i];
		delete [] noteText;

		for(int i=listSize-1; i>=0; --i)if(DATA_STYLE_enum == listStyle[i])
			delete ((ENUM_STYLE *)(dataPoint[i]));
		delete [] dataPoint;

		delete [] maxData;
		delete [] minData;
		delete [] listStyle;
	}

	void Init(int size)	//��ʼ��
	{
		ASSERT(size>0 && size<100);

		listSize = size;

		listStyle	= new enum DATA_STYLE	[listSize];
		minData		= new long				[listSize];
		maxData		= new long				[listSize];
		dataPoint	= new void *			[listSize];
		noteText	= new char *			[listSize];
		for(int i=listSize-1; i>=0; --i) noteText[i] = new char[NAME_LEN];
	}

	int GetListSize()const
	{
		return listSize;
	}

	void SetAMember(DATA_STYLE style, const char * note, void * data, int min = 1, int max = 0)
	//�����б��һ��,style != DATA_STYLE_enum,��min>max��ʾû�д�С����
	{
		ASSERT(gotoRow < listSize && style != DATA_STYLE_enum);

		listStyle[gotoRow] = style;
		minData[gotoRow] = min;
		maxData[gotoRow] = max;
		strcpy(noteText[gotoRow], note);
		dataPoint[gotoRow] = data;

		++gotoRow;
	}

	void SetAEnumMember(const char * note, void * data, STYLE_LIST style, int min = 1, int max = 0)
	//����style == DATA_STYLE_enum��һ��,dataָ��ָ��һ��ENUM_STYLEʵ��
	{
		ASSERT(gotoRow < listSize);

		listStyle[gotoRow] = DATA_STYLE_enum;
		minData[gotoRow] = min;
		maxData[gotoRow] = max;
		strcpy(noteText[gotoRow], note);
		dataPoint[gotoRow] = new ENUM_STYLE(style, (int *)data);

		++gotoRow;
	}

	ERROR_TYPE CheckAMember(int row, CWnd * wnd)
	//���һ��: row--��, chData--�ַ�������, enumData--ö������
	{
		char chData[NAME_LEN];
		int enumData;

		switch(listStyle[row])
		{
		case DATA_STYLE_float:
		case DATA_STYLE_double:
			wnd->GetWindowText(chData, NAME_LEN);
			if('\0' == chData[0])
				return ERROR_STRNULL;
			else if(!StaticClass::IsFloat(chData))
				return ERROR_FLOATMIX;
			break;

		case DATA_STYLE_UINT:
			wnd->GetWindowText(chData, NAME_LEN);
			if('\0' == chData[0])
			{
				return ERROR_STRNULL;
			}
			else if(!StaticClass::IsUnsignedInteger(chData))
			{
				return ERROR_UINTMIX;
			}
			else if(minData[row] <= maxData[row])	//�д�С����
			{
				enumData = atoi(chData);
				if(enumData < minData[row] || enumData > maxData[row])
					return ERROR_UINTOVER;
			}
			break;

		case DATA_STYLE_enum:
			enumData = ((CComboBox *)(wnd))->GetCurSel();
			if(enumData < 0 || enumData >= ((ENUM_STYLE*)dataPoint[row])->GetStyleNum())
				return ERROR_ENUMOVER;
			else if(minData[row] <= maxData[row])	//�д�С����
			{
				if(enumData < minData[row] || enumData > maxData[row])
					return ERROR_ENUMNOTALLOWED;
			}
			break;

		case DATA_STYLE_LPCTSTR:	//char[NAME_LEN]
			wnd->GetWindowText(chData, NAME_LEN);
			if(!StaticClass::IsNormalStr(chData))
				return ERROR_STRMIX;
			break;
		}
		return ERROR_NO;
	}

	void SaveAMember(int row, CWnd * wnd)	//���ؼ��û��޸ĵ���Ϣ���浽ָ��ָ�������
	{
		char tempStr[NAME_LEN];

		switch(listStyle[row])
		{
		case DATA_STYLE_float:	//float
			wnd->GetWindowText(tempStr, NAME_LEN);
			*((float *)dataPoint[row]) = (float)atof(tempStr);
			break;
		case DATA_STYLE_double:	//double
			wnd->GetWindowText(tempStr, NAME_LEN);
			*((double *)dataPoint[row]) = atof(tempStr);
			break;
		case DATA_STYLE_UINT:	//UINT
			wnd->GetWindowText(tempStr, NAME_LEN);
			*((int *)dataPoint[row]) = atoi(tempStr);
			break;
		case DATA_STYLE_bool:	//bool
			*((bool*)dataPoint[row]) = 
				((CButton *)wnd)->GetCheck() != 0;
			break;
		case DATA_STYLE_LPCTSTR:	//char[NAME_LEN]
			wnd->GetWindowText((char *)dataPoint[row],NAME_LEN);
			break;
		case DATA_STYLE_enum:	//enum
			*((ENUM_STYLE*)dataPoint[row])->GetDataPoint() = 
				((CComboBox *)wnd)->GetCurSel();
			break;
		}
	}

};


#endif	//!defined(AFX_DATALIST_FDEF)
