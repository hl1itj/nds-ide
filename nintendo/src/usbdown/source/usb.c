/************************************************************************
 *
 *                    Dall Shell for Nintendo DS
 *                   -----------------------------
 *           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
 *                      All Rights Reserved.
 *
 ************************************************************************/

/* FILE        : usb.c
 *
 * Description : Dallshell USB via AT32 Interface
 *
 * Created - 2010-02-01 by Minsuk Lee
 * Revised - 2010-XX-XX
 *         - 2010-XX-XX
 * Read    - 2010-XX-XX
 */

#include "dallshell.h"

static int usb_on = 0;

static
int
usb_connect(void)
{
//    int ret = 0;
	int ret = 1;
    // open AT ...
    usb_on = 1;
    return ret;
}

static
int
usb_rx_ready(void)
{
    // check usb data is available
    return 0;
}

static
int
usb_tx_ready(void)
{
    // check usb data is available
    return 0;
}

static unsigned char block_data;

static
int
usb_read(void *buf, int count)
{
	int i;
	int rcount = 0;
	int size;
	unsigned char c;
	unsigned char temp_buf[3] = {0, };
	if (count == 0)
		return count;

	while(count)
	{
		size = 1;
		c = recv_byte();
		*(unsigned char*)buf = c;

		buf += size;
		count -= size;
		rcount += size;
	}

	return rcount;
}

static
int
usb_write(void *buf, int count)
{
	int wcount = 0;
	int size;
	unsigned char c;

	if (count == 0)
		return count;

	while(count)
	{
		size = 1;
		sendByte(USB_DATA);

		c = *(unsigned char*)buf;

		sendByte(c);

		buf += size;
		count -= size;
		wcount += size;
	}

	return wcount;

}

static
void
usb_disconnect(void)
{
    usb_on = 0;
}

static
void
usb_cleanup(void)
{
    ;
}

int
usb_init(void)
{
    struct links *channel = CHANNELS + CHANNEL_USB;

    spiInit();

    channel->name = channel_name(CHANNEL_USB);
    channel->connect = usb_connect;
    channel->read = usb_read;
    channel->write = usb_write;
    channel->disconnect = usb_disconnect;
    channel->cleanup = usb_cleanup;
    return 0;
}
