#if !defined(AFX_LEAD_FDEF)
#define AFX_LEAD_FDEF


#include "Pointer.h"	//����ָ����


struct LEADSTEP	//����ʹ�õĴ洢���ߵĽṹ��
{
	POINT pos;
	LEADSTEP *next;
};


class LEAD	//������
{
	static unsigned long s_initNum;	//��ʼ������
	LEADSTEP coord;					//��㵽�յ����������
	unsigned long initNum;			//��ʼ������

public:

	int num;			//��ַ���
	enum COLOR color;	//5����ɫ:�ں������
	Pointer conBody[2];	//���ߵ�2�����Ӷ���

	double elec;		//�����ؼ��ĵ����� ��С(�ڷ������µĴ�С)
	ELEC_STATE elecDir;	//�����ؼ��ĵ����� ����

private:

	LEAD(const LEAD &);				//������ֱ�Ӹ��ƶ���
	void operator =(const LEAD &);	//������ֱ�Ӹ�ֵ����
	void Uninit();					//�ͷŵ���ռ�õĿռ�

	void CleanLead();						//ɾ������ͬ����ĵ��߽��
	void FitStart(int);						//ʹ���߲��ڵ����ӵĵ�1������
	void FitEnd(int);						//ʹ���߲��ڵ����ӵĵ�2������
	void MakeFit();							//���µĵ���λ�ø������ӵ�����ʱ,��������
	int  GetPosFit(int, int, int, bool);	//��2��ƽ�е���֮��������ҵ����ʵ���һ��ƽ�е��ߵ�λ��

public:
	LEAD(int, const Pointer &, const Pointer &, bool isInit=true, COLOR color=BLACK);
	~LEAD();
	LEAD * Clone(CLONE_PURPOSE);							//����
	unsigned long GetInitOrder()const;						//��ó�ʼ�����
	static void ResetInitNum();								//���ó�ʼ������
	void SaveToFile(FILE *)const;							//���浼����Ϣ���ļ�
	void ReadFromFile(FILE *, LEAD **, CRUN **, CTRL **);	//���ļ���ȡ������Ϣ
	void SaveToTextFile(FILE * fp)const;					//���浽�ı��ļ�,���Ժ���
	void PaintLead(CDC *)const;								//������
	void GetStartEndPos(POINT &, POINT &)const;				//��õ��߿�ʼλ�úͽ�β����
	int  GetBodyPos()const;									//��õ�������������������λ��
	void GetDataList(const char * name, LISTDATA * list);	//��CProperty������Ϣ
	void RefreshPos();										//������������ı�,���µ���λ��
	void EasyInitPos(POINT from, POINT to);					//�ֲڵĳ�ʼ����������
	bool Divide(int, POINT, LEADSTEP &, LEADSTEP &);		//��������һ��Ϊ��
	void ReplacePos(LEADSTEP);								//�滻ԭ��������
	int  At(POINT p)const;									//�������ڵ��ߵ�λ��
	bool Move(int dir, POINT pos, const int);				//�ƶ�����

};


#endif	//!defined(AFX_LEAD_FDEF)