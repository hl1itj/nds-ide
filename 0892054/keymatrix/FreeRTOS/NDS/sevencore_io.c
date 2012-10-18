/*
 * Virutal_io.c
 *
 *  Created on: 2010. 5. 27 by РќДіСи
 *  Rewrite on: 2010. 10. 7 by Minsuk Lee
 */

// Free RTOS Headers
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

// NDS / Sevencore Board Headers
#include <nds.h>
#include <sevencore_io.h>

#include "up.h"
#include "down.h"

#define printf  iprintf
xQueueHandle KeyQueue;

static PrintConsole bottomScreen;      	// for printf()
static int virtual_io_enabled;

#define LED_ON_RED      RGB(31,  0,  0) /* Bright Red   */
#define LED_ON_GREEN    RGB(0,  31,  0) /* Bright Green */
#define LED_ON_BLUE     RGB(0,   0, 31) /* Bright Blue  */
#define LED_OFF_COLOR   RGB(25, 25, 25) /* Gray         */
#define BAR_COLOR_RED   0x801F801F      /* Two pixels   */
#define BAR_COLOR_GREEN 0x83E083E0      /* Two pixels   */
#define BAR_COLOR_BLUE  0xFC00FC00      /* Two pixels   */
#define BAR_COLOR_OFF   0xE739E739      /* Two pixels   */
#define SEG_OFF_COLOR   RGB(3,   0,  0) /* Dark Red     */

#define NUM_LED         8       // 8 LED's in one BarLED
#define BAR_HEIGHT      24
#define LED_INTERVAL    8

#define BAR_POSX_1      52      // X Position of BARLED1
#define BAR_POSX_2      140     // X Position of BARLED2
#define BAR_POSY        32

static u8 barled1_old, barled2_old;

static
void
drawBARLED(u32 target, u8 value)
{
    int startX;
    int led, i;
    u8 bit;
    u32 color0, color1;
    u32 *targetPoint;

    if (target == BARLED1) {
        if (barled1_old == value)
            return;
        startX = BAR_POSX_1;
    } else {
        if (barled2_old == value)
            return;
        startX = BAR_POSX_2;
    }

    bit = 0x80;
    for (led = 0; led < NUM_LED; led++, bit /= 2) {
        if (target == BARLED1) {
            if ((barled1_old & bit) == (value & bit))
                continue;
        } else {
            if ((barled2_old & bit) == (value & bit))
                continue;
        }
        if (value & bit)
            color0 = (target == BARLED1) ? BAR_COLOR_RED : BAR_COLOR_GREEN;
        else
            color0 = BAR_COLOR_OFF;
        color1 = color0 & 0xFFFF;

        targetPoint = (u32 *)BG_GFX + (BAR_POSY * SCREEN_WIDTH + startX + led * LED_INTERVAL) / 2;
        for (i = 0; i < BAR_HEIGHT; i++) {
            *targetPoint = color0;          //color0;
            *(targetPoint + 1) = color0;    //color0;
            *(targetPoint + 2) = color0;    //color0;
            *(targetPoint + 3) = color1;    // Right-most 2 pixels
            targetPoint += (SCREEN_WIDTH / 2);
        }
    }
    if (target == BARLED1)
        barled1_old = value;
    else
        barled2_old = value;
}

#define NUM_SEGMENT     8
#define SEG_INTERVAL    16      // 16 Pixels
#define SEG_HEIGHT		8

#define SEG_POSX        68
#define SEG_POSY        128

#define SEG_COLOR_ON    BAR_COLOR_RED   // Two pixels
#define SEG_COLOR_OFF   0x80038003      // Pale Red

static u8 segment_old[NUM_7SEG_LED];

static int TargetOffset[NUM_7SEG_LED] = {      // Target Offset, 2 pixel unit
    (                       2) / 2, // a
    ((SCREEN_WIDTH *  2) + 10) / 2, // b
    ((SCREEN_WIDTH * 12) + 10) / 2, // c
    ((SCREEN_WIDTH * 20) +  2) / 2, // d
    ((SCREEN_WIDTH * 12)     ) / 2, // e
    ((SCREEN_WIDTH *  2)     ) / 2, // f
    ((SCREEN_WIDTH * 10) +  2) / 2, // g
    ((SCREEN_WIDTH * 20) + 12) / 2  // h
};

static u8 segdata[16] = {
    0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xF6, // 0,1,2,3,4,5,6,7,8,9
    0xEE, 0x3E, 0x9C, 0x7A, 0x9E, 0x8E                          // A,B,C,D,E,F
};

