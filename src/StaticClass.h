//#include <math.h>

class StaticClass
{
public:

	static bool IsZero(const double x)	//�ж�ĳ���������Ƿ����Ϊ0
	{
		return x > -(1e-9) && x < (1e-9);
	}
	/*static bool IsFloatEqual(const double a, const double b)	//�ж�2���������Ƿ�������
	{
		double max = fabs(a);
		if(fabs(b) > fabs(a)) max = fabs(b);
		return (fabs(a - b) <= (1e-7) * max);
	}*/
	static bool IsElecError(const ELEC_STATE e)	//�����Ƿ�����
	{
		return e < NORMALELEC || e > OPENELEC;
	}

	static bool IsFloat(const char * str)	//�ж��ַ����Ƿ��Ǹ�������
	{
		int count = 0;

		//����Ƿ����ֻ��һ��'.',û����������������ַ�
		while(*str != '\0')
		{
			if('.' == *str)
			{
				++count;
				if(count > 1) return false;
			}
			else if(!isdigit(* str))
			{
				return false;
			}

			++ str;
		}

		return true;
	}

	static bool IsUnsignedInteger(const char * str)	//�ж��ַ����Ƿ���������
	{
		while(*str != '\0' && isdigit(*str)) ++str;
		return *str == '\0';
	}

	static bool IsNormalStr(const char * str)	//�ж��ַ����Ƿ񲻺�[](){}
	{
		while(*str != '\0')
		{
			if(    '[' == *str || ']' == *str 
				|| '(' == *str || ')' == *str
				|| '{' == *str || '}' == *str)
				return false;
			++ str;
		}
		return true;
	}

	static bool IsCtrlDown()			//��������Ctrl���Ƿ���
	{
		return 0x1000 & GetKeyState(VK_CONTROL) || 0x1000 & GetKeyState(VK_RCONTROL);
	}

	static int SaveBitmapToFile(HBITMAP hBitmap, const char * fileName)
	//��ͼƬ����ļ�, hBitmapΪλͼ���, fileNameΪλͼ�ļ���
	{
		HDC hDC;					//�豸������
		int iBits;					//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
		WORD wBitCount;				//λͼ��ÿ��������ռ�ֽ���
		DWORD dwPaletteSize = 0;	//��ɫ���С
		DWORD dwBmBitsSize;			//λͼ�������ֽڴ�С
		DWORD dwDIBSize;			//λͼ�ļ���С
		DWORD dwWritten;			//д���ļ��ֽ���
		BITMAP Bitmap;				//λͼ���Խṹ
		BITMAPFILEHEADER bmfHdr;	//λͼ�ļ�ͷ�ṹ
		BITMAPINFOHEADER bi;		//λͼ��Ϣͷ�ṹ 
		LPBITMAPINFOHEADER lpbi;	//ָ��λͼ��Ϣͷ�ṹ
		HANDLE fh;					//�ļ����
		HANDLE hDib;				//�����ڴ���
		HANDLE hPal;				//��ɫ����
		HPALETTE hOldPal = NULL;	//ԭ�ȵ�ɫ����

		//����λͼ�ļ�ÿ��������ռ�ֽ���
		hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
		iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
		DeleteDC(hDC);
		if(iBits <= 1)
			wBitCount = 1;
		else if(iBits <= 4)
			wBitCount = 4;
		else if(iBits <= 8)
			wBitCount = 8;
		else if(iBits <= 24)
			wBitCount = 24;
		else
			wBitCount = 32;

		//�����ɫ���С
		if (wBitCount <= 8) dwPaletteSize = (1<<wBitCount) * sizeof(RGBQUAD);

		//����λͼ��Ϣͷ�ṹ
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
		bi.biSize			= sizeof(BITMAPINFOHEADER);
		bi.biWidth			= Bitmap.bmWidth;
		bi.biHeight			= Bitmap.bmHeight;
		bi.biPlanes			= 1;
		bi.biBitCount		= wBitCount;
		bi.biCompression	= BI_RGB;
		bi.biSizeImage		= 0;
		bi.biXPelsPerMeter	= 0;
		bi.biYPelsPerMeter	= 0;
		bi.biClrUsed		= 0;
		bi.biClrImportant	= 0;

		//Ϊλͼ���ݷ����ڴ�
		dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32) * 4 * Bitmap.bmHeight;

		/*xxxxxxxx����λͼ��С�ֽ�һ��(����һ����������)xxxxxxxxxxxxxxxxxxxx 
		//ÿ��ɨ������ռ���ֽ���Ӧ��Ϊ4���������������㷨Ϊ:
		int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
		if((Bitmap.bmWidth*wBitCount) % 32) biWidth++;	//�����������ļ�1
		biWidth *= 4;									//���������õ���Ϊÿ��ɨ���е��ֽ�����
		dwBmBitsSize = biWidth * Bitmap.bmHeight;		//�õ���С
		xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

		hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
		*lpbi = bi;

		// �����ɫ��   
		hPal = GetStockObject(DEFAULT_PALETTE);
		if(hPal)
		{
			hDC = ::GetDC(NULL);
			hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
			RealizePalette(hDC);
		}

		// ��ȡ�õ�ɫ�����µ�����ֵ
		GetDIBits(	hDC,
					hBitmap,
					0,
					(UINT)Bitmap.bmHeight,
					(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
					(BITMAPINFO *)lpbi,
					DIB_RGB_COLORS);

		//�ָ���ɫ��   
		if(hOldPal)
		{
			SelectPalette(hDC, hOldPal, TRUE);
			RealizePalette(hDC);
			::ReleaseDC(NULL, hDC);
		}

		//����λͼ�ļ�    
		fh = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if(fh == INVALID_HANDLE_VALUE) return FALSE;

		// ����λͼ�ļ�ͷ
		bmfHdr.bfType = 0x4D42;	// "BM"
		dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
		bmfHdr.bfSize = dwDIBSize;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

		// д��λͼ�ļ�ͷ
		WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

		// д��λͼ�ļ���������
		WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize , &dwWritten, NULL);

		//���
		GlobalUnlock(hDib);
		GlobalFree(hDib);
		CloseHandle(fh);
		return TRUE;
	}

};