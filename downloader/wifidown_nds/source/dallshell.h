/************************************************************************
 *
 *                    Dall Shell for Nintendo DS
 *                   -----------------------------
 *           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
 *                      All Rights Reserved.
 *
 ************************************************************************/

/* FILE        : dallshell.h
 *
 * Description : Dallshell main universal Header
 *
 * Created - 2009-12-27 by Minsuk Lee
 * Revised - 2009-01-25 Add some Fields
 *         - 2009-XX-XX
 * Read    - 2009-01-21
 */

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#endif

#ifdef _WIN32
#define iprintf printf
#else
#include <nds.h>
#include <nds/arm9/console.h>
#include <dswifi9.h>
#include <nds/ndstypes.h>
#include <fat.h>
#endif

#define MAX_FILE_NAME_LEN       32
enum Channel_ID {       // Data Link to Host PC
    CHANNEL_WIFI = 0,   // WIFI
    CHANNEL_USB,        // USB - over Atmel
    CHANNEL_SERIAL,     // SERIAL
    MAX_CHANNEL,
};


// Wifi, Serial, USB register next structure at xxx_init() function
struct links {
    char *name;                         // Name of The Channel
    int  (*connect)(void);              // Wait for Connection from Host
    int  (*read)(void *p, int len);     // Read Data from Channel
    int  (*write)(void *p, int len);    // write Data to Channel
    void (*disconnect)(void);           // Disconnect
    void (*cleanup)(void);              // Return all Channel Resources
};

extern struct links CHANNELS[MAX_CHANNEL];

#ifndef _WIN32
int wifi_init(void);
int usb_init(void);
int serial_init(void);
#endif

Wifi_AccessPoint ap;

struct dallshell_conf {
    int startup_mode;           // What to do, when startup, see defines below
    char autoexec[MAX_FILE_NAME_LEN];   // Starup File

    int console_channel;        // Channel to be used as Console
    int download_channel;       // Channel for File download
    int debug_channel;          // Channel for Remote Debugging

    //--- Configuration for Download options
    int as_host_wish;           // Follow Host's order after download
    int flash;                  // Write downloaded file into a File
    int run;                    // Run, after download
    int debug;                  // Run with Debug mode, after download

    //--- Configuration for Wifi (TCP/IP Connection)
    int single_host;            // Accept wifi connection from this host only
    struct sockaddr_in client;  // IP Address of Designated Host
    unsigned short port;        // Port Number (default: DOWNLOAD_PORT)

    // Wifi Configuration
    char SSID[32];              // SSID
    int dhcp;                   // if 1, get IP/GW/NS from AP
    unsigned long my_ip;        // My IP Address
    unsigned long subnet;       // Subnet mask
    unsigned long gateway;      // Default Gateway
    int autodns;                // if 1, get dns from AP
    unsigned long dns;          // Default Domain name server

    // Serial Configuration (8-Bit No Parity, 1 Stop Bit)
    int baud;                   // Baud Rate, 1200 ~ 115200
};

enum {
    STARTUP_MENU,       // Going to menu Screen
    STARTUP_RUN,        // Run Autoexec File After 1 Second to check Abort
    STARTUP_NETBOOT,    // Network Boot, search AP and then boot with BOOTP over Wifi
    STARTUP_DOWNLOAD,   // Wait for Download (from download_channel)
};

int   check_abort(void);        // return 1, if user abort requested
void  confirm_error(char *error_string, int yes_no);
int   download(struct links *channel, int *run, int *debug);
char *channel_name(int channel);
extern struct dallshell_conf Config;

//--- Target Binary

#define MAX_BINARY_SIZE     (1024 * 1024)   // 1MBytes
#define TARGET_BIN_ADDR     0x2000000
extern unsigned char        *_BIN_BUF;      // in jump.s

//extern char *BIN_BUF;
extern unsigned char BIN_BUF[];

//--- Dall Shell Error Code

enum {
    DSH_CHECKSUM = -1000,
    DSH_FILE_WRITE,
    DSH_FILE_OPEN,
    DSH_INVALID_LENGTH,
    DSH_INVALID_NAME,
    DSH_NO_MEMORY,
    DSH_TX_ERROR,
    DSH_USER_ABORT,
    DSH_WOULD_BLOCK,
    DSH_RX_ERROR,
    DSH_CONNECTION_CLOSED,
    DSH_CONNECTION_FAILED
};

//--- MESSAGE INTERFACE

#define TODO(s)     iprintf("TODO: %s\n", (s))
