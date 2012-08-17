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
    int ret = 0;
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
static
int
usb_read(void *buf, int count)
{
    int rcount = 0;
    if (count == 0)
        return count;
    while (usb_rx_ready()) {
        // GET DATA FROM AT32
        //rcount = at32_read(RBR, count, 0);
        if (rcount < 0)
            return DSH_RX_ERROR;
    }
    if (rcount == 0)
        return DSH_WOULD_BLOCK;
    return rcount;  // return 0 if connection closed
}

static
int
usb_write(void *buf, int count)
{
    int wcount = 0;
    if (count == 0)
        return count;
    while (usb_tx_ready()) {
        // PUT DATA FROM AT32
        //wcount = at32_write(TBR, count, 0);
        if (wcount < 0)
            return DSH_TX_ERROR;
    }
    if (wcount == 0)
        return DSH_WOULD_BLOCK;
    return wcount;  // return 0 if connection closed
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

    channel->name = channel_name(CHANNEL_USB);
    channel->connect = usb_connect;
    channel->read = usb_read;
    channel->write = usb_write;
    channel->disconnect = usb_disconnect;
    channel->cleanup = usb_cleanup;
    return 0;
}
