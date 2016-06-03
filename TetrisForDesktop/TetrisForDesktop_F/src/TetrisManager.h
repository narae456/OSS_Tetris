#ifndef _BOARD_H
#define _BOARD_H

#include "Block.h"
#include <time.h> //// 헤더파일 추가

enum GameStatus{
	PLAYING, END
};

#define BOARD_ROW_SIZE 24
#define BOARD_COL_SIZE 14

typedef struct _tetrisManager{
	char board[BOARD_ROW_SIZE][BOARD_COL_SIZE];
	Block block;
	int isHoldAvailable; // if hold block is empty or hold turn is passed, this variable is true
	Block shadow;
	int deletedLineCount;
	int speedLevel;
	int score;
	int isSplashMode; //// 안개 구조체 정의
	time_t randSpeedTimer; ///// 아이템 실행 시간 ( 5초를 재기 위해 사용 )
	int diff_speed; ////// 변한 속도 ( +2 또는 -2 )
}TetrisManager;

void TetrisManager_Init(TetrisManager* tetrisManager, int speedLevel);
void TetrisManager_ProcessDirection(TetrisManager* tetrisManager, int direction);
void TetrisManager_ProcessAuto(TetrisManager* tetrisManager);
void TetrisManager_ProcessDirectDown(TetrisManager* tetrisManager);
void TetrisManager_ProcessDeletingLines(TetrisManager* tetrisManager);
int TetrisManager_IsReachedToBottom(TetrisManager* tetrisManager, int blockType);
int TetrisManager_ProcessReachedCase(TetrisManager* tetrisManager);
void TetrisManager_Print(TetrisManager* tetrisManager);
void TetrisManager_PrintBoard(TetrisManager* tetrisManager);
void TetrisManager_PrintDetailInfomation(TetrisManager* tetrisManager);
DWORD TetrisManager_GetDownMilliSecond(TetrisManager* tetrisManager);
void TetrisManager_MakeHold(TetrisManager* tetrisManager);

void splash(TetrisManager* tetrisManager, int blockType, int isSplash);  //// 안개 아이템 함수
void TetrisManager_randSpeed(TetrisManager* tetrisManager); //// 랜덤 속도 변환 아이템 함수

#endif