
var DATA_NOTE_RESIST		= 0;
var DATA_NOTE_PRESS			= 1;
var DATA_NOTE_CURRENT		= 2;
var DATA_NOTE_RATING		= 3;
var DATA_NOTE_CAPA			= 4;
var DATA_NOTE_SWITCHONOFF	= 5;
var DATA_NOTE_HAVERESIST	= 6;



//��ǿؼ��Ƿ��ṩ��ѹ(1�ṩ,0���ṩ)
var PRESSURE_TYPE[CTRL_TYPE_COUNT] = new Array(true, false, false, false, false);

//��ǿؼ��Ƿ��е���(1�����е���,-1��·,0�޵���)
var RESISTANCE_TYPE[CTRL_TYPE_COUNT] = new Array(1, 1, 1, -1, 1);

//ÿ����ѧ���Զ�Ӧ��˵��
var DATA_NOTE = new Array(
	"����            (ŷķ-��)"	,
	"��ѹ             (����-U)"	,
	"����      (����/��-A/S)"	,
	"�����     (����-W)"		,
	"����          (΢��-��F)"	,
	"���رպ�"					,
	"�˵�Դ�е���"
);


//�ؼ���
var CTRL = {//!�����������@�ĺ�������8��,�����¿ؼ����Ͷ���ʱ��Ҫ��������͵Ĵ������

	//�ڵ�ȫ�ֳ�ʼ������
	globalInitOrder: 1,
	//����ȫ�ֳ�ʼ������
	ResetGlobalInitOrder: function() {
		return (CTRL.globalInitOrder = 1);
	},
	
	
	CreateNew: function(long memberIdx, x , y, ctrlStyle) {
		ASSERT(ctrlStyle >= 0 && ctrlStyle < CTRL_TYPE_COUNT);
		
		var initOrder = CTRL.globalInitOrder++;
		var newObj = {
			initOrder : initOrder,		//��ʼ�����
			index : memberIdx,			//�ڿؼ����������
			
			isPaintName : true,			//Ĭ����ʾ����ǩ
			name : "Ctrl" + initOrder,	//Ĭ������
			x : x, y : y,				//����
			lead : [null,null],			//������ӵ��ߵ�λ��,0��,1��,2��,3��*/
			
			dir : 0,					//�ؼ�Ĭ�Ϸ���
			style : ctrlStyle,
			
			elec : 0,					//�����ؼ��ĵ����� ��С(�ڷ������µĴ�С)
			elecDir : UNKNOWNELEC		//��������
		};
		
		this.InitDefaultData(ctrlStyle);
        return newObj;
	},
	// �����ؼ���Ϣ���µĿؼ�
	Clone: function(clonePurpose) {
		var newCtrl = CTRL.CreateNew(this.index, this.x, this.y, this.style);
		newCtrl.name = this.name;
		newCtrl.isPaintName = this.isPaintName;
		newCtrl.dir = this.dir;
		CloneCtrlData(newCtrl, this);

		if (CLONE_FOR_USE != clonePurpose) {
			newCtrl.initOrder = this.initOrder;
			--CTRL.globalInitOrder;
		}
		return newCtrl;
	},
	//������Ϣ��json
	GenerateStoreJsonObj: function() {
		var leadIndexArray = new Array();
		for (var i=0; i<2; ++i) {
			if (lead[i] != null)
				leadIndexArray.push(lead[i].index);
			else 
				leadIndexArray.push(-1);
		}

		var storeJsonObj = {
			isPaintName : this.isPaintName,
			name : this.name,
			x : this.x, y:this.y,
			lead : leadIndexArray,
			
			dir : this.dir,
			style : this.style,
		};
		return CloneCtrlData(storeJsonObj, this);
	},
	//��json��ȡ��Ϣ
	ReadFromStoreJsonObj: function(jsonObj, leadList) {
		ASSERT(jsonObj != null);
		ASSERT(leadList != null);

		var leadArray = new Array();
		for (var i=0; i<2; ++i) {
			if (jsonObj.lead[i] >= 0)
				leadArray.push(leadList[jsonObj.lead[i]]);
			else 
				leadArray.push(null);
		}
		
		this.isPaintName = jsonObj.isPaintName;
		this.name = jsonObj.name;
		this.x = jsonObj.x; this.y = jsonObj.y;
		this.lead = leadArray;
		
		this.dir = jsonObj.dir;
		this.style = jsonObj.style;

		CloneCtrlData(this, jsonObj);
	},
	
	// @��������, ��ʼ��Ĭ������
	InitDefaultData: function(ctrlStyle) {
		switch (ctrlStyle) {
		case SOURCE:
			newObj.pressure = 10;
			newObj.resist = 0;
			break;
		case RESIST:
			newObj.resist = 10;
			break;
		case BULB:
			newObj.rating = 10;
			newObj.resist = 5;
			break;
		case CAPA:
			newObj.capa = 10;
			newObj.resist = -1;
			break;
		case SWITCH:
			newObj.closed = false;
			newObj.resist = -1;
			break;
		}
	},
	// @���ƿؼ�����
	CloneCtrlData: function(toCtrl, fromCtrl) {
		toCtrl.resist = fromCtrl.resist;
		
		switch (fromCtrl.ctrlStyle) {
		case SOURCE:
			toCtrl.pressure = fromCtrl.pressure;
			break;
		case RESIST:
			break;
		case BULB:
			toCtrl.rating = fromCtrl.rating;
			break;
		case CAPA:
			toCtrl.capa = fromCtrl.capa;
			break;
		case SWITCH:
			toCtrl.closed = fromCtrl.closed;
			break;
		}
        return toCtrl;
	},
	// @��ÿؼ�����������
	GetSpecialData: function() {
		switch (style) {
		case SOURCE:
			return this.pressure;
		case RESIST:
			return this.resist;
		case BULB:
			return this.rating;
		case CAPA:
			return this.capa;
		case SWITCH:
			return this.closed;
		}

		return 0;
	},
	// ��ÿؼ��ĵ�ѹ
	GetPressure: function(direction) {
		if (this.hasOwnProperty("pressure")) {
			if (direction != 0)
				return - this.pressure;
			else
				return   this.pressure;
		}

		return 0;
	},

	//�ı�ؼ�����
	ChangeStyle: function(newStyle) {
		ASSERT(this.style != newStyle);
		this.style = newStyle;
		InitDefaultData(newStyle);
	},

	//��ÿؼ����ӵĵ�����
	GetConnectNum: function() {
		return (lead[0] != NULL) + (lead[1] != NULL); 
	},

	//Ѱ�ҵ������ĸ����� : 0��,1��,2��,3��
	GetDirect: function(l) {
		var i;
		for (i=0; i<2; ++i) {
			if (lead[i] == l) break;
		}
		if (i >= 2) return -1;	//û���ҵ�

		ASSERT(this.dir>=0 && this.dir<4);

		switch (this.dir)	//���ݿؼ������ж�
		{
		case 0: return 2 + i;	//0:2;1:3
		case 1: return i;		//0:0;1:1
		case 2: return 3 - i;	//0:3;1:2
		case 3: return 1 - i;	//0:1;1:0
		default: return 0;
		}
	},

	//�������ڿؼ���λ��
	At: function(xPos, yPos) {
		var ret = 0;

		var xInter = xPos - this.x - (BODYSIZE.cx>>1);
		var yInter = yPos - this.y - (BODYSIZE.cy>>1);

		if (0 == (dir&1)) {	//����
			if (xInter < 0) {
				xInter += (BODYSIZE.cx>>1);
				if (xInter*xInter + yInter*yInter <= DD*DD) {	//ѡ�������ӵ�
					if (0 == (dir&2)) ret = 1;
					else ret = 2;
				}
			} else {
				xInter -= (BODYSIZE.cx>>1);
				if (xInter*xInter + yInter*yInter <= DD*DD) {	//ѡ�������ӵ�
					if (0 == (dir&2)) ret = 2;
					else ret = 1;
				}
			}
		} else { //����
			if (yInter < 0) {
				yInter += (BODYSIZE.cy>>1);
				if (xInter*xInter + yInter*yInter <= DD*DD) {	//ѡ�������ӵ�
					if (0 == (dir&2)) ret = 1;
					else ret = 2;
				}
			} else {
				yInter -= (BODYSIZE.cy>>1);
				if (xInter*xInter + yInter*yInter <= DD*DD) {	//ѡ�������ӵ�
					if (0 == (dir&2)) ret = 2;
					else ret = 1;
				}
			}
		}

		if (ret != 0) {
			if (lead[ret-1] == null)
				return ret;
			else
				return -1;
		}

		if (xPos>=this.x && xPos<this.x+BODYSIZE.cx 
			&& yPos>=this.y && yPos<this.y+BODYSIZE.cy)
			return -1;	//�ڿؼ���

		return 0;
	},

	//��ת�ؼ�
	Rotate: function(rotateAngle90) {
		this.dir = (this.dir + rotateAngle90) % 4;
		if (lead[0]!=null) lead[0]->RefreshPos();
		if (lead[1]!=null) lead[1]->RefreshPos();
	},

	//@С�����Ƿ�ﵽ����ʶ�����
	IsBulbOn: function() {
		var sData = GetSpecialData();

		if (BULB != style)
			return false;	//����С����
		if (elecDir != LEFTELEC && elecDir != RIGHTELEC)
			return false;	//����û�м�����߲���������

		double tempData = GetResist() * elec * elec;

		return (!IsFloatZero(sData) && tempData >= sData);
	},

	//@���رպϻ��߶Ͽ�
	SwitchClosed: function(isSwitch) {
		if (SWITCH != style) return false;	//���ǿ���
		if (isSwitch) this.closed = !this.closed;
		return this.closed;
	},

	//@��CProperty������Ϣ
	GetDataList: function(list) {
		list.SetDataParent(this);

		list.SetAMember(DATA_TYPE_string, TITLE_NOTE, "name");
		list.SetAMember(DATA_TYPE_bool, TITLESHOW_NOTE, "isPaintName");

		switch (style) {
		case SOURCE:
			list.SetAMember(DATA_TYPE_float, DATA_NOTE[DATA_NOTE_PRESS], "pressure");
			list.SetAMember(DATA_TYPE_float, DATA_NOTE[DATA_NOTE_RESIST], "resist");
			break;

		case RESIST:
			list.SetAMember(DATA_TYPE_float, DATA_NOTE[DATA_NOTE_RESIST], "resist");
			break;

		case BULB:
			list.SetAMember(DATA_TYPE_float, DATA_NOTE[DATA_NOTE_RATING], "rating");
			list.SetAMember(DATA_TYPE_float, DATA_NOTE[DATA_NOTE_RESIST], "resist");
			break;

		case CAPA:
			list.SetAMember(DATA_TYPE_float, DATA_NOTE[DATA_NOTE_CAPA], "capa");
			break;

		case SWITCH:
			list.SetAMember(DATA_TYPE_bool, DATA_NOTE[DATA_NOTE_SWITCHONOFF], "closed");
			// ���޸������Ҫ��������resist
			break;
		}
	},
	//@CProperty��������֮��
	AfterSetProperty: function() {
		switch (style) {
		case SWITCH:
			if (this.closed);
				this.resist = 0;
			else
				this.resist = -1;
			break;
		}
	}

};