void
draw7SEGMENT(u8 value)
{
    u32 color;
    u32 *basePoint;     // Top Left corner address of 7 Segment
    u32 *targetPoint;   // Current Drawing Area
    u8  bit, segment;
    int led, seg, i;

    led = (value & 0x70) >> 4;
    segment = (value & 0x80) ? 0: segdata[value & 0x0F];	// Turn off if MSB set
	if (segment == segment_old[led])
		return;
    basePoint = (u32 *)BG_GFX + ((SEG_POSY * SCREEN_WIDTH) + SEG_POSX + (led * SEG_INTERVAL)) / 2;
	bit = 0x80;
	for (seg = 0; seg < NUM_SEGMENT; seg++, bit /= 2) {
		if ((segment & bit) == (segment_old[led] & bit))
			continue;
		color = (segment & bit) ? SEG_COLOR_ON : SEG_COLOR_OFF;
		targetPoint = basePoint + TargetOffset[seg];
		switch (seg) {
		  case 0 :  // a
		  case 3 :  // d
		  case 6 :  // g
			*targetPoint++ = color;
			*targetPoint++ = color;
			*targetPoint++ = color;
			*targetPoint   = color;
			targetPoint += ((SCREEN_WIDTH - 6) / 2);
			*targetPoint++ = color;
			*targetPoint++ = color;
			*targetPoint++ = color;
			*targetPoint   = color;
			break;
		  case 1 :  // b
		  case 2 :  // c
		  case 4 :  // e
		  case 5 :  // f
			for (i = 0; i < SEG_HEIGHT; i++) {
				*targetPoint = color;
				targetPoint += (SCREEN_WIDTH / 2);
			}
			break;
		  case 7 :  // h : point
			*targetPoint = color;
			targetPoint += (SCREEN_WIDTH / 2);
			*targetPoint = color;
			break;
        }
    }
	segment_old[led] = segment;
}

#define SW_WIDTH		30
#define SW_HEIGHT		30
#define NUM_SW_IN_ROW	9
#define NUM_SW_IN_COL	7

static volatile u8 virtual_matrix;

static
u8
readSW(void)
{
    int index;
    touchPosition touch;

	touchRead(&touch);
	touch.py = touch.py / SW_WIDTH;
	touch.px = touch.px / SW_HEIGHT;
	index = touch.py * NUM_SW_IN_ROW + touch.px;
	switch (index) {
	  case 19 : return VIRTUAL_SW_1;
	  case 20 : return VIRTUAL_SW_3;  // Needs bounce processing
	  case 28 : return VIRTUAL_SW_2;
	  case 29 : return VIRTUAL_SW_4;  // Needs bounce processing
	}
    return 0;
}

static
u8
readMatrix(void)
{
    touchPosition touch;

    if (!(virtual_matrix & 0xF0))
        return 0;
    touchRead(&touch);
    touch.py = (touch.py / SW_WIDTH) - 1;
    touch.px = (touch.px / SW_HEIGHT) - 4;
    if ((touch.py < 0) || (touch.py > 3) || (touch.px < 0) || (touch.px > 3))
    	return 0;
	//writeb_virtual_io(SEG7LED, 0x30 + touch.py);
	//writeb_virtual_io(SEG7LED, 0x40 + touch.px);
    if (!(virtual_matrix & (0x80 >> touch.py)))
    	return 0;
    return (0x08 >> touch.px);
}

int
init_virtual_io(u32 devices)
{
	if (virtual_io_enabled)
		return -1;		// Already Enabled
    virtual_io_enabled = devices;
    if (virtual_io_enabled & ENABLE_LED) {
        videoSetMode(MODE_5_2D);
        vramSetBankA(VRAM_A_MAIN_BG);
        bgInit(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);
        decompress(upBitmap, BG_GFX, LZ77Vram);
    }
    if (virtual_io_enabled & (ENABLE_SW | ENABLE_MATRIX)) {
    	if (bottomScreen.consoleInitialised)
    		return -1;	// Printf Enabled
        videoSetModeSub(MODE_5_2D);
        vramSetBankC(VRAM_C_SUB_BG);
        bgInitSub(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);
        decompress(downBitmap, BG_GFX_SUB, LZ77Vram);
    }
    return 0;
}

int
init_printf(void)
{
    if (virtual_io_enabled & (ENABLE_SW | ENABLE_MATRIX))
    	return -1;	// Bottom Screen is used for Touch
   	// Enable Bottom LCD for printf();
    videoSetModeSub(MODE_0_2D);
    vramSetBankC(VRAM_C_SUB_BG);
    consoleInit(&bottomScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
    consoleSelect(&bottomScreen);
    return 0;
}

u8
readb_virtual_io(u32 addr)
{
    switch (addr) {
      case PUSH_SW:
        if (virtual_io_enabled & ENABLE_SW)
        	return readSW();
        break;
      case KEY_MATRIX:
        if (virtual_io_enabled & ENABLE_MATRIX)
            return readMatrix();
        break;
      default:
        break;
    }
    return 0;
}

void
writeb_virtual_io(u32 addr, u8 value)
{
    switch (addr) {
      case SEG7LED:
        if (virtual_io_enabled & ENABLE_LED)
        	draw7SEGMENT(value);
        break;
      case BARLED1:
      case BARLED2:
        if (virtual_io_enabled & ENABLE_LED)
            drawBARLED(addr, value);
        break;
      case KEY_MATRIX:
        if (virtual_io_enabled & ENABLE_MATRIX)
            virtual_matrix = value & 0xF0;
        break;
      default:
        break;
    }
    return;
}

void
close_virtual_io(void)
{
    return;
}
