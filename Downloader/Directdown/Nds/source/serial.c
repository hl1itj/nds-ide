/************************************************************************
 *
 *                    Dall Shell for Nintendo DS
 *                   -----------------------------
 *           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
 *                      All Rights Reserved.
 *
 ************************************************************************/

/* FILE        : serial.c
 *
 * Description : Dallshell Serial via AT32 Interface
 *
 * Created - 2010-02-01 by Minsuk Lee
 * Revised - 2010-XX-XX
 *         - 2010-XX-XX
 * Read    - 2010-XX-XX
 */

#include "dallshell.h"

static int serial_on = 0;

static
int
serial_connect(void)
{
    int ret = 0;
    // open AT ...
    serial_on = 1;
    return ret;
}

static
int
serial_rx_ready(void)
{
    // check serial data is available
    return 0;
}

static
int
serial_tx_ready(void)
{
    // check serial data is available
    return 0;
}
static
int
serial_read(void *buf, int count)
{
    int rcount = 0;
    if (count == 0)
        return count;
    while (serial_rx_ready()) {
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
serial_write(void *buf, int count)
{
    int wcount = 0;
    if (count == 0)
        return count;
    while (serial_tx_ready()) {
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
serial_disconnect(void)
{
    serial_on = 0;
}

static
void
serial_cleanup(void)
{
    ;
}

int
serial_init(void)
{
    struct links *channel = CHANNELS + CHANNEL_SERIAL;

    channel->name = channel_name(CHANNEL_SERIAL);
    channel->connect = serial_connect;
    channel->read = serial_read;
    channel->write = serial_write;
    channel->disconnect = serial_disconnect;
    channel->cleanup = serial_cleanup;
    return 0;
}
