/*
 * ShellGraphic.c
 *
 *  Created on: 2011. 7. 24.
 *      Author: Jaewon Lee
 */

#include "Common.h"
#include "Graphic.h"

void loadBackground() {
	PA_LoadBackground(0, 3, &bg_down);
	PA_LoadBackground(1, 3, &bg_up);
	//PA_LoadBackground(1, 3, &bg_create);
}

void loadDialogBackground(int dialog_flag)
{
	if(dialog_flag)
		PA_LoadBackground(0, 1, &bg_dialog);
	else
		PA_LoadBackground(0, 1, &bg_edit_dialog);
}

/* 스프라이트 사용시 주의점
 * 팔레트와 오브젝트 넘버에 유의, 겹치거나 어긋날경우 이미지가 깨질 수 있음
 * 되도록이면 같은 번호로 사용하는 것이 좋음.
 */
void loadSprite() {
	PA_LoadSpritePal(0,0,(void*)selected_Pal);
	PA_LoadSpritePal(0,1,(void*)menu_del_Pal);
	PA_LoadSpritePal(0,2,(void*)menu_move_Pal);
	PA_LoadSpritePal(0,3,(void*)move_create_Pal);

	PA_LoadSpritePal(1,4,(void*)menu_wifi_Pal);
	PA_LoadSpritePal(1,5,(void*)menu_usb_Pal);
	PA_LoadSpritePal(1,6,(void*)menu_serial_Pal);

	PA_LoadSpritePal(1,7,(void*)sel_menu_wifi_Pal);
	PA_LoadSpritePal(1,8,(void*)sel_menu_usb_Pal);
	PA_LoadSpritePal(1,9,(void*)sel_menu_serial_Pal);

	PA_LoadSpritePal(0,10,(void*)menu_setting_Pal);
}

void createSprite() {
	//	PA_CreateSprite(0,  // screen
	//					0,  // object number
	//					(void*)selected_Sprite, OBJ_SIZE_8X8, // Sprite & size
	//					1,  // color style
	//					0,  // palette number
	//					10, // coord x
	//					55); // coord y
	PA_CreateSprite(1,7,(void*)sel_menu_wifi_Sprite, OBJ_SIZE_64X64, 1,7,20,100);
	PA_CreateSprite(1,5,(void*)menu_usb_Sprite, OBJ_SIZE_64X64, 1,5,90,100);
	PA_CreateSprite(1,6,(void*)menu_serial_Sprite, OBJ_SIZE_64X64, 1,6,160,100);

	PA_CreateSprite(0,0,(void*)selected_Sprite, OBJ_SIZE_8X8,1,0,10,55);
	PA_CreateSprite(0,1,(void*)menu_del_Sprite, OBJ_SIZE_32X32, 1,1,70,10);
	PA_CreateSprite(0,2,(void*)menu_move_Sprite, OBJ_SIZE_32X32, 1,2,110,10);
	PA_CreateSprite(0,3,(void*)move_create_Sprite, OBJ_SIZE_32X32, 1,3,150,10);

	PA_CreateSprite(0,10,(void*)menu_setting_Sprite, OBJ_SIZE_32X32,1,10,190,10);
}

void redrawSelectedDevice(int select_num) {
	PA_ResetSpriteSysScreen(1);
	switch(select_num)
	{
	case 0:
		PA_CreateSprite(1,7,(void*)sel_menu_wifi_Sprite, OBJ_SIZE_64X64, 1,7,20,100);
		PA_CreateSprite(1,5,(void*)menu_usb_Sprite, OBJ_SIZE_64X64, 1,5,90,100);
		PA_CreateSprite(1,6,(void*)menu_serial_Sprite, OBJ_SIZE_64X64, 1,6,160,100);
		break;
	case 1:
		PA_CreateSprite(1,4,(void*)menu_wifi_Sprite, OBJ_SIZE_64X64, 1,4,20,100);
		PA_CreateSprite(1,8,(void*)sel_menu_usb_Sprite, OBJ_SIZE_64X64, 1,8,90,100);
		PA_CreateSprite(1,6,(void*)menu_serial_Sprite, OBJ_SIZE_64X64, 1,6,160,100);
		break;
	case 2:
		PA_CreateSprite(1,4,(void*)menu_wifi_Sprite, OBJ_SIZE_64X64, 1,4,20,100);
		PA_CreateSprite(1,5,(void*)menu_usb_Sprite, OBJ_SIZE_64X64, 1,5,90,100);
		PA_CreateSprite(1,9,(void*)sel_menu_serial_Sprite, OBJ_SIZE_64X64, 1,9,160,100);
		break;
	}
}
