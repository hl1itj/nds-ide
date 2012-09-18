/************************************************************************
 *
 *                    Dall Shell for Nintendo DS
 *                   -----------------------------
 *           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
 *                      All Rights Reserved.
 *
 ************************************************************************/

/* FILE        : main.c
 *
 * Description : Dallshell main routine
 *
 * Created - 2009-12-27 by Minsuk Lee
 * Revised - 2009-XX-XX 
 *         - 2009-XX-XX
 * Read    - 2009-01-21
 */

#include "dallshell.h"
#include "download.h"

struct links CHANNELS[MAX_CHANNEL];

// DallShell Default Configuration
// user can change it, and saved in sav file

struct dallshell_conf Config = {
        STARTUP_MENU,       // Invoke Menu, when starrtup
        "autoexec.arm9",    // default Autoexec binary

        CHANNEL_SERIAL,     // Console
        CHANNEL_WIFI,       // Download
        CHANNEL_USB,        // Debug

        1,                  // if 1, ignore dallshell config
        1,                  // if 1, write into file after download
        0,                  // if 1, run after download
        0,                  // if 1, run with debug mode

        0,                  // if 1, accept connection from next host only
        { 0, },             // IP Address of Designated Host
        DOWNLOAD_PORT,      // Port Number (default: DOWNLOAD_PORT)

        "SSID",             // Default SSID
        1,                  // DHCP Enable
        0, 0, 0, 0          // IP, Subnet-Mask, GW, DNS
};


int wifi_init(void);
int usb_init(void);
int serial_init(void);

int
main(void)
{
    int ret;
    int run, debug;

#ifndef _WIN32
	consoleDemoInit();
	fatInitDefault();
#endif

	iprintf("DallShell, %s %s\n", __DATE__, __TIME__);
	iprintf("nds.sevencore.co.kr\n");

	switch (Config.download_channel) {
        case CHANNEL_WIFI   :
            if (wifi_init()) {
                iprintf("WIFI Initialize Error\n");
                return -1;
            }
            break;
        case CHANNEL_USB    :
            if (usb_init()) {
                iprintf("USB Initialize Error\n");
                return -1;
            }
            break;
        case CHANNEL_SERIAL :
            if (serial_init()) {
                iprintf("Serial Initialize Error\n");
                return -1;
            }
            break;
        default :
            iprintf("Configuration Error\n");
            break;
	}

loop:
	if ((ret = download(CHANNELS + Config.download_channel, &run, &debug)) == 0) {
	    iprintf("Download Succefully Done\n");
	} else {
        iprintf("Download Stoped by Error %d\n", ret);
        goto loop;
	}
	if (run) {
	    extern void _copy_bin_and_jump(void);
	    extern unsigned char XXX_BUF_[];
	    void (*calljump)(void);

	    calljump = (void(*)(void))XXX_BUF_;
	    iprintf("\n\n\n\n\nDOWNLOAD DONE and COPY\n\n\n\n");
	    _copy_bin_and_jump();                  // Call to jump.s
        iprintf("COPY DONE and JUMP\n\n\n\n");
        { unsigned char *p = XXX_BUF_;
          int i;
          for (i = 0; i < 48; i++)
              printf("%02X ", *p++);
          iprintf("\n----\n");
        }

        calljump();
	    // RIP : never return
	}
	iprintf("Now What to do ? - LOOP !\n");
	goto loop;
    return 0;
}

int
check_abort(void)
{
    // Check Abort Switch
    return 0;
}

void
confirm_error(char *error_string, int yes_no)
{
    iprintf("%s\n", error_string);
    // Confirming
}

char *
channel_name(int channel)
{
    switch (channel) {
        case CHANNEL_WIFI   : return "Wireless-LAN";
        case CHANNEL_USB    : return "USB";
        case CHANNEL_SERIAL : return "Serial";
        default             : return "UNKNOWN-CHANNEL";
    }
}
