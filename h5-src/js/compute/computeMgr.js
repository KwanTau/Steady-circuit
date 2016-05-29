var ComputeMgr = {};


ComputeMgr.CONVERT = function(a,b,size)	{return size*a-(a+1)*a/2+b-a-1;}	//����CRUNMAP��Ա�ķ���




ComputeMgr.CombineGroup = function(
							/*int*/from,
							/*int*/to,
							/*int **/group,
							/*int*/groupSize)
//��2�������� ����� �ϲ�
{
	ASSERT(group && groupSize>0 && from>=0 && to>=0);
	for (var i=groupSize-1; i>=0; --i) if (group[i]==from)
		group[i] = to;
};

/*char*/ComputeMgr.GetCrun2ConnectNum = function(/*int*/a, /*int*/b)
//���2��WmMgr.crun2���ֱ�����ӵ���·����
{
	/*int*/var i;
	/*char*/var connect = 0;
	/*CIRCU **/var temp;
	for (i=0; i<4; ++i)
	{
		temp = ComputeMgr.crun2[a].c[i];
		if (!temp)continue;
		if (temp.to == ComputeMgr.crun2[b] || temp.from == ComputeMgr.crun2[b]) ++connect;
	}
	return connect;
};

/*{c:CIRCU *,dir:int}*/ComputeMgr.GetCrun2FirstCircu = function(/*int*/ a, /*int*/ b)
//�ɽ���Ż�õ�һ���������ǵ���·
//�������ص� {��·ָ��, ��·��WmMgr.crun2[a].c[i]��i}
{
	/*CIRCU **/var temp;

	for (var i=0; i<4; ++i)
	{
		temp = ComputeMgr.crun2[a].c[i];
		if (!temp) continue;
		if (temp.to == ComputeMgr.crun2[b] || temp.from == ComputeMgr.crun2[b])
		{
			num = i;
			return {"c":temp, "dir":i};
		}
	}

	return null;
};

ComputeMgr.PutIntoBuf = function(/*int */fromGroup,
							/*int */toGroup,
							/*CRUNMAP * */map,
							/*double * */buf) {
//��from,to����һ��������·�ĵ���������뻺��
	/*int*/var i;
	/*CIRCU **/var c;
	ASSERT(map != null && buf != null);

	if (fromGroup < toGroup)
		i = CONVERT(fromGroup, toGroup, map.size);
	else
		i = CONVERT(toGroup, fromGroup, map.size);

	c = map.firstCircuit[i];

	if (indexOfArray(ComputeMgr.crun2, c.from) == map.crunOrderMap[fromGroup]) {
		buf[ c.indexInGroup ]  =  c.resistance;
		buf[ map.circuitCount ] += c.pressure;
	} else {
		buf[ c.indexInGroup ]  = -c.resistance;
		buf[ map.circuitCount ] -= c.pressure;
	}
};

ComputeMgr.CreateCrunEquation = function(/*CRUN2 * */inputCrun2, /*double * */buf)
//������㷽��,�����������
{
	/*CIRCU ***/var tempCircu = inputCrun2.c;
	/*int*/var connectNum = 0;
	/*int*/var i;

	for (i=0; i<4; ++i) if (tempCircu[i])
	{
		++ connectNum;

		if (inputCrun2 == tempCircu[i].from && i == tempCircu[i].dirFrom)
			buf[tempCircu[i].indexInGroup] += 1;
		else
			buf[tempCircu[i].indexInGroup] -= 1;
	}

	return connectNum;
};

