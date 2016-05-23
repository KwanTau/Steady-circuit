#if !defined(AFX_CIRCUITCLASS_FDEF)
#define AFX_CIRCUITCLASS_FDEF


#include "Pointer.h"			//����ָ����
#include <vector>				//vector������
using namespace std;


const long CTRL_BITMAP_TYPE_NUM = 7;					//�ؼ�λͼ����ĸ���
const long CTRL_BITMAP_NUM = CTRL_BITMAP_TYPE_NUM*4;	//�ؼ�λͼ�ĸ���(������ת֮���)

const long FILE_VERSION			= 13;					//�ļ��汾,��ͬ�汾�ļ������ȡ
const long FILE_RESERVE_SIZE	= 256;					//�ļ�������Ĵ�С
const long MAXMOVEBODYDIS		= 50;					//ʹ�÷����һ���ƶ�������뷶Χ1~MAX_MOVEBODYDIS
const long MAXLEAVEOUTDIS		= 15;					//���ڵ��ߺϲ����뷶Χ1~MAX_LEAVEOUTDIS


class	Equation;	//����NԪ1�η��������
class	CIRCU;		//��·,���������,�ɽ������
class	CRUN2;		//���ڼ���Ľ����
struct	CRUNMAP;	//����ÿ2�����֮���·��
struct	ROAD;		//2�����֮�����ӵ�·��
struct	ROADSTEP;	//2�����֮�����ӵ�·����һ��


#define CONVERT(a,b,size)	size*a-(a+1)*a/2+b-a-1	//����CRUNMAP��Ա�ķ���


const COLORREF LEADCOLOR[COLOR_TYPE_NUM + 1] =
{
	RGB(  0,   0,   0),
	RGB(240,  10,  10),
	RGB(200, 200,  10),
	RGB(  0, 200,   0),
	RGB(  0,   0, 230),
	RGB(200,  30, 200)
};	//6����ɫ��Ӧ��RGB��ֵ,���һ��ֻ�����⹦����ʾ��ʱ��ʹ��


struct CircuitInfo	//�洢��·����ʱ��Ҫ�洢����Ϣ
{
	CRUN	** crun;			//�洢���
	CTRL	** ctrl;			//�洢�ؼ�
	LEAD	** lead;			//�洢����
	unsigned short crunNum;		//���ĸ���
	unsigned short leadNum;		//�ؼ��ĸ���
	unsigned short ctrlNum;		//���ߵĸ���
	Pointer focusBody;			//��������
};
typedef vector<CircuitInfo> MyVector;
typedef vector<CircuitInfo>::iterator MyIterator;


class Manager
{
private:
	Manager(const Manager &);			//������ֱ�Ӹ��ƶ���
	void operator =(const Manager &);	//������ֱ�Ӹ�ֵ����

	//��ͼ����---------------------------------------------------------------------
	CDC ctrlDcMem[CTRL_BITMAP_NUM];
	CBitmap ctrlBitmap[CTRL_BITMAP_NUM];	//CTRL_BITMAP_NUM���ؼ�λͼ
	CDC showConnectDcMem;
	CBitmap showConnectBitmap;		//�����λͼ
	CDC crunDcMem;
	CBitmap crunBitmap;				//���λͼ

	CPen hp[COLOR_TYPE_NUM];		//COLOR_TYPE_NUM����ɫ����

	HCURSOR hcSizeNS;				//�ϱ�˫��ͷ
	HCURSOR hcSizeWE;				//����˫��ͷ
	HCURSOR hcShowConnect;			//���ӵ���ʱ���
	HCURSOR hcHand;					//��
	HCURSOR hcMoveHorz;				//�ƶ���ֱ����ʱ��ʾ
	HCURSOR hcMoveVert;				//�ƶ�ˮƽ����ʱ��ʾ
	HCURSOR hcAddCrun;				//�������ʱ��ʾ

	enum COLOR textColor;			//������ɫ
	enum LEADSTYLE focusLeadStyle;	//���㵼����ʽ
	enum COLOR focusCrunColor;		//��������ɫ
	enum COLOR focusCtrlColor;		//����ؼ���ɫ

	//������ʾ---------------------------------------------------------------------
	CWnd * wndPointer;			//��ǰ����ָ��
	CDC  * dc;					//��ǰ�����豸������
	CDC dcForRefresh;			//ʹˢ�²�����ʹ�õ�DC
	CBitmap bitmapForRefresh;	//ʹˢ�²�����ʹ�õ�bitmap

