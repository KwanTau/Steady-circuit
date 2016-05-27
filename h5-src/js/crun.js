//�����
var CRUN = {
	//�ڵ�ȫ�ֳ�ʼ������
	globalInitOrder: 1,
	//����ȫ�ֳ�ʼ������
	ResetGlobalInitNum: function() {
		return (globalInitOrder = 1);
	},

	CreateNew: function(memIdx, x, y) {
		var initOrder = globalInitOrder++;
		var newObj = {
			initOrder : initOrder,		//��ʼ�����
			index : memIdx,				//�ڽڵ����������
			isPaintName : false,		//Ĭ�ϲ���ʾ����ǩ
			name : "Crun" + initOrder,	//Ĭ������
			x:x, y:y,					//����
			lead:[null,null,null,null]	//������ӵ��ߵ�λ��,0��,1��,2��,3��*/
		};
        return newObj;
	},

	//����ڵ���Ϣ��json
	GenerateStoreJsonObj: function(jsonObj) {
		ASSERT(jsonObj != null);

		var leadIndexArray = new Array();
		for (var i=0; i<4; ++i) {
			if (lead[i])
				leadIndexArray.push(lead[i].index);
			else 
				leadIndexArray.push(-1);
		}
		
		var newObj = {
			index : this.index,
			isPaintName : this.isPaintName,
			name : this.name,
			x : this.x, y:this.y,
			lead : leadIndexArray
		};
        return newObj;
	},
	//��json��ȡ�����Ϣ
	ReadFromStoreJsonObj: function(jsonObj, allLead) {
		ASSERT(jsonObj != null);

		var leadArray = new Array();
		for (var i=0; i<4; ++i) {
			if (jsonObj.lead[i] >= 0)
				leadArray.push(allLead[jsonObj.lead[i]]);
			else 
				leadArray.push(null);
		}
		
		this.index = jsonObj.index;
		this.isPaintName = jsonObj.isPaintName;
		this.name = jsonObj.name;
		this.x = jsonObj.x; this.y = jsonObj.y;
		this.lead = leadArray;
	},

	//�������ڽ���λ��
	At: function (xPox, yPos) {
		var dis, disBetweenCenter;

		disBetweenCenter = (xPox-this.x)*(xPox-this.x)+(yPox-this.y)*(yPox-this.y);
		if (disBetweenCenter > 4 * DD * DD) return 0;	//�����Զ

		dis = (xPox-this.x)*(xPox-this.x)+(yPox-this.y+DD)*(yPox-this.y+DD);
		if (dis <= DD) {	//�������ӵ�
			if (lead[0] != null) return -1;
			else return 1;
		}

		dis = (xPox-this.x)*(xPox-this.x)+(yPox-this.y-DD)*(yPox-this.y-DD);
		if (dis <= DD) {	//�������ӵ�
			if (lead[1] != null) return -1;
			else return 2;
		}

		dis = (xPox-this.x+DD)*(xPox-this.x+DD)+(yPox-this.y)*(yPox-this.y);
		if (dis <= DD) {	//�������ӵ�
			if (lead[2] != null) return -1;
			else return 3;
		}

		dis = (xPox-this.x-DD)*(xPox-this.x-DD)+(yPox-this.y)*(yPox-this.y);
		if (dis <= DD)	//�������ӵ�
		{
			if (lead[3] != null) return -1;
			else return 4;
		}

		if (disBetweenCenter <= DD * DD) return -1;	//�ڵ���

		return 0;
	},

	//�����ؼ������Ϣ���µĽ��
	Clone: function(clonePurpose) {
		var newCrun = CRUN.CreateNew(this.index, this.x, this.y);
		newCrun.isPaintName = this.isPaintName;
		newCrun.name = this.name;

		if (CLONE_FOR_USE != clonePurpose) {
			newCrun.initOrder = this.initOrder;
			--globalInitOrder;
		}
		return newCrun;
	},

	//��CProperty����
	GetDataList: function (LISTDATA * list) {
		list.Init(2);
		list.SetAMember(TITLE_NOTE, name);
		list.SetAMember(TITLESHOW_NOTE, isPaintName);
	},

	//Ѱ�ҵ������ĸ�����
	GetDirect: function(l) {
		for(int i=0; i<4; ++i) if (lead[i] == l) return i;
		return -1;	//û���ҵ�
	},

	//��������˼�������
	GetConnectNum: function() {
		return  (lead[0] != null) + 
				(lead[1] != null) + 
				(lead[2] != null) + 
				(lead[3] != null);
	}
};