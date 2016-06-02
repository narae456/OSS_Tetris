#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "TetrisView.h"
#include "TetrisManager.h"
#include "Constant.h"
#include "Util.h"
#include "Menu.h"

#pragma comment(lib, "winmm.lib")

#define TETRIS_BACKGROUND_MUSIC_FILE_NAME "..\\res\\tetris_background_music.wav"

#define PROCESS_REACHED_CASE_COUNT 2
#define ADD_ID_POSITION_X_TO_PRINT 25
#define ADD_ID_POSITION_Y_TO_PRINT 10
#define SETTING_POSITION_X_TO_PRINT 30
#define SETTING_POSITION_Y_TO_PRINT 10

enum MenuStartPosition{
	MAIN_MENU_X = 18, MAIN_MENU_Y = 7, PAUSE_MENU_X = 5, PAUSE_MENU_Y = 12, END_MENU_X = 14, END_MENU_Y = 8
};

///// BGM ON/OFF�� �����ϴ� �Լ�
void TetrisView_Bgm(TetrisView* tetrisView, int mode){
	switch(mode)
	{
	case 1:
		PlaySound(TEXT(TETRIS_BACKGROUND_MUSIC_FILE_NAME), NULL, SND_ASYNC | SND_LOOP);
		break;
	case 2:
		PlaySound(NULL, 0, 0);
		break;
	}
}

void TetrisView_StartGame(TetrisView* tetrisView){
	if (!(tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL)){
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	TetrisManager_Init(&tetrisView->tetrisManager, tetrisView->level);
	system("cls");
	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);
	TetrisManager_PrintBoard(&tetrisView->tetrisManager);
	TetrisManager_PrintDetailInfomation(&tetrisView->tetrisManager);
}

void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction){

	//it is used to move left or right at bottom in case of space which you want to move is available
	static int processReachedCaseCount = 0;
	if (processType == DIRECTION){
		TetrisManager_ProcessDirection(&tetrisView->tetrisManager, direction);
	}
	else if (processType == DIRECT_DOWN){
		TetrisManager_ProcessDirectDown(&tetrisView->tetrisManager);
	}
	else if (processType == AUTO){
		TetrisManager_ProcessAuto(&tetrisView->tetrisManager);
	}
	if (TetrisManager_IsReachedToBottom(&tetrisView->tetrisManager, MOVING_BLOCK)){
		if (processType == DIRECT_DOWN){
			processReachedCaseCount = 0;
			if (TetrisManager_ProcessReachedCase(&tetrisView->tetrisManager) == END){
				TetrisView_EndGame(tetrisView);
				return;
			}
		}
		else{
			//if you are going to move the block which has bottom wall or bottom fixed block, permit the block to move the direction
			if (processReachedCaseCount == PROCESS_REACHED_CASE_COUNT){
				if (TetrisManager_ProcessReachedCase(&tetrisView->tetrisManager) == END){
					TetrisView_EndGame(tetrisView);
					return;
				}
				processReachedCaseCount = 0;
			}
			else{
				processReachedCaseCount++;
			}
		}
	}
	TetrisManager_ProcessDeletingLines(&tetrisView->tetrisManager);
}

void TetrisView_PauseGame(TetrisView* tetrisView){
	TetrisManager_PauseTotalTime(&tetrisView->tetrisManager);
	PlaySound(NULL, 0, 0);
	FontUtil_ChangeFontColor(LIGHT_YELLOW);
	TetrisView_ProcessPauseMenu(tetrisView);
	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);
	switch (tetrisView->pauseMenu){
	case RESUME_PAUSE_MENU:
		TetrisManager_StartTotalTime(&tetrisView->tetrisManager);
		break;
	case MAIN_MENU_PAUSE_MENU:
		TetrisManager_StopTotalTime(&tetrisView->tetrisManager);
		break;
	}
}

void TetrisView_EndGame(TetrisView* tetrisView){
	TetrisManager_PauseTotalTime(&tetrisView->tetrisManager);
	PlaySound(NULL, 0, 0);
	TetrisView_ProcessEndMenu(tetrisView);
}

void TetrisView_ShowRanking(TetrisView* tetrisView){
	RankingManager_Create(&tetrisView->rankingManager);
	RankingManager_Load(&tetrisView->rankingManager);
	RankingManager_Print(&tetrisView->rankingManager);
	RankingManager_Destroy(&tetrisView->rankingManager);
}