	//��·Ԫ������-----------------------------------------------------------------
	CRUN	* crun[MAXCRUNNUM];	//�洢���
	CTRL	* ctrl[MAXCTRLNUM];	//�洢�ؼ�
	LEAD	* lead[MAXLEADNUM];	//�洢����
	unsigned short crunNum;		//���ĸ���
	unsigned short leadNum;		//�ؼ��ĸ���
	unsigned short ctrlNum;		//���ߵĸ���

	//��Ա���---------------------------------------------------------------------
	POINT viewOrig;			//�ӽǳ�ʼ����
	SIZE mouseWheelSense;	//mouseWheel������
	UINT moveBodySense;		//���������Ҽ�һ�������ƶ��ľ���
	UINT maxLeaveOutDis;	//һ�ε���,�������ںϲ��ٽ����

	//�������Ϣ��¼-------------------------------------------------------------
	BODY_TYPE addState;		//��¼Ҫ��ӵ���������
	unsigned short motiNum;	//��¼��꼤��Ĳ����ĸ���,LBUTTONDOWN��Ϣ�����
	Pointer motiBody[2];	//��¼��꼤��Ĳ���
	POINT lButtonDownPos;	//��¼��һ�����������µ�����
	//�ϴ�����Ƿ���������,��LBUTTONDOWNʱ��¼,��LBUTTONUP�����ж�
	bool lButtonDownState;
	//��¼WM_LBUTTONDOWN������һ�����WM_LBUTTONDOWN����ǰ,��û�н��ܵ�WM_LBUTTONUP
	bool isUpRecvAfterDown;

	//����ƶ���Ϣ��¼-------------------------------------------------------------
	Pointer lastMoveOnBody;	//��һ����꼤��Ĳ���,MOUSEMOVE��Ϣ�����
	POINT lastMoveOnPos;	//��¼�ϴ�����ƶ���������,�����ƶ�����

	//����������������---------------------------------------------------------
	Pointer focusBody;		//��������

	//���а����-------------------------------------------------------------------
	Pointer clipBody;		//ָ����а�������,�����ڵ�ǰ��·,����һ���ڴ�,��Ҫ�ͷ�

	//�ļ�����·������-------------------------------------------------------------
	char fileName[256];		//���浱ǰ���в������ļ�·��

	//�������---------------------------------------------------------------------
	CRUN2 * crun2;				//CRUN2�ṹ��,����ͬ crun �� crunNum
	CIRCU * circu;				//��·,����<= crun*2 ,�� circuNum ��¼
	CRUNMAP * maps;				//�������е���·
	unsigned short circuNum;	//��·����
	unsigned short groupNum;	//����,ͬһ�����һ����ͨͼ��,���齨������
	Equation ** equation;		//���̴������

	//��ʾ���Ʋ����---------------------------------------------------------------
	Pointer pressStart;			//������Ʋ����ʼλ��,ֻ��ָ���߻��߽ڵ�
	Pointer pressEnd;			//������Ʋ�Ľ���λ��,ֻ��ָ���߻��߽ڵ�
	double startEndPressure;	//��¼pressStart��pressEnd֮��ĵ��Ʋ�

	//��������
	MyVector circuitVector;		//��·��Ϣ����
	MyIterator vectorPos;		//��ǰ��������λ��

private:

	//1��ʼ��������ռ�------------------------------------------------------------
	void InitBitmap();			//��ʼ��λͼ
	void UninitBitmap();		//�ͷ�λͼ

	//2��������--------------------------------------------------------------------
	void PutCircuitToVector();					//����ǰ��·��Ϣ���浽����
	void ReadCircuitFromVector(MyIterator);		//��������ȡ��·��Ϣ
	void DeleteVector(MyIterator, MyIterator);	//ɾ��������һ����������
	void CloneCircuitBeforeChange();			//���Ƶ�ǰ��·������
	void UpdateUnReMenuState();					//���³������ظ��˵�״̬

	//3��ͼ����--------------------------------------------------------------------
	void PaintCrun(const CRUN *, bool isPaintName=true);	//�����
	void PaintCrunText(const CRUN *)const;					//���������
	void PaintCtrl(CTRL *, bool isPaintName=true);			//���ؼ�
	void PaintCtrlText(const CTRL *)const;					//���ؼ�����
	void PaintLead(LEAD * l);								//������
	void PaintAllLead();									//�����е���
	void PaintMouseMotivate(const Pointer &);				//����Ч��λ�������ͼ
	void PaintLeadWithStyle(LEAD *, int, enum COLOR);		//��ָ����ʽ�����ߵ���
	void PaintCrunWithColor(CRUN *, enum COLOR);			//��ָ����ɫ�����
	void PaintCtrlWithColor(CTRL *, enum COLOR);			//��ָ����ɫ���ؼ�
	void PaintWithSpecialColor(const Pointer &, bool);		//�ñ�����ɫ(��ɫ)��ʾ
	void PaintInvertBodyAtPos(const Pointer &, POINT);		//��ָ��λ����ʾ����ķ���

