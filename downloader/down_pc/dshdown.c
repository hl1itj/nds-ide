/************************************************************************
*
*                    File downloader for Nintendo DS
*                   ---------------------------------
*           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
*                      All Rights Reserved.
*
************************************************************************/

/* FILE        : dshdown.c
*
* Description : File Downloader to Nintendo DS
*
* Created - 2009-12-27 by Minsuk Lee
* Revised - 2009-XX-XX 
*         - 2009-XX-XX
* Read    - 2009-XX-XX
*/

#ifdef WIN32

#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <io.h>

#else

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#endif

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// download.h include in the usbdown or wifidown
//#include "C:\nintendo\trunk\src\usbdown\source\download.h"
#include "download.h"

#pragma comment(lib, "ws2_32.lib")

#ifndef WIN32

#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1

static struct stat statinfo;

#endif

static int data_socket;
static struct header    FHeader;
static struct response  FResp;


int   Channel;  // 't' : tcp, 's' : Serial, 'u' : USB
char *Target;   // IP address or host for 't', COMx for 's'
int   Port;     // TCP port number for 't'
//int filelength;


#define CHANNEL_TCP     't'
#define CHANNEL_SERIAL  's'
#define CHANNEL_USB     'u'



int
connect_nds()
{
#ifdef WIN32
	SOCKADDR_IN nds_sin;
	WSADATA wsaData;
#else
	struct sockaddr_in nds_sin;
#endif
	
#ifdef WIN32
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
	{
		fprintf(stderr, "Error at WSAStartup()\n");
		return -1;
	}
#endif
	if ((data_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		fprintf(stderr, "Error at socket(): %d\n", data_socket);
		return -1;
	}
	
	nds_sin.sin_family = AF_INET;	
	nds_sin.sin_addr.s_addr = inet_addr(Target);	
	nds_sin.sin_port = htons(Port);
	
	if (connect(data_socket, (struct sockaddr*)&nds_sin, sizeof(nds_sin)) == SOCKET_ERROR)
	{
		printf("Failed to connect Nintendo thru %s errorcode:%d.\n", Target, data_socket);
		close(data_socket);
		return -1;
	}

	printf("Connection Made to %s:%d\n", Target, Port);
	
    return 0;
}

void
disconnect_nds()
{
	close(data_socket);
}

int
send_data(unsigned char *buf, int len)
{
    int ret;
    int size, count = len;
	
loop:
    while (count) {
		
		if ((size = send(data_socket, buf, count, 0)) <= 0) {
			fprintf(stderr, "TCP Send Error\n");
			
			return -1;  // Error
		}
		
        buf += size;
        count -= size;
    }
    return len;
}

int
recv_data(unsigned char *buf, int count)
{
    int size, tread = 0;
	
    while (count) {
		
		if ((size = recv(data_socket, buf, count, 0)) < 0) {
			fprintf(stderr, "TCP Recv Error\n");
			return -1;  // Error
		}
		
        if (size == 0)
            break;
		
        buf += size;
        count -= size;
        tread += size;
    }
    return tread;
}

int
recv_data_byte(unsigned char *buf, int count)
{
    int read_size = 1;
    int size, tread = 0;
	
    while (count) {
		
		if ((size = recv(data_socket, buf, count, 0)) < 0) {
			fprintf(stderr, "TCP Recv Error\n");
			return -1;  // Error
		}
		
        if(size == 1)
		{
			buf += read_size;
			count -= read_size;
			tread += read_size;
		}
    }
    return tread;
}

int
main(int argc, char* argv[])
{
    FILE *f2send;
    char *fname;
    unsigned char *BUF;
    int BUF_SIZE;
	
    int total_sent, size, ret, i, checksum;
	
    if (argc < 5) {
usage:  fprintf(stderr, "usage: dshdown FILENAME FRD|XXX server_ip_address| [tcp_port]\n");
        fprintf(stderr, "F|X:Flash, R|X:Run, D|X:Run w/ Debug After download\n");
        return -1;
    }
	
	//   Channel = argv[1][0];
	
#define USE_TCP
	if ((argc != 4) && (argc != 5))
		goto usage;
	Target = argv[3];
	if (argc == 5) {
		if (sscanf(argv[4], "%d", &Port) != 1) {
			fprintf(stderr, "Invalid port number\n");
			goto usage;
		}
	} else
		Port = DOWNLOAD_PORT;
	BUF_SIZE = 2048;
	
	
    fname = strrchr(argv[1], '\\');
    if (fname) {
w_path: fname++;
        if (strlen(fname) == 0) {
            fprintf(stderr, "No File Specified\n");
            goto usage;
        }
        strcpy(FHeader.filename, fname);
    } else {
        fname = strrchr(argv[1], '/');
        if (fname)
            goto w_path;
        strcpy(FHeader.filename, argv[1]);
    }
	
    if ((argv[2][0] != 'F') && (argv[2][0] != 'X'))
        goto usage;
    else
        FHeader.flash = argv[2][0];
    if ((argv[2][1] != 'R') && (argv[2][1] != 'X'))
        goto usage;
    else
        FHeader.run = argv[2][1];
    if ((argv[2][2] != 'D') && (argv[2][2] != 'X'))
        goto usage;
    else
        FHeader.debug = argv[2][2];
	
    if ((f2send = fopen(argv[1], "r+b")) == NULL) {
        perror(argv[1]);
        return -1;
    }

#ifdef WIN32
	if (_filelength(_fileno(f2send)) > MAX_FILE_SIZE) {
        	fprintf(stderr, "File size too big : MAX = %dBytes\n", MAX_FILE_SIZE);
       		goto leave0;
    	}
	sprintf(FHeader.filelength, "%d", _filelength(_fileno(f2send)));
#else
	fstat(fileno(f2send),&statinfo);

	if (statinfo.st_size > MAX_FILE_SIZE) {
		fprintf(stderr, "File size too big : MAX = %dBytes\n", MAX_FILE_SIZE);
		goto leave0;
    	}
	sprintf(FHeader.filelength, "%d", statinfo.st_size);
#endif
	
    ret = -1;   // if return in error clause, ret = -1
	
	BUF = (unsigned char*)malloc(sizeof(char) * BUF_SIZE + 1);
	
    printf("Downloader for Dall Shell\n(c) Copyright 2009, ");
	printf("by Minsuk Lee (minsuk@hansung.ac.kr)\n");
    if (connect_nds() < 0) {
        goto leave0;
    }
	
    if (send_data((unsigned char *)&FHeader, sizeof(FHeader)) != sizeof(FHeader)) {
        printf("Failed to send File Header\n");
        goto leave1;
    }
    printf("Header Sent\n");
    
	
    printf("Start Send Data\n");
    total_sent = checksum = 0;
    while (!feof(f2send)) {
        size = fread(BUF, 1, BUF_SIZE, f2send);
        if (!size)
            break;
        for (i = 0; i < size; i++)
            checksum += BUF[i];
		if (send_data(BUF, size) != size) {
            fprintf(stderr, "send error\n");
            goto leave1;
        }
		switch (Channel) {
		case CHANNEL_SERIAL:
		case CHANNEL_USB:
			//sleep(0);
			break;
		}
        total_sent += size;
        printf("\b\b\b\b\b\b\b\b\b\\b\b\b\b\b\b\b\b\b\b");
        printf("%dB Sent", total_sent);
		fflush(stdout);
    }
    printf("\n");
	
    sprintf(BUF, "%d", checksum);
	//fprintf(stderr, "checksum: %s\n", BUF);
    if (send_data(BUF, MAX_FILE_LENGTH_LEN) != MAX_FILE_LENGTH_LEN) {
        printf("Failed to send Checksum\n");
        goto leave1;
    }
    printf("Checksum Sent\n");
	
#ifdef WIN32
	printf("Transfer file: '%s' (%dB) Done!\n", FHeader.filename, _filelength(_fileno(f2send)));
#else
	printf("Transfer file: '%s' (%dB) Done!\n", FHeader.filename, statinfo.st_size);
#endif	
    
    
    ret = 0;
	
leave1:
	free(BUF);
    disconnect_nds();
leave0:
    fclose(f2send);
	
    return ret;
}



