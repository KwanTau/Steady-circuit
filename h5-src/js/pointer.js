
//ָ��3����������Ľṹ
var Pointer = {

	/*
	// ָ���������
	void * p;	
	
	// �����λ��:
	// -3,-5,-7,... ���� , -2,-4,-6,... ����
	// -1 ����(���,�ؼ�)
	// 1,2,3,4 �����������ӵ�
	// 0��ָ������
	// 5,6,7...���Ϸ�
	int atState;
	
	// ָ����������
	BODY_TYPE style;
	*/

	CreateNew: function() {
		return {p: null, atState: 0, style: BODY_NO};
	},
	//������Ϣ��json
	GenerateStoreJsonObj: function() {
		var index = 0;
		if (p != null)
			index = p.index;

		return {index:index, atState:atState, style:style};
	},
	//��json��ȡ��Ϣ
	ReadFromStoreJsonObj: function(jsonObj, leadList, crunList, ctrlList) {
		Clear();
		
		this.atState = jsonObj.atState;
		this.style = jsonObj.style;
		
		var index = jsonObj.index;
		if (this.IsOnLead()) {
			if (index >= 0 && index < leadList.length)
				SetOnLead(leadList[index]);
			else
				return false;
		} else if(this.IsOnCrun()) {
			if (index >= 0 && index < crunList.length)
				SetOnCrun(crunList[index]);
			else
				return false;
		} else if(this.IsOnCtrl()) {
			if (index >= 0 && index < ctrlList.length)
				SetOnCtrl(ctrlList[index]);
			else
				return false;
		} else {
			return false;
		}

		return true;
	},
	
	//���ָ��
	void Clear() {
		p = null;
		atState = 0;
		style = BODY_NO;
	},

	//����addState
	SetAtState(newState) {
		this.atState = newState;
	},
	//���addState
	GetAtState() {
		return this.atState;
	},
	//���style
	GetStyle() {
		return this.style;
	},
	//�жϽṹ���Ƿ�ָ������
	IsOnAny() {
		return atState != 0 && atState <= 4;
	},
	//�ж��Ƿ��ڵ�����
	IsOnLead() {
		return BODY_LEAD == style || atState <= -2;
	},
	//�ж��Ƿ���ˮƽ����
	IsOnHoriLead() {
		return atState <= -2 && (-atState)&1;
	},
	//�ж��Ƿ�����ֱ����
	IsOnVertLead() {
		return atState <= -2 && !( (-atState)&1 );
	},
	//�ж��Ƿ�������(����ؼ�)��
	IsOnBody(isNotIncludeConnPoint/*true*/) {
		if (isNotIncludeConnPoint == undefined) isNotIncludeConnPoint = true;
		
		if (isNotIncludeConnPoint)	//�ж��Ƿ���������,���������ӵ�
			return -1 == atState && (BODY_CRUN == style || IsCtrl(style));
		else		//�ж��Ƿ���������,�������ӵ�
			return BODY_CRUN == style || IsCtrl(style);
	},
	//�ж��Ƿ��ڽ����
	IsOnCrun() {
		return BODY_CRUN == style;
	},
	//�ж��Ƿ��ڿؼ���
	IsOnCtrl() {
		return IsCtrl(style);
	},
	//�ж��Ƿ������ӵ���
	IsOnConnectPos() {
		return atState >= 1 && atState <= 4;
	},
	
	//ָ����
	SetOnLead(lead, isSetAt/*true*/) {
		if (isSetAt == undefined) isSetAt = true;
		
		p = lead;
		style = BODY_LEAD;
		if (isSetAt) atState = -2;
	},
	//ָ����
	SetOnCrun(crun, isSetAt/*true*/) {
		if (isSetAt == undefined) isSetAt = true;
		
		p = crun;
		style = BODY_CRUN;
		if (isSetAt) atState = -1;
	},
	//ָ��ؼ�
	SetOnCtrl(ctrl, isSetAt) {
		p = ctrl;
		style = ctrl.GetStyle();	//����ؼ������ʼ�����
		if (isSetAt) atState = -1;
	},
	
	//�жϵ�ǰ�����Ƿ�ָ���������
	IsLeadSame(other) {
		return this.IsOnLead() && p == other;
	},
	//�жϵ�ǰ�����Ƿ�ָ��������
	IsCrunSame(other) {
		return this.IsOnCrun() && p == other;
	},
	//�жϵ�ǰ�����Ƿ�ָ������ؼ�
	IsCtrlSame(other) {
		return this.IsOnCtrl() && p == other;
	},
	//�ж�����Pointer�Ƿ�ָ��ͬһ������,���ж�atState
	IsBodySame(other) {
		return (this.style == other.style) && (this.p == other.p);
	},
	//�ж�����Pointer�ṹ�Ƿ�һ��,�ж�atState
	bool IsAllSame(other) {
		return (this.atState == other.atState)&& (this.style == other.style) && (this.p == other.p);
	},
	//������ӵ��Ӧ�ĵ��߱��(0,1,2,3)
	int GetLeadIndex() {
		return atState - 1;
	},
	// static����
	IsCtrl(type) {
		return (type >= SOURCE) && (type <= SWITCH);
	},

	//����������ӵ�λ�û�õ��߶˵�����
	GetPosFromBody() {
		var pos = {};
		var leadIndex = GetLeadIndex();

		if (this.IsOnCrun()) {	//���ӽ��
			pos.x = p.x;
			pos.y = p.y;
			switch (leadIndex) {
			case 0:	//���϶�
				pos.y -= DD;
				break;
			case 1:	//���¶�
				pos.y += DD - 1;	//��ʾ���������(-1)
				break;
			case 2:	//�����
				pos.x -= DD;
				break;
			case 3:	//���Ҷ�
				pos.x += DD - 1;	//��ʾ���������(-1)
				break;
			}
		} else if(this.IsOnCtrl()) {	//���ӿؼ�
			pos.x = p.x;
			pos.y = p.y;
			if (0 == (p.dir & 1)) {	//����
				pos.y += (BODYSIZE.cy>>1);
				if ((p.dir!=0) ^ (leadIndex!=0)) {	//���Ҷ�
					pos.x += BODYSIZE.cx - 1;	//��ʾ���������(-1)
				}
			} else {	//����
				pos.x += (BODYSIZE.cx>>1);
				if (((p.dir-1)!=0) ^ (leadIndex!=0)) {	//���¶�
					pos.y += BODYSIZE.cy - 1;	//��ʾ���������(-1)
				}
			}
		}
		
		return pos;
	},

	//������ӵ�λ��
	GetConnectPosDir: function() {
		ASSERT(IsOnConnectPos());
		
		if (this.IsOnCrun()) {
			return atState;
		} else { //if(this.IsOnCtrl())
			ASSERT(p.dir >=0 && p.dir <= 3);

			if (atState == 1) {
				switch (p.dir) {
				case 0:
					return 3;
				case 1: 
					return 1;
				case 2:
					return 4;
				case 3: 
					return 2;
				}
			} else { //if(atState == 2)
				switch (p.dir) {
				case 0:
					return 4;
				case 1: 
					return 2;
				case 2:
					return 3;
				case 3: 
					return 1;
				}
			}

			return -1;
		}
	}

};