	//4��������--------------------------------------------------------------------
	CDC * GetCtrlPaintHandle(const CTRL *);			//��ÿؼ���ͼ���
	void GetName(const Pointer &, char *)const;		//�������
	bool DeleteNote(const Pointer &);				//ɾ����ʾ
	void ClearCircuitState();						//�����·״̬
	Pointer GetBodyPointer(FOCUS_OR_POS &);			//�������ָ��

	//5�༭����--------------------------------------------------------------------
	void AddLead(Pointer, Pointer);	//�õ�������2������
	void AddCrun(POINT);			//��ӹ����Ľ��
	void AddCtrl(POINT, BODY_TYPE);	//��ӹ����Ŀؼ�
	void DeleteLead(LEAD *);		//���ɾ������2����������ߵľ������
	void DeleteSingleBody(Pointer);	//ɾ��һ������
	void Delete(Pointer);			//ɾ��Pointer�ṹָ�������
	bool ConnectBodyLead(POINT);	//����һ�����ӵ�͵���

	//6�����Ϣ������------------------------------------------------------------
	void PosBodyMove(Pointer *, POINT, POINT);			//����ж�����
	bool PosBodyClone(const Pointer *, POINT, POINT);	//���ƶ�λ�ø�������
	bool MotivateAll(POINT &);							//��������������ʲôλ��
	bool ShowAddLead(POINT);							//���ӵ��߹�����ʾ
	bool ShowAddBody(POINT);							//������������ʾ
	bool ShowMoveBody(POINT, bool);						//�ƶ����������ʾ
	bool ShowMoveLead(bool);							//�ƶ����߹�����ʾ

	//8���㺯��--------------------------------------------------------------------
	//��2�������� ����� �ϲ�
	void CombineGroup(int, int, int *, int);
	//���2��crun2���ֱ�����ӵ���·����
	char GetCrun2ConnectNum(int, int);
	//�ɽ���Ż�õ�һ���������ǵ���·
	CIRCU * GetCrun2FirstCircu(int, int, int &);
	//��from,to����һ��������·�ĵ���������뻺��
	void PutIntoBuf(int, int, CRUNMAP *, double *);
	//������㷽��,�����������
	int CreateCrunEquation(CRUN2 *, double *);
	//����һ�ε�·,���ÿ��Ⱥ�����·��ѧ��Ϣ
	void CollectCircuitInfo();
	//��ָ���������,����·������ֵ��������
	void TravelCircuitPutElec(Pointer, const CRUN *, int, double, ELEC_STATE);
	//��ָ���������,����·�������õ�����·
	void TravelCircuitFindOpenBody(Pointer, int);
	//��ָ���������,��õ�ѹ�͵�����Ϣ,�Լ����յ�
	ELEC_STATE TravelCircuitGetOrSetInfo(Pointer, int, double &, ELEC_STATE);
	//������ĵ�������ֲ���ÿ�����ߺͿؼ�
	void DistributeAnswer();
	//�γ�һ�����j �� ��������·��,����j k֮���ֱ������
	bool FindRoad(const CRUNMAP *, ROAD *, int, int);
	//������·��Ϣ,��Ⱥ�齨������
	void CreateEquation();

	//10������а庯��-------------------------------------------------------------
	void ClearClipboard();					//��ռ��а�
	void CopyToClipboard(const Pointer &);	//����bodyָ������嵽���а�

	//11��꽹�㺯��(���������������)-------------------------------------------
	void UpdateEditMenuState();				//���±༭�˵�״̬
	void FocusBodyClear(const Pointer *);	//ɾ������
	void FocusBodySet(const Pointer &);		//���ý���
	bool FocusBodyPaint(const Pointer *);	//������

public:

	//1��ʼ��������ռ�------------------------------------------------------------
	Manager(CWnd *);
	~Manager();

	//2��������--------------------------------------------------------------------
	void UnDo();		//����
	void ReDo();		//�ظ�

	//3��ͼ����--------------------------------------------------------------------
	void PaintAll();	//��ʾ���е�����

	//4��������--------------------------------------------------------------------
	void SetAddState(BODY_TYPE);		//������Ӻ�������
	void SaveAsPicture(const char *);	//�����·��ͼƬ

