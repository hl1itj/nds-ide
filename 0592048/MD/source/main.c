/******************** include부 ********************/
#include <PA9.h>        // PAlibrary의 헤더파일
#include "all_gfx.h"    // PAGfx를 이용해 가져온 이미지,스프라이트가 구조체로 담겨있다.
#include "all_gfx.c"    // PAGfx를 이용해 가져온 이미지,스프라이트가 구조체로 담겨있다.
#include "Startup.h"	// raw사운드파일 ; data폴더에 <사운드파일명>.raw를 두고, <사운드파일명>.raw
#include "trapcard.h"   // raw사운드파일

/******************** 매크로 선언부 ********************/
/* 스크린 인덱스 매크로 */
#define UP_SCREEN 1
#define DOWN_SCREEN 0

/* 배경그림  '층' 인덱스 매크로 */
#define BG0   0
#define BG1   1
#define BG2   2
#define BG3   3

/* 스프라이트 팔레트 인덱스 매크로 */
#define NPAL  0 // 넘버 스프라이트 팔레트
#define IPAL  1 // 아이템 스프라이트 팔레트

/* scr_Season의 반환값 */
#define GO_SHOP 0 // 상점
#define GO_NEXT 1 // 다음스테이지

/* scr_Game의 반환값 */
#define GAMEOVER 0 // 종료
#define GAMEPLAY 1 // 계속

/* 학기 구분 매크로*/
#define SEASON_MAX 8 // 총학기 수
#define SEASON_1ST 1
#define SEASON_2ND 2
#define SEASON_3RD 3
#define SEASON_4TH 4
#define SEASON_5TH 5
#define SEASON_6TH 6
#define SEASON_7TH 7
#define SEASON_8TH 8

#define SIZE      6    /* 블록 배열의 행,열 사이즈 */
#define NUM_OF_BLOCK 4 /* 블락 종류 최고 값( 0~4 ) */

/* 블락 종류 매크로 */
#define BLK_CASH  0 // 돈블락
#define BLK_HWANG 1
#define BLK_CHANG 2
#define BLK_MIN   3
#define BLK_DONG  4

/* boolean형 매크로 */
#define BOOL        int
#define TRUE		1
#define	FALSE		0

/* 블락상쇄 알고리즘 사용 매크로 */
/* destroy배열에서 사용 */
#define KILL        -1		// 상쇄블락식별
#define MONEYPLUS   -2		// 돈블락식별
/* blocks배열에서 사용 */
#define TARGET		1000	// 상쇄될 블록 표시

/* 아이템 정의 */
#define ITEM_BOMB_ROW	1	// 가로 한 줄 삭제
#define ITEM_BOMB_COL	2	// 세로 한 줄 삭제
#define ITEM_BOMB_SMALL	3	// 선택 블록 기준 9칸 삭제
#define ITEM_BOMB_BIG	4	// 선택 블록 기준 25칸 삭제
#define ITEM_TIME		5	// 시간 연장
#define ITEM_RELOCATE	6	// 모든 블록 재배치 (점수 반영은 안됨)
/* 아이템 가격 */
#define ITEM_PRICE_ROW      2000
#define ITEM_PRICE_COL      2000
#define ITEM_PRICE_SMALL    4000
#define ITEM_PRICE_BIG      6000
#define ITEM_PRICE_TIME     6000
#define ITEM_PRICE_RELOCATE 6000

/* 아이템  매크로 */
#define USER_ITEM_MAX 5        // 사용자가 보유할 수 있는 최대 아이템 갯수
#define NUM_OF_ITEMTYPE 6      // 아이템 종류의 수.
#define ITEM_FIRST_SPRITE 36   // 아이템스프라이트의 기준 스프라이트 인덱스.


// 유저정보 구조체 정의
struct userInfo {	// 게임이 진행되는 동안 지속적으로 갱신.
	int userMoney;  // 현재 돈.
	int userScore;  // 현재 스코어.
	int userStage;	// 현재 스테이지.
	int userItem[USER_ITEM_MAX]; // 소지 아이템 배열.
	int activeBlockCol;	// 활성화된 블록의 col인덱스
	int activeBlockRow; // 활성화된 블록의 row인덱스
	int activeItem;		// 활성화된 아이템의 인덱스
};


/******************** 함수 선언부 ********************/
/* 기본적인 PAlibrary의 화면 초기화 함수 */
void UnLoad_Screen();

/* 유저정보 초기화 */
void user_init();

/* 상태별 함수 */
void scr_Main();      // 메인
int scr_Game();       // 게임  _ return GAMEOVER or GAMEPLAY
int scr_Season();     // 분기점 _ return GO_SHOP or GO_NEXT
void scr_Shop();      // 상점
void scr_Gameover();  // 종료

/* 블락터치인식 */
int gettouchcol(int x); // 터치 입력이 이루어진 곳의 블락 열 인덱스를 반환.
int gettouchrow(int y); // 터치 입력이 이루어진 곳의 블락 행 인덱스를 반환.

/* 화면 갱신 함수 */
void stage_init(int ustage); // 스테이지를 갱신.
void block_init();           // 블락을 초기화.
void block_reload();         // 블락을 갱신.
void item_init();            // 아이템 초기화 및 갱신.

