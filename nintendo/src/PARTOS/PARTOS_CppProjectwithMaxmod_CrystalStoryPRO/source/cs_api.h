/*
 * cs_api.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: 컴퓨터공학과
 *description : Palib의 내용을 우리 프로젝트에서 사용하기 쉽게 api함수로 정리해놓은 것
 */

#ifndef CALC_H_
#define CALC_H_

#include <PA9.h>

// SCREEN OFFSET
#define UP_SCREEN 1
#define DOWN_SCREEN 0

// SPRITE PALETE NUM
#define SPRITE1_PAL 0
#define SPRITE2_PAL 1
#define SPRITE3_PAL 2
#define SPRITE4_PAL 3
#define SPRITE5_PAL 4
#define SPRITE6_PAL 5
#define SPRITE7_PAL 6
#define SPRITE8_PAL 7
#define SPRITE9_PAL 8

// OBJ NUM
#define OBJ1_NUM 0
#define OBJ2_NUM 1
#define OBJ3_NUM 2
#define OBJ4_NUM 3
#define OBJ5_NUM 4
#define OBJ6_NUM 5
#define OBJ7_NUM 6
#define OBJ8_NUM 7
#define OBJ9_NUM 8

// SPRITE NUM
#define SPRITE1  0
#define SPRITE2  1
#define SPRITE3  2
#define SPRITE4  3
#define SPRITE5  4
#define SPRITE6  5
#define SPRITE7  6
#define SPRITE8  7
#define SPRITE9  8

// BACKGROUND NUM
#define BG0  0
#define BG1  1
#define BG2  2
#define BG3  3

#define TRUE 1
#define FALSE 0


// 캐릭터 이동과 관련된 좌표와 관련된 클래스
class Coordi {
public :
	int x1; // coordination of sprite images
	int y1;
	int x2;
	int y2;

	Coordi(){
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
	}
};

// 스프라이트 이미지 출력 (SPRITE1 PAL)
void sprite_print(void* pal, void* sprite, int x, int y);
// 스프라이트 이미지 출력 (SPRITE9 PAL - 서로 다른 팔레트 관련)
void sprite_print2(void* pal, void* sprite, int x, int y);
// 듀얼 스프라이트 이미지 출력 (16X16)
void sprite_print_dual(void* pal, void* sprite,int pal_num, int count,int x, int y);
// 듀얼 스프라이트 이미지 출력 (16X16)
void sprite_print_dual2(void* pal, void* sprite,int pal_num,int count,int x, int y);


// 듀얼 배경 이미지 출력
void dualback_print(const PA_BgStruct* dualbg);
// 스프라이트 이미지의 이동
void move_sprite(u8 sprite_num, int x, int y); // with input
// 방향키를 이용한 이동
void press_key_move();
// 터치를 이용한 이동
void touch_key_move();
// 듀얼 스프라이트의 이미지 이동
void dual_move(u8 pal,int x, int y);


// A키를 눌렀을 때
u8 A_press_key();
// 이미지 reset
void UnLoad_Screen();
// 딜레이 시간 임의로 지정하기
void wait(int size);

#endif /* CALC_H_ */
