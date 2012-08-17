/*
 * virtual_io.h
 *
 *  Created on: 2010. 5. 27.
 *      Author: РќДіСи
 */

#ifndef VIRTUAL_IO_H_
#define VIRTUAL_IO_H_

/*****************************************************************
*
*                     Virtual Input/Output Devices
*                   --------------------------------
*           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
*                      All Rights Reserved.
*
*****************************************************************/

/* FILE        : virtual_io.c
*
* Description : Virtual Input/Output Definition
*
* Created - 2010-04-20 by Minsuk Lee
* Revised -
* Read    - 2009-04-21
*/
#include <nds.h>

#define SEG7COM 	0x400   // Output
#define SEG7DATA	0x404   // Output
#define BARLED1		0x408   // Output
#define BARLED2		0x40C   // Output
#define PUSHSW		0x414   // Input
#define KEYMATRIX	0x410   // Input/Output

#define NUM_7SEG_LED    8
#define NUM_KEY_ROWS	4

#define ENABLE_7SEGLED		0x01
#define ENABLE_BARLED1		0x02
#define ENABLE_BARLED2		0x04
#define ENABLE_SW1		0x08
#define ENABLE_SW2		0x10
#define ENABLE_SW3		0x20
#define ENABLE_SW4		0x40
#define ENABLE_KEYMATRIX	0x80

#define ENABLE_ALL_DEVICES	0xFFFFFFFF

extern int  init_virtual_io(u32 devices);
extern u8   readb_virtual_io(u32 addr);
extern void writeb_virtual_io(u32 addr, u8 value);
extern void close_virtual_io(void);

#endif /* VIRTUAL_IO_H_ */
