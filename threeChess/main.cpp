#include <graphics.h>
#include <mmSystem.h>
#include <stdio.h>
#include <conio.h>

#pragma comment(lib, "winmm.lib")

#define SCREEN_WIDTH  300
#define SCREEN_HEIGHT 300

// ���к����Լ�б�Խǽ����ж�,�Ƿ����������
#define ONE_ROW		(map[0][0] != -1 && map[1][0] != -1 && map[2][0] != -1) && \
					((map[0][0] == map[1][0]) && (map[1][0] == map[2][0]))		//��һ��

#define TWO_ROW		(map[0][1] != -1 && map[1][1] != -1 && map[2][1] != -1) && \
					((map[0][1] == map[1][1]) && (map[1][1] == map[2][1]))		//�ڶ���

#define THREE_ROW   (map[0][2] != -1 && map[1][2] != -1 && map[2][2] != -1) && \
					((map[0][2] == map[1][2]) && (map[1][2] == map[2][2]))		//������

#define ONE_COLUMN		(map[0][0] != -1 && map[0][1] != -1 && map[0][2] != -1) && \
						((map[0][0] == map[0][1]) && (map[0][1] == map[0][2]))	//��һ��

#define TWO_COLUMN		(map[1][0] != -1 && map[1][1] != -1 && map[1][2] != -1) && \
						((map[1][0] == map[1][1]) && (map[1][1] == map[1][2]))	//�ڶ���

#define THREE_COLUMN	(map[2][0] != -1 && map[2][1] != -1 && map[2][2] != -1) && \
						((map[2][0] == map[2][1]) && (map[2][1] == map[2][2]))	//������

#define LEFT_OBLIQUE		(map[0][0] != -1 && map[1][1] != -1 && map[2][2] != -1) && \
							((map[0][0] == map[1][1]) && (map[1][1] == map[2][2]))	//��б��

#define RIGHT_OBLIQUE		(map[2][0] != -1 && map[1][1] != -1 && map[0][2] != -1) && \
							((map[2][0] == map[1][1]) && (map[1][1] == map[0][2]))	//��б��

// ���̵�ͼ, -1�����λ��
int map[3][3] =
{
	-1, -1, -1,
	-1, -1, -1,
	-1, -1, -1
};

int x = 0, y = 0;	//��ǰ���������
int camp = 0;		//��Ӫ(�Լ����ͼ��) 0ΪOͼ��, 1ΪXͼ��
bool resWin[3];		//�ж�Ӯ������һ����

// ������Դ�Լ���ʼ������
void initChessboard();

// �������¼�
void detectMouseMsg(MOUSEMSG& mouse, IMAGE* img);

// �����Ϸ�Ƿ����
bool chechEnd();

// ������Ϸ
void resetGame();

int main(void)
{
	IMAGE img[2];
	MOUSEMSG mouse;

	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);			//��ʼ������

	loadimage(&img[0], _T("o.png"), 0, 0, true);	//������Դͼ
	loadimage(&img[1], _T("x.png"), 0, 0, true);

	initChessboard();	//��ʼ������

	// ��������
	mciSendString(_T("play Ϊ��������紵.mp3 repeat"), 0, 0, 0);

	while (true)
	{
		// ������λ��, ����
		detectMouseMsg(mouse, img);

		if (chechEnd())		//�ж��Ƿ������Ϸ��
		{
			Sleep(500);
			cleardevice();		//�������
			initChessboard();
			resetGame();		//������Ϸ�Ĳ���
		}
	}

	system("pause");
	return 0;
}

void initChessboard()
{
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);

	// ������
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
	mouse = GetMouseMsg();		// ��ȡ�����Ϣ

	if (mouse.uMsg == WM_LBUTTONDOWN)	//�ж�����Ƿ��������
	{
		// ��������
		x = mouse.x / 100;
		y = mouse.y / 100;

		front = map[x][y];		//��¼��ǰλ�õ�ֵ
		map[x][y] = camp;		//�����������

		if (front == -1) //-1�����λ, �������ڵ�ǰλ������
		{
			putimage(x * 100 + 5, y * 100 + 5, &img[map[x][y]]);
			(camp == 1) ? camp = 0 : camp = 1;	//������Ӫ
		}
	}
}

bool chechEnd()
{
	setlinecolor(YELLOW);		//�����ߵ���ɫ
	setlinestyle(PS_SOLID, 5);	//�����ߵķ��

	if ((resWin[0] = ONE_ROW) || (resWin[1] = TWO_ROW) || (resWin[2] = THREE_ROW))
	{
		if (resWin[0] == true)
			line(5, 45, 295, 45);
		else if (resWin[1] == true)
			line(5, 155, 295, 155);
		else if (resWin[2] == true)
			line(5, 245, 295, 245);

		outtextxy(140, 140, "��Ӯ��!");
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

		outtextxy(140, 140, "��Ӯ��!");
		Sleep(100);
		return true;
	}
	else if ((resWin[0] = LEFT_OBLIQUE) || (resWin[1] = RIGHT_OBLIQUE))
	{
		if (resWin[0] == true)
			line(5, 5, 295, 295);
		else if (resWin[1] == true)
			line(295, 5, 5, 295);

		outtextxy(140, 140, "��Ӯ��!");
		Sleep(100);
		return true;
	}

	// �ж��Ƿ��п�λ
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (map[i][j] == -1) return false;

	outtextxy(140, 140, "ƽ��!");
	Sleep(70);
	return true;
}

void resetGame()
{
	// ��������,����mapֵ
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
