
var MyPropertyDlg = {
	labelIdPrefix : "MPDLabelElement",
	tagIdPrefix : "MPDTagElement",
	
	CreateNew : function(list, readOnly, model, windowTitle, wndParent) {
		var inter = {x: 20, y: 15};
		var firstNotePos = {x: 20, y: 20};
		var noteTextSize = {cx: 180, cy: 20};
		var firstCtrlPos = {x: firstNotePos.x + noteTextSize.cx + inter.x, y: firstNotePos.y};	//��һ���ؼ���ʼ����
		var tagSize = {cx: 300, cy: noteTextSize.cy};	//�ؼ���С
		var okButtonPos = {x: firstCtrlPos.x, y: firstCtrlPos.y + (tagSize.cy + inter.y) * list.GetListSize()};
		var cancelButtonPos = {x: okButtonPos.x + 150 + m_inter.x, y: okButtonPos.y};
		var wndSize = {cx: firstCtrlPos.x + tagSize.cx + inter.x + 10, cy: cancelButtonPos.y + 70};
		
		return {
			m_windowTitle: windowTitle,	//��������
			m_model: model,				//ʾ��
			m_readOnly: readOnly,		//�Ƿ�ֻ��
			m_list: list,				//�����б�

			m_inter: inter,	// x:�ؼ� �� ������ʾtext �ļ��; y:�ؼ�֮�� ���� ������ʾtext ֮�� �ļ��

			m_firstNotePos: firstNotePos,	//��һ��������ʾtext��ʼ����
			m_noteTextSize: noteTextSize,	//������ʾtext��С

			m_firstCtrlPos: firstCtrlPos,	//��һ���ؼ���ʼ����
			m_tagSize: tagSize,			//�ؼ���С
			
			m_okButtonPos: okButtonPos,	//ȷ����ť������
			m_cancelButtonPos: cancelButtonPos,	//ȡ����ť������
			
			m_wndSize: wndSize,	//���ڴ�С
			
			__proto__: MyPropertyDlg
		};
	},

	DoModal : function() {
		var div = $("<div></div>");
		if (this.m_model != null) {
			MyPropertyDlg.CreateLabel(0, "ʾ��").appendTo(div);
			$("<img src='" + this.m_model + "'></img>").appendTo(div);
		}

		for (var i=0; i<this.m_list.GetListSize(); ++i) {
			MyPropertyDlg.CreateLabel(i+1, this.m_list.noteText[i]).appendTo(div);

			switch (this.m_list.dataTypeList[i]) {
			case DATA_STYLE_float:
			case DATA_TYPE_uint:
			case DATA_TYPE_string:
				MyPropertyDlg.CreateInput(i+1, this.m_list.GetRowData(i), this.m_list.dataTypeList[i]).appendTo(div);
				break;

			case DATA_STYLE_bool:
				MyPropertyDlg.CreateCheck(i+1, this.m_list.GetRowData(i)).appendTo(div);
				break;

			case DATA_STYLE_enum:
				var optionArray = this.m_list.memberNameList[i].noteList;
				MyPropertyDlg.CreateSelect(i+1, optionArray, this.m_list.GetRowData(i)).appendTo(div);
				break;
				
			case DATA_TYPE_color:
				break;
			}
		}
		
		this.layerIndex = layer.open({
			type: 1,
			scrollbar: false,
			
			title: this.m_windowTitle,
			content: div,
			yes: ProperyDlg.OnOK
		});
	},
	
	//����label�ؼ�
	CreateLabel : function(id, text) {
		var element = $("<span id=" + MyPropertyDlg.labelIdPrefix+id + ">" + text + "</span>");
		element.css({"width": this.m_noteTextSize.cx+"px", "height": this.m_noteTextSize.cy+"px"});
		if (id & 2)
			element.css({"border": "1px lightgrey solid"});
		
		return element;
	},
	//����checkbox�ؼ�
	CreateCheck : function(id, checked) {
		var element = $("<input type='checkbox' id=" + MyPropertyDlg.tagIdPrefix+id + " />");
		element.css({"width": this.m_tagSize.cx+"px", "height": this.m_tagSize.cy+"px"});
		if (id & 2)
			element.css({"border": "1px lightgrey solid"});
		if (this.m_readOnly)
			element.attr("disabled", "disabled");
		
		if (checked)
			element.attr("checked", "checked");
		
		return element;
	},
	//����select�ؼ�
	CreateSelect : function(id, optionNoteArray, selIndex) {
		var element = $("<select id=" + MyPropertyDlg.tagIdPrefix+id + "></select>");
		element.css({"width": this.m_tagSize.cx+"px", "height": this.m_tagSize.cy+"px"});
		if (id & 2)
			element.css({"border": "1px lightgrey solid"});
		if (this.m_readOnly)
			element.css({"disabled": "disabled"});
		
		for (var i=0; i<optionNoteArray.length; ++i) {
			var option = $("<option value=" + optionNoteArray[i] + ">" + optionNoteArray[i] + "</option>").appendTo(element);
			if (selIndex == i) 
				option.attr("selected", "true");
		}
		
		return element;
	},
	// ����input�ؼ�
	CreateInput : function(id, initValue, valueType) {
		var element = $("<input id=" + MyPropertyDlg.tagIdPrefix+id + " />");
		element.css({"width": this.m_tagSize.cx+"px", "height": this.m_tagSize.cy+"px"});
		if (id & 2)
			element.css({"border": "1px lightgrey solid"});
		if (this.m_readOnly)
			element.attr("disabled", "disabled");
		
		switch (valueType) {
		case DATA_STYLE_float:
			element.attr("type", "text");
			element.attr("maxLength", 17);
			break;
		case DATA_TYPE_uint:
			element.attr("type", "number");
			element.attr("maxLength", 9);
			break;
		case DATA_TYPE_string:
			element.attr("type", "text");
			element.attr("maxLength", NAME_LEN);
			break;
		default:
			return null;
		}
		
		return element;
	},


	//��ȷ����ť
	OnOK : function(index, layero) {
		if (this.m_readOnly) {	//ֻ��״̬��������Ч��Ϣ
			parent.layer.close(this.layerIndex);
			return;
		}

		var errorType = ERROR_NO;
		var i;

		//��������
		for (i = this.m_list.GetListSize()-1; i>=0; --i) {
			errorType = this.m_list.CheckAMember(i, GetDlgItem(CTRLID(i)));
			if(errorType != ERROR_NO) break;
		}

		//������ʾ
		if (errorType != ERROR_NO) {
			var showText;
			var errorText;

			switch (errorType) { //��������
			case ERROR_STRNULL:
				errorText = "�������Ϊ��!";
				break;
			case ERROR_FLOATMIX:
				errorText = "������������ֻ�������ֺ����һ��С����!";
				break;
			case ERROR_UINTMIX:
				errorText = "������������������������ַ�!";
				break;
			case ERROR_UINTOVER:
				errorText = "������������ķ�Χ��!";
				break;
			case ERROR_ENUMOVER:
				errorText = "û��ѡ��ѡ���е�ĳһ��!";
				break;
			case ERROR_STRMIX:
				errorText = "��ǩ�в��ܰ��� [ ] ( ) { }";
				break;
			case ERROR_ENUMNOTALLOWED:
				errorText = "ѡ��������ѡ����ѧԪ����ɫ����Ϊ��ɫ !";
				break;
			}

			if (1 == this.m_list.GetListSize()) {
				showText = errorText + "\n����������!";
			} else {
				showText = "��"+(i+1)+"��������:\n\t"+this.m_list.noteTextList[i]+"\n"+errorText+"\n����������!";
			}
			alert(showText);
			document.getElementById(tagIdPrefix+(i+1)).focus();	//���ݲ��Ϸ��ؼ���ý���
			return;
		}

		//���Գɹ�д������
		for (i = this.m_list.GetListSize()-1; i>=0; --i)
			this.m_list.SaveAMember(i, GetDlgItem(CTRLID(i)));

		parent.layer.close(this.layerIndex);
	}
};
