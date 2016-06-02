#ifndef _TETRIS_VIEW_H
#define _TETRIS_VIEW_H

#include "TetrisManager.h"
#include "RankingManager.h"

typedef struct _tetrisView{
	TetrisManager tetrisManager;
	RankingManager rankingManager;
	int level;
	int mainMenu;
	int pauseMenu;
	int endMenu;
}TetrisView;

void TetrisView_Bgm(TetrisView* tetrisView, int mode);
void TetrisView_StartGame(TetrisView* tetrisView);
void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction);
void TetrisView_PauseGame(TetrisView* tetrisView);
void TetrisView_EndGame(TetrisView* tetrisView);
void TetrisView_ShowRanking(TetrisView* tetrisView);
void TetrisView_AddRanking(TetrisView* tetrisView);
void TetrisView_ShowSetting(TetrisView* tetrisView);
void TetrisView_ProcessMainMenu(TetrisView* tetrisView);
void TetrisView_ProcessPauseMenu(TetrisView* tetrisView);
void TetrisView_ProcessEndMenu(TetrisView* tetrisView);

//make temporarily for calling from main function
DWORD TetrisView_GetDownMilliSecond(TetrisView* tetrisView);
void TetrisView_MakeHold(TetrisView* tetrisView);

void TetrisView_Item_RemoveOneRow(TetrisView* tetrisView);	//������1 : �� �� ����
void TetrisView_Item_RemoveTwoRow(TetrisView* tetrisView);	//������2 : �� �� ����
void TetrisView_Item_RemoveAllRow(TetrisView* tetrisView);	//������3 : ��ü �� ����
void TetrisView_ChangeNextBlock(TetrisView* tetrisView);//�������� �ٴ����� �ٲٱ�

int level_scanf(int x, int y); // ���� �Է¹޴� �Լ�

#endif