/* 블록 함수 */
BOOL isNearBlock(int row, int col, int oldrow, int oldcol); // 두 행열의 쌍이 상하좌우 중 특정위치에 근접해 있으면 TRUE를 리턴, 아니면 FALSE를 리턴.
void swapBlock(int row, int col, int oldrow, int oldcol);   // 두 행열의 쌍이 가리키는, 두 블락의 값을 바꾼다.
void swapsprite(int row, int col, int oldrow, int oldcol,int oldsprite,int newsprite); // 두 행열의 쌍이 가리키는, 두 스프라이트의 위치를 바꾼다.
BOOL blocksCrack(int combo); /* 상쇄될 블록그룹 체크 */
int checkRow(int row);/* 행 체크 */
int checkCol(int col);/* 열 체크 */

/* 아이템 함수 */
int itemgettouchrow(int y);                        // 터치 입력이 이루어진 곳의 아이템 인덱스를 반환.
void useOneClickItem(int item);                    // 목표 지정이 필요없는 아이템 사용.
BOOL useTwoClickItem(int item, int row, int col);  // 목표 지정이 필요한 아이템 사용.
int getitemindex();                                // 비어있는 사용자의 아이템 배열 인덱스를 반환.


/******************** 전역변수 선언부 ********************/
static int stageScore;   //다음스테이지로 넘어갈 수 있는 만족 값
static int stageTime;	 //스테이지 총 시간은 스테이지별 다름

struct userInfo uinfo;   // 사용자 정보 객체 생성.

int blocks[SIZE][SIZE];				// 블록 배열
int destroy[SIZE][SIZE];			// 파괴될 블록 정보 배열
int Blksprite[SIZE][SIZE];          // 블록 스프라이트 배열
static int stageTime = 60;		    // 스테이지  제한시간
static int restTime = 60;		    // 남은 시간

BOOL newGame=1;                     // 새로운 게임여부
BOOL isplaying = FALSE;             // 배경음악 실행여부


//Main...
int main(void){
    /* PALibrary초기화(주의!PALibrary에서 사용하는 변수들도 초기화된다.) */
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	// 사운드 환경 초기화
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);

	// 사운드 재생.AS_SoundDefaultPlay((u8*)<사운드파일명>, (u32)<사운드파일명>_size, 127, 64, <루프 여부>, 0);
	AS_SoundDefaultPlay((u8*)Startup, (u32)Startup_size, 127, 64, false, 0);

	/* 프로그램 루프 */
	while(1){ // Infinite loop


		user_init();                            // 유저정보 초기화

		scr_Main();			                    // 메인
		while ( scr_Game() != GAMEOVER ) {		// 게임
			while ( scr_Season() == GO_SHOP ) 	// 학기전환분기점
				scr_Shop();                     // 상점
		}
		scr_Gameover();                         // 종료

		PA_WaitForVBL();	                    // PALibraby 함수 _ 화면출력을 60fps로 동기화한다.
	}

	UnLoad_Screen();
	return 0;
}

/* 게임메인 */
void scr_Main() {
    /* 화면초기화  및 갱신 */
	UnLoad_Screen();
	PA_Init();
	PA_InitVBL();
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);			// 상단 디스플레이-타이틀화면 설정.
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Title_Down);	// 하단 디스플레이-타이틀화면 설정.

	while (1) {
		if ( Stylus.Newpress ) {
			return;                                 // 터치 입력이 있으면 scr_Main을 빠져나감.
		}
		PA_WaitForVBL();
	}
}

