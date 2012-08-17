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

#ifdef _WIN32
#define ioctl       ioctlsocket
#define errno       WSAGetLastError()
#define EWOULDBLOCK WSAEWOULDBLOCK
#pragma comment(lib, "ws2_32.lib")
#endif

static struct in_addr MyIP, gateway, mask, dns1, dns2;
static int data_socket = -1;

static
int
wifi_connect(void)
{
    int listen_socket;
    struct sockaddr_in sock_addr;
    int addr_len = sizeof (struct sockaddr_in);
    int ret;
    int one = 1;

    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        confirm_error("wifi: Listen Socket Open", 0);
        return DSH_CONNECTION_FAILED;
    }
    //iprintf("Socket Open\n");

    // Set Nonblocking Mode
    if ((ret = ioctl(listen_socket, FIONBIO, &one)) < 0) {
        confirm_error("wifi: Listen_Socket Non-blocking Mode Failed", 0);
        ret = DSH_CONNECTION_FAILED;
        goto leave;
    }

    sock_addr.sin_family = AF_INET;
    if (Config.single_host)
        sock_addr = Config.client;
    else
        sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(Config.port);

    if ((ret = bind(listen_socket, (struct sockaddr *)&sock_addr, addr_len)) < 0) {
        confirm_error("bind Error", 0);
        ret = DSH_CONNECTION_FAILED;
        goto leave;
    }
    if ((ret = listen(listen_socket, 1)) < 0) {
        confirm_error("listen Error", 0);
        ret = DSH_CONNECTION_FAILED;
        goto leave;
    }

    // minsuk: need to source code refactoring after multithreading is available
    while (1) {
        // We don't use timer, User can abort the connection anytime
        if (check_abort()) {
            confirm_error("User Abort", 0);
            ret = DSH_USER_ABORT;
            goto leave;
        }
        if ((data_socket = accept(listen_socket, (struct sockaddr *)&sock_addr, &addr_len)) >= 0)
            break;  // Connected !
        if (errno != EWOULDBLOCK) {
            iprintf("wifi: accept error code:%d\n", data_socket);
            confirm_error("Connection Failed", 0);
            ret = DSH_CONNECTION_FAILED;
            goto leave;
        }
    }
    // Set data_socket, Non-blocking mode
    if ((ret = ioctl(data_socket, FIONBIO, &one)) < 0) {
        confirm_error("Data Socket: Non-blocking mode Failed", 0);
        ret = DSH_CONNECTION_FAILED;
        closesocket(data_socket);
    }
leave:
    closesocket(listen_socket);
    listen_socket = -1;
    return ret;
}

static
int
wifi_read(void *buf, int count)
{
    int rcount;
    if (count == 0)
        return count;
    rcount = recv(data_socket, buf, count, 0);
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
    wcount = send(data_socket, buf, count, 0);
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
    closesocket(data_socket);
    data_socket = -1;
}

static
void
wifi_cleanup(void)
{
    if (data_socket >= 0)
        closesocket(data_socket);
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
    unsigned char BUF[6];
    int ret, i;


    iprintf("WIFI: Finding AP...\n");
    ret = Wifi_InitDefault(WFC_CONNECT);

    iprintf("MAC : ");
    Wifi_GetData(WIFIGETDATA_MACADDRESS, 6, BUF);
    for (i = 0; i < 6; i++) {
        iprintf("%02X", BUF[i]);
        if (i < 5)
            iprintf(":");
    }
    iprintf("\n");

    if(ret == 0) {
        confirm_error("WIFI:Failed to connect AP!\n", 0);
        return DSH_CONNECTION_FAILED;
    }
    iprintf("WIFI: Connected to AP\n"); // Mark that WIFI is available

    // Print my IP Somewhere
    MyIP = Wifi_GetIPInfo(&gateway, &mask, &dns1, &dns2);
    iprintf("IP  : %s\n", inet_ntoa(MyIP));
#endif

    channel->name = channel_name(CHANNEL_WIFI);
    channel->connect = wifi_connect;
    channel->read = wifi_read;
    channel->write = wifi_write;
    channel->disconnect = wifi_disconnect;
    channel->cleanup = wifi_cleanup;
    return 0;
}
