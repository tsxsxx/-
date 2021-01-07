#include <graphics.h>
#include <mmSystem.h>
#include <stdio.h>
#include <conio.h>

#pragma comment(lib, "winmm.lib")

#define SCREEN_WIDTH  300
#define SCREEN_HEIGHT 300

// 对行和列以及斜对角进行判断,是否可以连成线
#define ONE_ROW		(map[0][0] != -1 && map[1][0] != -1 && map[2][0] != -1) && \
					((map[0][0] == map[1][0]) && (map[1][0] == map[2][0]))		//第一排

#define TWO_ROW		(map[0][1] != -1 && map[1][1] != -1 && map[2][1] != -1) && \
					((map[0][1] == map[1][1]) && (map[1][1] == map[2][1]))		//第二排

#define THREE_ROW   (map[0][2] != -1 && map[1][2] != -1 && map[2][2] != -1) && \
					((map[0][2] == map[1][2]) && (map[1][2] == map[2][2]))		//第三排

#define ONE_COLUMN		(map[0][0] != -1 && map[0][1] != -1 && map[0][2] != -1) && \
						((map[0][0] == map[0][1]) && (map[0][1] == map[0][2]))	//第一列

#define TWO_COLUMN		(map[1][0] != -1 && map[1][1] != -1 && map[1][2] != -1) && \
						((map[1][0] == map[1][1]) && (map[1][1] == map[1][2]))	//第二列

#define THREE_COLUMN	(map[2][0] != -1 && map[2][1] != -1 && map[2][2] != -1) && \
						((map[2][0] == map[2][1]) && (map[2][1] == map[2][2]))	//第三列

#define LEFT_OBLIQUE		(map[0][0] != -1 && map[1][1] != -1 && map[2][2] != -1) && \
							((map[0][0] == map[1][1]) && (map[1][1] == map[2][2]))	//左斜角

#define RIGHT_OBLIQUE		(map[2][0] != -1 && map[1][1] != -1 && map[0][2] != -1) && \
							((map[2][0] == map[1][1]) && (map[1][1] == map[0][2]))	//右斜角

// 棋盘地图, -1代表空位置
int map[3][3] =
{
	-1, -1, -1,
	-1, -1, -1,
	-1, -1, -1
};

int x = 0, y = 0;	//当前下棋的坐标
int camp = 0;		//阵营(自己玩的图案) 0为O图形, 1为X图形
bool resWin[3];		//判断赢得是哪一条线

// 加载资源以及初始化棋盘
void initChessboard();

// 检查鼠标事件
void detectMouseMsg(MOUSEMSG& mouse, IMAGE* img);

// 检查游戏是否结束
bool chechEnd();

// 重置游戏
void resetGame();

int main(void)
{
	IMAGE img[2];
	MOUSEMSG mouse;

	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);			//初始化界面

	loadimage(&img[0], _T("o.png"), 0, 0, true);	//加载资源图
	loadimage(&img[1], _T("x.png"), 0, 0, true);

	initChessboard();	//初始化棋盘

	// 播放音乐
	mciSendString(_T("play 为你我受冷风吹.mp3 repeat"), 0, 0, 0);

	while (true)
	{
		// 检测鼠标位置, 走棋
		detectMouseMsg(mouse, img);

		if (chechEnd())		//判断是否结束游戏了
		{
			Sleep(500);
			cleardevice();		//清除窗口
			initChessboard();
			resetGame();		//重置游戏的参数
		}
	}

	system("pause");
	return 0;
}

void initChessboard()
{
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);

	// 画棋盘
	for (int i = 1; i < 3; i++)
	{
		line(0, i * 100, 300, i * 100);
		line(i * 100, 0, i * 100, 300);
	}
	line(300, 0, 300, 300);
}

void detectMouseMsg(MOUSEMSG& mouse, IMAGE* img)
{
	int front = -1;
	mouse = GetMouseMsg();		// 获取鼠标信息

	if (mouse.uMsg == WM_LBUTTONDOWN)	//判断鼠标是否左键按下
	{
		// 计算坐标
		x = mouse.x / 100;
		y = mouse.y / 100;

		front = map[x][y];		//记录当前位置的值
		map[x][y] = camp;		//鼠标点击的区域

		if (front == -1) //-1代表空位, 即可以在当前位置下棋
		{
			putimage(x * 100 + 5, y * 100 + 5, &img[map[x][y]]);
			(camp == 1) ? camp = 0 : camp = 1;	//交换阵营
		}
	}
}

bool chechEnd()
{
	setlinecolor(YELLOW);		//设置线的颜色
	setlinestyle(PS_SOLID, 5);	//设置线的风格

	if ((resWin[0] = ONE_ROW) || (resWin[1] = TWO_ROW) || (resWin[2] = THREE_ROW))
	{
		if (resWin[0] == true)
			line(5, 45, 295, 45);
		else if (resWin[1] == true)
			line(5, 155, 295, 155);
		else if (resWin[2] == true)
			line(5, 245, 295, 245);

		outtextxy(140, 140, "你赢了!");
		Sleep(100);
		return true;
	}
	else if ((resWin[0] = ONE_COLUMN) || (resWin[1] = TWO_COLUMN) || (resWin[2] = THREE_COLUMN))
	{
		if (resWin[0] == true)
			line(45, 5, 45, 295);
		else if (resWin[1] == true)
			line(155, 5, 155, 295);
		else if (resWin[2] == true)
			line(245, 5, 245, 295);

		outtextxy(140, 140, "你赢了!");
		Sleep(100);
		return true;
	}
	else if ((resWin[0] = LEFT_OBLIQUE) || (resWin[1] = RIGHT_OBLIQUE))
	{
		if (resWin[0] == true)
			line(5, 5, 295, 295);
		else if (resWin[1] == true)
			line(295, 5, 5, 295);

		outtextxy(140, 140, "你赢了!");
		Sleep(100);
		return true;
	}

	// 判断是否还有空位
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (map[i][j] == -1) return false;

	outtextxy(140, 140, "平局!");
	Sleep(70);
	return true;
}

void resetGame()
{
	// 擦除界面,重置map值
	setfillcolor(BLACK);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			map[i][j] = -1;
			solidrectangle(i * 100 + 5, j * 100 + 5, i * 100 + 95, j * 100 + 95);
		}
	}
}
