function true1AndFalse0(flag) { if (flag) return 1; else return 0; }

function zeroArray(array) {
    for (var i = array.length - 1; i >= 0; --i) array[i] = 0;
}
function indexOfArray(array, element) {
    for (var i=array.length-1; i>=0; --i) if (array[i] == element) return i;
    return -1;
}
function nextElementOfArray(array, element) {
    for (var i=array.length-1; i>=0; --i) {
		if (array[i] == element) {
			if (i+1 < array.length)
				return array[i+1];
			else
				return null;
		}
	}
    return null;
}
function genrateArrayWithElementInitFunc(initFunc, size) {
    var a = new Array(size);
    for (var i=size-1; i>=0; --i) 
        a[i] = initFunc();
    return a;
}
function arrayCopyAll(to, from) {
    for (var i = from.length - 1; i >= 0; --i) to[i] = from[i];
}
function arrayCopyWithSize(to, from, size) {
    for (var i = size - 1; i >= 0; --i) to[i] = from[i];
}
function subArray(array, startIndex, endIndex/*array.length*/) {
	var newArray = new Array();
	if (endIndex <= 0)
		return newArray;
	
	if (endIndex == undefined || endIndex > array.length)
		endIndex = array.length;
    for (var i = startIndex; i < endIndex; ++i) newArray.push(array[i]);
	return newArray;
}

// ��ȸ��ƶ���
function deepCopy(source) { 
	var result = {};
	for (var key in source) {
		result[key] = typeof source[key]==='object'? deepCopy(source[key]) : source[key];
	}
	return result;
}


function ASSERT(flag) {
    if (!flag)
        console.log("error");
    return flag;
}

function IsFloatZero(/*double*/x) {   //�ж�ĳ���������Ƿ����Ϊ0
    return x > -(1e-9) && x < (1e-9);
}

function IsElecError(/*const ELEC_STATE */e)	//�����Ƿ�����
{
	return e < NORMALELEC || e > OPENELEC;
}

/*bool*/function IsFloat(/*const char * */str)	//�ж��ַ����Ƿ��Ǹ�������
{
	/*int*/var count = 0;

	//����Ƿ����ֻ��һ��'.',û����������������ַ�
	for (var i=0; i<str.length; ++i)
	{
	    var c = str.charCodeAt(i);
		if (46/*'.'*/ == c)
		{
			++count;
			if (count > 1) return false;
		}
        else if (c < 48 || c > 57)
		{
			return false;
		}
	}

	return true;
}

/*bool*/function IsUnsignedInteger(/*const char * */str)	//�ж��ַ����Ƿ���������
{
    for (var i=0; i<str.length; ++i) {
        var c = str.charCodeAt(i);
        if (c < 48 || c > 57)
            return false;
    }
    return true;
}