ComputeMgr.CollectCircuitInfo = function()
//����һ�ε�·,���ÿ��Ⱥ�����·��ѧ��Ϣ
{
	/*Pointer*/var now, pre;	//��ǰ������
	/*int*/var dir;			    //��һ�������ڵ�ǰ����ķ���
	/*int*/var i, j, tempVar;	//ѭ������
	/*int*/var endCrunIndex;	//��·���������
	/*int **/var group;		    //�������
	/*int*/var groupSize = 0;	//��ǰʹ�õ�group����Ĵ�С

	//1,��ʼ��----------------------------------------------------
	ComputeMgr.groupCount = 0;	//����,ͬһ�����һ����ͨͼ��,���齨������
	ComputeMgr.circuitCount = 0;	//��·��
	group = new Array(ComputeMgr.crunCount);		//�������ᳬ��ComputeMgr.crunCount
	ComputeMgr.crun2 = genrateArrayWithElementInitFunc(CRUN2.CreateNew, ComputeMgr.crunCount);		//���ڼ���Ľ��
	ComputeMgr.circu = genrateArrayWithElementInitFunc(CIRCU.CreateNew, ComputeMgr.crunCount*2);	//��·�����ᳬ��ComputeMgr.crunCount*2
	for (i=ComputeMgr.crunCount-1; i>=0; --i)group[i] = i;

	//2��������·,�Խ��Ϊͷ��β-----------------------------------
	for (i=ComputeMgr.crunCount-1; i>=0; --i) if (crun[i].GetConnectNum() >= 3)
	//���������ӵ��߸���������3,�����㲻��Ҫ����
	//0--����ǹ�����;1--����·;2--����൱�ڵ���
		for (j=0; j<4; ++j) if (crun[i].lead[j] && ComputeMgr.crun2[i].c[j] == null)
	//���㵱ǰ�����е������� ���� û�м�����(ComputeMgr.crun2[i].c[j] == null)
	{
		now = lead[j];
		dir = true1AndFalse0(now.conBody[0].p == crun[i]);

		ComputeMgr.circu[ComputeMgr.circuitCount].resistance = 0;	//������0
		ComputeMgr.circu[ComputeMgr.circuitCount].pressure   = 0;	//��ѹ��0

		while (true)	//������һ���������岻��2���Ľ�����
		{
			pre = now;

			if (now.hasOwnProperty("resist"))	//�ؼ�
			{
				//����ؼ�
				if (now.resist < 0 || now.lead[dir] == null)	//��·��
				{
					ComputeMgr.circu[ComputeMgr.circuitCount].resistance = -1;
					break;
				}
				ComputeMgr.circu[ComputeMgr.circuitCount].resistance += now.resist;
				ComputeMgr.circu[ComputeMgr.circuitCount].pressure   += now.GetPressure(1-dir);	//�����෴

				//����һ������
				now = pre.lead[dir];
				dir = true1AndFalse0(now.conBody[0].p == pre);
			}
			else	//����,����һ������
			{
				now = pre.conBody[dir].p;
				if (IsOnCrun(now))	//�����������岻��2���Ľ�����
				{
					tempVar = now.GetConnectNum();
					if (tempVar >= 3)	//ͨ·
					{
						dir = now.GetDirect(pre);	//��¼�½����Ľ��ͷ���
						break;
					}
					else if (tempVar == 2)	//����,�൱�ڵ���
					{
						//�ҵ�������ӵ���һ������
						for (dir=0; dir<4; ++dir)
							if (now.lead[dir]!=null && pre != now.lead[dir]) break;
						//ת��������ӵ���һ������
						pre = now;
						now = pre.lead[dir];
						dir = true1AndFalse0(now.conBody[0].p == pre);
					}
					else if (tempVar == 1)	//��·
					{
						ComputeMgr.circu[ComputeMgr.circuitCount].resistance = -1;
						break;	//��·����
					}
					else
					{
						throw "��·�ļ��д� !�������ӽ��,���ǽ�㲻���ӵ��� !";
						break;
					}
				}
				else if (now.hasOwnProperty("resist"))
				{
					dir = true1AndFalse0(now.lead[0] == pre);
				}
				else if (IsOnLead(now))
				{
					throw "2������ֱ�����ӳ��ֻ�������� !";
					break;
				}
				else
				{
					throw "��·�ļ��д� !����ֻ����һ������ !";
					break;
				}
				
			}
		}//while (true)

		if (ComputeMgr.circu[ComputeMgr.circuitCount].resistance >= 0)
		{
			endCrunIndex = now.num;

			ComputeMgr.circu[ComputeMgr.circuitCount].eleIndex = ComputeMgr.circuitCount;
			ComputeMgr.circu[ComputeMgr.circuitCount].from = ComputeMgr.crun2[i];
			ComputeMgr.circu[ComputeMgr.circuitCount].dirFrom = j;
			ComputeMgr.circu[ComputeMgr.circuitCount].to = ComputeMgr.crun2[endCrunIndex];
			ComputeMgr.circu[ComputeMgr.circuitCount].dirTo = dir;
			ComputeMgr.crun2[endCrunIndex].c[dir] = ComputeMgr.crun2[i].c[j] = ComputeMgr.circu[ComputeMgr.circuitCount];
			++ ComputeMgr.circuitCount;	//���Ƕ�·,������Ч��·

			if (ComputeMgr.crun2[i].group >= 0)
			{
				if (ComputeMgr.crun2[endCrunIndex].group >= 0 && group[ComputeMgr.crun2[i].group] == group[ComputeMgr.crun2[endCrunIndex].group])
					continue;
				if (ComputeMgr.crun2[endCrunIndex].group >= 0)	//group�ϲ�
				{
					CombineGroup(   group[ComputeMgr.crun2[endCrunIndex].group],
									group[ComputeMgr.crun2[i].group],
									group,
									groupSize);
					--ComputeMgr.groupCount;	//�ϲ�
				}
				else	//�̳����������group
				{
					ComputeMgr.crun2[endCrunIndex].group = ComputeMgr.crun2[i].group;
				}
			}
			else
			{
				if (ComputeMgr.crun2[endCrunIndex].group>=0)	//�̳����������group
				{
					ComputeMgr.crun2[i].group=ComputeMgr.crun2[endCrunIndex].group;
				}
				else	//�����µ�group
				{
					ComputeMgr.crun2[i].group = ComputeMgr.crun2[endCrunIndex].group = groupSize;
					++groupSize;
					++ComputeMgr.groupCount;	//�����µ�
				}
			}

		}//if ( ComputeMgr.circu[i].resistance >= 0 )

	}//for

	//3,��group���гɴ�0��ʼ����������-----------------------------------
	dir = 0;
	for (i=ComputeMgr.groupCount-1; i>=0; --i)
	{
		for (; dir<groupSize; ++dir) if (group[dir] >= 0) break;
		for (j=dir+1; j<groupSize; ++j) if (group[j]==group[dir]) group[j] = -i - 1;
		group[dir] = -i - 1;
		++dir;
	}
	for (j=groupSize-1; j>=0; --j) group[j] = -group[j] - 1;

	//4,����WmMgr.crun2��group������������ӱ�־,����ָ��group�����ָ��-------
	//����ת��Ϊ��������ӱ�־
	for (i=ComputeMgr.crunCount-1; i>=0; --i) if (ComputeMgr.crun2[i].group >= 0) ComputeMgr.crun2[i].group = group[ComputeMgr.crun2[i].group];

	group = null;//delete [] group;
};

