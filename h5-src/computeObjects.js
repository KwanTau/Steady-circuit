
//����״̬ö��
var UNKNOWNELEC	= -2,	//����δ����
ERRORELEC		= -1,	//�������
NORMALELEC		= 0,	//��������
LEFTELEC		= 0,	//����������
RIGHTELEC		= 1,	//�������ҵ���
OPENELEC		= 6,	//��·
SHORTELEC		= 7,	//��·
UNCOUNTABLEELEC	= 8;	//�����޷�����ķ�֧


// �ṹ����
//CIRCU;		//��·,���������,�ɽ������
//CRUN2;		//���ڼ���Ľ����
//CRUNMAP;	    //����ÿ2�����֮���·��
//ROAD;		    //2�����֮�����ӵ�·��
//ROADSTEP;


var CRUN2 = {
	//CIRCU * c[4];     //ָ�����·
	//int group;        //���ڵ�Ⱥ,�����ӵĵ���,�ڵ�,�ؼ����
	createNew: function() {
        var c = new Array(4);
		c[0] = c[1] = c[2] = c[3] = null;   

        var newObj = {"c":c, "group":-1};   //group -1��������Ⱥ��
        return newObj;
	}
};

var CIRCU = {	//��·,���������,�ɽ������,��������Ϊfrom->to
	createNew: function() {
        var newObj = {
            eleNum:0,				//��·�������,Ĭ�ϱ���ǵ�ַ���
	        pressure:0,		        //������յ�ĵ��Ʋ�(U0 - U1)
	        resistance:0,		    //����(һ��>=0, <0���������,��·)
	        elecDir:NORMALELEC,		//����״̬���
	        elec:0,     			//��·������С
	        from:null, to:null,		//�����յ���
	        dirFrom:0, dirTo:0,	    //�����յ���ķ���
	        numInGroup:0			//��Ⱥ���ڵ����
         };

         return newObj;
	},

	ConvertWhenElecLessZero: function(circu) {	//����������ʱ��Ϊ����,����ת��������
		if(circu.elec >= 0) return;
		if(circu.elecDir != NORMALELEC) return;

		circu.pressure = -circu.pressure;
		circu.elec = -circu.elec;

		var tempCrun2 = circu.from;
		circu.from = circu.to;
		circu.to = tempCrun2;

		var tempDir = circu.dirFrom;
		circu.dirFrom = circu. dirTo;
		circu.dirTo = tempDir;
	}
};

var ROADSTEP = { //2�����֮��·���ϵ�һ�����
    createNew: function() {
        var newObj = {
	        crunNum:0,
	        next:null,
	        pre:null
        };
        return newObj;
    }
};

var ROAD = {
    createNew: function() {
        var newObj = {
	        first:null,
	        last:null
        };
        return newObj;
    },

	/*~ROAD() {
		if(first == null || last == null) return;
		ROADSTEP * now = first, * next;
		while(now)
		{
			next = now->next;
			delete now;
			now = next;
		}
	}*/

    Clone: function(road, newRoad) {
	    var temp, now, pre;

	    newRoad.first = newRoad.last = null;
	    if (road.first == null) return;

	    temp = road.first;
	    now = newRoad.first = ROADSTEP.createNew();
	    now.crunNum = temp.crunNum;
	    now.pre = null;
	    if (temp.next) {
		    now.next = ROADSTEP.createNew();
		    pre = now;
		    now = now.next;
		    now.pre = pre;

		    temp = temp.next;
	    } else {
		    now.next = null;
		    newRoad.last = now;
		    return;
	    }

	    while (temp) {
		    now.crunNum = temp.crunNum;
		    if (temp.next) {
			    now.next = ROADSTEP.createNew();
			    pre = now;
			    now = now.next;
			    now.pre = pre;

			    temp = temp.next;
		    } else {
			    now.next = null;
			    newRoad.last = now;
			    return;
		    }
	    }
    },

    //�ж��Ƿ��н��point
    HaveRoadPoint: function(road, point) {
	    var now = road.first;
	    while (now) {
		    if(now.crunNum == point)
			    return true;
		    now = now.next;
	    }
	    return false;
    },

    //�ж��Ƿ���from->to·��
    HaveRoadStep: function(road, from, to) {
	    if (!road.first)
            return false;
	    var pre = road.first;
	    var now = pre.next;
	    while (now != null) {
		    if (pre.crunNum == from && now.crunNum == to 
			    || pre.crunNum == to && now.crunNum == from)
			    return true;
		    pre = now;
		    now = now.next;
	    }
	    return false;
    },

    //����������
    InsertPointAtTail: function(road, crunNum) {
	    var now;

	    if (road.first) {
		    road.last.next = now = ROADSTEP.createNew();
		    now.pre = road.last;
		    road.last = now;
		    now.crunNum = crunNum;
		    now.next = null;
	    } else {
		    road.first = road.last = now = ROADSTEP.createNew();
		    now.crunNum = crunNum;
		    now.next = now.pre = null;
	    }
    }
};

//����ÿ�������֮���Ƿ�ֱ������
var CRUNMAP = {
	/*int size;			//�����Ľ����
	int circuNum;		//��������·��
	int * crunTOorder;	//��ɢ�Ľ���Ŷ�Ӧ�� 0 ~ size-1

	//-1 �м������ ;
	//0  ������(Ŀǰû���ҵ�·��) ;
	//1  ֻ����һ��ֱ������,Ŀǰû���ҵ�·�� ;
	//2  �ж���·��,����ֻ��һ����ֱ������,�����ҵ���·�� ;
	char *	direct;

	CIRCU ** firstcircu;	//����2�����ĵ�һ����·
	int * dir;				//����2�����ĵ�һ����·��������С�Ľ��ĵ��߱��(0,1,2,3)
    */

    createNew: function(size) {
        var bufSize = size*(size-1)/2;
        var newObj = {
            "size":         size,
            "circuNum":     0,
	        "crunTOorder":  new Array(size),
            "direct":       new Array(bufSize),
		    "firstcircu":   new Array(bufSize),
		    "dir":          new Array(bufSize)
        };
        return newObj;
    }
};




