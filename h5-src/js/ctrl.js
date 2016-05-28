struct SOURCEDATA	//��Դ����
{
	double pressure;	//��ѹV
	double resist;		//���覸
	bool   haveResist;	//�Ƿ��е���
};

struct RESISTDATA	//��������
{
	double resist;	//���覸
};

struct BULBDATA		//С��������
{
	double rating;	//�����W
	double resist;	//���覸
};

struct CAPACITYDATA	//����������
{
	double capa;	//���ݦ�F
};

struct SWITCHDATA	//��������
{
	bool onOff;		//����
};

const long DATA_NOTE_RESIST			= 0;
const long DATA_NOTE_PRESS			= 1;
const long DATA_NOTE_CURRENT		= 2;
const long DATA_NOTE_RATING			= 3;
const long DATA_NOTE_CAPA			= 4;
const long DATA_NOTE_SWITCHONOFF	= 5;
const long DATA_NOTE_HAVERESIST		= 6;



//��ǿؼ��Ƿ��ṩ��ѹ(1�ṩ,0���ṩ)
const bool PRESSURE_TYPE[CTRL_TYPE_NUM] = {1, 0, 0, 0, 0};

//��ǿؼ��Ƿ��е���(1�����е���,-1��·,0�޵���)
const char RESISTANCE_TYPE[CTRL_TYPE_NUM] = {1, 1, 1, -1, 1};

//�ؼ����������
const long DATA_ITEM_NUM[CTRL_TYPE_NUM] = {3, 1, 2, 1, 1};

//ÿ����ѧ���Զ�Ӧ��˵��
const char DATA_NOTE[][NAME_LEN]=
{
	"����            (ŷķ-��)"	,
	"��ѹ             (����-U)"	,
	"����      (����/��-A/S)"	,
	"�����     (����-W)"		,
	"����          (΢��-��F)"	,
	"���رպ�"					,
	"�˵�Դ�е���"
};


class CTRL	//�ؼ���
{//!�����������@�ĺ�������7��,�����¿ؼ����Ͷ���ʱ��Ҫ��������͵Ĵ������

	static unsigned long s_initNum;		//��ʼ������
	BODY_TYPE style;					//�ؼ�����
	void * data;						//�洢�ؼ����е���Ϣ,���ݲ�ͬ�Ŀؼ�����
	unsigned long initNum;				//��ʼ��˳��

	CTRL(const CTRL &);					//������ֱ�Ӹ��ƶ���
	void operator =(const CTRL &);		//������ֱ�Ӹ�ֵ����

public:
	//��ÿؼ��ĵ�ѹ
	GetPressure: function(/*int*/direction) {
		/*double*/var pressure;	//���ص�ѹ

		if (this.hasOwnProperty("pressure")) 
		{
			if (direction != 0)
				return - this.pressure;
			else
				return   this.pressure;
		}

		return 0;
	};

public:

	long num;			//��ַ���
	bool isPaintName;	//�Ƿ���ʾ��ǩ
	char name[NAME_LEN];//�ؼ���
	POINT coord;		//����
	long dir;			//�ؼ�����

	LEAD * lead[2];		//a,b�������ӵ���

	double elec;		//�����ؼ��ĵ����� ��С(�ڷ������µĴ�С)
	ELEC_STATE elecDir;	//�����ؼ��ĵ����� ����

private:

	double GetSpecialData()const;	//@��ÿؼ�����������
	void InitData(BODY_TYPE);		//��ʼ���ؼ����ݲ���

public:

	CTRL(long memNum, POINT pos, BODY_TYPE ctrlStyle, bool isInit = true);
	~CTRL();
	CTRL * Clone(CLONE_PURPOSE)const;			//�����ؼ���Ϣ���µĿؼ�
	int  GetConnectNum()const;					//��ÿؼ����ӵĵ�����
	int  GetDirect(const LEAD *)const;			//Ѱ�ҵ������ĸ�����
	unsigned long GetInitOrder()const;			//��ó�ʼ�����
	static void ResetInitNum();					//���ó�ʼ������
	BODY_TYPE GetStyle()const;					//��ÿؼ�����
	void ChangeStyle(BODY_TYPE);				//�ı�ؼ�����
	void Rotate(int rotateAngle);				//��ת�ؼ�
	int  At(POINT)const;						//�������ڿؼ���λ��
	double GetPress(int direction)const;		//@��ÿؼ��ĵ�ѹ
	double GetResist()const;					//@��ÿؼ��ĵ���
	bool IsBulbOn()const;						//@С�����Ƿ�ﵽ�����
	bool SwitchOnOff(bool isSwitch = 1)const;	//@���رպϻ��߶Ͽ�
	void SaveToFile(FILE *)const;				//����ؼ����ݵ��ļ�
	void ReadFromFile(FILE *, LEAD **);			//���ļ���ȡ�ؼ�����
	void GetDataList(LISTDATA *)const;			//@��CProperty������Ϣ
	void SaveToTextFile(FILE *)const;			//@��������ʽ����,���Ժ���

};