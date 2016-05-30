
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
	
	CreateNew: function(enumType, int * dataPoint) {
		var note;
		switch (enumType) {
		case CTRL_TYPE_ENUM:	//�ؼ�����
			note = CTRL_TYPE_NAMES;
			break;
		case LEAD_STYLE_ENUM:	//������ʽ
			note = LEAD_STYLE_NAMES;
			break;
		}
		
		return {"data": dataPoint, "note":note};
	},

	GetDataPoint: function() {
		return data;
	},

	GetOptionCount: function() {
		return note.length;
	},

	GetOptionNoteList: function() {
		return note;
	}
};

var LISTDATA = {	//�����б���Ϣ��

	CreateNew: function() {
		return {
			dataType: new Array(),	//ÿ������������
			minData: new Array(),	//�����short,int,long����������,����Сֵ
			maxData: new Array(),	//�����short,int,long����������,�����ֵ
			dataPoint: new Array(),	//����
			noteText: new Array()};	//ÿ��������������ʾ��Ϣ
	},

	Unint: function() {
		dataType = null;
		minData = null;
		maxData = null;
		dataPoint = null;
		noteText = null;
	},

	GetListSize() {
		return dataPoint.length;
	},

	//�����б��һ��, dataType != DATA_TYPE_enum, ��min>max��ʾû�д�С����
	void SetAMember(DATA_STYLE dataType, note, data, min/*1*/, max/*0*/) {
		ASSERT(dataType != DATA_TYPE_enum);
		if (min == undefined || max == undefined) {
			min = 1;
			max = 0;
		}

		dataType.push(dataType);
		minData.push(min);
		maxData.push(max);
		noteText.push(note);
		dataPoint.push(data);
	},

	//����style == DATA_TYPE_enum ��һ��, dataPoint ָ��һ�� ENUM_DATA_HANLDER
	void SetAEnumMember(note, data, enumType, min/*1*/, max/*0*/) {
		if (min == undefined || max == undefined) {
			min = 1;
			max = 0;
		}
		
		dataType.push(DATA_TYPE_enum);
		minData.push(min);
		maxData.push(max);
		noteText.push(note);
		dataPoint.push(ENUM_DATA_HANLDER.CreateNew(enumType, data));
	},

	//���һ��: row--��, chData--�ַ�������, enumData--ö������
	ERROR_TYPE CheckAMember(row, pageElement) {
		var chData;
		var enumData;

		switch (dataType[row]) {
		case DATA_TYPE_float:
			chData = pageElement.value;
			if (chData == null || chData.length == 0)
				return ERROR_STRNULL;
			else if (!IsFloatZero(chData))
				return ERROR_FLOATMIX;
			break;

		case DATA_TYPE_uint:
			chData = pageElement.value;
			if (chData == null || chData.length == 0)
				return ERROR_STRNULL;
			} else if (!IsUnsignedInteger(chData)) {
				return ERROR_UINTMIX;
			} else if (minData[row] <= maxData[row]) {	//�д�С����
				enumData = parseInt(chData);
				if (enumData < minData[row] || enumData > maxData[row])
					return ERROR_UINTOVER;
			}
			break;

		case DATA_TYPE_enum:
			enumData = pageElement.selectedIndex;
			if (enumData < 0 || enumData >= ((ENUM_STYLE*)dataPoint[row])->GetStyleNum()) {
				return ERROR_ENUMOVER;
			} else if (minData[row] <= maxData[row]) {	//�д�С����
				if (enumData < minData[row] || enumData > maxData[row])
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
	void SaveAMember(int row, pageElement) {
		var tmpData;

		switch (dataType[row]) {
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
		
		dataPoint[row].setValue(tmpData);
	}

};