/* 게임 */
int scr_Game() {

	// 블락 핸들링 변수들...
	BOOL blockchanged = TRUE;
	int col = -1;
	int row = -1;
	int i, j;
	int combo = 1;
	int newNum = 0;
	int ScoreCount = 0;
	int moneyCount = 0;

	// 선택 아이템 인덱스 변수
	int select = -1;

	/* 타이머 변수_초. */
	int lastcount = 0;      // 이전 초.
	int currentcount = 0;   // 현재 초.

	/* busy-waiting 카운트 변수*/
	int waitcount = 0;

	/* PALibrary 초기화 및 화면 초기화,갱신*/
	UnLoad_Screen();
	PA_Init();
	PA_InitVBL();
    /* 텍스트 출력 환경 초기화 */
	PA_InitText(UP_SCREEN, BG2);
	PA_SetTextTileCol(UP_SCREEN, TEXT_BLACK);
    /* 상하단 디스플레이 출력 */
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);		// 타이틀 디스플레이
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Play_Down);	// 플레이 디스플레이
	/* 스프라이트 팔레트 생성 */
	PA_LoadSpritePal(DOWN_SCREEN, NPAL, (void*)number_Pal); // 블락 스프라이트 팔레트.
	PA_LoadSpritePal(DOWN_SCREEN, IPAL, (void*)item_Pal);   // 아이템 스프라이트 팔레트.

	stage_init(uinfo.userStage); // 스테이지 갱신
	block_init();                // 블락 및 블락 출력 초기화
	item_init();                 // 아이템 및 아이템 출력 갱신

	/* 현재 정보 출력 */
	PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
	PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
	PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
	PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);

	/* 카운터 초기화 및 시작 */
	PA_VBLCountersReset(); // 카운터 초기화.
	PA_VBLCounterStart(0); // 카운터 시작.

	/* 배경음악 플레이 */
	if ( isplaying==FALSE ) {
		AS_SoundDefaultPlay((u8*)trapcard, (u32)trapcard_size, 127, 64, true, 0);
		isplaying = TRUE;
	}

	while (1) {
		/* 카운터 값에 따라 현재 남은 시간(restTime)을 변경,갱신 */
		currentcount = (PA_VBLCounter[0]/60);
		if ( lastcount!=currentcount ) {
			restTime = restTime - ( currentcount-lastcount );
			lastcount = currentcount;
		}
		/* 변경된 정보를 출력 */
		PA_ClearTextBg(UP_SCREEN);
		PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
		PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);
		PA_OutputText(UP_SCREEN, 5, 17, "Mission Time  : %d", stageTime);
		PA_OutputText(UP_SCREEN, 5, 19, "Rest Time  : %d", restTime);

		/* 입력에 따른 핸들링 */
		if( Stylus.Newpress ){
			/******************** PAUSE ********************/
			if ( (Stylus.X>=214&&Stylus.X<=241)&&(Stylus.Y>=164&&Stylus.Y<=172) ) {// 화면상 pause 터치.
				/* PALibrary 초기화 및 화면초기화,갱신 */
				PA_Init();
				PA_InitVBL();
				UnLoad_Screen();
				PA_EasyBgLoad(UP_SCREEN, BG3, Title);
				PA_EasyBgLoad(DOWN_SCREEN, BG3, Pause_Down);	// Pause화면 하단 화면 디스플레이

				/* 선택된 요소 변수 초기화 */
				uinfo.activeBlockRow = -1;
				uinfo.activeBlockCol = -1;
				col = -1;
				row = -1;
				select = -1;
				uinfo.activeItem = -1;

				while( 1 ){

					if ( Stylus.Newpress ) { // 터치  가 이루어지면 PAUSE상태를 벗어남
						/* PALibrary 초기화 및 화면초기화,갱신 */
						UnLoad_Screen();
						PA_InitText(UP_SCREEN, BG2);
						PA_SetTextTileCol(UP_SCREEN, TEXT_BLACK);
						PA_EasyBgLoad(UP_SCREEN, BG3, Title);
						PA_EasyBgLoad(DOWN_SCREEN, BG3, Play_Down);	// Play화면 하단 화면 디스플레이
						/* 현재 정보 출력 */
						PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
						PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
						PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);

						/* 블락 및 아이템 출력 갱신 */
						block_reload();
						PA_LoadSpritePal(DOWN_SCREEN, IPAL, (void*)item_Pal); //스프라이트 팔레트 생성.
						item_init();

						/* 카운터 초기화 */
						lastcount = 0;
						currentcount = 0;
						PA_VBLCountersReset();
						PA_VBLCounterStart(0); // 카운터 시작.

						break;

					}

					PA_WaitForVBL();
				}

			/******************** EXIT ********************/
			} else if( (Stylus.X>=209&&Stylus.X<=240)&&(Stylus.Y>=175&&Stylus.Y<=183) ) {		//메인으로
				return GAMEOVER;

			/******************** BLOCK ********************/
			} else if ( (Stylus.X>=0&&Stylus.X<=192)&&(Stylus.Y>=0&&Stylus.Y<=192) ) {		//블록처리
				col = gettouchcol(Stylus.X);
				row = gettouchrow(Stylus.Y);

				/* 선택해지 */
				if( (row==uinfo.activeBlockRow)&&(col==uinfo.activeBlockCol) ){

					uinfo.activeBlockRow = -1;
					uinfo.activeBlockCol = -1;
					PA_SetSpriteAnim( DOWN_SCREEN, Blksprite[row][col], (PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[row][col])-(NUM_OF_BLOCK+1)) ); // 스프라이트 갱신.
				}

				/* 위치변경 */
				else if( ((uinfo.activeBlockRow != -1) && (uinfo.activeBlockCol != -1)) &&
						((row != uinfo.activeBlockRow) || (col != uinfo.activeBlockCol)) )
				{
					if ( isNearBlock(row, col, uinfo.activeBlockRow, uinfo.activeBlockCol) ) {
						PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol], PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol])-5);
						swapBlock(row, col, uinfo.activeBlockRow, uinfo.activeBlockCol);		// 블락 스프라이트 위치 교환.

						blockchanged = TRUE;
						uinfo.activeBlockRow = -1;
						uinfo.activeBlockCol = -1;
					} else {
						; // 필요시 에러음 구현요망.
					}
				}

				/* 아이템 누르고 블록을 눌렀을때 처리. */
				else if (select != -1) {
					blockchanged = useTwoClickItem(uinfo.userItem[uinfo.activeItem],row,col);			// 활성화된 아이템 사용
					PA_SetSpriteAnim(DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, 0); // 아이템란 비움 그래픽 효과
					uinfo.userItem[uinfo.activeItem] = -1;						// 아이템란을 비움
					uinfo.activeItem = -1;										// 비활성화 처리
					uinfo.activeBlockCol = -1;									// 블록 비활성화 처리
					uinfo.activeBlockRow = -1;
					select = -1;                                                // 선택아이템 인덱스 초기화
				}

				/* 블락 선택 */
				else {
					uinfo.activeBlockRow = row;													//	블록 활성화 (그래픽 효과도 넣어야 함)
					uinfo.activeBlockCol = col;
					PA_SetSpriteAnim( DOWN_SCREEN, Blksprite[row][col], (PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[row][col])+(NUM_OF_BLOCK+1)) );
				}

			/******************** ITEM ********************/
			} else if ( (Stylus.X>=224&&Stylus.X<=256)&&(Stylus.Y>=0&&Stylus.Y<=160) ) {
				/* 블락이 선택되어 있을시, 초기화 */
				if ( uinfo.activeBlockRow!=-1 ) {
					PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol], PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol])-5);
					uinfo.activeBlockRow = -1;
					uinfo.activeBlockCol = -1;
				}
				/* 선택 아이템 인덱스 갱신 */
				select = itemgettouchrow(Stylus.Y);

				if (uinfo.userItem[select] != -1) { // 터치한 아이템란에 아이템이 있을때
					/* 선택취소 */
					if (uinfo.activeItem == select)	{
						PA_SetSpriteAnim( DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, (PA_GetSpriteAnimFrame(DOWN_SCREEN,ITEM_FIRST_SPRITE+uinfo.activeItem)-NUM_OF_ITEMTYPE) ); // 비활성화 그래픽 효과.
						uinfo.activeItem = -1;
					}
					/* 효과 즉시 발동 아이템 선택 */
					else if ((uinfo.userItem[select] >= 5) && (uinfo.userItem[select] <= 6)) {	// 선택한 아이템이 즉시 사용되는 것이면!!합칠 때 적절하게 바꿔야 함
						useOneClickItem(uinfo.userItem[select]);	// 아이템 사용
						PA_SetSpriteAnim(DOWN_SCREEN, ITEM_FIRST_SPRITE+select, 0); // 아이템란을 비움 그래픽 효과
						uinfo.userItem[select] = -1;				// 아이템란을 비움 (그래픽 효과 필요)
						uinfo.activeItem = -1;						// 비활성화 처리 (그래픽 효과 필요)
						uinfo.activeBlockCol = -1;					// 블록 비활성화 처리 (그래픽 효과 필요?)
						uinfo.activeBlockRow = -1;
						select = -1;
					/* 블락선택 필요 아이템 선택*/
					} else	{
						if ( uinfo.activeItem!=-1 ) { // 이전에 선택된 아이템이 있으면 비활성화.
							PA_SetSpriteAnim(DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, PA_GetSpriteAnimFrame(DOWN_SCREEN,ITEM_FIRST_SPRITE+uinfo.activeItem)-NUM_OF_ITEMTYPE); // 비활성화 그래픽 효과
						}
						uinfo.activeItem = select; // 현재 선택 아이템 활성화
						PA_SetSpriteAnim( DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, (uinfo.userItem[uinfo.activeItem]+NUM_OF_ITEMTYPE) ); // 그래픽 효과
					}
				}

			}
		} // ~ 터치처리 끝.


		/* 블락 위치변경시 블락 연산 */
		if ( blockchanged==TRUE ){
			/* 상쇄대상 블락 연산 */
			while ( blocksCrack(combo) == TRUE ) {

				for (i=0; i<SIZE; i++) {		// 상쇄될 블락 표시
					for (j=0; j<SIZE; j++) {
						if (destroy[i][j] == KILL) {
							blocks[i][j] = TARGET;
							ScoreCount++;
						}
						if (destroy[i][j] == MONEYPLUS) {
							blocks[i][j] = TARGET;
							moneyCount++;
						}
					}
				}

				uinfo.userMoney += 100 * moneyCount * combo;	// 돈 계산
				PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney); // 변경된 돈 갱신.

				// 블록상쇄효과를 줌.
				for (i=0; i<SIZE; i++) {		// 새로운 블락 생성
					for (j=0; j<SIZE; j++) {
						if (blocks[i][j]  == TARGET) {
							PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][j], 10, 14, 15, ANIM_LOOP, 1);
						}
					}
				}

				// 변경된 스코어 갱신.
				PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);

				// 애니메이션 효과가 끝나길 기다림
				while (1) {
					waitcount++;
					if (waitcount==25) {
						waitcount=0;
						break;
					}
					PA_WaitForVBL();
				}

				/* 새로운 블락 값 지정과 함께 스프라이트 갱신. */
				for (i=0; i<SIZE; i++) {		// 새로운 블락 값 갱신
					for (j=0; j<SIZE; j++) {
						if (blocks[i][j]  == TARGET) {
							newNum = PA_RandMax(NUM_OF_BLOCK);
							blocks[i][j] = newNum;
							PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]);
						}
					}
				}
				// 콤보 발동전 시각적으로 확인할 수 있는 시간을 줌.
				while (1) {
					waitcount++;
					if (waitcount==20) {
						waitcount=0;
						break;
					}
					PA_WaitForVBL();
				}

				/* 사용된 변수 초기화 */
				for(i=0; i<SIZE; i++) {
					for(j=0; j<SIZE; j++) {
						destroy[i][j] = 0; // destroy 배열 초기화
					}
				}
				combo++;
				ScoreCount = 0;
				moneyCount = 0;
			}

			/* 콤보 및 블락위치변경 변수 초기화 */
			combo = 1;
			blockchanged = FALSE;
		}

		if(uinfo.userScore >= stageScore){ //단계에 지정된 스코어 값을 충족시키면 다음 스테이지
			(uinfo.userStage)++;
			return GAMEPLAY;
		}

		if( restTime == 0 ) {	  // 시간소모에 따른 게임 종료
			return GAMEOVER;
		}

		PA_WaitForVBL();
	}
}

