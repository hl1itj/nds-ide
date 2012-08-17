/*
 * VirtualDevice.cpp
 *
 *  Created on: 2010. 5. 27.
 *      Author: РќДіСи
 */

#include "FreeRTOS.h"
#include "virtual_io.h"
#include "task.h"
#include "portmacro.h"
#include "up.h"
#include "down.h"

typedef enum {myfalse = 0, mytrue = 1} myboolean;

struct {
	xTaskHandle upperPartScreenTaskHandle;
	xTaskHandle touchpadTaskHandle;
	myboolean runningUpperPartScreen;
	myboolean runningLowerPartScreen;
    const unsigned int *upperPartBackground;
    const unsigned int *lowerPartBackground;
    u8 selected7segmentled;
    u8 selectedKeypad;
    u32 stateVirtualIos;
    u16 stateKeypad;
    u8 stateSwitchs;
} virtual_io_resister = {NULL,       /* xTaskHandle upperPartScreenTaskHandle   */
			             NULL,       /* xTaskHandle touchpadTaskHandle          */
						 myfalse,    /* myboolean runningUpperPartScreen        */
						 myfalse,    /* myboolean runningLowerPartScreen        */
						 upBitmap,   /* const unsigned int *upperPartBackground */
						 downBitmap, /* const unsigned int *lowerPartBackground */
						 0,          /* u8 selected7segmentled                  */
						 0,          /* u8 selectedKeypad                       */
						 0,          /* u32 stateVirtualIos                     */
						 0,          /* u32 stateKeypad                         */
                         0};         /* u8 stateSwitchs                         */


#define RGB(R, G, B) (1 << 15 | (R) << 10 | (G) << 5 | (B))

static portTASK_FUNCTION( touchPadTask, pvParameters )
{
	int i;
	touchPosition touch;

	struct Rect {
		int left;
		int top;
		int width;
		int height;
	};

	struct Rect keypad[] = {
				{113, 37, 30, 30},
				{143, 37, 30, 30},
				{173, 37, 30, 30},
				{203, 37, 30, 30},
				{113, 67, 30, 30},
				{143, 67, 30, 30},
				{173, 67, 30, 30},
				{203, 67, 30, 30},
				{113, 97, 30, 30},
				{143, 97, 30, 30},
				{173, 97, 30, 30},
				{203, 97, 30, 30},
				{113,127, 30, 30},
				{143,127, 30, 30},
				{173,127, 30, 30},
				{203,127, 30, 30}
	};
	struct Rect switchs[] = {
				{ 27, 67, 30, 30},
				{ 27, 97, 30, 30},
				{ 57, 67, 30, 30},
				{ 57, 97, 30, 30}
	};

	while (1) {
		touchRead(&touch);
		virtual_io_resister.stateKeypad = 0;
		for (i = 0; i < 16; i++) {
			if ((keypad[i].left <= touch.px) &&
				(touch.px <= (keypad[i].left + keypad[i].width)) &&
				(keypad[i].top <= touch.py) &&
				(touch.py <= (keypad[i].top + keypad[i].height))) {
				virtual_io_resister.stateKeypad |= (1 << i);
			}
		}

		virtual_io_resister.stateSwitchs = 0;
		for (i = 0; i < 4; i++) {
			if ((switchs[i].left <= touch.px) &&
				(touch.px <= (switchs[i].left + switchs[i].width)) &&
				(switchs[i].top <= touch.py) &&
				(touch.py <= (switchs[i].top + switchs[i].height))) {
				virtual_io_resister.stateSwitchs |= (1 << i);
			}
		}
	}
}

