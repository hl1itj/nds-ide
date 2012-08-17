/*
 * cs_api.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: 컴퓨터공학과
 *description : Palib의 내용을 우리 프로젝트에서 사용하기 쉽게 api함수로 정리해놓은 것
 */

#include "cs_api.h"

static Coordi* coordi;

// 스프라이트 이미지 출력 (SPRITE1 PAL)
void sprite_print(void* pal, void* sprite, int x, int y)
{
	PA_LoadSpritePal(DOWN_SCREEN, SPRITE1_PAL, (void*)pal);
	PA_CreateSprite(DOWN_SCREEN, SPRITE1,
			(void*)sprite, OBJ_SIZE_16X32,
			TRUE, SPRITE1_PAL, x, y);
}
// 스프라이트 이미지 출력 (SPRITE9 PAL - 서로 다른 팔레트 관련)
void sprite_print2(void* pal, void* sprite, int x, int y)
{
	PA_LoadSpritePal(DOWN_SCREEN, SPRITE9_PAL, (void*)pal);
	PA_CreateSprite(DOWN_SCREEN, SPRITE9,
			(void*)sprite, OBJ_SIZE_16X32,
			TRUE, SPRITE9_PAL, x, y);
}

// 듀얼 스프라이트 이미지 출력 (16X16)
void sprite_print_dual(void* pal, void* sprite,int pal_num, int count, int x, int y)
{
	PA_DualLoadSpritePal(pal_num, (void*)pal);
	PA_DualCreateSprite(count, (void*)sprite,
			OBJ_SIZE_16X16, TRUE, pal_num, x, y);
}
// 듀얼 스프라이트 이미지 출력 (16X16)
void sprite_print_dual2(void* pal, void* sprite,int pal_num,int count ,int x, int y)
{
	PA_DualLoadSpritePal(pal_num, (void*)pal);
	PA_DualCreateSprite(count, (void*)sprite,
			OBJ_SIZE_32X32, TRUE, pal_num, x, y);
}

// 듀얼 백그라운드 이미지 출력
void dualback_print(const PA_BgStruct* dualbg)
{
	PA_DualLoadBackground(1, //background number (0-3)
			dualbg); // Name
}

// 스프라이트 이미지의 이동
void move_sprite(u8 sprite_num, int x, int y)
{
	PA_SetSpriteXY(DOWN_SCREEN,sprite_num,x,y);
}

// 듀얼 스프라이트의 이미지 이동
void dual_move(u8 sprite_num, int x, int y)
{
	PA_DualSetSpriteXY(sprite_num, x, y);
}

// 방향키를 이용한 이동
void press_key_move()
{
	if(Pad.Held.Right | Pad.Held.Left | Pad.Held.Down | Pad.Held.Up )
	{
		coordi->x1 += Pad.Held.Right - Pad.Held.Left;
	}
	if(!((Pad.Held.Left)||(Pad.Held.Right))) {
		PA_StartSpriteAnim(0, 0, 0, 0, 4);
	}
	if(Pad.Newpress.Right) {
		PA_StartSpriteAnim(0, 0, 4, 7, 6);
		PA_SetSpriteHflip(0, 0, 0);
	}
	if(Pad.Newpress.Left) {
		PA_StartSpriteAnim(0, 0, 4, 7, 6);
		PA_SetSpriteHflip(0, 0, 1);
	}
}

// 터치를 이용한 이동
void touch_key_move()
{
	if(Stylus.X | Stylus.Y)
	{
		coordi->x2 = Stylus.X;
		coordi->y2 = Stylus.Y;
	}
}

// A키를 눌렀을 때
u8 A_press_key()
{
	return Pad.Newpress.A;
}

// 이미지 reset
void UnLoad_Screen()
{
	int i;
	for(i=0; i<2; ++i)
	{
		PA_ResetBgSys();
		PA_ResetBgSysScreen(i);
		PA_ResetSpriteSysScreen(i);
	}
}
// 딜레이 시간 임의로 지정하기
void wait(int size)
{
	int cnt=0;
	while(cnt <= size){
		PA_WaitForVBL();
		cnt++;
	}
}