var Equation = {
/*private:
	double ** a, * x;	//a�洢����������m*n,x�Ƿ��̵Ľ�
	int m, n;			//n-1==�����ĸ���
	int gotoRow;		//��¼�Ѿ����뵽����
	int * c;			//c[i]�洢��i�е�һ������0����
*/

	createNew: function(/*int */crunNum, /*int */eleNum)
    {
	    gotoRow = 0;
	    m = -1;
	    n = 0;
	    c = NULL;
	    x = NULL;
	    a = NULL;

	    if(eleNum <= 0 || crunNum <= 0) return;	//�����ʼ��

	    m = eleNum + crunNum - 1;
	    n = eleNum + 1;

	    x = new Array(eleNum);
	    zeroArray(x);
	
	    c = new Array(m);
        zeroArray(c);
	
	    a = new Array(m);
	    for(/*int*/var i=m-1; i>=0; --i)
	    {
		    a[i] = new Array(n);
		    zeroArray(a[i]);
	    }
    },

    /*Equation::~Equation()
    {
	    if(m > 0 && n > 1)
	    {
		    free(c);
		    free(x);
		    for(int i=m-1; i>=0; --i) free(a[i]);
		    free(a);
	    }
    }*/

    /*const double **/GetAnswer: function()
    //��÷��̽������
    {
	    return x;
    },

    InputARow: function(/*const double * */buf)
    //�����gotoRows��ʼ��1�����ݵ���������
    {
	    ASSERT(gotoRow < m);

        arrayCopyWithSize(a[gotoRow], buf, n);
	    ++gotoRow;
    },

    OutputToFile: function(/*FILE * */fp)
    //�����̱��浽�ļ�,���Ժ���
    {
	    /*int*/var i, j;

	    fprintf(fp, "�����뵽 �� %d �� .\n\n", gotoRow);

	    fputs("x����(��):\n", fp);
	    for(i=0; i<n-1; ++i) fprintf(fp, "%6.2f ", x[i]);

	    fputs("\n\nc����(ĳһ�е�һ������0������λ��):\n", fp);
	    for(i=0; i<m; ++i) fprintf(fp, "%3d ", c[i]);

	    fputs("\n\na����(������,�洢nԪһ�η���):\n", fp);
	    for(i=0; i<m; ++i)
	    {
		    for(j=0; j<n; ++j) fprintf(fp, "%6.2f ", a[i][j]);
		    fputc('\n', fp);
	    }
	    fputc('\n', fp);
    },

    /*ELEC_STATE*/Count: function()
    {
	    /*const int*/var m = gotoRow;	//��¼�Ѿ����뵽����,������this->m
        /*int*/var i, j, l, k, w;
        /*double*/var temp;
	    if(m <= 0 || n <= 1) return NORMALELEC;	//�������
	    w = n<m-1 ? n : m-1;					//w��ֵΪm-1,n�Ľ�Сֵ

	    for(i=n-2; i>=0; --i) x[i] = 0;

	    //������-----------------------------------------------------------------------------
	    for(l=0,k=0; l<w; ++l,++k)
	    {
		    while(k < n)
		    {
			    for(i=l; i<m; ++i)
				    if(!IsFloatZero(a[i][k])) break;
			    if(i == m)
			    {
				    --w; 
				    ++k;
			    }
			    else 
			    {
				    break;
			    }
		    }

		    if(k == n)
		    {
			    if(l == 0) return NORMALELEC;	//l==0,������Ϊ0
			    break;
		    }

		    if(k == n-1) return SHORTELEC;	//��·��·

		    if(i != l)
		    {
			    for(j=k; j<n; ++j)
			    {
				    temp = a[i][j];
				    a[i][j] = a[l][j];
				    a[l][j] = temp;
			    }
		    }

		    for(i=l+1; i<m; ++i)
		    {
			    if(!IsFloatZero(a[i][k]))
			    {
				    temp = a[i][k] / a[l][k];
				    for(j=k; j<n; ++j) a[i][j] -= a[l][j] * temp;
			    }
			    else a[i][k] = 0;
		    }
	    }

	    //�жϲ����ص���---------------------------------------------------------------------
	    w = n - 1;	//m�����з�0ֵ�еĸ���
	    for(i=0; i<w; ++i)
	    {
		    for(j=i; j<n; ++j) if(!IsFloatZero(a[i][j])) break;
		    c[i] = j;

		    if(j > i)
		    {
			    if(j == n-1)
				    return SHORTELEC;		//��·��·
			    else
				    return UNCOUNTABLEELEC;	//�޷�����
		    }
	    }

	    for(i=0; i<w; ++i)
	    {
		    for(j=n-1; j>c[i]; --j) a[i][j] /= a[i][c[i]];
		    a[i][c[i]] = 1;
	    }

	    for(l=w-1; l>0; --l) 
	    {
		    ASSERT(!IsFloatZero(a[l][c[l]]));

		    for(i=0; i<l; ++i)
		    {
			    for(j=c[l]+1; j<n; ++j) a[i][j] -= a[l][j] * a[i][c[l]];
			    a[i][c[l]] = 0;
		    }
	    }

	    for(i=n-2; i>=0; --i) x[i] = a[i][n-1];							//������������
	    for(i=n-2; i>=0; --i) if(IsFloatZero(x[i])) x[i] = 0;	//����0������Ϊ0

	    return NORMALELEC;	//��������
    }

};
