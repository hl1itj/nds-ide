/*
 * input.h
 *
 *  Created on: 2010. 7. 6.
 *      Author: 컴퓨터공학과
 */

#ifndef INPUT_H_
#define INPUT_H_

#include "all_gfx.h"

typedef enum Keycontrol{
	DALLKEY_UP=200,
	DALLKEY_DOWN,
	DALLKEY_LEFT,
	DALLKEY_RIGHT,
	DALLKEY_A,
	DALLKEY_B,
	DALLKEY_X,
	DALLKEY_Y,
	DALLKEY_L,
	DALLKEY_R,
	DALLKEY_START,
	DALLKEY_SELECT
}Keycontrol;

typedef enum Keyboardname {
	CHANGE_SSID,
	CHANGE_IP,
	CHANGE_GATE,
	CHANGE_SUBNET,
	CHANGE_DNS1,
	CHANGE_DNS2,
	CHANGE_CHANNEL
}Keyboardname;


extern void ui_loop(void);
int key_behavior(Pads key);
extern int check_location(void);
extern int selected;
extern int count;
extern int numAp;
extern u64 current_flag;
extern void key_inputString();
void redraw_screen();
#endif /* INPUT_H_ */