/*bool*/ComputeMgr.FindRoad = function(/*const CRUNMAP * */map, /*ROAD * */roads, /*int */j, /*int */k)
//�γ�һ�����j �� ��������·��,����j k֮���ֱ������
{
	/*const int*/var size = map.size;
	/*bool*/var state;			//��¼�Ƿ�ı���
	/*int*/var i, next;		    //ѭ������
	/*bool **/var interFlag;	//j �Ƿ��ҵ���ĳ�����ߵı��

	interFlag = new Array(size);
	for (i=size-1; i>j; --i) interFlag[i] = map.direct[CONVERT(j,i,size)] > 0;
	for (i=j-1; i>=0; --i) interFlag[i] = map.direct[CONVERT(i,j,size)] > 0;
	interFlag[j] = interFlag[k] = false;

	do
	{
		state = false;	//����ϴ�״̬

		for (i=size-1; i>=0; --i) if (i!=j && i!=k && interFlag[i])
		{
			for (next=size-1; next>i; --next)
				if (next-j && !interFlag[next] && map.direct[CONVERT(i,next,size)]>0)
			{
				state = true;	//�ı���
				interFlag[next] = true;
				roads[i].Clone(roads[next]);
				roads[next].InsertPointAtTail(i);
			}

			if (interFlag[k]) {state = false;break;}	//�˳�ѭ��

			for (next=i-1; next>=0; --next)
			{
				if (next != j && !interFlag[next] && map.direct[CONVERT(next,i,size)] > 0)
				{
					state = true;	//�ı���
					interFlag[next] = true;
					roads[i].Clone(roads[next]);
					roads[next].InsertPointAtTail(i);
				}
			}

			if (interFlag[k]) {state = false;break;}	//�˳�ѭ��
		}

	}
	while (state);


	state = interFlag[k];
	interFlag = null;//delete [] interFlag;
	return state;	//�����Ƿ��ҵ� j.k ����·
}

