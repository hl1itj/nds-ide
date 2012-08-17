/*
 * game.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: ywyw
 */

#include "game.h"
#include "menu.h"
#include "cs_api.h"
#include "all_gfx.h"
#include "hero.h"
#include "princess.h"
#include "water.h"
#include "crystal.h"
#include "crystal_Ball.h"
#include "monster.h"

#define SCREEN_MAX_WATER	10

// 초기 캐릭터 좌표
int x =160;
int y =160;
int move_cnt =0; // 물방울 이동 차
int water_cnt = 0; // hero 의 물방울 갯수


// 게임프로세스 객체 생성
static GameProcess gp;

Coordi* coordi;

// 싱글 플레이 초기화
void init_singleplay() {
 PA_LoadBackground(DOWN_SCREEN, BG2, &bg_stage1_init);
 PA_WaitForVBL();
 //UnLoad_Screen();
 //PA_WaitForVBL();
 ret_next(); // next state
 dualback_print(&stage2); // 게임배경화면 출력 (듀얼)

 /* 텍스트 출력 부분 */
 PA_InitText(DOWN_SCREEN, BG0);
 PA_SetTextTileCol(DOWN_SCREEN, TEXT_WHITE); //
}

// 싱글 플레이 스타트
void singleplay_start() {
	// 각각의 객체 생성
	hero player;
	princess prin;
	monster mons1;
	water drop_water1;
	water drop_water2;
	water drop_water3;
	water drop_water4;
	crystal_Ball ball;

	// 랜덤으로 x좌표지정
	drop_water1.x = PA_Rand()%250;
	drop_water2.x = PA_Rand()%250;
	drop_water3.x = PA_Rand()%250;
	drop_water4.x = PA_Rand()%250;
	mons1.x = PA_Rand()%250;

	// 각각의 객체 이미지 출력
	// 물방울 그림 출력의 경우 (첫번째 인자 - spriteNUM, 두번째인자 - objNUM)
	drop_water1.water_setting(SPRITE2_PAL, OBJ2_NUM);
	drop_water2.water_setting(SPRITE3_PAL, OBJ3_NUM);
	drop_water3.water_setting(SPRITE4_PAL, OBJ4_NUM);
	drop_water4.water_setting(SPRITE5_PAL, OBJ5_NUM);
	ball.ball_setting(SPRITE6_PAL, OBJ6_NUM);
	mons1.monster_setting(SPRITE7_PAL, OBJ7_NUM, mons1.x);

	while(1) // 여기에서 게임이 동작!!
	{
		/* 텍스트 출력부분- 게임정보 (계속적으로 출력)*/
		PA_OutputText(DOWN_SCREEN, 1, 21, "Stage%d", gp.stage_num);
		PA_OutputText(DOWN_SCREEN, 9, 21, "Score:%d", gp.score);
		PA_OutputText(DOWN_SCREEN, 18, 21, "Odd Monster:%d",gp.oddmonster);
		PA_OutputText(DOWN_SCREEN, 18, 22, "My Crystals:%d",gp.crystals);

		// 객체 이동
		player.move();
		prin.move(1);
		mons1.move(1);
		drop_water1.move(3);
		drop_water2.move(2);
		drop_water3.move(3);
		drop_water4.move(2);

		// 몬스터와 크리스탈이 충돌했을 때,
		if (PA_Distance(mons1.x, mons1.y, ball.x, ball.y) < 32*32) {
			gp.score = gp.score+50;
			mons1.y = mons1.y-100; // 몬스터가 뒤로 밀리는 현상
			mons1.hp = mons1.hp-15; // 몬스터의 피가 15씩 감소
			ball.x = -100;
			ball.y = -100;
			if(mons1.hp<0)
			{
				gp.oddmonster--;
				gp.score = gp.score+100;
				mons1.~monster();
			}
		}
		// 물방울을 주인공이 먹었을 때
		if (PA_Distance(drop_water1.x, drop_water1.y-240, player.x, player.y) < 16*8) {
			drop_water1.x = PA_Rand()%250;
			drop_water1.y = 0;
			gp.crystals++;
		}
		if(PA_Distance(drop_water2.x, drop_water2.y-240, player.x, player.y) < 16*8) {
			drop_water2.x = PA_Rand()%250;
			drop_water2.y = 0;
			gp.crystals++;
		}
		if(PA_Distance(drop_water3.x, drop_water3.y-240, player.x, player.y) < 16*8) {
			drop_water3.x = PA_Rand()%250;
			drop_water3.y = 0;
			gp.crystals++;
		}
		if(PA_Distance(drop_water4.x, drop_water4.y-240, player.x, player.y) < 16*8) {
			drop_water4.x = PA_Rand()%250;
			drop_water4.y = 0;
			gp.crystals++;
		}

		// 크리스탈 카운트 ( 떨어지는 물방울을 먹은 개수 만큼 볼 생성 )
		if(gp.crystals > 0 )
			gp.crystals = ball.move(player.x, gp.crystals); // 볼 생성

		/******************************** Motion Area ****************************************/

		move_sprite(SPRITE1, player.x, player.y); // hero 좌표 변경 후 출력
		move_sprite(SPRITE9, prin.x, prin.y); // princess 좌표 변경 후 출력

		dual_move(SPRITE2, drop_water1.x, drop_water1.y); // 물방울 좌표 변경 후 출력
		dual_move(SPRITE3, drop_water2.x, drop_water2.y); // 물방울 좌표 변경 후 출력
		dual_move(SPRITE4, drop_water3.x, drop_water3.y); // 물방울 좌표 변경 후 출력
		dual_move(SPRITE5, drop_water4.x, drop_water4.y); // 물방울 좌표 변경 후 출력

		if(mons1.y > 375)
			break;
		else
			dual_move(SPRITE7, mons1.x, mons1.y); // 몬스터 좌표 변경 후 출력

		// 크리스탈 좌표 변경 후 출력
		if(gp.crystals > 0)
			dual_move(SPRITE6, ball.x, ball.y);

		PA_WaitForVBL();
	}

	UnLoad_Screen();
	PA_LoadBackground(UP_SCREEN, BG2, &gameover);
	PA_LoadBackground(DOWN_SCREEN, BG2, &gameover);
	wait(100);
}
