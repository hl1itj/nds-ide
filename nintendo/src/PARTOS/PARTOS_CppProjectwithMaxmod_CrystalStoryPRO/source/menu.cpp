/*
 * menu.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: 컴퓨터공학과
 */
#include "cs_api.h"
#include "menu.h"

void ret_next() // 아무데나 Touch를 눌러 다음화면으로 전환
{
	while (1) {
		if ( Stylus.Newpress )
			return;
		PA_WaitForVBL();
	}
}

int sel_menu() // Touch를 눌러 메뉴를 선택. 선택된 메뉴의 번호를 리턴
{
	int select=0;
	while(1)
	{
		if(Stylus.Newpress) {
			if(Stylus.X > 28 && Stylus.Y < 226 && Stylus.X > 18 && Stylus.Y < 62) {
		        select = 1;
		        break;
			}
			else if(Stylus.X > 28 && Stylus.X < 226 && Stylus.X > 73 && Stylus.Y < 119) {
				select = 2;
				break;
			}
			else if(Stylus.X > 28 && Stylus.X < 226 && Stylus.X > 128 && Stylus.Y < 176) {
				select = 3;
				break;
			}
		}
		PA_WaitForVBL();
	}
	return select;
}