ComputeMgr.CreateEquation = function()
//������·��Ϣ,��Ⱥ�齨������
{
	/*int*/var group, i, j, k, size;
	/*CRUNMAP **/var nowMap;
	/*CRUNMAP **/var maps;

	//1 ��ʼ��maps---------------------------------------------------------
	//1.1 ��ʼ��ÿ��group��crun��Ա����
    var mapsSizeArray = new Array(ComputeMgr.groupCount);
    zeroArray(mapsSizeArray);
    for (i=ComputeMgr.crunCount-1; i>=0; --i) if (ComputeMgr.crun2[i].group >= 0)
		++ mapsSizeArray[ComputeMgr.crun2[i].group];

	ComputeMgr.maps = maps = new Array(ComputeMgr.groupCount);
	for (i=ComputeMgr.groupCount-1; i>=0; --i)
	{
		maps[i] = CRUNMAP.CreateNew(mapsSizeArray[i]);
		maps[i].size = 0;
	}
	for (i=ComputeMgr.crunCount-1; i>=0; --i) if (ComputeMgr.crun2[i].group >= 0)
	{
		nowMap = maps[ComputeMgr.crun2[i].group];
		nowMap.crunOrderMap[nowMap.size] = i;
		++nowMap.size;
	}

	//1.2 ��ʼ��ÿ��group��WmMgr.circu��Ա����
	for (i=ComputeMgr.groupCount-1; i>=0; --i) maps[i].circuitCount = 0;
	for (i=ComputeMgr.circuitCount-1; i>=0; --i)
	{
		nowMap = maps[ComputeMgr.circu[i].from.group];
		ComputeMgr.circu[i].indexInGroup = nowMap.circuitCount;
		++nowMap.circuitCount;
	}

	//1.3 ��ʼ��ÿ��group��direct��Ա����
	for (group=ComputeMgr.groupCount-1; group>=0; --group)
	{
		nowMap = maps[group];
		size = nowMap.size;

		for (j=size-2; j>=0; --j) for (k=size-1; k>j; --k)
		{
			i = CONVERT(j, k, size);
			nowMap.direct[i] = GetCrun2ConnectNum(nowMap.crunOrderMap[j], nowMap.crunOrderMap[k]);
		}
	}

	//1.4  ��ʼ������2�����ĵ�һ����·
	for (group=ComputeMgr.groupCount-1; group>=0; --group)
	{
		nowMap = maps[group];
		size = nowMap.size;

		for (j=size-2; j>=0; --j) for (k=size-1; k>j; --k)
		{
			i = CONVERT(j, k, size);
            var crunfc = GetCrun2FirstCircu(nowMap.crunOrderMap[j], nowMap.crunOrderMap[k]);
			nowMap.firstCircuit[i] = crunfc.c;
            nowMap.dir[i] = crunfc.dir;
		}
	}


	//2	2�����֮����>=2��ֱ�����ӵ���·,	-----------------------------
	//	����֮���γɻ�·,�õ����ַ���		-----------------------------
	/*double **/var outPutBuf;	//��������̵Ļ�������
	/*double*/var saveForBuf;	//���沿������
	/*CRUN2 **/var crunNum1, crunNum2;
	/*int*/var connect, firstConnect, nextConnect;

	ComputeMgr.equation = new Array(ComputeMgr.groupCount);	//����ָ��
	for (group=ComputeMgr.groupCount-1; group>=0; --group)
	{
		nowMap = maps[group];
		size = nowMap.size;

		outPutBuf = new Array(nowMap.circuitCount+1);	//��ʼ����������̵�����
		ComputeMgr.equation[group] = Equation.CreateNew(size, nowMap.circuitCount);	//��ʼ��������

		for (j=size-2; j>=0; --j) for (k=size-1; k>j; --k)
		{
			ZeroArray(outPutBuf);	//��������
			i = CONVERT(j, k, size);
			if (nowMap.direct[i] < 2) continue;

			crunNum1 = ComputeMgr.crun2[nowMap.crunOrderMap[j]];
			crunNum2 = ComputeMgr.crun2[nowMap.crunOrderMap[k]];
			firstConnect = nowMap.dir[i];	//��һ������

			//��ȡ�����沿����·����
			if (crunNum1.c[firstConnect].from == crunNum2)
			{
				outPutBuf[crunNum1.c[firstConnect].indexInGroup] =
					-crunNum1.c[firstConnect].resistance;
				saveForBuf = -crunNum1.c[firstConnect].pressure;
			}
			else
			{
				outPutBuf[crunNum1.c[firstConnect].indexInGroup] =
					crunNum1.c[firstConnect].resistance;
				saveForBuf = crunNum1.c[firstConnect].pressure;
			}
			nextConnect = firstConnect + 1;

			//2�����֮����>=2��ֱ�����ӵ���·,����֮���γɻ�·,�õ����ַ���
			for (connect=nowMap.direct[i]-2; connect>=0; --connect)
			{
				//1,Ѱ����һ�����ӵ���·
				while (	crunNum1.c[nextConnect] == null
						|| 
						(	crunNum1.c[nextConnect].to != crunNum2 
							&& 
							crunNum1.c[nextConnect].from != crunNum2
						)
					 )
					++ nextConnect;

				//2,������,��ѹ����
				outPutBuf[nowMap.circuitCount] = saveForBuf;	//д�뱣�������
				if (crunNum1.c[nextConnect].from == crunNum2)
				{
					outPutBuf[crunNum1.c[nextConnect].indexInGroup] =
						crunNum1.c[nextConnect].resistance;
					outPutBuf[nowMap.circuitCount] += 
						crunNum1.c[nextConnect].pressure;
				}
				else
				{
					outPutBuf[crunNum1.c[nextConnect].indexInGroup] =
						- crunNum1.c[nextConnect].resistance;
					outPutBuf[nowMap.circuitCount] -= 
						crunNum1.c[nextConnect].pressure;
				}

				//3,���뷽��
				ComputeMgr.equation[group].InputARow(outPutBuf);

				//4,�ָ�
				outPutBuf[crunNum1.c[nextConnect].indexInGroup] = 0;

				//5,��һ��
				++ nextConnect;
			}
		}

		outPutBuf = null;//delete [] outPutBuf;	//�ͷŻ���
	}//for (group


	//3 ������һ�����Ļ�·,ֱ�Ӽ��������뷽��  ----------------------
	for (i=ComputeMgr.circuitCount-1; i>=0; --i) if (ComputeMgr.circu[i].from == ComputeMgr.circu[i].to)
	{
		//��ʼ������
		group  = ComputeMgr.circu[i].from.group;
		nowMap = maps[group];
		outPutBuf = new Array(nowMap.circuitCount+1);	//��������̵Ļ���
		ZeroArray(outPutBuf);	//��������

		if (IsFloatZero(ComputeMgr.circu[i].resistance) && IsFloatZero(ComputeMgr.circu[i].pressure))
		{//�����ѹ����0;�����Ϊ1,��ѹΪ0
			outPutBuf[ComputeMgr.circu[i].indexInGroup]	= 1;
			outPutBuf[nowMap.circuitCount]		= 0;
		}
		else
		{//����������·���
			outPutBuf[ComputeMgr.circu[i].indexInGroup]	= ComputeMgr.circu[i].resistance;
			outPutBuf[nowMap.circuitCount]		= ComputeMgr.circu[i].pressure;
		}

		ComputeMgr.equation[group].InputARow(outPutBuf);	//���뷽�̵�һ��

		outPutBuf = null;//delete [] outPutBuf;	//�ͷŻ���
	}


	//4	��ֱ����·���ӵ�2�����, �γ�һ����·��		---------------------
	//	�û�·����һ������������ǵ���·,			---------------------
	//	�ɻ�·��Ϣ�ó����� .						---------------------
	for (group=ComputeMgr.groupCount-1; group>=0; --group)
	{
		nowMap = maps[group];
		size = nowMap.size;
		outPutBuf = new Array(nowMap.circuitCount+1);	//��������̵Ļ���
		/*ROAD **/var roads ;

		for (j=size-2; j>=0; --j) for (k=size-1; k>j; --k)
		{
			//��ʼ��
			i = CONVERT(j, k, size);
			if (nowMap.direct[i] <= 0) continue;
			
			roads = genrateArrayWithElementInitFunc(ROAD.CreateNew, size);
			ZeroArray(outPutBuf);	//��������

			//���·��,��������
			if (FindRoad(nowMap, roads, j, k))	//��·���õ����̵�һ��
			{
				/*ROADSTEP **/var prep = roads[k].first;
				/*ROADSTEP **/var nowp;

				if (prep == null) continue;	//����

				nowp = prep.next;
				PutIntoBuf(j, prep.crunIndex, nowMap, outPutBuf);

				while (nowp != null)
				{
					PutIntoBuf(prep.crunIndex, nowp.crunIndex, nowMap, outPutBuf);

					//��һ��
					prep = nowp;
					nowp = nowp.next;
				}

				PutIntoBuf(prep.crunIndex, k, nowMap, outPutBuf);

				PutIntoBuf(k, j, nowMap, outPutBuf);	//������j��k�ĵ�һ����·

				ComputeMgr.equation[group].InputARow(outPutBuf);	//���뷽��
			}
			else if (1 == nowMap.direct[i])	//û��·��,�õ��ߵ�����0
			{
				//���ߵ�����Ϊ0
				outPutBuf[nowMap.firstCircuit[i].indexInGroup] = 1;
				outPutBuf[nowMap.circuitCount] = 0;

				ComputeMgr.equation[group].InputARow(outPutBuf);	//���뷽��
			}

			roads = null;//delete [] roads;
		}

		outPutBuf = null;//delete [] outPutBuf;	//�ͷŻ���
	}//for (group


	//5�γɽ�㷽��------------------------------------------------------
	for (group=ComputeMgr.groupCount-1; group>=0; --group)
	{
		nowMap = maps[group];
		size = nowMap.size;
		outPutBuf = new Array(nowMap.circuitCount+1);	//��������̵Ļ���

		for (k=size-2; k>=0; --k)	//ֻ������k-1����㷽��
		{
			ZeroArray(outPutBuf);	//��������
			if (CreateCrunEquation(ComputeMgr.crun2[nowMap.crunOrderMap[k]], outPutBuf))	//��������
				ComputeMgr.equation[group].InputARow(outPutBuf);	//���뷽��
		}

		outPutBuf = null;//delete [] outPutBuf;	//�ͷŻ���
	}
};

