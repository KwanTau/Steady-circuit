
//��������:
var ERROR_NO				= 0;	//�޴�
var ERROR_STRNULL			= 1;	//�ַ���Ϊ��
var ERROR_FLOATMIX			= 2;	//���������зǷ��ַ�
var ERROR_UINTMIX			= 3;	//���������зǷ��ַ�
var ERROR_UINTOVER			= 4;	//���������ڷ�Χ
var ERROR_ENUMOVER			= 5;	//ö�ٲ���ѡ���ĳһ��
var ERROR_STRMIX			= 6;	//���Ʊ�ǩ���зǷ��ַ� [](){}
var ERROR_ENUMNOTALLOWED	= 7;	//ö����һЩ����²�����һЩֵ(��:����������ɫ����Ϊ��ɫ)


// ������������
var ENUM_DATA_HANLDER = {
	
	CreateNew: function(enumType, memeberName) {
		var noteList;
		switch (enumType) {
		case CTRL_TYPE_ENUM:	//�ؼ�����
			noteList = CTRL_TYPE_NAMES;
			break;
		case LEAD_STYLE_ENUM:	//������ʽ
			noteList = LEAD_STYLE_NAMES;
			break;
		}
		
		return {"memeberName": memeberName, "noteList":noteList};
	},

	GetMemeberName: function() {
		return memeberName;
	},

	GetOptionCount: function() {
		return noteList.length;
	},

	GetOptionNoteList: function() {
		return noteList;
	}
};

var LISTDATA = {	//�����б���Ϣ��

	CreateNew: function() {
		return {
			dataParent : null,
			dataTypeList: new Array(),		//ÿ������������
			dataMinList: new Array(),		//�����short,int,long����������,����Сֵ
			dataMaxList: new Array(),		//�����short,int,long����������,�����ֵ
			memeberNameList: new Array(),	//����
			noteTextList: new Array()};		//ÿ��������������ʾ��Ϣ
	},
	SetDataParent: function(dataParent) {
		this.dataParent = dataParent;
	},

	Unint: function() {
		dataTypeList = null;
		dataMinList = null;
		dataMaxList = null;
		memeberNameList = null;
		noteTextList = null;
	},

	GetListSize() {
		return memeberNameList.length;
	},

	//�����б��һ��, dataType != DATA_TYPE_enum, ��min>max��ʾû�д�С����
	void SetAMember(dataType, note, data, min/*1*/, max/*0*/) {
		ASSERT(dataType != DATA_TYPE_enum);
		if (min == undefined || max == undefined) {
			min = 1;
			max = 0;
		}

		dataTypeList.push(dataType);
		dataMinList.push(min);
		dataMaxList.push(max);
		noteTextList.push(note);
		memeberNameList.push(data);
	},

	//����style == DATA_TYPE_enum ��һ��, memeberNameList ָ��һ�� ENUM_DATA_HANLDER
	void SetAEnumMember(note, data, enumType, min/*1*/, max/*0*/) {
		if (min == undefined || max == undefined) {
			min = 1;
			max = 0;
		}
		
		dataTypeList.push(DATA_TYPE_enum);
		dataMinList.push(min);
		dataMaxList.push(max);
		noteTextList.push(note);
		memeberNameList.push(ENUM_DATA_HANLDER.CreateNew(enumType, data));
	},

	//���һ��: row--��, chData--�ַ�������, enumData--ö������
	ERROR_TYPE CheckAMember(row, pageElement) {
		var chData;
		var enumData;

		switch (dataTypeList[row]) {
		case DATA_TYPE_float:
			chData = pageElement.value;
			if (chData == null || chData.length == 0)
				return ERROR_STRNULL;
			else if (!IsStrPositiveFloat(chData))
				return ERROR_FLOATMIX;
			break;

		case DATA_TYPE_uint:
			chData = pageElement.value;
			if (chData == null || chData.length == 0)
				return ERROR_STRNULL;
			} else if (!IsUnsignedInteger(chData)) {
				return ERROR_UINTMIX;
			} else if (dataMinList[row] <= dataMaxList[row]) {	//�д�С����
				enumData = parseInt(chData);
				if (enumData < dataMinList[row] || enumData > dataMaxList[row])
					return ERROR_UINTOVER;
			}
			break;

		case DATA_TYPE_enum:
			enumData = pageElement.selectedIndex;
			if (enumData < 0 || enumData >= memeberNameList[row].GetOptionCount()) {
				return ERROR_ENUMOVER;
			} else if (dataMinList[row] <= dataMaxList[row]) {	//�д�С����
				if (enumData < dataMinList[row] || enumData > dataMaxList[row])
					return ERROR_ENUMNOTALLOWED;
			}
			break;

		case DATA_TYPE_string:
			//chData = pageElement.value;
			//if (!IsNormalStr(chData)) return ERROR_STRMIX;
			break;
		}
		return ERROR_NO;
	},

	//���ؼ��û��޸ĵ���Ϣ���浽ָ��ָ�������
	void SaveAMember(row, pageElement) {
		var tmpData;

		switch (dataTypeList[row]) {
		case DATA_TYPE_float:
			tmpData = parseFloat(pageElement.value);
			break;
		case DATA_TYPE_uint:
			tmpData = parseInt(pageElement.value);
			break;
		case DATA_STYLE_bool:
			tmpData = pageElement.checked;
			break;
		case DATA_TYPE_string:
			tmpData = pageElement.value;
			break;
		case DATA_TYPE_enum:
			tmpData = pageElement.selectedIndex;
			break;
		}
		
		if (dataTypeList[row] == DATA_TYPE_enum)
			dataParent[eval(memeberNameList[row].GetMemeberName())] = tmpData;
		else
			dataParent[eval(memeberNameList[row])] = tmpData;
	}

};