/* 분기점 */
int scr_Season() {

	PA_Init();
	PA_InitVBL();
	UnLoad_Screen();
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Season);	/* 종료스크린 출력*/

	while (1) {
		if ( Stylus.Newpress ) {
			// shop
			if ( (Stylus.X>=21&&Stylus.X<=140)&&(Stylus.Y>=42&&Stylus.Y<=75) ) {
				return GO_SHOP;
			// next
			} else if ( (Stylus.X>=116&&Stylus.X<=231)&&(Stylus.Y>=131&&Stylus.Y<=164) ) {
				return GO_NEXT;
			}
		}
		PA_WaitForVBL();
	}
}

/* 상점 */
void scr_Shop() {
	int itemindex = -1;
	int selectItem = -1;

	PA_Init();
	PA_InitVBL();
	UnLoad_Screen();
	PA_InitText(UP_SCREEN, BG2);
	PA_SetTextTileCol(UP_SCREEN, TEXT_BLACK);
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Shop);
	PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
	PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
	PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
	PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);
	/* 사용자 아이템란 출력 */
	PA_LoadSpritePal(DOWN_SCREEN, IPAL, (void*)item_Pal); //스프라이트 팔레트 생성.
	item_init();

	while (1) {
		/* 아이템 구매 */
		if ( Stylus.Newpress ) {
			// Row Bomb
			if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=25&&Stylus.Y<=46) ) {
				selectItem = ITEM_BOMB_ROW;
			// Col Bomb
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=48&&Stylus.Y<=69) ) {
				selectItem = ITEM_BOMB_COL;
			// Small Bomb
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=73&&Stylus.Y<=92) ) {
				selectItem = ITEM_BOMB_SMALL;
			// Big Bomb
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=96&&Stylus.Y<=116) ) {
				selectItem = ITEM_BOMB_BIG;
			// Time ++
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=120&&Stylus.Y<=142) ) {
				selectItem = ITEM_TIME;
			// New Blocks
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=144&&Stylus.Y<=164) ) {
				selectItem = ITEM_RELOCATE;
			// Exit
			} else if ( (Stylus.X>=218&&Stylus.X<=242)&&(Stylus.Y>=171&&Stylus.Y<=181) ) {
				return;
			}

			itemindex = getitemindex(); // 빈 아이템 란의 인덱스를 구한다.

			if ( itemindex!=-1 ) {			// 빈 아이템 란이 존재하면
				// 아이템 선택.
				switch ( selectItem ) {
				case ITEM_BOMB_ROW:
					if ( uinfo.userMoney < ITEM_PRICE_ROW ) { // 사용자가 충분한 돈을 가지고 있지않으면.
						; // 필요시 에러음 구현요망.
						selectItem = -1; // 아이템 선택 취소.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_ROW;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_BOMB_COL:
					if ( uinfo.userMoney < ITEM_PRICE_COL ) { // 사용자가 충분한 돈을 가지고 있지않으면.
						; // 필요시 에러음 구현요망.
						selectItem = -1; // 아이템 선택 취소.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_COL;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_BOMB_SMALL:
					if ( uinfo.userMoney < ITEM_PRICE_SMALL ) { // 사용자가 충분한 돈을 가지고 있지않으면.
						; // 필요시 에러음 구현요망.
						selectItem = -1; // 아이템 선택 취소.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_SMALL;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_BOMB_BIG:
					if ( uinfo.userMoney < ITEM_PRICE_BIG ) { // 사용자가 충분한 돈을 가지고 있지않으면.
						; // 필요시 에러음 구현요망.
						selectItem = -1; // 아이템 선택 취소.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_BIG;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_TIME:
					if ( uinfo.userMoney < ITEM_PRICE_TIME ) { // 사용자가 충분한 돈을 가지고 있지않으면.
						; // 필요시 에러음 구현요망.
						selectItem = -1; // 아이템 선택 취소.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_TIME;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_RELOCATE:
					if ( uinfo.userMoney < ITEM_PRICE_RELOCATE ) { // 사용자가 충분한 돈을 가지고 있지않으면.
						; // 필요시 에러음 구현요망.
						selectItem = -1; // 아이템 선택 취소.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_RELOCATE;
					}
					break;
				default:
					break;
				}

				/* 아이템 구매 */
				if ( selectItem!=-1 ) {
					uinfo.userItem[itemindex] = selectItem;
					item_init();
					PA_ClearTextBg(UP_SCREEN);
					PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
					PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
					PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);
					selectItem = -1;

				// 살 돈이 부족한 경우.
				} else {
					; // 필요시 에러음 구현요망.
				}

			// 아이템 살 공간이 부족할 경우.
			} else {
				; // 필요시 에러음 구현요망.
			}
		}
		PA_WaitForVBL();
	}

}

/* 게임 오버 */
void scr_Gameover()
{
	PA_Init();
	PA_InitVBL();
	UnLoad_Screen();
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Gameover);	/* 종료스크린 출력*/

	while (1) {
		if ( Stylus.Newpress ) {
			newGame = TRUE; // 새로운 게임.
			return;
		}
		PA_WaitForVBL(); // while문 안에 꼭 있어야함. 중요!
	}
}