ComputeMgr.TravelCircuitPutElec = function(/*Pointer */now,
									/*const CRUN * */last,
									/*int */dir,
									/*double */elec,
									/*ELEC_STATE */flag)
//��ָ���������,����·������ֵ��������
{
	/*Pointer*/var pre;

	do
	{
		pre = now;

		if (now.hasOwnProperty("resist"))	//�ؼ�
		{
			if (NORMALELEC == flag) //������·
			{
				now.elec	= elec;
				now.elecDir	= (1-dir);	//�����෴
			}
			else	//��������·
			{
				now.elecDir = flag;
			}

			//����һ������
			now = pre.lead[dir];
			dir = true1AndFalse0(now.conBody[0].p == pre);
		}
		else	//����,����һ������
		{
			if (NORMALELEC == flag) //������·
			{
				now.elec	= elec;
				now.elecDir	= (1-dir);	//�����෴
			}
			else	//��������·
			{
				now.elecDir = flag;
			}

			now = now.conBody[dir].p;
			if (IsOnCrun(now))	//�����յ�(last���)����
			{
				if (now == last) break;	//�����յ�
				else //����,�൱�ڵ���
				{
					//�ҵ�������ӵ���һ������
					for (dir=0; dir<4; ++dir)
						if (now.lead[dir]!=null && pre != now.lead[dir]) break;
					//ָ��ָ�������ӵ���һ������
					pre = now;
					now = pre.lead[dir];
					dir = true1AndFalse0(now.conBody[0].p == pre);
				}
			}
			else if (IsOnLead(now))
			{
				throw "2������ֱ�����ӳ��ֻ�������� !";
			}
			else //now.IsOnBody
			{
				dir = true1AndFalse0(now.lead[0] == pre);
			}
		}
	}//do
	while (!IsOnCrun(now) || now != last);	//�����յ�(last���)����
};

