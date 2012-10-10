/*
 * realio.h
 *
 *  Created on: 2010. 10. 6.
 *      Author: Minsuk Lee
 */

#ifndef REALIO_H_
#define REALIO_H_

// Return Value of KeyCurrent()
#define KEY_A		0x0001
#define KEY_B		0x0002
#define KEY_X		0x0400
#define KEY_Y		0x0800
#define KEY_L		0x0200
#define KEY_R		0x0100
#define KEY_START	0x0008
#define KEY_SELECT	0x0004
#define KEY_UP		0x0040
#define KEY_DOWN	0x0080
#define KEY_LEFT	0x0020
#define KEY_RIGHT	0x0010

#define NDS_SWITCH()	(keysCurrent() & 0xFFFF)

#endif /* REALIO_H_ */
