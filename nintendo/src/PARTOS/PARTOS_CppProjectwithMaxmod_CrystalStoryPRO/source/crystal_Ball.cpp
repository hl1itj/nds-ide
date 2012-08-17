/*
 * crystal_Ball.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: ywyw
 */

#include "crystal_Ball.h"
#include <PA9.h>
#include "all_gfx.h"
#include "game.h"
#include "cs_api.h"

crystal_Ball::crystal_Ball() {
	x =-200;
	y = 380;
	go_ball = 0;
}

crystal_Ball::~crystal_Ball() {

}
int crystal_Ball::move(int spcae_x, int water_cnt){

	if(A_press_key()&&(go_ball == 0)){
			go_ball = 1;
			x=spcae_x;
			water_cnt--;
	}
	if(go_ball==1){
		if(y <= 0){
			go_ball = 0;
		   x = -200;
		   y = 380;
		  }
		  y = y - 4;
	}
	return water_cnt;
}

void crystal_Ball::ball_setting(int pal_num, int obj_num)
{
	 sprite_print_dual((void*)water_Pal, (void*)water_Sprite, pal_num, obj_num, x, y); // 캐릭터 이미지 출력
}