ComputeMgr.TravelCircuitFindOpenBody = function(/*Pointer */now, /*int */dir)
//��ָ���������,����·�������õ�����·
//�յ�����:��·�ؼ�,������������2�Ľ��
{
	/*const Pointer*/var first = now;
	/*Pointer*/var pre;

	do
	{
		pre = now;

		if (now.hasOwnProperty("resist"))	//�ؼ�
		{
			//�������
			now.elecDir = OPENELEC;
			now.elec = 0;

			//����һ������
			now = pre.lead[dir];
			if (now == null) break;	//��������
			dir = true1AndFalse0(now.conBody[0].p == pre);
		}
		else	//����,����һ������
		{
			//�������
			now.elecDir = OPENELEC;
			now.elec = 0;

			now = now.conBody[dir].p;
			if (IsOnCrun(now))	//�����յ�(last���)����
			{
				if (2 != now.GetConnectNum())	//�����յ�
				{
					break;
				}
				else //����,�൱�ڵ���
				{
					//�ҵ�������ӵ���һ������
					for (dir=0; dir<4; ++dir)
						if (now.lead[dir]!=null && pre != now.lead[dir]) break;
					//ָ��ָ�������ӵ���һ������
					pre = now;
					now = pre.lead[dir];
					dir = true1AndFalse0(now.conBody[0].p == pre);
				}
			}
			else if (IsOnLead(now))
			{
				throw "2������ֱ�����ӳ��ֻ��������";
			}
			else //now.IsOnBody()
			{
				dir = true1AndFalse0(now.lead[0] == pre);
			}
		}
	}//do
	while (now != first);	//�������յ�
};

