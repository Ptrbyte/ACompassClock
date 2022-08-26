#include <graphics.h> 
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

const double PI = acos(-1.0);
#define Width 800
#define Height 800

// str代表绘制的字符串
// variable 代表每次的变量
// fors 每次需要循环的次数总数
// R 该圈的半径
// Radian 累加的弧度
void DrawCircle(TCHAR str[25], int variable, int fors, int R, double Radian);		// 绘制一圈字符									
int monthdasy(int y, int m);														// 某年某月的天数 y 代表年份 m 代表月份

																					// 一个圈的结构体
struct TimeCircle
{
	int fors;											// 每一圈分成的份数
	int R;												// 圈的半径
	double NextTime;									// 上一个时刻时间
	double Radian;										// 累加的弧度
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
		case 0:TC[i].fors = 1; break;							// 年
		case 1:TC[i].fors = 12; break;							// 月
		case 2:TC[i].fors = 30; break;							// 日
		case 3:TC[i].fors = 7; break;							// 周
		case 4:TC[i].fors = 24; break;							// 时
		case 5:TC[i].fors = 60; break;							// 分
		case 6:TC[i].fors = 60; break;							// 秒
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
				case 0: _stprintf_s(str, _T("%d年"), ti.wYear);
					break;
				case 1: _stprintf_s(str, _T("%d月"), 
					(i + ti.wMonth) % TC[j].fors ?
					(i + ti.wMonth) % TC[j].fors : TC[j].fors); 
					break;
				case 2: _stprintf_s(str, _T("%d号"), 
					(i + ti.wDay) % TC[j].fors ?
					(i + ti.wDay) % TC[j].fors : i + ti.wDay);
					break;
				case 3: str[0] = L"周"[0];
					str[1] = L"日一二三四五六"[(i + ti.wDayOfWeek) % 7];
					str[2] = L""[0]; 
					break;
				case 4: _stprintf_s(str, _T("%d时"), 
					(i + ti.wHour) % TC[j].fors); 
					break;
				case 5: _stprintf_s(str, _T("%d分"), 
					(i + ti.wMinute) % TC[j].fors);
					break;
				case 6: _stprintf_s(str, _T("%d秒"),
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
	settextstyle(22, 0, L"微软雅黑",variable * 3600/ fors,
		variable * 3600 / fors, 0, false, false, false, 
		DEFAULT_CHARSET, //字符集基于本地操作系统
		OUT_DEFAULT_PRECIS,//默认的映射行为
		CLIP_DEFAULT_PRECIS, //默认的剪辑行为
		ANTIALIASED_QUALITY, //输出质量抗锯齿
		DEFAULT_PITCH);//默认间距

	a = (fors == 60) ? ((variable + 1) * PI * 2 / fors - Radian) : 
		(variable * PI * 2 / fors);	// 计算字符串弧度 a
	
	w = textwidth(str);				// 计算字符串宽 w、高 h
	h = textheight(str);

	//x1 = R * cos(a);		// 计算输出字符串的左上角位置
	//y1 = R * sin(a);

	//x0 = x1 * cos(-a) - y1 * sin(-a);			// 将字符串绕原点顺时针旋转 a 弧度
	//y0 = y1 * cos(-a) + x1 * sin(-a);

	x0 = R;
	y0 = 0;

	x0 -= w / 2;	// 将字符串向左上偏移 w/2、h/2
	y0 += h / 2;	// 绘图坐标向下为正

	x = (int)(x0 * cos(a) - y0 * sin(a));	// 将字符串绕原点逆时针旋转 a 弧度
	y = (int)(y0 * cos(a) + x0 * sin(a));

	outtextxy(int(Width / 2 + x+ 0.5), int(Height / 2 - y + 0.5), str);	// 绘图坐标向下为正
}