/* 스테이지 별 초기화 */
void stage_init(int ustage) {
	switch(ustage) {
		case SEASON_1ST:
			stageTime=300;					//스테이지 시간 초기화
			stageScore=1500;				//스테이지 스코어 초기화
			break;
		case SEASON_2ND:
			stageTime=270;					//스테이지 시간 초기화
			stageScore=3000;				//스테이지 스코어 초기화
			break;
		case SEASON_3RD:
			stageTime=240;					//스테이지 시간 초기화
			stageScore=5000;					//스테이지 스코어 초기화
			break;
		case SEASON_4TH:
			stageTime=210;					//스테이지 시간 초기화
			stageScore=8000;					//스테이지 스코어 초기화
			break;
		case SEASON_5TH:
			stageTime=180;					//스테이지 시간 초기화
			stageScore=12000;					//스테이지 스코어 초기화
			break;
		case SEASON_6TH:
			stageTime=150;					//스테이지 시간 초기화
			stageScore=15000;					//스테이지 스코어 초기화
			break;
		case SEASON_7TH:
			stageTime=120;					//스테이지 시간 초기화
			stageScore=20000;					//스테이지 스코어 초기화
			break;
		case SEASON_8TH:
			stageTime=90;					//스테이지 시간 초기화
			stageScore=25000;					//스테이지 스코어 초기화
			break;
	}

	restTime = stageTime; // 남은시간 갱신.
}

