/*
 * Ball.cpp
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#include "Ball.h"
#include "includefile.h"

#define DOWN_SCREEN 0
#define UP_SCREEN 1
#define BALL_NUM 2

#define SCREENHOLE 48
#define BALL_RADIUS 16

#define SIZE_L4		64
#define SIZE_L3		32
#define SIZE_L2		16
#define SIZE_L1		8

Ball::Ball() {
	move_flag = 0;
	size = SIZE_L4;
	/*PA_LoadSpritePal(DOWN_SCREEN, BALL_NUM, (void*)sprite1_Pal);	// 공...
	//PA_LoadSpritePal(DOWN_SCREEN, pimg, (void*)sprite1_Pal);	// 공...
	//PA_CreateSprite(생성할 위치 스크린 번호, 할당시킬 스프라이트 번호, 스프라이트 그림 변수, 크기, 256색이면 1, 팔렛번호, x좌표,y좌표)
	//PA_CreateSprite(DOWN_SCREEN, BALL_NUM,(void*)circle16x16_Sprite, OBJ_SIZE_16X16,1, 0, x, y); // 공 생성
	PA_CreateSprite(DOWN_SCREEN, BALL_NUM,(void*)circle_Sprite, OBJ_SIZE_32X32,1, 0, x, y); // 공 생성
	*/
	puck.ball_x = 128; puck.ball_y = 96+192+SCREENHOLE; // central position on bottom screen
	puck.vx = 1; puck.vy = 2; // No speed
}

Ball::~Ball() {
}


void Ball::move(){
	if(move_flag){
	//if(size == 32){
		puck.ball_x += puck.vx;	//puck.x , puck.y 가 움직이는 공의 x,y좌표.
		puck.ball_y += puck.vy;	//puck.vx, puck.vy 는 속도에 따른 공의 이동 좌표.
//		공의 위치는 중심을 기준으로 하고 0,0은  위 화면의 0,0임.
//		공의  반지름은 16이다.

	// If the sprite touches the left or right border, flip the horizontal speed

		if ((puck.ball_x - BALL_RADIUS <= 0) && (puck.vx < 0)) puck.vx = -puck.vx;			//X좌표 왼쪽벽 끝
		else if ((puck.ball_x + size - BALL_RADIUS >= 256)&&(puck.vx > 0)) puck.vx = -puck.vx;	//X좌표 오른쪽 벽 끝

	// Same thing, for top and bottom limits...
		if ((puck.ball_y - BALL_RADIUS <= 240) && (puck.vy < 0)) puck.vy = -puck.vy;	//Y좌표를 밑 스크린의 위부터
		else if ((puck.ball_y + size - BALL_RADIUS >= 192 + 192 + SCREENHOLE)&& (puck.vy > 0)) puck.vy = -puck.vy;//Y좌표 맨밑스크린 바닥까지

	// The bottom limit is at the bottom of the bottom screen, so that would be 2 screen heights, plus the space in between...
	//	PA_OutputText(UP_SCREEN, 0, 6, "b1 x : %d ", puck.ball_x);
	//	PA_OutputText(UP_SCREEN, 0, 7, "b1 y : %d ", puck.ball_y);
/*	}else{
		puck.ball_x += puck.vx;	//puck.x , puck.y 가 움직이는 공의 x,y좌표.
		puck.ball_y += puck.vy;	//puck.vx, puck.vy 는 속도에 따른 공의 이동 좌표.
		//		공의 위치는 중심을 기준으로 하고 0,0은  위 화면의 0,0임.
		//		공의  반지름은 16이다.

			// If the sprite touches the left or right border, flip the horizontal speed
		if ((puck.ball_x - BALL_RADIUS <= 0) && (puck.vx < 0)) puck.vx = -puck.vx;			//X좌표 왼쪽벽 끝
		else if ((puck.ball_x + BALL_RADIUS >= 256)&&(puck.vx > 0)) puck.vx = - puck.vx;	//X좌표 오른쪽 벽 끝

			// Same thing, for top and bottom limits...
		if ((puck.ball_y - BALL_RADIUS <= 240) && (puck.vy < 0)) puck.vy = -puck.vy;	//Y좌표를 밑 스크린의 위부터
		else if ((puck.ball_y + BALL_RADIUS >= 192 + 192 + SCREENHOLE)&& (puck.vy > 0)) puck.vy = - puck.vy;//Y좌표 맨밑스크린 바닥까지
		PA_OutputText(UP_SCREEN, 0, 8, "b2 x : %d ", puck.ball_x);
		PA_OutputText(UP_SCREEN, 0, 9, "b2 y : %d ", puck.ball_y);
	}*/
	}
}
void Ball::setimg(int pimg){
	x = 16;
	y = 16;

	//PA_LoadSpritePal(스크린번호, 파렛트 번호, 파렛트)

		//	PA_LoadSpritePal(DOWN_SCREEN, pimg, (void*)sprite1_Pal);	// 공...
			//PA_CreateSprite(생성할 위치 스크린 번호, 할당시킬 스프라이트 번호, 스프라이트 그림 변수, 크기, 256색이면 1, 팔렛번호, x좌표,y좌표)
			//PA_CreateSprite(DOWN_SCREEN, BALL_NUM,(void*)circle16x16_Sprite, OBJ_SIZE_16X16,1, 0, x, y); // 공 생성
	if(size == SIZE_L4){
		PA_LoadSpritePal(DOWN_SCREEN, BALL_NUM+pimg, (void*)circle64x64_Pal);	// 공...
		PA_CreateSprite( DOWN_SCREEN, BALL_NUM+pimg, (void*)circle64x64_Sprite, OBJ_SIZE_64X64,1, BALL_NUM+pimg, x, y); // 공 생성
	}
	else if(size == SIZE_L3){
		PA_LoadSpritePal(DOWN_SCREEN, BALL_NUM+pimg, (void*)sprite1_Pal);	// 공...
		PA_CreateSprite( DOWN_SCREEN, BALL_NUM+pimg, (void*)circle_Sprite, OBJ_SIZE_32X32,1, BALL_NUM+pimg, x, y); // 공 생성
	}else if(size == SIZE_L2){
		PA_LoadSpritePal(DOWN_SCREEN, BALL_NUM+pimg, (void*)circle16x16_Pal);	// 공...
		PA_CreateSprite( DOWN_SCREEN, BALL_NUM+pimg, (void*)circle16x16_Sprite, OBJ_SIZE_16X16,1, BALL_NUM+pimg, x, y); // 공 생성
	}else if(size == SIZE_L1){
		PA_LoadSpritePal(DOWN_SCREEN, BALL_NUM+pimg, (void*)circle8x8_Pal);	// 공...
		PA_CreateSprite( DOWN_SCREEN, BALL_NUM+pimg, (void*)circle8x8_Sprite, OBJ_SIZE_8X8,1, BALL_NUM+pimg, x, y); // 공 생성
	}
}
void Ball::setlocate(int x, int y, int dx, int dy){
	this->puck.ball_x = x;
	this->puck.ball_y = y;
	this->puck.vx = dx;
	this->puck.vy = dy;
}
void Ball::setsize(int ball_size){
	this->size = ball_size;
	if(puck.vx > 0)
		puck.vx++;
	else
		puck.vx--;
	if(puck.vy > 0)
		puck.vy++;
	else
		puck.vy--;
}
