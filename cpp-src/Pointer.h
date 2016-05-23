#if !defined(AFX_ALLBODY_FDEF)
#define AFX_ALLBODY_FDEF


class Pointer	//ָ��3����������Ľṹ
{
public:

	union
	{
		LEAD	* p1;	//ָ����
		CRUN	* p2;	//ָ����
		CTRL	* p3;	//ָ��ؼ�
	};

private:

	//�����λ��:
	// -3,-5,-7,... ���� , -2,-4,-6,... ����
	// -1 ����(���,�ؼ�)
	// 1,2,3,4 �����������ӵ�
	// 0��ָ������
	// 5,6,7...���Ϸ�
	int atState;
	BODY_TYPE style;

public:

	void Clear()	//���ָ��
	{
		p1 = NULL;
		atState = 0;
		style = BODY_NO;
	}
	Pointer()
	{
		Clear();
	}
	void SetAtState(int newState)	//����addState
	{
		atState = newState;
	}
	int GetAtState()const	//���addState
	{
		return atState;
	}
	BODY_TYPE GetStyle()const	//���style
	{
		return style;
	}
	bool IsOnAny()const	//�жϽṹ���Ƿ�ָ������
	{
		return atState != 0 && atState <= 4;
	}
	bool IsOnLead()const	//�ж��Ƿ��ڵ�����
	{
		return BODY_LEAD == style || atState <= -2;
	}
	bool IsOnHoriLead()const	//�ж��Ƿ���ˮƽ����
	{
		return atState <= -2 && (-atState)&1;
	}
	bool IsOnVertLead()const	//�ж��Ƿ�����ֱ����
	{
		return atState <= -2 && !( (-atState)&1 );
	}
	bool IsOnBody(bool type = true)const//�ж��Ƿ�������(����ؼ�)��
	{
		if(type)	//�ж��Ƿ���������,���������ӵ�
			return -1 == atState && (BODY_CRUN == style || IsCtrl(style));
		else		//�ж��Ƿ���������,�������ӵ�
			return BODY_CRUN == style || IsCtrl(style);
	}
	bool IsOnCrun()const	//�ж��Ƿ��ڽ����
	{
		return BODY_CRUN == style;
	}
	bool IsOnCtrl()const	//�ж��Ƿ��ڿؼ���
	{
		return IsCtrl(style);
	}
	bool IsOnConnectPos()const	//�ж��Ƿ������ӵ���
	{
		return atState >= 1 && atState <= 4;
	}
	void SetOnLead(LEAD * lead, bool isSetAt = true)	//ָ����
	{
		p1 = lead;
		style = BODY_LEAD;
		if(isSetAt) atState = -2;
	}
	void SetOnCrun(CRUN * crun, bool isSetAt = false)	//ָ����
	{
		p2 = crun;
		style = BODY_CRUN;
		if(isSetAt) atState = -1;
	}
	void SetOnCtrl(CTRL * ctrl, bool isSetAt = false);	//ָ��ؼ�
	bool IsLeadSame(const LEAD * other)const
	//�жϵ�ǰ�����Ƿ�ָ���������
	{
		return IsOnLead() && p1 == other;
	}
	bool IsCrunSame(const CRUN * other)const
	//�жϵ�ǰ�����Ƿ�ָ��������
	{
		return IsOnCrun() && p2 == other;
	}
	bool IsCtrlSame(const CTRL * other)const
	//�жϵ�ǰ�����Ƿ�ָ������ؼ�
	{
		return IsOnCtrl() && p3 == other;
	}
	bool IsBodySame(const Pointer * other)const
	//�ж�����Pointer�Ƿ�ָ��ͬһ������,���ж�atState
	{
		if(this->style != other->style) return false;
		if(IsOnLead())
			return this->p1 == other->p1;
		else if(IsOnCrun())
			return this->p2 == other->p2;
		else if(IsOnCtrl())
			return this->p3 == other->p3;
		return false;
	}
	bool IsAllSame(const Pointer * other)const
	//�ж�����Pointer�ṹ�Ƿ�һ��,�ж�atState
	{
		if(this->atState != other->atState) return false;
		return IsBodySame(other);
	}
	int GetLeadNum()const//������ӵ��Ӧ�ĵ��߱��(0,1,2,3)
	{
		return atState - 1;
	}
	static bool IsCtrl(const BODY_TYPE &type)
	{
		return type >= SOURCE && type <= SWITCH;
	}
	void GetPosFromBody(POINT & pos)const;					//����������ӵ�λ�û�õ��߶˵�����
	void SaveToFile(FILE *)const;							//����Pointer�ṹ�嵽�ļ�
	bool ReadFromFile(FILE *, LEAD **, CRUN **, CTRL **);	//���ļ���ȡPointer�ṹ
	int  GetConnectPosDir()const;							//������ӵ�λ��
	void SaveToTextFile(FILE *);							//���Ժ���

};


#endif	//!defined(AFX_ALLBODY_FDEF)