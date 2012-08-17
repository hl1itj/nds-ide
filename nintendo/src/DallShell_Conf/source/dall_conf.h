/*
 * dall_conf.h
 *
 *  Created on: 2010. 5. 30.
 *      Author: 컴퓨터공학과
 */

#ifndef DALLSHELL_CONF_H_
#define DALLSHELL_CONF_H_

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
#include <PA9.h>
#include <nds/arm9/console.h>
#include <dswifi9.h>
#include <nds/ndstypes.h>
#include <fat.h>
#endif

#define MAX_FILE_NAME_LEN       32
const char* addr_text;

Wifi_AccessPoint ap; // for information related AP

enum { CHANNEL_WIFI = 0, CHANNEL_SERIAL =1, CHANNEL_USB=2 };

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
    unsigned char wepkeys[4][32]; // WEPkey password
    int wepkeyid, wepmode;      // WEP KeyID(0,1,2,3) , WEP Mode(32bit, 64bit, 128bit)
    struct in_addr my_ip;        // My IP Address
    struct in_addr subnet;       // Subnet mask
    struct in_addr gateway;      // Default Gateway
    struct in_addr dns1;          // Default Domain name server first
    struct in_addr dns2;          // Default Domain name server second

    // Serial Configuration (8-Bit No Parity, 1 Stop Bit)
    int baud;                   // Baud Rate, 1200 ~ 115200
};

struct dallshell_conf Config;


//--- Wifi Configuration Function
extern int wifi_searchAP();
extern void wifi_Manualconf();
extern void wifi_APconn();
extern void wifi_Test_Dns();
extern int wifi_SaveConf();

//--- Serial Configuration Function







//--- USB Configuration Function







//--- Key Input Function
void key_inputString();
void addr_key_inputString(int num, unsigned long editaddr);

//--- Developer Definition Function
void wait(int n); // Multiple use to PA_WaitForVBL();


#endif /* DALLSHELL_CONF_H_ */
