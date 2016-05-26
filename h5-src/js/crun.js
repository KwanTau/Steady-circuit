//�����
var CRUN = {
	//�ڵ�ȫ�ֳ�ʼ������
	globalInitOrder: 1,
	//����ȫ�ֳ�ʼ������
	ResetGlobalInitNum: function() {
		return (globalInitOrder = 1);
	}

	CreateNew: function(memIdx, x, y) {
		var initOrder = globalInitOrder++;
		var newObj = {
			initOrder : initOrder,		//��ʼ�����
			index : memIdx,				//�ڽڵ����������
			isPaintName : false,		//Ĭ�ϲ���ʾ����ǩ
			name : "Crun" + initOrder,	//Ĭ������
			x:x, y:y,					//����
			lead:[-1,-1,-1,-1]			//������ӵ��ߵ�λ��,0��,1��,2��,3��*/
		};
        return newObj;
	}

	//��������Ϣ���ļ�
	SaveToFile: function(fp) {
		/*int*/var j, t;
		ASSERT(fp != NULL);

		fwrite(&coord, sizeof(POINT), 1, fp);
		fwrite(&isPaintName, sizeof(bool), 1, fp);
		fwrite(name, 1, NAME_LEN, fp);

		for(j=0; j<4; ++ j)
		{
			if(lead[j])
				t = lead[j]->index;
			else
				t = -1;
			fwrite(&t, sizeof(int), 1, fp);
		}
	}

	void CRUN::ReadFromFile(FILE * fp, LEAD ** allLead)
	//���ļ���ȡ�����Ϣ
	{
		int j, t;
		ASSERT(fp != NULL);

		fread(&coord, sizeof(POINT), 1, fp);
		fread(&isPaintName, sizeof(bool), 1, fp);
		fread(name, 1, NAME_LEN, fp);

		for(j = 0; j < 4; ++ j)
		{
			fread(&t, sizeof(t), 1, fp);
			if(t >= 0 && t < MAXLEADNUM)
				lead[j] = allLead[t];
			else 
				lead[j] = NULL;
		}
	}

	int CRUN::At(POINT p)const
	//�������ڽ���λ��
	{
		int dis, disBetweenCenter;

		disBetweenCenter = (p.x-coord.x)*(p.x-coord.x)+(p.y-coord.y)*(p.y-coord.y);
		if(disBetweenCenter > 4 * DD * DD) return 0;	//�����Զ,���������Աߵ����ӵ�

		dis = (p.x-coord.x)*(p.x-coord.x)+(p.y-coord.y+DD)*(p.y-coord.y+DD);
		if(dis <= DD)	//�������ӵ�
		{
			if(lead[0] != NULL) return -1;
			else return 1;
		}

		dis = (p.x-coord.x)*(p.x-coord.x)+(p.y-coord.y-DD)*(p.y-coord.y-DD);
		if(dis <= DD)	//�������ӵ�
		{
			if(lead[1] != NULL) return -1;
			else return 2;
		}

		dis = (p.x-coord.x+DD)*(p.x-coord.x+DD)+(p.y-coord.y)*(p.y-coord.y);
		if(dis <= DD)	//�������ӵ�
		{
			if(lead[2] != NULL) return -1;
			else return 3;
		}

		dis = (p.x-coord.x-DD)*(p.x-coord.x-DD)+(p.y-coord.y)*(p.y-coord.y);
		if(dis <= DD)	//�������ӵ�
		{
			if(lead[3] != NULL) return -1;
			else return 4;
		}

		if(disBetweenCenter <= DD * DD) return -1;//�ڵ���

		return 0;
	}

	CRUN * CRUN::Clone(CLONE_PURPOSE cp)const
	//�����ؼ������Ϣ���µĽ��
	{
		CRUN * newCrun = new CRUN(index, coord);
		strcpy(newCrun->name, name);

		if(CLONE_FOR_USE != cp)
		{
			newCrun->initOrder = this->initOrder;
			--globalInitOrder;
		}
		return newCrun;
	}

	void CRUN::GetDataList(LISTDATA * list)const
	//��CProperty����
	{
		list->Init(2);
		list->SetAMember(DATA_STYLE_LPCTSTR, TITLE_NOTE, (void *)name);
		list->SetAMember(DATA_STYLE_bool, TITLESHOW_NOTE, (void *)(&isPaintName));
	}

	int CRUN::GetDirect(const LEAD * l)const
	//Ѱ�ҵ������ĸ�����
	{
		for(int i=0; i<4; ++i) if(lead[i] == l) return i;
		return -1;	//û���ҵ�
	}

	int CRUN::GetConnectNum()const
	//��������˼�������
	{
		return  (lead[0] != NULL) + 
				(lead[1] != NULL) + 
				(lead[2] != NULL) + 
				(lead[3] != NULL);
	}
};