
//������
var LEAD = {
	
	//ȫ�ֳ�ʼ������
	globalInitOrder: 1,
	//����ȫ�ֳ�ʼ������
	ResetGlobalInitNum: function() {
		return (LEAD.globalInitOrder = 1);
	},


	CreateNew: function(memberIdx, color, p1, p2, isInit) {
		var initOrder = CRUN.globalInitOrder++;
		
		var newObj = {
			initOrder : initOrder,		//��ʼ�����
			index : memberIdx,			//�����������
			
			color : color,
			elecDir : UNKNOWNELEC,	//��������
			coord : new Array(),
			conBody : new Array(null, null)
		};
		
		if (isInit) {
			newObj.conBody[0] = p1;	//��������
			newObj.conBody[1] = p2;	//��������
			RefreshPos();		//������������
		}
		
		return newObj;
	},

	Clone: function(clonePurpose) {
		var newLead = LEAD.CreateNew(index, color, conBody[0], conBody[1], false);

		//��������
		newLead.coord = deepCopy(this.coord);

		if (CLONE_FOR_USE != clonePurpose) {
			newLead.initOrder = this.initOrder;
			--LEAD.globalInitOrder;
		}
		return newLead;
	},
	
	//������Ϣ��json
	GenerateStoreJsonObj: function() {
		return {
			color : color,
			coord : deepCopy(this.coord),
			conBody  : new Array(conBody[0].GenerateStoreJsonObj(), conBody[1].GenerateStoreJsonObj())
		};
	},
	//��json��ȡ��Ϣ
	ReadFromStoreJsonObj: function(jsonObj, leadList, crunList, ctrlList) {
		ASSERT(jsonObj!=null && leadList!=null && crunList!=null && ctrlList!=null);

		this.color = jsonObj.color;
		this.coord = deepCopy(jsonObj.coord);
		this.conBody[0] = Pointer.ReadFromStoreJsonObj(jsonObj, leadList, crunList, ctrlList);
		this.conBody[1] = Pointer.ReadFromStoreJsonObj(jsonObj, leadList, crunList, ctrlList);
	},

	//��õ�������������������λ��
	//�ֽ����һλ:
	//			0 ������� �� �յ����� ����
	//			1 ������� �� �յ����� ����
	//�ֽڵ����ڶ�λ:
	//			0 ������� �� �յ����� ����
	//			1 ������� �� �յ����� ����
	GetBodyPos: function() {
		var a = conBody[0].p;
		var b = conBody[1].p;

		return ((a.x > b.x) << 1) + (a.y > b.y);
	},

	//��CProperty������Ϣ
	GetDataList: function(list, name) {
		list.Init(this, 1);
		list.SetAEnumMember(name, "color");
	},

	//�ֲڵĳ�ʼ����������
	EasyInitPos: function(from, to) {
		//�ͷ�����ռ�ÿռ�
		this.coord.length = 0;

		//��ʼ����
		coord.push({x:from.x, y:from.y});

		//�м������
		if (from.x != to.x && from.y != to.y) {
			coord.push({x:from.x, y:to.y});
		}

		//�յ�����
		coord.push({x:to.x, y:to.y});
	},

	//��������һ��Ϊ��
	Divide: function(int atState, pos, leadCoord) {
		ASSERT(atState <= -2);
		atState = (-atState - 2) >> 1;

		if (this.coord.length < atState+2)
			return false;
		
		// ������ӵ�����
		var nowPos = this.coord[atState];
		var nowNextPos = this.coord[atState+1];
		var dividePos = {x:pos.x, y:pos.y};
		if (nowPos.x == nowNextPos.x)	//��������
			dividePos.x = nowPos.x;
		else	//�ں�����
			dividePos.y = nowPos.y;
		
		// ǰ���
		leadCoord.first = new Array();
		for (var i=0; i<=atState; ++i) {
			leadCoord.first.push({x:this.coord[i].x, y:this.coord[i].y});
		}
		leadCoord.first.push({x:dividePos.x, y:dividePos.y});
		
		// ����
		leadCoord.second = new Array();
		leadCoord.second.push({x:dividePos.x, y:dividePos.y});
		for (var i=atState+1; i<this.coord.length; ++i) {
			leadCoord.second.push({x:this.coord[i].x, y:this.coord[i].y});
		}

		//���ٵ�������
		this.coord.length = 0;

		return true;
	},

	//�滻ԭ��������
	ReplacePos: function(newPosArray) {
		this.coord.length = 0;
		this.coord = newPosArray;
	},

	//�������ڵ��ߵ�λ�� : -3,-5,-7,...���߲���; -2,-4,-6,...���߲���; 0����.
	At: function(xPos, yPos) {
		var min, max;
		var i = -2;

		var nowIndex = 1;
		var pre = this.coord[nowIndex-1];
		var now = this.coord[nowIndex];
		while (nowIndex < this.coord.length) {
			if (pre.x == now.x) {	//��������
				if (now.y > pre.y) {
					min = pre.y; 
					max = now.y;
				} else {
					min = now.y; 
					max = pre.y;
				}
				if (yPos > min 
					&& yPos < max 
					&& xPos > now.x - DD 
					&& xPos < now.x + DD)
					return i;
			} else {	//�ں�����
				if (now.x > pre.x) {
					min = pre.x; 
					max = now.x;
				} else {
					min = now.x; 
					max = pre.x;
				}
				if (xPos > min 
					&& xPos < max 
					&& yPos > now.y - DD
					&& yPos < now.y + DD)
					return i-1;
			}

			pre = now;
			++nowIndex;
			now = this.coord[nowIndex];
			
			i -= 2;
		}

		return 0;
	},

	//ɾ������ͬ����ĵ��߽��
	CleanLead: function() {
		if (this.coord.length <= 2) return;	//ֻ��2���ڵ�ĵ��߲�����
		
		var p1 = this.coord[0];
		var p2 = this.coord[1];
		var p3 = this.coord[2];
		var currentIndex = 2;

		if (p1.x == p2.x && p1.y == p2.y) {	// leave out p2
			var newCoord = new Array();
			for (var i=0; i<currentIndex-1; ++i)
				newCoord.push(this.coord[i]);
			for (var i=currentIndex; i<this.coord.length; ++i)
				newCoord.push(this.coord[i]);
			this.coord = newCoord;
			return;
		}
		while (true) {
			if (p2.x == p3.x && p2.y == p3.y) {
				if (currentIndex+1 < this.coord.length) {	// leave out p2,p3
					var newCoord = new Array();
					for (var i=0; i<currentIndex-1; ++i)
						newCoord.push(this.coord[i]);
					for (var i=currentIndex+1; i<this.coord.length; ++i)
						newCoord.push(this.coord[i]);
					this.coord = newCoord;
					return;
				} else {	// leave out p3
					--this.coord.length;
					return;
				}
			}

			++currentIndex;
			if (currentIndex >= this.coord.length) break;
			p1 = p2;
			p2 = p3;
			p3 = this.coord[currentIndex];
		}
	},

	// static����, ����Ҫthis����
	//��2��ƽ�е���֮��������ҵ����ʵ���һ��ƽ�е��ߵ�λ��
	GetPosFit: function(pos1, pos2, dis, isEnd) {
		int dis2 = -2;
		if (isEnd) dis2 = 2;

		if (pos2 - pos1 > dis || pos1 - pos2 > dis) {
			return (pos2 + pos1)/2 + dis2;
		} else if (pos1 < 300) {
			if (pos2 >= pos1)
				return pos2 + dis + dis2;
			else
				return pos1 + dis + dis2;
		} else {
			if (pos2 <= pos1)
				return pos2 - dis - dis2;
			else
				return pos1 - dis - dis2;
		}
	},

	//ʹ���߲��ڵ����ӵĵ�1������
	FitStart: function(dis) {
		ASSERT(this.coord.length >= 2);
		
		//��ʼ������ -------------------------------------------------------
		vardir = conBody[0].GetConnectPosDir();
		var dirSum = dir + conBody[1].GetConnectPosDir();
		
		var oppositeFlag = (dirSum == 3 || dirSum == 7);
		var dis2 = 15;
		if (dir & 1) dis2 = -15;
		
		var next = coord[1];
		var next2 = null;
		if (this.coord.length >= 3)
			next2 = coord[2];
		
		var temp, now, theLast;

		//�ж�ִ������ -----------------------------------------------------
		switch (dir) {
		case 1:	//�����ӵ�
			if (this.coord[0].x != next.x || this.coord[0].y >= next.y)
				return;
			break;

		case 2:	//�����ӵ�
			if (this.coord[0].x != next.x || this.coord[0].y <= next.y)
				return;
			break;

		case 3:	//�����ӵ�
			if (this.coord[0].y != next.y || this.coord[0].x >= next.x)
				return;
			break;

		case 4:	//�����ӵ�
			if (this.coord[0].y != next.y || this.coord[0].x <= next.x)
				return;
			break;
		}

		//����ֻ��2���ڵ� ---------------------------------------------------
		if (this.coord.length == 2) {
			switch (dir) {
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				this.coord.length = 1;
				
				theLast = this.coord[0];
				this.coord.push({x:theLast.x, y:theLast.y + dis2});
				theLast = this.coord[1];
				this.coord.push({x:theLast.x - dis, y:theLast.y});
				theLast = this.coord[2];
				this.coord.push({x:theLast.x, y:theLast.y - dis2*2 + next.y - this.coord[0].y});
				theLast = this.coord[3];
				this.coord.push({x:theLast.x + dis, y:theLast.y});
				this.coord.push(next);
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				this.coord.length = 1;
				
				theLast = this.coord[0];
				this.coord.push({y:theLast.y, x:theLast.x + dis2});
				theLast = this.coord[1];
				this.coord.push({y:theLast.y - dis, x:theLast.x});
				theLast = this.coord[2];
				this.coord.push({y:theLast.y, x:theLast.x - dis2*2 + next.x - this.coord[0].x});
				theLast = this.coord[3];
				this.coord.push({y:theLast.y + dis, x:theLast.x});
				this.coord.push(next);
				return;
			}
		}
		
		//����ֻ��3���ڵ� ---------------------------------------------------
		else if (this.coord.length == 3) {
			if (oppositeFlag) {
				switch (dir) {
				case 1:	//�����ӵ�
				case 2:	//�����ӵ�
					this.coord.length = 1;
					
					theLast = this.coord[0];
					this.coord.push({x:theLast.x, y:theLast.y + dis2});
					theLast = this.coord[1];
					this.coord.push({x:GetPosFit(this.coord[0].x, next2.x, dis, false), y:theLast.y});
					theLast = this.coord[2];
					this.coord.push({x:theLast.x, y:next2.y - dis2});
					theLast = this.coord[3];
					this.coord.push({x:next2.x, y:theLast.y});
					this.coord.push(next2);
					return;

				case 3:	//�����ӵ�
				case 4:	//�����ӵ�
					this.coord.length = 1;
					
					theLast = this.coord[0];
					this.coord.push({y:theLast.y, x:theLast.x + dis2});
					theLast = this.coord[1];
					this.coord.push({y:GetPosFit(this.coord[0].y, next2.y, dis, false), x:theLast.x});
					theLast = this.coord[2];
					this.coord.push({y:theLast.y, x:next2.x - dis2});
					theLast = this.coord[3];
					this.coord.push({y:next2.y, x:theLast.x});
					this.coord.push(next2);
					return;
				}
			} else {
				switch (dir) {
				case 1:	//�����ӵ�
				case 2:	//�����ӵ�
					this.coord.length = 1;
					
					theLast = this.coord[0];
					this.coord.push({x:theLast.x, y:theLast.y + dis2});
					theLast = this.coord[1];
					this.coord.push({x:next2.x, y:theLast.y});
					this.coord.push(next2);
					return;

				case 3:	//�����ӵ�
				case 4:	//�����ӵ�
					this.coord.length = 1;
					
					theLast = this.coord[0];
					this.coord.push({y:theLast.y, x:theLast.x + dis2});
					theLast = this.coord[1];
					this.coord.push({y:next2.y, x:theLast.x});
					this.coord.push(next2);
					return;
				}
			}
		}
		
		//����ֻ��4���ڵ������ӵ���� ---------------------------------------
		else if (oppositeFlag && this.coord.length == 4) {
			var next3 = coord[3];
			switch (dir) {
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				this.coord.length = 1;
				
				theLast = this.coord[0];
				this.coord.push({x:GetPosFit(theLast.x, next2.x, dis, false), y:theLast.y});
				
				theLast = this.coord[1];
				this.coord.push({x:theLast.x, y:next.y});
				this.coord.push(next2);
				this.coord.push(next3);
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				this.coord.length = 1;
				
				theLast = this.coord[0];
				this.coord.push({y:GetPosFit(theLast.y, next2.y, dis, false), x:theLast.x});
				
				theLast = this.coord[1];
				this.coord.push({y:theLast.y, x:next.x});
				this.coord.push(next2);
				this.coord.push(next3);
				return;
			}
		}
		
		// (����������5���ڵ�) or (����������4���ڵ� and ���ӵ㲻���) ------
		else {
			switch (dir) {
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				next2.y = next.y = this.coord[0].y + dis2;
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				next2.x = next.x = this.coord[0].x + dis2;
				return;
			}
		}
	},

	//ʹ���߲��ڵ����ӵĵ�2������
	FitEnd: function(dis) {
		ASSERT(coord.next != null);
		
		//��ʼ������ -------------------------------------------------------
		LEADSTEP * temp, * now, * pre1, * pre2, * next, * next2;
		const int dir = conBody[1].GetConnectPosDir();
		const int dirOther = conBody[0].GetConnectPosDir();
		int dis2 = 15;
		if (dir & 1) dis2 = -15;

		pre2 = null;
		pre1 = &coord;
		now  = coord.next;
		while (now.next != null)
		{
			pre2 = pre1;
			pre1 = now;
			now = now.next;
		}
		next = coord.next;
		next2 = next.next;

		//�ж�ִ������ -----------------------------------------------------
		switch(dir)
		{
		case 1:	//�����ӵ�
			if (now.x != pre1.x || now.y >= pre1.y)
				return;
			break;

		case 2:	//�����ӵ�
			if (now.x != pre1.x || now.y <= pre1.y)
				return;
			break;

		case 3:	//�����ӵ�
			if (now.y != pre1.y || now.x >= pre1.x)
				return;
			break;

		case 4:	//�����ӵ�
			if (now.y != pre1.y || now.x <= pre1.x)
				return;
			break;
		}

		//����ֻ��2���ڵ� ---------------------------------------------------
		if (next2 == null)
		{
			switch(dir)
			{
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				coord.next = temp = {};
				temp.y = this.coord[0].y;
				if (dirOther == 4)
					temp.x = this.coord[0].x - dis;
				else
					temp.x = this.coord[0].x + dis;
				now = temp;

				now.next = temp = {};
				temp.x = now.x;
				temp.y = next.y + dis2;
				now = temp;

				now.next = temp = {};
				temp.y = now.y;
				temp.x = next.x;

				temp.next = next;
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				coord.next = temp = {};
				temp.x = this.coord[0].x;
				if (dirOther == 2)
					temp.y = this.coord[0].y - dis;
				else
					temp.y = this.coord[0].y + dis;
				now = temp;

				now.next = temp = {};
				temp.y = now.y;
				temp.x = next.x + dis2;
				now = temp;

				now.next = temp = {};
				temp.x = now.x;
				temp.y = next.y;

				temp.next = next;
				return;
			}
		}

		//����ֻ��3���ڵ� ---------------------------------------------------
		else if (next2.next == null)
		{
			switch(dir)
			{
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				//next.x
				if (dirOther == 4)
					next.x = next2.x + dis;
				else if (dirOther == 3)
					next.x = next2.x - dis;
				else
					next.x = GetPosFit(this.coord[0].x, next2.x, dis, true);

				//add point
				next.next = temp = {};
				temp.x = next.x;
				temp.y = next2.y;
				temp.next = next2;
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				//next.y
				if (dirOther == 2)
					next.y = next2.y + dis;
				else if (dirOther == 1)
					next.y = next2.y - dis;
				else
					next.y = GetPosFit(this.coord[0].y, next2.y, dis, true);

				//add point
				next.next = temp = {};
				temp.y = next.y;
				temp.x = next2.x;
				temp.next = next2;
				return;
			}
		}
		
		//����ֻ��4���ڵ� ----------------------------------------------
		else if (next2.next.next == null)
		{
			switch(dir)
			{
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				if (dirOther == 3 || dirOther == 4)
				{
					pre1.y = pre2.y = now.y + dis2;
				}
				else //dir != dirOther
				{
					next2.next = temp = {};
					temp.next = now;

					temp.y = now.y;
					next2.x = temp.x = GetPosFit(this.coord[0].x, next2.x, dis, true);
				}
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				if (dirOther == 1 || dirOther == 2)
				{
					pre1.x = pre2.x = now.x + dis2;
				}
				else //dir != dirOther
				{
					next2.next = temp = {};
					temp.next = now;

					temp.x = now.x;
					next2.y = temp.y = GetPosFit(this.coord[0].y, next2.y, dis, true);
				}
				return;
			}
		}

		//����������5���ڵ� --------------------------------------------
		else
		{
			switch(dir)
			{
			case 1:	//�����ӵ�
			case 2:	//�����ӵ�
				pre1.y = pre2.y = now.y + dis2;
				return;

			case 3:	//�����ӵ�
			case 4:	//�����ӵ�
				pre1.x = pre2.x = now.x + dis2;
				return;
			}
		}
	},

	//���µĵ���λ�ø������ӵ�����ʱ,��������
	MakeFit: function() {
		ASSERT(coord.next != null);

		if (conBody[0].IsOnCrun()) {
			FitStart(DD*3);
		} else { //if (conBody[0].IsOnCtrl())
			FitStart(BODYSIZE.cx);
		}

		if (conBody[1].IsOnCrun()) {
			FitEnd(DD*3);
		} else { //if (conBody[1].IsOnCtrl())
			FitEnd(BODYSIZE.cx);
		}
	},

	//�ƶ�����
	Move: function(int dir, POINT pos, const int dis) {
		LEADSTEP * pre2 = null;
		LEADSTEP * pre  = &coord;
		LEADSTEP * now  = pre.next;
		LEADSTEP * next = null;
		int i = -2;
		int inter = 0;

		//1,�ҵ�ָ��----------------------------------
		while (now != null)
		{
			if (pre.x == now.x)	//��������
			{
				if (i == dir) break;
			}
			else	//�ں�����
			{
				if (i-1 == dir) break;
			}
			pre2 = pre;
			pre = now;
			now = now.next;
			i -= 2;
		}
		if (now == null) return false;	//û���ҵ�
		else next = now.next;

		//2����������������--------------------------
		if (pre.x == now.x)	//��������
		{
			if (pos.x == pre.x) return false;	//����ı�

			//2.1����pre��ͷ.........................
			if (pre2 == null)	//pre��ͷ
			{
				if (next != null)	//now���ǽ�β
				{
					inter = pos.x - next.x;
					if (inter < 0)inter = -inter;
				}

				if (next != null && inter <= dis)	//now���ǽ�β
				{
					if (next.next != null)	//next���ǽ�β
					{
						delete now;
						pre.next = next;
						next.y = pre.y;
					}
					else	//next�ǽ�β
					{
						now.x = next.x;
						now.y = pre.y;
					}
				}
				else if (next != null)	//now���ǽ�β
				{
					pre2  = {};
					pre2.x = pos.x;
					pre2.y = pre.y;
					pre2.next = now;

					pre.next = pre2;

					now.x = pos.x;
				}
				else	//now�ǽ�β
				{
					pre2 = {};
					pre2.x = pos.x;
					pre2.y = pre.y;

					next = {};
					next.x = pos.x;
					next.y = now.y;

					pre.next = pre2;
					pre2.next = next;
					next.next = now;
				}

				goto end;
			}//����pre����ͷ

			//2.2����now�ǽ�β.........................
			if (next == null)	//now�ǽ�β
			{
				inter = pos.x - pre2.x;
				if (inter < 0)inter = -inter;

				if (inter <= dis)
				{
					if (pre2 != &coord)	//pre2����ͷ
					{
						delete pre;
						pre2.next = now;
						pre2.y = now.y;
					}
					else	//pre2��ͷ
					{
						pre.x = pre2.x;
						pre.y = now.y;
					}
				}
				else
				{
					next  = {};
					next.x = pos.x;
					next.y = now.y;

					pre.x = pos.x;

					pre.next = next;
					next.next = now;
				}

				goto end;
			}//����now���ǽ�β

			//2.3������ǰ��ϲ�..........................
			inter = pos.x - pre2.x;
			if (inter < 0)inter = -inter;

			if (inter <= dis)	//���ߺϲ�
			{
				if (pre2 != &coord)	//pre2����ͷ
				{
					delete pre;
					delete now;
					pre2.next = next;
					pre2.y = next.y;
				}
				else	//pre2��ͷ
				{
					delete pre;
					pre2.next = now;
					now.x = pre2.x;

					if (now.x == next.x && now.y == next.y)
					{
						delete now;
						pre2.next = next;
					}
				}
				
				goto end;
			}

			//2.4���������ϲ�..........................
			inter = pos.x - next.x;
			if (inter < 0)inter = -inter;

			if (inter <= dis)	//���ߺϲ�
			{
				if (next.next != null)	//next���ǽ�β
				{
					delete pre;
					delete now;
					pre2.next = next;
					next.y = pre2.y;
				}
				else	//next�ǽ�β
				{
					delete now;
					pre.next = next;
					pre.x = next.x;
				}
				goto end;
			}

			//2.5�����������..........................
			now.x = pos.x;
			pre.x = pos.x;
			goto end;

		}//����������������

		//3�������ú�������--------------------------
		if (pre.y == now.y)	//�ں�����
		{
			if (pos.y == pre.y) return false;	//����ı�

			//3.1����pre��ͷ.........................
			if (pre2 == null)	//pre��ͷ
			{
				if (next != null)	//now���ǽ�β
				{
					inter = pos.y - next.y;
					if (inter < 0)inter = -inter;
				}

				if (next != null && inter <= dis)	//now���ǽ�β
				{
					if (next.next != null)	//next���ǽ�β
					{
						delete now;
						pre.next = next;
						next.x = pre.x;
					}
					else	//next�ǽ�β
					{
						now.y = next.y;
						now.x = pre.x;
					}
				}
				else if (next != null)	//now���ǽ�β
				{
					pre2  = {};
					pre2.y = pos.y;
					pre2.x = pre.x;
					pre2.next = now;

					pre.next = pre2;

					now.y = pos.y;
				}
				else	//now�ǽ�β
				{
					pre2 = {};
					pre2.y = pos.y;
					pre2.x = pre.x;

					next = {};
					next.y = pos.y;
					next.x = now.x;

					pre.next = pre2;
					pre2.next = next;
					next.next = now;
				}

				goto end;
			}//����pre����ͷ

			//3.2����now�ǽ�β.........................
			if (next == null)	//now�ǽ�β
			{
				inter = pos.y - pre2.y;
				if (inter < 0)inter = -inter;

				if (inter <= dis)
				{
					if (pre2 != &coord)	//pre2����ͷ
					{
						delete pre;
						pre2.next = now;
						pre2.x = now.x;
					}
					else	//pre2��ͷ
					{
						pre.y = pre2.y;
						pre.x = now.x;
					}
				}
				else
				{
					next  = {};
					next.y = pos.y;
					next.x = now.x;

					pre.y = pos.y;

					pre.next = next;
					next.next = now;
				}

				goto end;
			}//����now���ǽ�β

			//3.3������ǰ��ϲ�..........................
			inter = pos.y - pre2.y;
			if (inter < 0)inter = -inter;

			if (inter <= dis)	//���ߺϲ�
			{
				if (pre2 != &coord)	//pre2����ͷ
				{
					delete pre;
					delete now;
					pre2.next = next;
					pre2.x = next.x;
				}
				else	//pre2��ͷ
				{
					delete pre;
					pre2.next = now;
					now.y = pre2.y;

					if (now.x == next.x && now.y == next.y)
					{
						delete now;
						pre2.next = next;
					}
				}

				goto end;
			}

			//3.4���������ϲ�..........................
			inter = pos.y - next.y;
			if (inter < 0)inter = -inter;

			if (inter <= dis)	//���ߺϲ�
			{
				if (next.next != null)	//next���ǽ�β
				{
					delete pre;
					delete now;
					pre2.next = next;
					next.x = pre2.x;
				}
				else	//next�ǽ�β
				{
					delete now;
					pre.next = next;
					pre.y = next.y;
				}
				goto end;
			}

			//3.5�����������..........................
			now.y = pos.y;
			pre.y = pos.y;
			goto end;

		}	//�������ú�������

	end:

		CleanLead();	//ɾ������ͬ����ĵ��߽��
		//MakeFit();		//��������

		return true;
	},

	//������������ı�,���µ���λ��
	RefreshPos: function() {
		POINT from, to;
		LEADSTEP * now;
		LEADSTEP * p1, * p2, * p3;

		//���»�������˵�����
		conBody[0].GetPosFromBody(from);
		conBody[1].GetPosFromBody(to);

		//��ʼ��
		if (coord.next == null || coord.next.next == null) {
			EasyInitPos(from, to);
			MakeFit();	//��������
			return;
		}

		now = &coord;
		
		//�������ı�
		if (from.x != now.x || from.y != now.y)
		{
			p1 = now.next;
			p2 = p1.next;
			if (p2 != null) p3 = p2.next;
			else p3 = null;
			
			if (p1.x != now.x || p1.y != now.y)
			{//ǰ2�����겻ͬ
				if (p1.x == now.x)
					p1.x = from.x;
				else
					p1.y = from.y;
				now.pos = from;
			}
			else if (p1.x != p2.x || p1.y != p2.y)
			{//��2,3�����겻ͬ
				if (p1.x == p2.x)
					p1.y = from.y;
				else
					p1.x = from.x;
				now.pos = from;
			}
			else
			{
				EasyInitPos(from, to);	//��ʼ��
				MakeFit();	//��������
				return;
			}
		}
		
		//�õ��յ�����
		p1 = p2 = p3 = null;
		while (now.next != null)
		{
			p3 = p2;
			p2 = p1;
			p1 = now;
			now = now.next;
		}
		
		//�յ�����ı�
		if (to.x != now.x || to.y != now.y)
		{
			if (p1.x != now.x || p1.y != now.y)
			{//��2�����겻ͬ
				if (p1.x == now.x)
					p1.x = to.x;
				else
					p1.y = to.y;
				now.pos = to;
			}
			else if (p1.x != p2.x || p1.y != p2.y)
			{//����2,3�����겻ͬ
				if (p1.x == p2.x)
					p1.y = to.y;
				else
					p1.x = to.x;
				now.pos = to;
			}
			else
			{
				EasyInitPos(from, to);	//��ʼ��
				MakeFit();	//��������
				return;
			}
		}

		CleanLead();	//ȥ����ͬ����ĵ��߽ڵ�
		MakeFit();		//��������
	},

	//������
	PaintLead: function(CDC * dc) {
		ASSERT(dc != null);

		const LEADSTEP * temp = &coord;
		dc.MoveTo(temp.pos);
		temp = temp.next;
		while (temp != null)
		{
			dc.LineTo(temp.pos);
			temp = temp.next;
		}
	},

	//��õ��߿�ʼλ�úͽ�β����
	GetStartEndPos: function(POINT &pos1, POINT &pos2) {
		const LEADSTEP * temp = &coord;
		while (temp.next != null) temp = temp.next;
		pos1 = coord.pos;
		pos2 = temp.pos;
	}

};