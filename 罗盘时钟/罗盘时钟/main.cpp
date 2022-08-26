#include <graphics.h> 
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

const double PI = acos(-1.0);
#define Width 800
#define Height 800

// str������Ƶ��ַ���
// variable ����ÿ�εı���
// fors ÿ����Ҫѭ���Ĵ�������
// R ��Ȧ�İ뾶
// Radian �ۼӵĻ���
void DrawCircle(TCHAR str[25], int variable, int fors, int R, double Radian);		// ����һȦ�ַ�									
int monthdasy(int y, int m);														// ĳ��ĳ�µ����� y ������� m �����·�

																					// һ��Ȧ�Ľṹ��
struct TimeCircle
{
	int fors;											// ÿһȦ�ֳɵķ���
	int R;												// Ȧ�İ뾶
	double NextTime;									// ��һ��ʱ��ʱ��
	double Radian;										// �ۼӵĻ���
};

int main()
{
	initgraph(Width, Height, EW_SHOWCONSOLE);
	SYSTEMTIME ti;
	TimeCircle TC[7];
	TCHAR str[25];

	for (int i = 0; i < 7; i++)
	{
		TC[i].R = (i + 1) * 50;
		TC[i].Radian = 0;
		TC[i].NextTime = 0;

		switch (i)
		{
		case 0:TC[i].fors = 1; break;							// ��
		case 1:TC[i].fors = 12; break;							// ��
		case 2:TC[i].fors = 30; break;							// ��
		case 3:TC[i].fors = 7; break;							// ��
		case 4:TC[i].fors = 24; break;							// ʱ
		case 5:TC[i].fors = 60; break;							// ��
		case 6:TC[i].fors = 60; break;							// ��
		}
	}

	BeginBatchDraw();
	while (true)
	{
		GetLocalTime(&ti);
		TC[2].fors = monthdasy(ti.wYear, ti.wMonth);
		for (int j = 0; j < 7; j++)
		{
			if (TC[5].NextTime != ti.wMinute)
			{
				TC[5].NextTime = ti.wMinute;
				TC[5].Radian = 0;
			}
			else
			{
				TC[5].Radian = TC[5].Radian +
					(2 * PI / TC[5].fors - TC[5].Radian) / 10;
			}

			if (TC[6].NextTime != ti.wSecond)
			{
				TC[6].NextTime = ti.wSecond;
				TC[6].Radian = 0;
			}
			else
			{
				TC[6].Radian = TC[6].Radian +
					(2 * PI / TC[6].fors - TC[6].Radian) / 10;
			}

			for (int i = 0; i < TC[j].fors; i++)
			{
				switch (j)
				{
				case 0: _stprintf_s(str, _T("%d��"), ti.wYear);
					break;
				case 1: _stprintf_s(str, _T("%d��"), 
					(i + ti.wMonth) % TC[j].fors ?
					(i + ti.wMonth) % TC[j].fors : TC[j].fors); 
					break;
				case 2: _stprintf_s(str, _T("%d��"), 
					(i + ti.wDay) % TC[j].fors ?
					(i + ti.wDay) % TC[j].fors : i + ti.wDay);
					break;
				case 3: str[0] = L"��"[0];
					str[1] = L"��һ����������"[(i + ti.wDayOfWeek) % 7];
					str[2] = L""[0]; 
					break;
				case 4: _stprintf_s(str, _T("%dʱ"), 
					(i + ti.wHour) % TC[j].fors); 
					break;
				case 5: _stprintf_s(str, _T("%d��"), 
					(i + ti.wMinute) % TC[j].fors);
					break;
				case 6: _stprintf_s(str, _T("%d��"),
					(i + ti.wSecond) % TC[j].fors);
					break;
				}
				DrawCircle(str, i, TC[j].fors, TC[j].R, TC[j].Radian);;
			}
		}
		FlushBatchDraw();
		cleardevice();
	}
	EndBatchDraw();
	return 0;
}

int monthdasy(int y, int m)
{
	if (m == 2)
		return ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ? 29 : 28;
	else
		return 31 - (m - 3) % 5 % 2;
}

void DrawCircle(TCHAR str[25], int variable, int fors, int R, double Radian)
{
	settextcolor(variable ? HSLtoRGB((360.f / fors) * variable, 1, 0.5f) : WHITE);
	double a, x0, y0, w, h, x1, y1;
	int x, y;
	settextstyle(22, 0, L"΢���ź�",variable * 3600/ fors,
		variable * 3600 / fors, 0, false, false, false, 
		DEFAULT_CHARSET, //�ַ������ڱ��ز���ϵͳ
		OUT_DEFAULT_PRECIS,//Ĭ�ϵ�ӳ����Ϊ
		CLIP_DEFAULT_PRECIS, //Ĭ�ϵļ�����Ϊ
		ANTIALIASED_QUALITY, //������������
		DEFAULT_PITCH);//Ĭ�ϼ��

	a = (fors == 60) ? ((variable + 1) * PI * 2 / fors - Radian) : 
		(variable * PI * 2 / fors);	// �����ַ������� a
	
	w = textwidth(str);				// �����ַ����� w���� h
	h = textheight(str);

	//x1 = R * cos(a);		// ��������ַ��������Ͻ�λ��
	//y1 = R * sin(a);

	//x0 = x1 * cos(-a) - y1 * sin(-a);			// ���ַ�����ԭ��˳ʱ����ת a ����
	//y0 = y1 * cos(-a) + x1 * sin(-a);

	x0 = R;
	y0 = 0;

	x0 -= w / 2;	// ���ַ���������ƫ�� w/2��h/2
	y0 += h / 2;	// ��ͼ��������Ϊ��

	x = (int)(x0 * cos(a) - y0 * sin(a));	// ���ַ�����ԭ����ʱ����ת a ����
	y = (int)(y0 * cos(a) + x0 * sin(a));

	outtextxy(int(Width / 2 + x+ 0.5), int(Height / 2 - y + 0.5), str);	// ��ͼ��������Ϊ��
}
