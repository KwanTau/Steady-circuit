#if !defined(AFX_KMP_FDEF)
#define AFX_KMP_FDEF


class KMP
{
private:
	int * m_next;
	char m_subString[NAME_LEN*2];
	const int m_subStringLen;
	const bool m_isWholeWord;
	const bool m_isMatchCase;

	int IndexKMP(const char * string, int pos = 0)
	{
		int i = pos, j = 0;
		int stringLen = strlen(string);

		while(i < stringLen && j < m_subStringLen)
		{
			if(j == -1 || string[i] == m_subString[j])
			{
				++i;
				++j;
			}
			else
			{
				j = m_next[j];
			}
		}

		if(j == m_subStringLen)
			return i - j;
		else
			return -1;
	}

public:

	KMP(const char * subString, bool isWholeWord, bool isMatchCase)
		: m_subStringLen(strlen(subString)),
		m_isWholeWord(isWholeWord),
		m_isMatchCase(isMatchCase)
	{
		int i = 0, j = -1;

		strcpy(m_subString, subString);	//�����Ӵ�

		if(!m_isMatchCase) strupr(m_subString);	//�����ִ�Сд

		if(m_isWholeWord) return;	//ȫ��ƥ��

		m_next = new int[m_subStringLen + 1];
		m_next[0] = -1;

		while(i < m_subStringLen)
		{
			if(j == -1 || m_subString[i] == m_subString[j])
			{
				++i;
				++j;
				m_next[i] = j;
			}
			else
			{
				j = m_next[j];
			}
		}
	}

	~KMP()
	{
		if(!m_isWholeWord) delete [] m_next;
	}

	bool IsMatch(const char * str)
	//�ж��ַ����Ƿ�ƥ��
	{
		char tempStr[128];
		const char * p;

		if(!m_isMatchCase)	//�����ִ�Сд
		{
			strcpy(tempStr, str);
			strupr(tempStr);
			p = tempStr;
		}
		else	//���ִ�Сд
		{
			 p = str;
		}

		if(m_isWholeWord)	//ȫ��ƥ��
		{
			return 0 == strcmp(p, m_subString);
		}
		else	//�����Ӵ�
		{
			return m_subStringLen == 0 || IndexKMP(p) >= 0;
		}
	}
};


#endif // !defined(AFX_KMP_FDEF)