/* 터치 입력한 블락의 열 인덱스를 반환.*/
int gettouchcol(int x) {

	if ( (x>=0)&&(x<=32) ) {
		return 0;

	} else if ( (x>=33)&&(x<=64) ) {
		return 1;

	} else if ( (x>=65)&&(x<=96) ) {
		return 2;

	} else if ( (x>=97)&&(x<=128) ) {
		return 3;

	} else if ( (x>=129)&&(x<=160) ) {
		return 4;

	} else if ( (x>=161)&&(x<=192) ) {
		return 5;

	} else {
		return -1;
	}
}

/* 터치 입력한 블락의 행 인덱스를 반환.*/
int gettouchrow(int y) {

	if ( (y>=0)&&(y<=32) ) {
		return 0;

	} else if ( (y>=33)&&(y<=64) ) {
		return 1;

	} else if ( (y>=65)&&(y<=96) ) {
		return 2;

	} else if ( (y>=97)&&(y<=128) ) {
		return 3;

	} else if ( (y>=129)&&(y<=160) ) {
		return 4;

	} else if ( (y>=161)&&(y<=192) ) {
		return 5;

	} else {
		return -1;
	}
}

/* 화면초기화 함수. 비디오메모리를 초기화한다. */
void UnLoad_Screen()
{
	int i;
	for(i=0; i<2; ++i)
	{
		PA_ResetBgSysScreen(i);
		PA_ResetSpriteSysScreen(i);
	}
}

/* 두 블록이 인접한지 판별 */
BOOL isNearBlock(int row, int col, int oldrow, int oldcol)
{
	/* 블록이 인접한 경우는 상하좌우 4가지 임 */
	if ( ((col == oldcol) && (row+1 == oldrow)) ||	// 아래쪽 인접
		 ((col == oldcol) && (row-1 == oldrow)) ||	// 위쪽 인접
		 ((col-1 == oldcol) && (row == oldrow)) ||	// 오른쪽 인접
		 ((col+1 == oldcol) && (row == oldrow)) )		// 왼쪽 인접
		return TRUE;
	else
		return FALSE;

}

/* 두 블록의 값 바꿈*/
void swapBlock(int row, int col, int oldrow, int oldcol)
{
	int temp;


	temp = blocks[row][col];
	blocks[row][col] = blocks[oldrow][oldcol];
	blocks[oldrow][oldcol] = temp;

	// 블록 스프라이트 위치 변경
	swapsprite(row,col,oldrow,oldcol,Blksprite[oldrow][oldcol], Blksprite[row][col]);
}

/* 두 블록의 스프라이트 바꿈*/
void swapsprite(int row, int col, int oldrow, int oldcol,int oldsprite,int newsprite) {
	int temp_sprite; // 스프라이트 넘버 임시 저장변수.
	int oldX = PA_GetSpriteX(DOWN_SCREEN,oldsprite);
	int oldY = PA_GetSpriteY(DOWN_SCREEN,oldsprite);
	int newX = PA_GetSpriteX(DOWN_SCREEN,newsprite);
	int newY = PA_GetSpriteY(DOWN_SCREEN,newsprite);

	// 블록 스프라이트 배열 갱신.
	temp_sprite = Blksprite[row][col];
	Blksprite[row][col] = Blksprite[oldrow][oldcol];
	Blksprite[oldrow][oldcol] = temp_sprite;

	PA_SetSpriteXY(DOWN_SCREEN,oldsprite,newX,newY);
	PA_SetSpriteXY(DOWN_SCREEN,newsprite,oldX,oldY);
}

/* 아이템 초기화 및 갱신 */
void item_init() {
	int i;
	int itemspriteindex = ITEM_FIRST_SPRITE;

	if ( newGame==TRUE ) {
		for ( i=0;i<(USER_ITEM_MAX);i++ ) {
			uinfo.userItem[i]=i+1; 			// 시작 아이템 설정.
			PA_CreateSprite(DOWN_SCREEN, itemspriteindex,(void*)item_Sprite, OBJ_SIZE_32X32,1,IPAL, 224, (i)*32);
			PA_SetSpriteAnim(DOWN_SCREEN, itemspriteindex, uinfo.userItem[i]); // 블록배열의 값을 스프라이트에 반영.
			itemspriteindex++;
		}
		newGame = FALSE;
	} else {
		for ( i=0;i<(USER_ITEM_MAX);i++ ) {
			PA_CreateSprite(DOWN_SCREEN, itemspriteindex,(void*)item_Sprite, OBJ_SIZE_32X32,1,IPAL, 224, (i)*32);
			if ( uinfo.userItem[i]==-1 ) {
				PA_SetSpriteAnim(DOWN_SCREEN, itemspriteindex, 0); // 블록배열의 값을 스프라이트에 반영.
			} else {
				PA_SetSpriteAnim(DOWN_SCREEN, itemspriteindex, uinfo.userItem[i]); // 블록배열의 값을 스프라이트에 반영.
			}
			itemspriteindex++;
		}
	}
}

/* 블락 초기화 */
void block_init()
{
	int i, j;
	int randnum;
	int k = 0; // 스프라이트 넘버.

	for ( i=0;i<SIZE;i++ ) {
		for ( j=0;j<SIZE;j++ ){
			randnum = PA_RandMax(NUM_OF_BLOCK);
			blocks[j][i] = randnum;
			PA_CreateSprite(DOWN_SCREEN, k,(void*)number_Sprite, OBJ_SIZE_32X32,1,NPAL, i*32, j*32);
			Blksprite[j][i] = k; // 해당 블락스프라이트정보배열 값  초기화
			PA_SetSpriteAnim(DOWN_SCREEN, k, blocks[j][i]); // 블록배열의 값을 스프라이트에 반영.
			k++;
		}
	}
}