void TetrisView_AddRanking(TetrisView* tetrisView){
	Ranking ranking;
	int x = ADD_ID_POSITION_X_TO_PRINT;
	int y = ADD_ID_POSITION_Y_TO_PRINT;
	int i;
	char id[ID_SIZE + 1] = { '\0', };
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("������������������������������");
	CursorUtil_GotoXY(x, y++);
	printf("��Input ID (%d chars limit!)��", ID_SIZE);
	CursorUtil_GotoXY(x, y++);
	printf("��:                         ��");
	CursorUtil_GotoXY(x, y++);
	printf("������������������������������");
	x += 4;
	y -= 2;
	CursorUtil_GotoXY(x, y++);
	CursorUtil_Show();
	fgets(id, ID_SIZE + 1, stdin);
	CursorUtil_Hide();
	for (i = ID_SIZE; i >= 0; i--){
		if (id[i] == '\n'){
			id[i] = '\0';
			break;
		}
	}
	if (i == -1){
		while (getchar() != '\n');
	}
	strcpy(ranking.id, id);
	ranking.score = tetrisView->tetrisManager.score;
	ranking.level = tetrisView->tetrisManager.speedLevel;
	ranking.deletedLineCount = tetrisView->tetrisManager.deletedLineCount;
	ranking.totalTime = tetrisView->tetrisManager.totalTime;
	ranking.timestamp = time(NULL);
	RankingManager_Create(&tetrisView->rankingManager);
	RankingManager_Load(&tetrisView->rankingManager);
	RankingManager_Add(&tetrisView->rankingManager, ranking);
	RankingManager_Save(&tetrisView->rankingManager);
	RankingManager_Destroy(&tetrisView->rankingManager);
}

void TetrisView_ShowSetting(TetrisView* tetrisView){
	int x = SETTING_POSITION_X_TO_PRINT;
	int y = SETTING_POSITION_Y_TO_PRINT;
	if (!(tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL)){
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("����������������������");
	CursorUtil_GotoXY(x, y++);
	printf("��Current Level : %2d��", tetrisView->level);
	CursorUtil_GotoXY(x, y++);
	printf("��New Level (%d ~ %d)��", MIN_SPEED_LEVEL, MAX_SPEED_LEVEL);
	CursorUtil_GotoXY(x, y++);
	printf("��:                 ��");
	CursorUtil_GotoXY(x, y++);
	printf("����������������������");
	x += 4;
	y -= 3;
	CursorUtil_GotoXY(x, y++);
	CursorUtil_Show();
	//scanf("%d", &tetrisView->level); // ���� �Է¹޴� �Լ��� ���� ������� ������ ����
	tetrisView->level = level_scanf(x, y); // ���� �Է¹޴� �Լ�

	CursorUtil_Hide();
	if (tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL){

	}
	else if (tetrisView->level < MIN_SPEED_LEVEL){
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	else if (tetrisView->level > MAX_SPEED_LEVEL){
		tetrisView->level = MAX_SPEED_LEVEL;
	}
	else{
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	//while (getchar() != '\n'); // ���� �Է¹޴� �Լ��� ���� ������� ������ ����
}

void TetrisView_ProcessMainMenu(TetrisView* tetrisView){
	Menu menu = { 0, };
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE] = {
		"[1] S T A R T", "[2] R A N K I N G", "[3] S E T T I N G", "[4] E X I T"
	};
	int menuCount = 4;
	int x = MAIN_MENU_X;
	int y = MAIN_MENU_Y;
	WindowUtil_ChangeWindowSize(WINDOW_LINE_SIZE, WINDOW_COL_SIZE);
	system("cls");
	FontUtil_ChangeFontColor(RED);
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_RED);
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("����");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_YELLOW);
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(GREEN);
	CursorUtil_GotoXY(x, y++);
	printf("���");
	CursorUtil_GotoXY(x, y++);
	printf("�� ��");
	CursorUtil_GotoXY(x, y++);
	printf("���");
	CursorUtil_GotoXY(x, y++);
	printf("�� ��");
	CursorUtil_GotoXY(x, y++);
	printf("��  ��");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_BLUE);
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	x += 4;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_PURPLE);
	CursorUtil_GotoXY(x, y++);
	printf(" ���");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("    ��");
	CursorUtil_GotoXY(x, y++);
	printf(" ���");
	x -= 22;
	y += 2;
	Menu_Create(&menu, items, menuCount, x, y, DEFAULT_FONT_COLOR);
	Menu_Print(&menu);
	tetrisView->mainMenu = Menu_ProcessKey(&menu) + 1;
	tetrisView->pauseMenu = 0;
	tetrisView->endMenu = 0;
}

void TetrisView_ProcessPauseMenu(TetrisView* tetrisView){
	Menu menu = { 0, };
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE] = {
		"[1] R E S U M E", "[2] M A I N M E N U"
	};
	int menuCount = 2;
	int x = PAUSE_MENU_X;
	int y = PAUSE_MENU_Y;
	Menu_Create(&menu, items, menuCount, x, y, LIGHT_YELLOW);
	Menu_Print(&menu);
	tetrisView->pauseMenu = Menu_ProcessKey(&menu) + 1;
	tetrisView->mainMenu = 0;
	tetrisView->endMenu = 0;
	if (tetrisView->pauseMenu == RESUME_PAUSE_MENU){
		TetrisManager_PrintBoard(&tetrisView->tetrisManager);
	}
}

