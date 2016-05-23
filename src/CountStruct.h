#if !defined(AFX_COUNTSTRUCT_FDEF)
#define AFX_COUNTSTRUCT_FDEF


//����ṹ��--------------------------------------------
class	CIRCU;		//��·,���������,�ɽ������
class	CRUN2;		//���ڼ���Ľ����
struct	CRUNMAP;	//����ÿ2�����֮���·��
struct	ROAD;		//2�����֮�����ӵ�·��
struct	ROADSTEP;
//����ṹ��--------------------------------------------


//����ṹ��--------------------------------------------
class CRUN2
{
public:
	CIRCU * c[4];	//ָ�����·
	int group;		//���ڵ�Ⱥ,�����ӵĵ���,�ڵ�,�ؼ����
	CRUN2()
	{
		c[0] = c[1] = c[2] = c[3] = NULL;
		group = -1;	//-1��������Ⱥ��
	}
};

class CIRCU	//��·,���������,�ɽ������,��������Ϊfrom->to
{
public:
	int eleNum;				//��·�������,Ĭ�ϱ���ǵ�ַ���
	double pressure;		//������յ�ĵ��Ʋ�(U0 - U1)
	double resistance;		//����(һ��>=0, <0���������,��·)
	ELEC_STATE elecDir;		//����״̬���
	double elec;			//��·������С
	CRUN2 * from, * to;		//�����յ���
	char dirFrom, dirTo;	//�����յ���ķ���
	int numInGroup;			//��Ⱥ���ڵ����

	CIRCU()
	{
		eleNum = 0;
		resistance = elec = 0;
		elecDir = NORMALELEC;
		from = to = NULL;
		dirFrom = dirTo = 0;
		numInGroup = 0;
	}

	void ConvertWhenElecLessZero()	//����������ʱ��Ϊ����,����ת��������
	{
		if(elec >= 0) return;
		if(elecDir != NORMALELEC) return;

		pressure = -pressure;
		elec = -elec;

		CRUN2 * tempCrun2 = from;
		from = to;
		to = tempCrun2;

		char tempDir = dirFrom;
		dirFrom = dirTo;
		dirTo = tempDir;
	}
};

struct ROADSTEP //2�����֮��·���ϵ�һ�����
{
	int crunNum;
	struct ROADSTEP * next;
	struct ROADSTEP * pre;
};

struct ROAD
{
	ROADSTEP * first;
	ROADSTEP * last;

	ROAD()
	{
		first = last = NULL ;
	}

	~ROAD()
	{
		if(first == NULL || last == NULL) return;
		ROADSTEP * now = first, * next;
		while(now)
		{
			next = now->next;
			delete now;
			now = next;
		}
	}

	void Clone(ROAD &);				//����
	bool HaveRoadPoint(int);		//�ж��Ƿ��н��point
	bool HaveRoadStep(int, int);	//�ж��Ƿ���·��: from->to ���� to->from
	void InsertPointAtTail(int);	//����������
	void SaveRoadToFile(FILE *);	//����·�����ļ�,���Ժ���

};

struct CRUNMAP
//����ÿ�������֮���Ƿ�ֱ������
{
	int size;			//�����Ľ����
	int circuNum;		//��������·��
	int * crunTOorder;	//��ɢ�Ľ���Ŷ�Ӧ�� 0 ~ size-1

	//-1 �м������ ;
	//0  ������(Ŀǰû���ҵ�·��) ;
	//1  ֻ����һ��ֱ������,Ŀǰû���ҵ�·�� ;
	//2  �ж���·��,����ֻ��һ����ֱ������,�����ҵ���·�� ;
	char *	direct;

	CIRCU ** firstcircu;	//����2�����ĵ�һ����·
	int * dir;				//����2�����ĵ�һ����·��������С�Ľ��ĵ��߱��(0,1,2,3)

	void Init(int size)//��ʼ���ڴ�
	{
		int bufSize = size*(size-1)/2;

		this->size	= size;
		crunTOorder	= new int		[size];
		direct		= new char		[bufSize];
		firstcircu	= new CIRCU *	[bufSize];
		dir			= new int		[bufSize];
	}

	void Uninit()//�ͷ��ڴ�
	{
		delete [] crunTOorder;
		delete [] direct;
		delete [] firstcircu;
		delete [] dir;
	}
};
//����ṹ��------------------------------------------

//ROAD��Ա����----------------------------------------
void ROAD::Clone(ROAD &newRoad)
//����
{
	ROADSTEP * temp, * now, * pre;

	newRoad.first = newRoad.last = NULL;
	if(first == NULL) return;

	temp = first;
	now = newRoad.first = new ROADSTEP;
	now->crunNum = temp->crunNum;
	now->pre = NULL;
	if(temp->next != NULL)
	{
		now->next = new ROADSTEP;
		pre = now;
		now = now->next;
		now->pre = pre;

		temp = temp->next;
	}
	else
	{
		now->next = NULL;
		newRoad.last = now;
		return;
	}

	while(temp != NULL)
	{
		now->crunNum = temp->crunNum;
		if(temp->next != NULL)
		{
			now->next = new ROADSTEP;
			pre = now;
			now = now->next;
			now->pre = pre;

			temp = temp->next;
		}
		else
		{
			now->next = NULL;
			newRoad.last = now;
			return;
		}
	}

}

bool ROAD::HaveRoadPoint(int point)
//�ж��Ƿ��н��point
{
	ROADSTEP * now = first;
	while(now != NULL)
	{
		if(now->crunNum == point)
			return true;
		now = now->next;
	}
	return false;
}

bool ROAD::HaveRoadStep(int from, int to)
//�ж��Ƿ���from->to·��
{
	if(!first)return false;
	ROADSTEP * pre = first;
	ROADSTEP * now = pre->next;
	while(now != NULL)
	{
		if(pre->crunNum == from && now->crunNum == to 
			|| pre->crunNum == to && now->crunNum == from)
			return true;
		pre = now;
		now = now->next;
	}
	return false;
}

void ROAD::InsertPointAtTail(int crunNum)
//����������
{
	ROADSTEP * now;

	if(first != NULL)
	{
		last->next = now = new ROADSTEP;
		now->pre = last;
		last = now;
		now->crunNum = crunNum;
		now->next = NULL;
	}
	else
	{
		first = last = now = new ROADSTEP;
		now->crunNum = crunNum;
		now->next = now->pre = NULL;
	}
}

void ROAD::SaveRoadToFile(FILE * fp)
//��������ʽ����,���Ժ���
{
	ROADSTEP * now = first;
	while(now != NULL)
	{
		fprintf(fp, "%4d->", now->crunNum);
		now = now->next;
	}
}
//ROAD��Ա����---------------------------------------------


#endif	//!defined(AFX_COUNTSTRUCT_FDEF)