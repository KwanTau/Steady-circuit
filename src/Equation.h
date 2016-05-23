#if !defined(AFX_EQUATION_FDEF)
#define AFX_EQUATION_FDEF


class Equation
{
private:
	double ** a, * x;	//a�洢����������m*n,x�Ƿ��̵Ľ�
	int m, n;			//n-1==�����ĸ���
	int gotoRow;		//��¼�Ѿ����뵽����
	int * c;			//c[i]�洢��i�е�һ������0����

	Equation(const Equation &);			//������ֱ�Ӹ��ƶ���
	void operator =(const Equation &);	//������ֱ�Ӹ�ֵ����

public:
	Equation(int crunNum, int eleNum);
	~Equation();

	void InputARow(const double * buf);	//�����gotoRows��ʼ��1�����ݵ���������
	ELEC_STATE Count();					//������,���NԪһ�η���
	const double * GetAnswer();			//��÷��̽������
	void OutputToFile(FILE *);			//�����̱��浽�ļ�,���Ժ���

};


#endif	//!defined(AFX_EQUATION_FDEF)