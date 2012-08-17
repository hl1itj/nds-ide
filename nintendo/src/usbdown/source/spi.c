#include "dallshell.h"
#include <nds.h>

#define ARM9

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
	unsigned short enable_irq;
} CARD_SPI_SETTINGS;


static CARD_SPI_SETTINGS card_spi_settings;

static int interrupt_flag = 0;

extern int g_cnt;

/*-------------------------------------------------------------------------------*/
void cardLineIrqHandler() {
/*-------------------------------------------------------------------------------*/
	interrupt_flag = 1;
}

/*-------------------------------------------------------------------------------*/
void spiInit()
/*-------------------------------------------------------------------------------*/
{
	cardSpiInit(CLOCK_512KHZ);

	// cardSpiSetHandler(cardLineIrqHandler);
}

/*-------------------------------------------------------------------------------*/
int cardSpiBusy() {
/*-------------------------------------------------------------------------------*/
	return CARD_CR1 & CR1_BUSY;
}

void cardSpiStart(bool hold) {
	/* disable IRQ because it's start of a critical section */
	CARD_CR1 = card_spi_settings.clock | (hold ? CR1_HOLD_CS : 0) | CR1_CE | /* card_spi_settings.enable_irq | */ CR1_ENABLE_SLOT;
}

void cardSpiStop() {
	/* re-enable IRQ if they should be re-enabled to end critical section */
	CARD_CR1 = card_spi_settings.clock | card_spi_settings.enable_irq;
}

/*-------------------------------------------------------------------------------*/
void cardSpiInit(CARD_SPI_CLOCK clock) {
/*-------------------------------------------------------------------------------*/
	card_spi_settings.clock = clock;
	card_spi_settings.enable_irq = 0;
#ifdef ARM9
	sysSetCardOwner(BUS_OWNER_ARM9);
#else
	sysSetCardOwner(BUS_OWNER_ARM7);
#endif
	//cardSpiSetHandler(NULL);
	cardSpiStop();
}

/*-------------------------------------------------------------------------------*/
void cardSpiSetHandler(void (*irqHandler)(void)) {
/*-------------------------------------------------------------------------------*/
	card_spi_settings.enable_irq = irqHandler == NULL ? 0 : CR1_ENABLE_IRQ;
	irqSet(IRQ_CARD_LINE, irqHandler);
	irqEnable(IRQ_CARD_LINE);
	cardSpiStop();					/* update CR1 register */
}

/*-------------------------------------------------------------------------------*/
void cardSpiEnable() {
/*-------------------------------------------------------------------------------*/
	card_spi_settings.enable_irq = CR1_ENABLE_IRQ;
	irqEnable(IRQ_CARD_LINE);
	cardSpiStop();					/* update CR1 register */
}

/*-------------------------------------------------------------------------------*/
void cardSpiDisable() {
/*-------------------------------------------------------------------------------*/
	card_spi_settings.enable_irq = 0;
	irqDisable(IRQ_CARD_LINE);
	cardSpiStop();					/* update CR1 register */
}

/*-------------------------------------------------------------------------------*/
char cardSpiTransfer(char c) {
/*-------------------------------------------------------------------------------*/
	char data;

	cardSpiStart(false);			/* enable card SPI */

	CARD_EEPDATA = c;				/* send charactr */
	while(cardSpiBusy());			/* busy wait */
	cardSpiStop();					/* disable card SPI */

#ifdef ARM9
	swiDelay(500);
#else
	swiDelay(12);
#endif

	return CARD_EEPDATA;
}

/*-------------------------------------------------------------------------------*/
void cardSpiWriteBuffer(char *out, uint16 count) {
/*-------------------------------------------------------------------------------*/
	uint16 i;

	if (count == 0) {
		return;
	} else if (count == 1) {
		cardSpiTransfer(*out);
		return;
	}

	/* enable card SPI with CS hold */
	cardSpiStart(true);
	swiDelay(10);
	for ( i = 0; i < count-1; i++ ) {
		CARD_EEPDATA = out[i];		/* send character */
		while(cardSpiBusy());		/* busy wait */
		swiDelay(2000);
	}

	/* last character has to be transferred with chip select hold disabled */
	cardSpiStart(false);
	swiDelay(10);
	CARD_EEPDATA = out[count-1];	/* send last character */
	while(cardSpiBusy());			/* busy wait */

	/* disable card SPI */
	cardSpiStop();

#ifdef ARM9
	swiDelay(2000);
#else
	swiDelay(12);
#endif
}

/*-------------------------------------------------------------------------------*/
void cardSpiTransferBuffer(char *out, char *in, uint16 count) {
/*-------------------------------------------------------------------------------*/
	uint16 i;

	if (count == 0) {
		return;
	} else if (count == 1) {
		*in = cardSpiTransfer(*out);
		return;
	}

	/* enable card SPI with CS hold */
	cardSpiStart(true);

	for ( i = 0; i < count-1; i++ ) {
		CARD_EEPDATA = out[i];		/* send character */
		while(cardSpiBusy());		/* busy wait */
		in[i] = CARD_EEPDATA;		/* receive character */
	}

	/* last character has to be transferred with chip select hold disabled */
	cardSpiStart(false);

	CARD_EEPDATA = out[count-1];	/* send last character */
	while(cardSpiBusy());			/* busy wait */
	in[count-1] = CARD_EEPDATA;		/* receive character */

	/* disable card SPI */
	cardSpiStop();

#ifdef ARM9
	swiDelay(24);
#else
	swiDelay(12);
#endif
}

static uint8_t old_ch;

/*-------------------------------------------------------------------------------*/
unsigned char recvByte()
/*-------------------------------------------------------------------------------*/
{
	unsigned char ch;

retry:
	ch = cardSpiTransfer(USB_DATA_REQ);
	if (old_ch == ch && ch != POLL_DATA)
		goto retry;

	if (ch == POLL_DATA) {
retry2:
		swiDelay(3000);
		ch = cardSpiTransfer(USB_DATA_REQ);
		if (ch == POLL_DATA)
			goto retry2;
	}

	old_ch = ch;

	return ch;
}

static unsigned char prev_ch;

unsigned char poll_spi()
{
	unsigned char ch;

retry:
	ch = cardSpiTransfer(USB_DATA_REQ);
	if(prev_ch == ch)
		goto retry;

	prev_ch = ch;

	return ch;
}

unsigned char recv_byte()
{
	unsigned char ch;
	unsigned char buf[32] = {0, };
	int i;

	i = 0;

	while(1)
	{
		ch  = poll_spi();
		buf[i] = ch;
		if(i >= 1)
		{
			if(((unsigned char)(buf[i] - 3 )== buf[i-1]))
			{
				return buf[i];
			}
		}
		i++;
	}

}

void recv_data(void *buf, int size)
{
	unsigned char *ptr;
	unsigned char prev_ch = 0, ch = 0;

	int index = 0;;

	ptr = (unsigned char *)buf;

	while(1)
	{
		ch = poll_spi();

		if(ch == (prev_ch + 3))
			ptr[index++] = ch;

		if(index == size)
			break;

		prev_ch = ch;
	}
	g_cnt++;
}

/*-------------------------------------------------------------------------------*/
void sendByte(unsigned char data)
/*-------------------------------------------------------------------------------*/
{
	cardSpiStart(false);			/* enable card SPI */

	CARD_EEPDATA = data;		/* send charactr */
	while(cardSpiBusy());			/* busy wait */
	cardSpiStop();					/* disable card SPI */

#ifdef ARM9
	swiDelay(2000);
#else
	swiDelay(12);
#endif
}