/*ELEC_STATE*/ComputeMgr.TravelCircuitGetOrSetInfo = function(/*Pointer */now, /*int */dir, /*double &*/elec, /*ELEC_STATE */flag)
//��ָ���������,��õ�ѹ�͵�����Ϣ,�������յ�
//ָ�����岻������·�а����Ľ��,������������ѭ��
{
	/*double*/var press = 0;
	/*double*/var resist = 0;
	/*const Pointer*/var self = now;	//��¼�����
	/*Pointer*/var pre;
	if (IsOnCrun(now)) return ERRORELEC;	//ָ�����岻������·�а����Ľ��

	do
	{
		pre = now;

		if (now.hasOwnProperty("resist"))	//�ؼ�
		{
			if (UNKNOWNELEC == flag)	//��õ�ѹ����
			{
				resist	+= now.resist;
				press	+= now.GetPressure(1-dir);	//�����෴
			}
			else if (NORMALELEC == flag)	//��������������Ϣ
			{
				now.elecDir	= (1-dir);	//�����෴
				now.elec	= elec;
			}
			else	//���벻����������Ϣ
			{
				now.elecDir = flag;
			}

			//����һ������
			now = pre.lead[dir]);
			if (now == null) return ERRORELEC;	//��������,��������Ǵ���
			dir = true1AndFalse0(now.conBody[0].p == pre);
		}
		else	//����,����һ������
		{
			if (NORMALELEC == flag)	//��������������Ϣ
			{
				now.elecDir	= (1-dir);	//�����෴
				now.elec	= elec;
			}
			else if (UNKNOWNELEC != flag)	//���벻����������Ϣ
			{
				now.elecDir = flag;
			}

			now = now.conBody[dir].p;
			if (IsOnCrun(now))	//��ʱ���һ������2������,����,�൱�ڵ���
			{
				//�ҵ�������ӵ���һ������
				for (dir=0; dir<4; ++dir)
					if (now.lead[dir]!=null && pre != now.lead[dir]) break;
				//ָ��ָ�������ӵ���һ������
				pre = now;
				now = pre.lead[dir];
				dir = true1AndFalse0(now.conBody[0].p == pre);
			}
			else if (IsOnLead(now))
			{
				throw "2������ֱ�����ӳ��ֻ��������";
			}
			else	//now.IsOnBody
			{
				dir = true1AndFalse0(now.lead[0] == pre);
			}
		}
	}//do
	while (now!=self);	//�������յ�

	if (UNKNOWNELEC == flag)	//��õ�ѹ����
	{
		if (!IsFloatZero(resist))	//����--���費��0
		{
			elec = press/resist;
			return NORMALELEC;
		}
		else if (IsFloatZero(press))	//����--�����ѹ����0
		{
			elec = 0;
			return NORMALELEC;
		}
		else	//��·
		{
			elec = 0;
			return SHORTELEC;
		}
	}

	return NORMALELEC;
};

ComputeMgr.DistributeAnswer = function()
//������ĵ�������ֲ���ÿ������,�ؼ�
{
	/*int*/var i;			//ѭ������
	/*int*/var dir;		//��һ�������ڵ�ǰ����ķ���
	/*Pointer*/var now;	//��ǰ���ʵ���·�ؼ�
	/*CRUN **/var end;		//��·���յ�
	/*double*/var elec;

	//1,��ʼ��ÿ�����ߺ͵�ѧԪ����elecDir,�������ʹ��
	for (i=leadNum-1; i>=0; --i) lead[i].elecDir = UNKNOWNELEC;
	for (i=ctrlNum-1; i>=0; --i) ctrl[i].elecDir = UNKNOWNELEC;

	//2,��WmMgr.circu�Ľ���ֲ���ÿ����·�е�����
	for (i=ComputeMgr.circuitCount-1; i>=0; --i)
	{
		//1,�ҵ���·�����,end����ʱ����
		end = crun[indexOfArray(ComputeMgr.crun2, ComputeMgr.circu[i].from)];
		now = end.lead[ComputeMgr.circu[i].dirFrom];

		//2,ȷ�����ҷ���,end����ʱ����
		dir = true1AndFalse0(now.conBody[0].p == end);

		//3,�ҵ���·���յ�,end����յ���ָ��
		end = crun[indexOfArray(ComputeMgr.crun2, ComputeMgr.circu[i].to)];

		//4,������·
		TravelCircuitPutElec(now, end, dir, ComputeMgr.circu[i].elec, ComputeMgr.circu[i].elecDir);
	}

	//���WmMgr.circu,ComputeMgr.crun2���ڴ�
	ComputeMgr.circu = null;
	ComputeMgr.circuitCount = 0;
	ComputeMgr.crun2 = null;

	//3,�ҵ������ؼ�,��������Ϣ����Ϊ��·
	for (i=ctrlNum-1; i>=0; --i) if (!ctrl[i].lead[0] && !ctrl[i].lead[1])
	{
		ctrl[i].elecDir = OPENELEC;
		ctrl[i].elec = 0;
	}

	//4,�ҵ�һ�������ӵĿؼ�,�����ӵ����ж�·���������Ϣ���ú�
	//˼·:(ÿһ������������·��Ϣд��)
	//		1,�ҵ���·����
	//		2,�������յ�(�յ�����:��·�ؼ�,������������2�Ľ��)
	for (i=ctrlNum-1; i>=0; --i) if (1 == ctrl[i].GetConnectNum())
	{
		//1,�ҵ���·�����
		now = ctrl[i];

		//2,ȷ�����ҷ���
		dir = true1AndFalse0(now.lead[1] != null);

		//3,������·
		TravelCircuitFindOpenBody(now, dir);
	}
	for (i=ComputeMgr.crunCount-1; i>=0; --i) if (1 == crun[i].GetConnectNum())
	{
		//1,�ҵ���·�����
		for (dir=0;dir<4;dir++) if (crun[i].lead[dir]) break;
		now = crun[i].lead[dir];

		//2,ȷ�����ҷ���
		dir = true1AndFalse0(now.conBody[0].p == crun[i]);

		//3,������·
		TravelCircuitFindOpenBody(now, dir);
	}

	//5,����UNKNOWNELEC == elecDir,���ҿؼ������߶�������,����resist<0
	//	�������������ԭ��:��·���ж�·Ԫ��(���������),������·�������
	//	������·��·,�Ѷ�·��Ϣд������
	for (i=ctrlNum-1; i>=0; --i)
	{
		if (UNKNOWNELEC != ctrl[i].elecDir || ctrl[i].resist >= 0) continue;

		//1,������·�����
		now = ctrl[i];

		//2,��2���������
		TravelCircuitFindOpenBody(now, 0);
		TravelCircuitFindOpenBody(now, 1);
	}

	//6,���� UNKNOWNELEC == elecDir������,��������ͷ���
	//�������������ԭ��:��·��û�нڵ�,���ɵ��ߺ͵�ѧԪ�����ӵĻ�·
	//˼· :(ÿһ����Ҫ��¼���߹��ĵ���͵�ѹ,��Ϊ��Ҫ����)
	// 1 ��������߿�ʼ��,һֱ�ҵ��Լ�ֹͣ
	// 2 ���������С,���±���һ��,����Ϣ���뵽������
	for (i=ctrlNum-1; i>=0; --i)
	{
		if (UNKNOWNELEC != ctrl[i].elecDir) continue;

		//1,������·�����
		now = ctrl[i];

		//2,����߱���,��õ���͵�ѹ
		dir = TravelCircuitGetOrSetInfo(now, 0, elec, UNKNOWNELEC);

		//3,�ѽ����������
		if (ERRORELEC == dir)
		{
			throw "����������ִ���!!!";
		}
		else
		{
			if (NORMALELEC == dir && elec < 0)
			{
				//������Ϊ����,��ת��������
				elec = -elec;
				TravelCircuitGetOrSetInfo(now, 1, elec, dir);
			}
			else 
			{
				TravelCircuitGetOrSetInfo(now, 0, elec, dir);
			}
		}
	}

	//7,������·��ֻ����: ����(����) �� ����(2�����ӵ��ߵĽ��), ����������Ϊ0
	//	�������������ԭ��:��·û�г���3�����ߵĽڵ�,������뵽��·�б���
	//	��·��û�ж�·,ǰ��Ҳ�����鵽;������ֻ���ؼ�Ҳ�����鵽
	for (i=leadNum-1; i>=0; --i) if (UNKNOWNELEC == lead[i].elecDir)
	{
		lead[i].elecDir = LEFTELEC;
		lead[i].elec = 0;
	}
};

