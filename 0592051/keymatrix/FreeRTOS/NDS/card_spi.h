/*
 * DSerial Library
 *
 * Copyright (c) 2008, Alexei Karpenko
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CARD_SPI_H
#define CARD_SPI_H

#include <nds.h>

// Card bus (Old version)
#define CARD_CR1       (*(vuint16*)0x040001A0)
#define CARD_CR1H      (*(vuint8*)0x040001A1)
#define CARD_EEPDATA   (*(vuint8*)0x040001A2)
#define CARD_CR2       (*(vuint32*)0x040001A4)

#define CR1_HOLD_CS		(1<<6)
#define CR1_BUSY		(1<<7)
#define CR1_CE			(1<<13)
#define CR1_ENABLE_IRQ	(1<<14)
#define CR1_ENABLE_SLOT (1<<15)

typedef enum {
	CLOCK_4MHZ = 0,
	CLOCK_2MHZ = 1,
	CLOCK_1MHZ = 2,
	CLOCK_512KHZ = 3
} CARD_SPI_CLOCK;

typedef struct {
	CARD_SPI_CLOCK clock;
	uint16 enable_irq;
} CARD_SPI_SETTINGS;

#ifdef __cplusplus
extern "C" {
#endif
unsigned char poll_spi();
unsigned char getDebugChar();
void putDebugChar(unsigned char sendByte);

void initSpi();
void cardLineIrqHandler();

inline bool cardSpiBusy();
void cardSpiInit(CARD_SPI_CLOCK clock);
void cardSpiSetHandler(void (*irqHandler)(void));

void cardSpiEnable();
void cardSpiDisable();

inline void cardSpiStart(bool hold);
inline void cardSpiStop();

char cardSpiTransfer(char c);
void cardSpiTransferBuffer(char *out, char *in, uint16 count);
void cardSpiWriteBuffer(char *out, uint16 count);

#ifdef __cplusplus
};
#endif

#endif // CARD_SPI_H
