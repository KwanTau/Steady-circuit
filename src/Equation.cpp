/* �Ⱥ��·��ѧģ����
   ��Ȩ���У�C�� 2013 <����>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
   
#include "StdAfx.h"
#include "StaticClass.h"	//����static��������
#include "Equation.h"		//��ǰ��


Equation::Equation(int crunNum, int eleNum)
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

	x = (double *)malloc(eleNum * sizeof(double));
	ZeroMemory(x, eleNum * sizeof(double));
	
	c = (int *)malloc(m * sizeof(int));
	ZeroMemory(c, m * sizeof(int));
	
	a = (double **)malloc(m * sizeof(double *));
	for(int i=m-1; i>=0; --i)
	{
		a[i] = (double *)malloc(n * sizeof(double));
		ZeroMemory(a[i], n * sizeof(double));
	}
}

Equation::~Equation()
{
	if(m > 0 && n > 1)
	{
		free(c);
		free(x);
		for(int i=m-1; i>=0; --i) free(a[i]);
		free(a);
	}
}

const double * Equation::GetAnswer()
//��÷��̽������
{
	return x;
}

void Equation::InputARow(const double * buf)
//�����gotoRows��ʼ��1�����ݵ���������
{
	ASSERT(gotoRow < m);

	memcpy((void *)a[gotoRow], (const void *)buf, n * sizeof(double));
	++gotoRow;
}

void Equation::OutputToFile(FILE * fp)
//�����̱��浽�ļ�,���Ժ���
{
	int i, j;

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
}

ELEC_STATE Equation::Count()
{
	const int m = gotoRow;	//��¼�Ѿ����뵽����,������this->m
    int i, j, l, k, w;
    double temp;
	if(m <= 0 || n <= 1) return NORMALELEC;	//�������
	w = n<m-1 ? n : m-1;					//w��ֵΪm-1,n�Ľ�Сֵ

	for(i=n-2; i>=0; --i) x[i] = 0;

	//������-----------------------------------------------------------------------------
	for(l=0,k=0; l<w; ++l,++k)
	{
		while(k < n)
		{
			for(i=l; i<m; ++i)
				if(!StaticClass::IsZero(a[i][k])) break;
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
			if(!StaticClass::IsZero(a[i][k]))
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
		for(j=i; j<n; ++j) if(!StaticClass::IsZero(a[i][j])) break;
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
		ASSERT(!StaticClass::IsZero(a[l][c[l]]));

		for(i=0; i<l; ++i)
		{
			for(j=c[l]+1; j<n; ++j) a[i][j] -= a[l][j] * a[i][c[l]];
			a[i][c[l]] = 0;
		}
	}

	for(i=n-2; i>=0; --i) x[i] = a[i][n-1];							//������������
	for(i=n-2; i>=0; --i) if(StaticClass::IsZero(x[i])) x[i] = 0;	//����0������Ϊ0

	return NORMALELEC;	//��������
}
