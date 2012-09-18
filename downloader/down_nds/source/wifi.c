/************************************************************************
 *
 *                    Dall Shell for Nintendo DS
 *                   -----------------------------
 *           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
 *                      All Rights Reserved.
 *
 ************************************************************************/

/* FILE        : wifi.c
 *
 * Description : Dallshell Wifi Interface
 *
 * Created - 2009-12-27 by Minsuk Lee
 * Revised - 2009-01-25 Remove open() and merge into init() by Minsuk
 *         - 2009-XX-XX
 * Read    - 2009-01-21
 */

#include "dallshell.h"
#include "download.h"

#ifdef _WIN32
#define ioctl       ioctlsocket
#define errno       WSAGetLastError()
#define EWOULDBLOCK WSAEWOULDBLOCK
#pragma comment(lib, "ws2_32.lib")
#endif

static struct in_addr MyIP, gateway, mask, dns1, dns2;
static int listen_socket;

static
int
number_connect(void)
{
   // int listen_socket;
    struct sockaddr_in sock_addr;
    int addr_len = sizeof (sock_addr);
    int ret;
    int one = 1;

    if ((listen_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        confirm_error("wifi: Listen Socket Open", 0);
        return DSH_CONNECTION_FAILED;
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(DOWNLOAD_IP);// server addr ip input!
    sock_addr.sin_port = htons(DOWNLOAD_PORT);                   // server port input! 

    if((ret = connect(listen_socket,(struct sockaddr*)&sock_addr,addr_len)) == -1){
	iprintf("wifi: Connect error code:%d\n", listen_socket);
            confirm_error("Connection Failed", 0);
            ret = DSH_CONNECTION_FAILED;	
	 closesocket(listen_socket);
    }
    return ret;
}

static
int
wifi_connect(int Port_number)
{
   // int listen_socket;
    struct sockaddr_in sock_addr;
    int addr_len = sizeof (sock_addr);
    int ret;
    int one = 1;

    if ((listen_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        confirm_error("wifi: Listen Socket Open", 0);
        return DSH_CONNECTION_FAILED;
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(DOWNLOAD_IP);// server addr ip input!
    sock_addr.sin_port = htons(Port_number);                   // server port input! 

    if((ret = connect(listen_socket,(struct sockaddr*)&sock_addr,addr_len)) == -1){
	iprintf("wifi: Connect error code:%d\n", listen_socket);
            confirm_error("Connection Failed", 0);
            ret = DSH_CONNECTION_FAILED;	
	 closesocket(listen_socket);
    }
    return ret;
}

static
int
wifi_read(void *buf, int count)
{
    int rcount;
    if (count == 0)
        return count;
    rcount = recv(listen_socket, buf, count, 0);
    if (rcount < 0) {
        if (errno == EWOULDBLOCK)
            return DSH_WOULD_BLOCK;
        else
            return DSH_RX_ERROR;
    }
   
    return rcount;  // return 0 if connection closed
}

static
int
wifi_write(void *buf, int count)
{
    int wcount;
    if (count == 0)
        return count;
    wcount = send(listen_socket, buf, count, 0);
    if (wcount < 0) {
        if (errno == EWOULDBLOCK)
            return DSH_WOULD_BLOCK;
        else
            return DSH_TX_ERROR;
    }
    return wcount;
}

static
void
wifi_disconnect(void)
{
    closesocket(listen_socket);
    //data_socket = -1;
}

static
void
wifi_cleanup(void)
{
    if (listen_socket >= 0)
        closesocket(listen_socket);
#ifdef _WIN32
    // if any, do something after close socket
    // WSA Cleanup
#else
    Wifi_DisconnectAP();
    Wifi_DisableWifi();
#endif
}

int
wifi_init(void)
{
    struct links *channel = CHANNELS + CHANNEL_WIFI;
#ifdef _WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR) {
        confirm_error("WIFI: Error at WSAStartup()\n", 0);
        return DSH_CONNECTION_FAILED;
    }
#else
    char readtext[33];
    char iptext[33]; // ip address ONLY
    int ret, i, j, oldstatus;

    FILE *fp;

    unsigned char BUF[6];

    iprintf("WIFI: Finding AP...\n");

    fp = fopen("DallShell_Conf.txt","rb");

    if(fp == NULL)
    {
    	ret = Wifi_InitDefault(WFC_CONNECT);
    }
    else
    {
    	Wifi_InitDefault(INIT_ONLY);

    	Wifi_EnableWifi();

    	for(i=0;i<8;i++)
    	{
    		// DallShell Conf ���α׷����� txt���� ���,
    		// �ԷµǴ� ���ڿ� �迭�� ũ�⸦ �÷��� (11.08.29)
    		fgets(readtext,33,fp);
    		memset(iptext,0,33); // iptext�� memory�ʱ�ȭ (11.08.29)
    		switch(i){
    		case 0 : strcpy(ap.ssid,readtext); break;
    		case 1 : ap.ssid_len = atoi(readtext); break;
    		case 2 : ap.channel = atoi(readtext); break;
    		case 3 : memcpy(iptext, readtext, strlen(readtext)-1);
    		MyIP.s_addr = inet_addr(iptext);
    		break;
    		case 4 : memcpy(iptext, readtext, strlen(readtext)-1);
    		gateway.s_addr = inet_addr(iptext);
    		break;
    		case 5 : memcpy(iptext, readtext, strlen(readtext)-1);
    		mask.s_addr = inet_addr(iptext);
    		break;
    		case 6 : memcpy(iptext, readtext, strlen(readtext)-1);
    		dns1.s_addr = inet_addr(iptext);
    		break;
    		case 7 : memcpy(iptext, readtext, strlen(readtext)-1);
    		dns2.s_addr = inet_addr(iptext);
    		break;
    		}
    	}
    	ret = Wifi_ConnectAP(&ap,WEPMODE_NONE,0,0);
    }

    iprintf("MAC : ");
    Wifi_GetData(WIFIGETDATA_MACADDRESS, 6, BUF);
    for (i = 0; i < 6; i++) {
        iprintf("%02X", BUF[i]);
        if (i < 5)
            iprintf(":");
    }
    iprintf("\n\n");

    if(fp == NULL)
    {
    	if(ret == 0)
    	{
    		confirm_error("WIFI:Failed to connect AP!\n", 0);
    		return DSH_CONNECTION_FAILED;
    	}
    	iprintf("NULL");
    	MyIP = Wifi_GetIPInfo(&gateway, &mask, &dns1, &dns2);
    }
    else
    {
    	if(ret != -1)
    	{
    		iprintf("AP Connecting...\n");
    		while(j != ASSOCSTATUS_ASSOCIATED && j!= ASSOCSTATUS_CANNOTCONNECT)
    		{
    			oldstatus = j;
    			j = Wifi_AssocStatus();
    			if(oldstatus != j) {
    				iprintf("%s\n",ASSOCSTATUS_STRINGS[j]);
    				if(j == ASSOCSTATUS_ASSOCIATED )
    				{
    					Wifi_SetIP(MyIP.s_addr,gateway.s_addr,mask.s_addr,dns1.s_addr,dns2.s_addr);
    				}
    			}
    		}
    	}
    }

    iprintf("WIFI: Connected to AP\n"); // Mark that WIFI is available
    iprintf("IP  : %s\n", inet_ntoa(MyIP));
    iprintf("SVIP: %s\n", DOWNLOAD_IP);
#endif

    channel->name = channel_name(CHANNEL_WIFI);
    channel->number =number_connect; // number connect
    channel->connect = wifi_connect;
    channel->read = wifi_read;
    channel->write = wifi_write;
    channel->disconnect = wifi_disconnect;
    channel->cleanup = wifi_cleanup;
    return 0;
}