ComputeMgr.ComputeElec = function()
//���γɵ�nԪ1�η��̼��������·����ֵ
{
	/*int*/var group;
	/*int*/var i;
	/*ELEC_STATE*/var flag;
	/*const double **/var ans;

	ClearCircuitState();	//�����·״̬��Ϣ
	CollectCircuitInfo();	//����һ�ε�·,���ÿ��Ⱥ�����·��ѧ��Ϣ
	CreateEquation();		//������·��Ϣ,��Ⱥ�齨������

	for (group=0; group<ComputeMgr.groupCount; ++group)	//��Ⱥ�����
	{
		flag = ComputeMgr.equation[group].Count();	//���㷽��

		if (NORMALELEC == flag)	//��·����
		{
			ans = ComputeMgr.equation[group].GetAnswer();	//��ý������ָ��
			for (i=ComputeMgr.circuitCount-1; i>=0; --i) if (group == ComputeMgr.circu[i].from.group)
			{
				ComputeMgr.circu[i].elecDir = NORMALELEC;
				ComputeMgr.circu[i].elec = ans[ComputeMgr.circu[i].indexInGroup];
				ComputeMgr.circu[i].ConvertWhenElecLessZero();	//����������ʱ��Ϊ����,����ת��������
			}
		}
		else	//��·���޷�ȷ������
		{
			for (i=ComputeMgr.circuitCount-1; i>=0; --i) if (group == ComputeMgr.circu[i].from.group)
			{
				ComputeMgr.circu[i].elecDir = flag;
			}
		}

		ComputeMgr.maps[group] = null;//ComputeMgr.maps[group].Uninit();	//ɾ��һ����·ͼ
		ComputeMgr.equation[group] = null;//delete ComputeMgr.equation[group];	//ɾ��һ������
	}

	ComputeMgr.maps = null;//delete [] ComputeMgr.maps;		//ɾ����·ͼ����
	ComputeMgr.equation = null;//delete [] ComputeMgr.equation;	//ɾ����������ָ��
	DistributeAnswer();	//������ֲ���ÿ������,�������ͷ���WmMgr.circu��WmMgr.crun2
};