/* 블락 스프라이트 갱신*/
void block_reload()
{
	int i, j;
	int k = 0; // 스프라이트 넘버.

	PA_LoadSpritePal(DOWN_SCREEN, NPAL, (void*)number_Pal); //스프라이트 팔레트 생성.

	for ( i=0;i<SIZE;i++ ) {
		for ( j=0;j<SIZE;j++ ){
			PA_CreateSprite(DOWN_SCREEN, k,(void*)number_Sprite, OBJ_SIZE_32X32,1,NPAL, i*32, j*32);
			Blksprite[j][i] = k; // 해당 블락스프라이트정보배열 값  초기화
			PA_SetSpriteAnim(DOWN_SCREEN, k, blocks[j][i]); // 블록배열의 값을 스프라이트에 반영.
			k++;
		}
	}
}


/* 행 체크 */
int checkRow(int row)
{
	int base;
	int i, j;
	int match = 0;
	int count = 0;
	int checkcount = 0;

	for(i=0; i<(SIZE-2); i++) {
		match = 0;
		base = blocks[row][i];
		for(j=i+1; j<SIZE; j++) {
			if (base == blocks[row][j]) {
				match++;

				if (match == 2) {
					if(blocks[row][j] == BLK_CASH){
						destroy[row][j-2] = MONEYPLUS;
						destroy[row][j-1] = MONEYPLUS;
						destroy[row][j] = MONEYPLUS;
					}else{
						destroy[row][j-2] = KILL;
						destroy[row][j-1] = KILL;
						destroy[row][j] = KILL;
						checkcount++;
					}
				}

				if (match >= 3){
					if(blocks[row][j] == BLK_CASH){
						destroy[row][j] = MONEYPLUS;
					}else{
						destroy[row][j] = KILL;
					}
				}

			} else
				break;
		}
	}

	for(i=0; i<SIZE; i++) {
		if ((destroy[row][i] == KILL) && (checkcount != 0))
			count++;
	}

	return count;
}

/* 열 체크 */
int checkCol(int col)
{
	int base;
	int i, j;
	int match = 0;
	int count = 0;
	int checkcount = 0;

	for(i=0; i<(SIZE-2); i++) {
		match = 0;
		base = blocks[i][col];
		for(j=i+1; j<SIZE; j++) {
			if (base == blocks[j][col]) {
				match++;

				if (match == 2) {
					if(blocks[j][col] == BLK_CASH){
						destroy[j-2][col] = MONEYPLUS;
						destroy[j-1][col] = MONEYPLUS;
						destroy[j][col] = MONEYPLUS;
					}
					else{
						destroy[j-2][col] = KILL;
						destroy[j-1][col] = KILL;
						destroy[j][col] = KILL;
						checkcount++;
					}
				}

				if (match >= 3){
					if(blocks[j][col] == BLK_CASH){
						destroy[j][col] = MONEYPLUS;
					}else{
						destroy[j][col] = KILL;
					}
				}

			} else
				break;
		}
	}
	for(i=0; i<SIZE; i++) {
		if ((destroy[i][col] == KILL) && (checkcount != 0))
			count++;
	}

	return count;
}

/* 상쇄될 블록그룹 체크 */
BOOL blocksCrack(int combo) {
	int i, j, temp;

	for(i=0; i<SIZE; i++){
		/* 행 처리 */
		temp = checkRow(i);
		if (temp == 3)										// 상쇄 블록 수 만큼 점수 반영
			uinfo.userScore += 50 * combo;
		else if (temp == 4)
			uinfo.userScore += 70 * combo;
		else if (temp == 5)
			uinfo.userScore += 100 * combo;
		else if (temp == 6)
			uinfo.userScore += 200 * combo;

		/* 열 처리 */
		temp = checkCol(i);
		if (temp == 3)										// 상쇄 블록 수 만큼 점수 반영
			uinfo.userScore += 50 * combo;
		else if (temp == 4)
			uinfo.userScore += 70 * combo;
		else if (temp == 5)
			uinfo.userScore += 100 * combo;
		else if (temp == 6)
			uinfo.userScore += 200 * combo;
	}

	for (i=0; i<SIZE; i++) {
		for (j=0; j<SIZE; j++) {
			if (destroy[i][j] == KILL) {
				return TRUE;
			}
			else if (destroy[i][j] == MONEYPLUS) {
				return TRUE;
			}
		}
	}
	return FALSE;
}


/* 선택된 아이템란 인덱스 반환 */
int itemgettouchrow(int y) {


	if ( (y>=0)&&(y<=32) ) {
		return 0;

	} else if ( (y>=33)&&(y<=64) ) {
		return 1;

	} else if ( (y>=65)&&(y<=96) ) {
		return 2;

	} else if ( (y>=97)&&(y<=128) ) {
		return 3;

	} else if ( (y>=129)&&(y<=160) ) {
		return 4;

	} else {
		return -1;
	}
}

/* 즉시 발동 아이템 */
void useOneClickItem(int item)
{
	int i, j;

	switch (item) {
	case ITEM_TIME:
		restTime = restTime + 20;		// 20초 연장
		if (restTime > stageTime)
			restTime = stageTime;

		break;

	case ITEM_RELOCATE:					// 블록 재배치
		for (i=0; i<6; i++) {
			for (j=0; j<6; j++) {
				blocks[i][j] = PA_RandMax(NUM_OF_BLOCK);	// 새로운 블락 값 갱신
				PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]); // 블록배열의 값을 스프라이트에 반영.
			}
		}
		break;

	default :
		return;
	}
}