static void enableUpperPartBackground(void)
{
	if (virtual_io_resister.runningUpperPartScreen == myfalse) {
		videoSetMode(MODE_5_2D);
		vramSetBankA(VRAM_A_MAIN_BG);
		bgInit(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		decompress(virtual_io_resister.upperPartBackground, BG_GFX, LZ77Vram);
		virtual_io_resister.runningUpperPartScreen = mytrue;
	}
}

static void enableLowerPartBackground(void)
{
	if (virtual_io_resister.runningLowerPartScreen == myfalse) {
		videoSetModeSub(MODE_5_2D);
		vramSetBankC(VRAM_C_SUB_BG);
		bgInitSub(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		decompress(virtual_io_resister.lowerPartBackground, BG_GFX_SUB, LZ77Vram);
		virtual_io_resister.runningLowerPartScreen = mytrue;
	}
}

static void enableTouchpadTask(void)
{
	if (virtual_io_resister.touchpadTaskHandle == NULL) {
		xTaskCreate( touchPadTask,
				     (const signed char * const)"touchPadTask",
				     2048,
				     (  void * ) NULL,
				     tskIDLE_PRIORITY + 1,
				     &(virtual_io_resister.touchpadTaskHandle));
	}
}

static void disableTouchpadTask(void)
{
	if (virtual_io_resister.touchpadTaskHandle != NULL) {
		vTaskDelete(virtual_io_resister.touchpadTaskHandle);
		virtual_io_resister.touchpadTaskHandle = NULL;
	}
}

static void setVirtualIoBarled(u8 target, u8 value)
{
	int startX = 32;
	int startY = 32;
	int interval = 11;
	int ledLocation, i, j;
	volatile u16 color;
	u16 *targetPoint;

	if (target) {
		startX = 135;
	} else {
		startX = 32;
	}

	for (ledLocation = 0; ledLocation < 8; ledLocation++) {
		if (value & (0x80 >> ledLocation)) {
			color = RGB(0, 31, 0);
		} else {
			color = RGB(25, 25, 25);
		}

		for (j = 0; j < 50; j++) {
			targetPoint = (BG_GFX + (startY + j) * SCREEN_WIDTH + startX + ledLocation * interval);
			for (i = 0; i < 10; i++) {
				*(targetPoint + i) = color;
			}
		}
	}
}

static void select7segmentled(u8 value)
{
	virtual_io_resister.selected7segmentled = value;
}

static void setVirtualIo7segmentled(u8 target, u8 value)
{
	struct Led {
			char mask;
			int left;
			int top;
			int width;
			int height;
	} ledPoint[] = {
			{0x80, 3, 0, 13, 3},
			{0x40, 16, 3, 3, 15},
			{0x20, 16, 21, 3, 15},
			{0x10, 3, 36, 13, 3},
			{0x8, 0, 21, 3, 15},
			{0x4, 0, 3, 3, 15},
			{0x2, 3, 18, 13, 3},
			{0x1, 20, 36, 3, 3}
	};
	int startX = 33;
	int startY = 110;
	u16 onColor = RGB(0, 31, 0);
	u16 offColor = RGB(25, 25, 25);
	int legmentLocation, i, j;
	u16 drawColor;
	u16 *targetPoint;

	for (legmentLocation = 0; legmentLocation < 8; legmentLocation++) {
		if (ledPoint[legmentLocation].mask & value) {
			drawColor = onColor;
		} else {
			drawColor = offColor;
		}

		for (i = 0; i < ledPoint[legmentLocation].height; i++) {
			targetPoint = (BG_GFX + (startY + i + ledPoint[legmentLocation].top) * SCREEN_WIDTH + startX + ((7 - target) * 24) + ledPoint[legmentLocation].left);
			for (j = 0; j < ledPoint[legmentLocation].width; j++) {
				*(targetPoint + j ) = drawColor;
			}
		}
	}
}

static void selectKeypad(u8 value)
{
	virtual_io_resister.selectedKeypad = (value >> 4);
}

static u8 readKeypad(void)
{
	u8 shift;
	switch (virtual_io_resister.selectedKeypad) {
		case 0x1:
			shift = 0;
			break;
		case 0x2:
			shift = 1;
			break;
		case 0x4:
			shift = 2;
			break;
		case 0x8:
			shift = 3;
			break;
		default:
			return 0;
			break;
	}
	return (virtual_io_resister.stateKeypad >> (shift << 2)) & 0x0f;
}

static u8 readSwitchs(void)
{
	return virtual_io_resister.stateSwitchs;
}

static void setVirtualIo7segmentleds(u8 value)
{
	u8 target7segmentLed;
	for (target7segmentLed = 0; target7segmentLed < NUM_7SEG_LED; target7segmentLed++) {
		if (virtual_io_resister.selected7segmentled & (1 << target7segmentLed)) {
			setVirtualIo7segmentled(target7segmentLed, value);
		}
	}
}

int  init_virtual_io(u32 devices)
{
	virtual_io_resister.stateVirtualIos |= devices;
	switch (devices) {
	case ENABLE_7SEGLED:
	case ENABLE_BARLED1:
	case ENABLE_BARLED2:
		enableUpperPartBackground();
		break;
	case ENABLE_SW1:
	case ENABLE_SW2:
	case ENABLE_SW3:
	case ENABLE_SW4:
	case ENABLE_KEYMATRIX:
		enableLowerPartBackground();
		enableTouchpadTask();
		break;
	case ENABLE_ALL_DEVICES:
		enableUpperPartBackground();
		enableLowerPartBackground();
		enableTouchpadTask();
		break;
	default:
		return 1;
		break;
	}
	return 0;
}

u8   readb_virtual_io(u32 addr)
{
	switch (addr) {
	case PUSHSW:
		return readSwitchs();
		break;
	case KEYMATRIX:
		return readKeypad();
		break;
	default:
		break;
	}
	return 0;
}

void writeb_virtual_io(u32 addr, u8 value)
{
	switch (addr) {
	case SEG7COM:
		select7segmentled(value);
		break;
	case SEG7DATA:
		setVirtualIo7segmentleds(value);
		break;
	case BARLED1:
		setVirtualIoBarled(0, value);
		break;
	case BARLED2:
		setVirtualIoBarled(1, value);
		break;
	case KEYMATRIX:
		selectKeypad(value);
		break;
	default:
		break;
	}
	return;
}

void close_virtual_io(void)
{
	disableTouchpadTask();
	return;
}
