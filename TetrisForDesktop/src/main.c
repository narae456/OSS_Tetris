#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "Constant.h"
#include "TetrisView.h"
#include <time.h> ////헤더파일 추가
#include <stdlib.h> ////헤더파일 추가

int main(int argc, char* argv[]){
	TetrisView tetrisView;
	int processType;
	int direction;
	int mode; ///// 배경음악 on/off를 조절하기 위한 변수
	DWORD tickCount;
	int isSetMainMenu = False;
	
	time_t splashTimer = 0; //// 안개 아이템 시작 시간 ( 5초를 측정하기 위해 사용 )
	time_t curTimer = 0; //// 현재시간
	time_t gameStartTimer = 0;//// 게임시작시간을 구함

	int splashOnFlag = 0; //// 안개 실행 후 다시 실행 방지 
	int isSplash = 0; //// 0 : 정상, 1 : 안개, 2 : 원상태 복구
	mode=1; ///// 기본값 1. 1일 경우 배경음악 ON
	do{
		TetrisView_ProcessMainMenu(&tetrisView);
		switch (tetrisView.mainMenu){
		case START_MAIN_MENU:
			TetrisView_StartGame(&tetrisView);
			TetrisView_Bgm(&tetrisView, mode);
			tickCount = GetTickCount();
			gameStartTimer = time(NULL); //// 게임 실행 시간 구함( 단위 : 초 )
			tetrisView.tetrisManager.randSpeedTimer = 0; //// 변수사용을 위해 초기화

			while (True){
				processType = AUTO;
				direction = DOWN;
				isSetMainMenu = False;
				while (True){
					//// 현재 시간 구함( 단위 : 초 )
					curTimer = time(NULL);

					//// 현재시간이 게임 시작 후 60초가 지난 시간이면 안개 이벤트 시작
					if (curTimer - gameStartTimer > 60 && splashOnFlag == 0)
					{
						splashTimer = time(NULL);
						isSplash = 1;
						tetrisView.tetrisManager.isSplashMode = 1;
						splashOnFlag = 1;
					}
					//// splash 후 5초가 지났으면 원상복구
					if (splashTimer != 0 && curTimer - splashTimer >= 5)
					{
						isSplash = 0;
						splashTimer = 0;
						tetrisView.tetrisManager.isSplashMode = 0;
					}

					if (GetTickCount() - tickCount > TetrisView_GetDownMilliSecond(&tetrisView)){
						processType = AUTO;
						direction = DOWN;
						tickCount = GetTickCount();
						break;
					}
					if (_kbhit()){
						int key = _getch();
						if (key == 0xE0 && _kbhit()){
							key = _getch();
							if (key == UP_KEY_CODE){
								processType = DIRECTION;
								direction = UP;
								break;
							}
							if (key == DOWN_KEY_CODE){
								processType = DIRECTION;
								direction = DOWN;
								tickCount = GetTickCount();
								break;
							}
							if (key == LEFT_KEY_CODE){
								processType = DIRECTION;
								direction = LEFT;
								break;
							}
							if (key == RIGHT_KEY_CODE){
								processType = DIRECTION;
								direction = RIGHT;
								break;
							}
						}
						else{
							if (key == SPACE_BAR_KEY_CODE){
								processType = DIRECT_DOWN;
								tickCount = GetTickCount();
								break;
							}
							if (key == ESC_KEY_CODE){
								TetrisView_PauseGame(&tetrisView);
								if (tetrisView.pauseMenu == MAIN_MENU_PAUSE_MENU){
									isSetMainMenu = True;
									break;
								}
								processType = AUTO;
								tickCount = GetTickCount();
								break;
							}

							if (key == L_KEY_CODE){
								TetrisView_MakeHold(&tetrisView);
							}

							/*//아이템1 : 한 줄 제거
							if (key == P_KEY_CODE){
								TetrisView_Item_RemoveOneRow(&tetrisView);
							}

							//아이템2 : 두 줄 제거
							if (key == O_KEY_CODE){
								TetrisView_Item_RemoveTwoRow(&tetrisView);
							}

							//아이템3 : 전체 줄 제거
							if (key == I_KEY_CODE){
								TetrisView_Item_RemoveAllRow(&tetrisView);
							}*/

							if (key == ITEM_FIRST_CODE){  //첫번째 아이템 사용 Z
								TetrisManager_UseItem(&tetrisView, 1);
								break;
							}
							if (key == ITEM_SECOND_CODE){ //두번째 아이템 사용 X
								TetrisManager_UseItem(&tetrisView, 2);
								break;
							}
							if (key == ITEM_THRID_CODE){  //세번째 아이템 사용 C
								TetrisManager_UseItem(&tetrisView, 3);
								break;
							}
							if (key == ITEM_FOURTH_CODE){ //네번째 아이템 사용 V
								TetrisManager_UseItem(&tetrisView, 4);
								break;
							}

							//다음블럭과 다다음블럭 바꾸기
							if (key == A_KEY_CODE){
								TetrisView_ChangeNextBlock(&tetrisView);
							}
						}
						///// 숫자키 1을 누를 경우 mode=1. 배경음악 ON
						if (key == ONE_KEY_CODE){
							mode=1;
							TetrisView_Bgm(&tetrisView, mode);
							break;
						}
						///// 숫자키 2를 누를 경우 mode-2. 배경음악 OFF
						if (key == TWO_KEY_CODE){
							mode=2;
							TetrisView_Bgm(&tetrisView, mode);
							break;
						}
					}
				}

				//for selecting main menu at pause menu
				if (isSetMainMenu){
					break;
				}
				TetrisView_ProcessGame(&tetrisView, processType, direction, isSplash); //// isSplash 매개변수 추가
				if (tetrisView.endMenu == RANKING_END_MENU){
					TetrisView_AddRanking(&tetrisView);
					TetrisView_ShowRanking(&tetrisView);
					break;
				}
				else if (tetrisView.endMenu == MAIN_MENU_END_MENU){
					break;
				}
				else if (tetrisView.endMenu == EXIT_END_MENU){
					tetrisView.mainMenu = EXIT_MAIN_MENU;
					break;
				}
			}
			break;
		case RANKING_MAIN_MENU:
			TetrisView_ShowRanking(&tetrisView);
			break;
		case SETTING_MAIN_MENU:
			TetrisView_ShowSetting(&tetrisView);
			break;
		}
	} while (tetrisView.mainMenu != EXIT_MAIN_MENU);
	return 0;
}