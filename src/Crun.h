#if !defined(AFX_CRUN_FDEF)
#define AFX_CRUN_FDEF


class CRUN	//�����
{
	static unsigned long s_initNum;	//��ʼ������
	unsigned long initNum;			//��ʼ������
	CRUN(const CRUN &);				//������ֱ�Ӹ��ƶ���
	void operator =(const CRUN &);	//������ֱ�Ӹ�ֵ����

public:

	int num;				//��ַ���
	bool isPaintName;		//�Ƿ���ʾ��ǩ
	char name[NAME_LEN];	//�����
	POINT coord;			//����
	class LEAD * lead[4];	//������ӵ��ߵ�λ��,0��,1��,2��,3��

	CRUN(int memNum, POINT p);
	unsigned long GetInitOrder()const;		//��ȡ��ʼ�����
	void SaveToFile(FILE * fp)const;		//��������Ϣ���ļ�
	void ReadFromFile(FILE *, LEAD **);		//���ļ���ȡ�����Ϣ
	int At(POINT)const;						//�������ڽ���λ��
	CRUN * Clone(CLONE_PURPOSE)const;		//�����ؼ������Ϣ���µĽ��
	void GetDataList(LISTDATA * )const;		//��CProperty����
	int GetDirect(const LEAD *)const;		//Ѱ�ҵ������ĸ�����
	int GetConnectNum()const;				//��������˼�������
	static void ResetInitNum();				//���ó�ʼ������

};


#endif	//!defined(AFX_CRUN_FDEF)