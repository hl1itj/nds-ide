/*
 * main.c
 *
 *  Created on: 2010. 5. 30.
 *      Author: 컴퓨터공학과
 */
#include "dall_conf.h"
#include "graphic.h"
#include "input.h"

int main(){
	PA_Init();
	PA_InitVBL();
	consoleDemoInit();
	fatInitDefault();

	ap.ssid_len=7;
	strcpy(ap.ssid,"linksys");
	ap.rssi=0;
	ap.channel=1;
	ap.flags=0;
	Config.dhcp = 1; // 초기할당

	iprintf("DallShell Setting ver1.0, %s %s\n", __DATE__, __TIME__);
	iprintf("made by Jaewon Lee, Heejin Kwak\n");

	while(1) {
		ui_loop();
		PA_WaitForVBL();
	}

}