	//6�����Ϣ������------------------------------------------------------------
	void Property(FOCUS_OR_POS &, bool);	//��ʾ�͸ı���������
	void ChangeCtrlStyle(FOCUS_OR_POS &);	//�ı�ؼ�����
	bool ShowBodyElec(FOCUS_OR_POS &);		//��ʾ����ĳ������ĵ���
	bool LButtonDown(POINT);				//����WM_LBUTTONDOWN
	bool LButtonUp(POINT);					//����WM_LBUTTONUP
	void MouseMove(POINT, bool);			//����WM_MOUSEMOVE
	bool AddBody(POINT);					//�������(�ؼ�����)
	bool Delete(FOCUS_OR_POS &);			//ɾ��
	void RotateCtrl(FOCUS_OR_POS &, int);	//��ת�ؼ�
	BODY_TYPE PosBodyPaintRect(POINT);		//ͻ���һ�����
	void Help(POINT pos);					//��F1Ѱ�����
	bool SearchNext(SEARCH_BY, BODY_TYPE, bool, bool, char *);	//������һ������
	bool SearchPre(SEARCH_BY, BODY_TYPE, bool, bool, char *);	//������һ������

	//7�ļ�����--------------------------------------------------------------------
	const char * GetFilePath();		//��ȡ�ļ�·��
	bool SaveFile(const char *);	//�����·���ļ�
	bool ReadFile(const char *);	//���ļ���ȡ��·
	void CreateFile();				//�����µĿ��ļ�

	//8���㺯��--------------------------------------------------------------------
	void CountElec();	//�������

	//9���Ժ���--------------------------------------------------------------------
	void SaveCircuitInfoToTextFile();	//�����·��Ϣ���ı��ļ�
	void SaveCountInfoToTextFile();		//���������̵��ı��ļ�

	//10������а庯��-------------------------------------------------------------
	bool GetClipboardState();			//��ȡ���а��Ƿ����
	Pointer CopyBody(FOCUS_OR_POS &);	//�������嵽���а�
	void CutBody(FOCUS_OR_POS &);		//�������嵽���а�
	bool PasteBody(POINT pos);			//ճ�����嵽����

	//11�������庯��---------------------------------------------------------------
	void FocusBodyChangeUseTab();	//�û���Tab���л����㴦��
	bool FocusBodyMove(int);		//�û����������Ҽ��ƶ���������

	//12���ú���-------------------------------------------------------------------
	void SetViewOrig(int, int);		//���û�ͼ�ĳ�ʼ����
	void SetMoveBodySense();		//���ð������һ���ƶ�����ľ���
	void SetTextColor();			//����������ɫ
	void SetFocusLeadStyle();		//���ý��㵼����ʽ
	void SetFocusCrunColor();		//���ý�������ɫ
	void SetFocusCtrlColor();		//���ý���ؼ���ɫ
	void SetLeaveOutDis();			//��������ߺϲ�����

	//13��ʾ���Ʋ��-------------------------------------------------------------
	void ClearPressBody();			//�����ʾ���Ʋ�ĳ�Ա����
	bool SetStartBody(POINT);		//���ü�����Ʋ����ʼλ��
	bool NextBodyByInputNum(UINT);	//�û���������1,2,3,4���ƶ����Ʋ��βλ��
	bool ShowPressure();			//��ʾ����ʼλ�õ���βλ�õĵ��Ʋ�(U0-U1)

};

////////////////////////////////////////////////////////////////////////////////
//��Ҫʹ��DPtoLP�ĺ��� : (����������豸����ת��Ϊ�߼�����
//PaintAll��Ϊ�˻�����ԭ�ͻ���������Ҫ��ת��
//�����������ǽ����˴��ڴ����������ת��)
//PaintAll,MotivateAll,LButtonUp,ShowAddLead,ShowMoveBody,PasteBody,AddThing
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//!	���㺯������˼·:
//	1,		û�н��Ļ�·,��ʱ�������,������㱣��,��DistributeAnswer���� ;
//	2,		2�����֮����2��������ֱ�����ӵĵ���,����֮���γɵĻ�·,�õ����ַ���
//	3		������һ�����Ļ�·,ֱ�Ӽ������·����,���뷽��
//	4,		�γ��ɽ����ɵ���·��Ϣ : (2�����֮�������2����������
//			ֱ��������Ϊһ��,����directֻ����Ϊ2,�����ټ��㻷·.)
//			������ֻ��һ��ֱ���������ӵ�2�����,
//			�γ� ����ֻ��һ������ֱ������ �Ļ�·.
//			�ɻ�·��Ϣ�ó�����
//	5,		Ȼ���ڷ�������ӽ�㷽��
//	6,		2~5�Ѿ�����������ķ���,�ֱ������
//	7,		�ڽ�������ʾ����Ľ��(�ڱ����������Ƶ������)
/////////////////////////////////////////////////////////////////////////////////

#endif	//!defined(AFX_CIRCUITCLASS_FDEF)