/* 블락 선택 요구 아이템  */
BOOL useTwoClickItem(int item, int row, int col)
{
	int i, j;
	int targetRow = row;
	int targetCol = col;
	int waitcount=0;

	/* 선택된 블록을 기준으로 효과적용 */
	switch (item) {
	/* 가로 한 줄 삭제 */
	case ITEM_BOMB_ROW:
		// 블록  상쇄 그래픽 효과
		for (i=0; i<SIZE; i++) {
			PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[targetRow][i], 10, 14, 15, ANIM_LOOP, 1);
		}
		// 돈계산
		for (i=0; i<SIZE; i++) {
			if (blocks[targetRow][i] == BLK_CASH) {			// 돈 블록이 존재하면 돈 획득
				uinfo.userMoney += 100;
			}
		}
		uinfo.userScore += 200;								// 점수 계산.
		// 변경된 스코어 갱신.
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);

		// 애니메이션 효과가 끝나길 기다림
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		// 새로운 값 지정과 함께 스프라이트 갱신.
		for (i=0; i<SIZE; i++) {		// 새로운 블락 생성
			blocks[targetRow][i] = PA_RandMax(NUM_OF_BLOCK);
			PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[targetRow][i], blocks[targetRow][i]);
		}

		// 콤보 발동전 시각적으로 확인할 수 있는 시간을 줌.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}

		break;

	/* 세로 한 줄 삭제 */
	case ITEM_BOMB_COL:
		for (i=0; i<SIZE; i++) {	// 블록 상쇄 그래픽 효과
			PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][targetCol], 10, 14, 15, ANIM_LOOP, 1);
		}
		for (i=0; i<SIZE; i++) {
			if (blocks[i][targetCol] == BLK_CASH) {			// 돈 블록이 존재하면 돈 획득
				uinfo.userMoney += 100;
			}
		}
		uinfo.userScore += 200;								// 점수 계산
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);

		// 애니메이션 효과가 끝나길 기다림
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}

		// 새로운 값 지정과 함께 스프라이트 갱신.
		for (i=0; i<SIZE; i++) {		// 새로운 블락 생성
			blocks[i][targetCol] = PA_RandMax(NUM_OF_BLOCK);
			PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][targetCol], blocks[i][targetCol]);
		}
		// 콤보 발동전 시각적으로 확인할 수 있는 시간을 줌.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		break;

	/* 9칸 삭제	*/
	case ITEM_BOMB_SMALL:
		for (i=targetRow-1; i<targetRow+2; i++) {
			for (j=targetCol-1; j<targetCol+2; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][j], 10, 14, 15, ANIM_LOOP, 1);
				}
			}
		}
		for (i=targetRow-1; i<targetRow+2; i++) {
			for (j=targetCol-1; j<targetCol+2; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					if (blocks[targetRow][i] == BLK_CASH)	// 돈 블록이 존재하면 돈 획득
						uinfo.userMoney += 100;
				}
			}
		}
		uinfo.userScore += 300;								// 점수 계산
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
		// 애니메이션 효과가 끝나길 기다림
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		// 새로운 값 지정과 함께 스프라이트 갱신.
		for (i=targetRow-1; i<targetRow+2; i++) {
			for (j=targetCol-1; j<targetCol+2; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					blocks[i][j] = PA_RandMax(NUM_OF_BLOCK);
					PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]);
				}
			}
		}
		// 콤보 발동전 시각적으로 확인할 수 있는 시간을 줌.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		break;

	/* 25칸 삭제 */
	case ITEM_BOMB_BIG:
		// 그래픽 효과.
		for (i=targetRow-2; i<targetRow+3; i++) {
			for (j=targetCol-2; j<targetCol+3; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][j], 10, 14, 15, ANIM_LOOP, 1);
				}
			}
		}
		for (i=targetRow-2; i<targetRow+3; i++) {
			for (j=targetCol-2; j<targetCol+3; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					if (blocks[targetRow][i] == BLK_CASH) {	// 돈 블록이 존재하면 돈 획득
						uinfo.userMoney += 100;
					}
				}
			}
		}
		uinfo.userScore += 500;								// 점수 계산
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
		// 애니메이션 효과가 끝나길 기다림
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		// 새로운 값 지정과 함께 스프라이트 갱신.
		for (i=targetRow-2; i<targetRow+3; i++) {
			for (j=targetCol-2; j<targetCol+3; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					blocks[i][j] = PA_RandMax(NUM_OF_BLOCK);
					PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]);
				}
			}
		}
		// 콤보 발동전 시각적으로 확인할 수 있는 시간을 줌.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}


		break;

	/* 유효한 아이템이 아닐 때 */
	default:
		return FALSE;
	}

	return TRUE;
}

int getitemindex() {
	int i;

	for ( i=0;i<(USER_ITEM_MAX);i++ ) {
		if ( uinfo.userItem[i]==-1 ) {
			return i;
		}
	}

	return -1;
}

/* 사용자정보초기화 */
void user_init() {
	uinfo.userMoney=0;
	uinfo.userScore=0;
	uinfo.userStage=1;
	uinfo.userItem[0]=-1;
	uinfo.userItem[1]=-1;
	uinfo.userItem[2]=-1;
	uinfo.userItem[3]=-1;
	uinfo.userItem[4]=-1;
	uinfo.activeBlockCol = -1;
	uinfo.activeBlockRow = -1;
	uinfo.activeItem = -1;
}