void TetrisView_ProcessEndMenu(TetrisView* tetrisView){
	Menu menu = { 0, };
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE] = {
		"[1] R A N K I N G", "[2] M A I N M E N U", "[3] E X I T"
	};
	int menuCount = 3;
	int x = END_MENU_X;
	int y = END_MENU_Y;
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("����  ��  ��  ����          ����  ��  ��  ���");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ��  ��  ��              ��       �� ��  �� ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ����  ����  ����  ����  ����  ��  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ��  ��  ��              ��      �� ��   �� ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ��  ��  ����          ����  ��  ��  ���");
	x += 19;
	y += 2;
	Menu_Create(&menu, items, menuCount, x, y, DEFAULT_FONT_COLOR);
	Menu_Print(&menu);
	tetrisView->endMenu = Menu_ProcessKey(&menu) + 1;
	tetrisView->mainMenu = 0;
	tetrisView->pauseMenu = 0;
}

DWORD TetrisView_GetDownMilliSecond(TetrisView* tetrisView){
	return TetrisManager_GetDownMilliSecond(&tetrisView->tetrisManager);
}

void TetrisView_MakeHold(TetrisView* tetrisView){
	TetrisManager_MakeHold(&tetrisView->tetrisManager);
}

void TetrisView_Item_RemoveOneRow(TetrisView* tetrisView){
	//������1 : �� �� ����
	TetrisManager_Item_RemoveOneRow(&tetrisView->tetrisManager);
}

void TetrisView_Item_RemoveTwoRow(TetrisView* tetrisView){
	//������2 : �� �� ����
	TetrisManager_Item_RemoveTwoRow(&tetrisView->tetrisManager);
}

void TetrisView_Item_RemoveAllRow(TetrisView* tetrisView){
	//������3 : ��ü �� ����
	TetrisManager_Item_RemoveAllRow(&tetrisView->tetrisManager);
}

//�������� �ٴ����� �ٲٱ�
void TetrisView_ChangeNextBlock(TetrisView* tetrisView){
	TetrisManager_ChangeNextBlock(&tetrisView->tetrisManager);
}

int level_scanf(int x, int y) {
	int ch[3] = {-1,-1,-1}; // ���� �Է¹޾� ������ �迭�� ����
	while(1) {
		CursorUtil_GotoXY(x, y); // Ŀ�� ��ġ �̵�
		CursorUtil_Show(); // Ŀ�� ǥ��
		ch[0] = _getch(); // ù��° �Է��� �޴´�
		if (ch[0] == '1') // ù��° �Է��� 1�� ���
		{
			printf("%c", ch[0]); // ȭ�鿡 �Է¹��� ���ڸ� ǥ�� (getch: ���� �Է½� ȭ�� ��� ����)
			while(1) {
				CursorUtil_GotoXY(x+1, y); // Ŀ���� �ι�° �ڸ��� �̵�
				CursorUtil_Show(); // Ŀ�� ǥ��
				ch[1] = _getch(); // �ι�° �Է��� �޴´� 
				if (ch[1] == '0') // ù��° �Է��� 1�̰�, �ι�° �Է��� 0�� ���
				{
					printf("%c", ch[1]); // ȭ�鿡 �Է¹��� ���ڸ� ǥ�� (getch: ���� �Է½� ȭ�� ��� ����)
					while (ch[2] != ENTER_KEY_CODE) // ����° �Է��� ������ �� ���� �ݺ��� ����
					{
						CursorUtil_GotoXY(x+2, y); // Ŀ���� ����° �ڸ��� �̵�
						CursorUtil_Show(); // Ŀ�� ǥ��
						ch[2] = _getch(); // ����° �Է��� �޴´�
					}
					return 10; // 10�� ��ȯ
					break;
				}
				else if (ch[1] == ENTER_KEY_CODE) // ù��° �Է��� 1�̰�, �ι�° �Է��� ������ ���
				{
					return 1; // 1�� ��ȯ
					break;
				}
			}
			break;
		}
		else if (isdigit(ch[0])) // ù��° �Է��� 1�� �ƴ� �ٸ� ������ ��� (2~9)
		{	
			printf("%c", ch[0]); // ȭ�鿡 �Է¹��� ���ڸ� ǥ�� (getch: ���� �Է½� ȭ�� ��� ����)
			while (ch[1] != ENTER_KEY_CODE) // �ι�° �Է��� ������ �� ���� �ݺ��� ����
			{
				CursorUtil_GotoXY(x, y); // Ŀ���� ù��° �ڸ��� �̵�
				CursorUtil_Show(); // Ŀ�� ǥ��
				ch[1] = _getch(); // �ι�° �Է��� �޴´�
			}
			return ch[0]-48; // ù��° �Է¹��� ���� ��ȯ
			break;
		}
		else // ù��° �Է��� ���ڰ� �ƴ� ���
			continue; // �ٽ� �Է¹޴´�
	}
}