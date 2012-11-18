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

#include <winsock2.h>
#include <windows.h>
//#include <ws2tcpip.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <io.h>


// download.h include in the usbdown or wifidown
//#include "C:\nintendo\trunk\src\usbdown\source\download.h"
#include "download.h"

#pragma comment(lib, "ws2_32.lib")

//typedef int SOCKET;

//#define INVALID_SOCKET	-1
//#define SOCKET_ERROR	-1

//fuction Override
//#define closesocket close
//#define Sleep sleep

static SOCKET data_socket;
static struct header    FHeader;
static struct response  FResp;

int   Channel;  // 't' : tcp, 's' : Serial, 'u' : USB
char *Target;   // IP address or host for 't', COMx for 's'
int   Port;     // TCP port number for 't'

//HANDLE S_Port;

#define CHANNEL_TCP     't'
#define CHANNEL_SERIAL  's'
#define CHANNEL_USB     'u'
/*
void
TestSerial(void)
{
    int size;
    char ch;

    while (1) {
        if (_kbhit()) {
            ch = _getch();
            WriteFile(NULL, &ch, 1, &size, NULL);
            if (size != 1)
                printf("(%d)", size);
        }
        ReadFile(NULL, &ch, 1, &size, NULL);
        if (size == 1)
            printf("%c", ch);
        //else
        //    printf("[%d]", size);
    }       
}
 */
int
connect_nds()
{
#ifdef WIN32
	SOCKADDR_IN nds_sin;
	WSADATA wsaData;
	DCB PortDCB;
	COMMTIMEOUTS Timeout;
#endif

	//struct sockaddr_in nds_sin;

	switch (Channel) {
	case CHANNEL_TCP :
		
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR) {
            fprintf(stderr, "Error at WSAStartup()\n");
            return -1;
        }
		
		if ((data_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
			fprintf(stderr, "Error at socket(): %d\n", data_socket);
			//fprintf(stderr, "Error at socket(): %ld\n", WSAGetLastError());
			return -1;
		}
		else
			printf("socket success");

		nds_sin.sin_family = AF_INET;
		nds_sin.sin_addr.s_addr = inet_addr(Target);
		nds_sin.sin_port = htons(Port);

		if (connect(data_socket, (struct sockaddr *)&nds_sin, sizeof(nds_sin)) == SOCKET_ERROR) {
			printf("Failed to connect Nintendo thru %s errorcode:%d.\n", Target, data_socket);
			closesocket(data_socket);
			return -1;
		}

		break;
	case CHANNEL_SERIAL :
		/*
        S_Port = CreateFile (Target, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        PortDCB.DCBlength = sizeof (DCB); 

        GetCommState (S_Port, &PortDCB);             // Get the default port setting information.

        PortDCB.BaudRate = 115200;					// Current baud 
        PortDCB.fBinary = TRUE;                     // Binary mode; no EOF check 
        PortDCB.fParity = FALSE;                    // Disable parity checking 
        PortDCB.fOutxCtsFlow = FALSE;               // No CTS output flow control 
        PortDCB.fOutxDsrFlow = FALSE;               // No DSR output flow control 
        PortDCB.fDtrControl = DTR_CONTROL_DISABLE;  // DTR flow control type 
        PortDCB.fDsrSensitivity = FALSE;            // DSR sensitivity 
        PortDCB.fTXContinueOnXoff = TRUE;           // XOFF continues Tx 
        PortDCB.fOutX = FALSE;                      // No XON/XOFF out flow control 
        PortDCB.fInX = FALSE;                       // No XON/XOFF in flow control 
        PortDCB.fErrorChar = FALSE;                 // Disable error replacement 
        PortDCB.fNull = FALSE;                      // Disable null stripping 
        PortDCB.fRtsControl = RTS_CONTROL_DISABLE;  // RTS flow control 
        PortDCB.fAbortOnError = FALSE;              // Do not abort reads/writes on error
        PortDCB.ByteSize = 8;                       // Number of bits/byte, 4-8 
        PortDCB.Parity = NOPARITY;                  // 0-4=no,odd,even,mark,space 
        PortDCB.StopBits = ONESTOPBIT;              // 0,1,2 = 1, 1.5, 2 

        if (!SetCommState (S_Port, &PortDCB)) {
            fprintf(stderr, "Serial Port Confifuration Error\n");
            return -1;
        }

        Timeout.ReadIntervalTimeout = 0;
        Timeout.ReadTotalTimeoutMultiplier = 0;
        Timeout.ReadTotalTimeoutConstant = 1;
        Timeout.WriteTotalTimeoutMultiplier = 0;
        Timeout.WriteTotalTimeoutConstant = 1;

        SetCommTimeouts(S_Port, &Timeout);
        printf("Serial port:%s Open\n", Target);
        //TestSerial();*/
		return 0;
	case CHANNEL_USB :
		//OpenUSBChannel();
		//break;
		printf("Failed to connect Nintendo thru USB\n");
		return -1;
	default :
		fprintf(stderr, "Illegal Channel Specified\n");
		return -1;
	}
	return 0;
}

void
disconnect_nds()
{
	switch (Channel) {
	case CHANNEL_TCP :
		closesocket(data_socket);
		//WSACleanup();
		break;
	case CHANNEL_SERIAL :
		//CloseHandle(S_Port);
		break;
	case CHANNEL_USB :
		//OpenUSBChannel();
		break;
	default :
		break;
	}
}

int
send_data(unsigned char *buf, int len)
{
	int ret;
	int size, count = len;

	loop:
	while (count) {
		switch (Channel) {
		case CHANNEL_TCP :
			if ((size = send(data_socket, buf, count, 0)) <= 0) {
				printf("size = %d", size); //yongsu
				printf("send error: %s\n",strerror(errno)); //yongsu
				fprintf(stderr, "TCP Send Error\n");
				return -1;  // Error
			}
			break;
		case CHANNEL_SERIAL :
			/*
            if (!(ret = WriteFile(NULL, buf, count, &size, NULL))) {
				goto loop;
				fprintf(stderr, "\nSize : %d\n", size);
                fprintf(stderr, "Serial Write Error\n");
                return -1;
            }*/
			break;
		case CHANNEL_USB :
			//write_to usb channel();
			break;
		default :
			break;
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
		switch (Channel) {
		case CHANNEL_TCP :
			if ((size = recv(data_socket, buf, count, 0)) < 0) {
				fprintf(stderr, "TCP Recv Error\n");
				return -1;  // Error
			}
			break;
		case CHANNEL_SERIAL :
			/*
            if (!ReadFile(NULL, buf, count, &size, NULL)) {
                fprintf(stderr, "Serial Read Error\n");
                return -1;  // Error
            }*/
			break;
		case CHANNEL_USB :
			//write_to usb channel();
			break;
		default :
			break;
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
		switch (Channel) {
		case CHANNEL_TCP :
			if ((size = recv(data_socket, buf, count, 0)) < 0) {
				fprintf(stderr, "TCP Recv Error\n");
				return -1;  // Error
			}
			break;
		case CHANNEL_SERIAL :
			//ReadFile(NULL, buf, read_size, &size, NULL);
			break;
		case CHANNEL_USB :
			//write_to usb channel();
			break;
		default :
			break;
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

	//struct stat statinfo;

	if (argc < 5) {
		usage:  fprintf(stderr, "usage: dshdown t|s|u FILENAME FRD|XXX nds_ip_address|COMx [tcp_port]\n");
		fprintf(stderr, "               t:tcp, s:serial, u:usb\n");
		fprintf(stderr, "               F|X:Flash, R|X:Run, D|X:Run w/ Debug After download\n");
		return -1;
	}

	if (argv[1][1])
		goto usage;
	Channel = argv[1][0];
	switch (argv[1][0]) {
	case CHANNEL_TCP :
#define USE_TCP
		if ((argc != 5) && (argc != 6))
			goto usage;
		Target = argv[4];
		if (argc == 6) {
			if (sscanf(argv[5], "%d", &Port) != 1) {
				fprintf(stderr, "Invalid port number\n");
				goto usage;
			}
		} else
			Port = DOWNLOAD_PORT;
		BUF_SIZE = 2048;
		break;
	case CHANNEL_SERIAL :
#define USE_SERIAL
		if (argc != 5)
			goto usage;
		Target = argv[4];
		BUF_SIZE = 8;
		break;
	case CHANNEL_USB :
#define USE_USB
		BUF_SIZE = 64;
		break;
	default  :
		goto usage;
	}

	fname = strrchr(argv[2], '\\');
	if (fname) {
w_path: fname++;
		if (strlen(fname) == 0) {
			fprintf(stderr, "No File Specified\n");
			goto usage;
		}
		strcpy(FHeader.filename, fname);
	} else {
		fname = strrchr(argv[2], '/');
		if (fname)
			goto w_path;
		strcpy(FHeader.filename, argv[2]);
	}

	if ((argv[3][0] != 'F') && (argv[3][0] != 'X'))
		goto usage;
	else
		FHeader.flash = argv[3][0];
	if ((argv[3][1] != 'R') && (argv[3][1] != 'X'))
		goto usage;
	else
		FHeader.run = argv[3][1];
	if ((argv[3][2] != 'D') && (argv[3][2] != 'X'))
		goto usage;
	else
		FHeader.debug = argv[3][2];

	if ((f2send = fopen(argv[2], "r+b")) == NULL) {
		perror(argv[1]);
		return -1;
	}
	
	/*
	stat(argv[2], &statinfo);
	
	if (statinfo.st_size > MAX_FILE_SIZE) {
		fprintf(stderr, "File size too big : MAX = %dBytes\n", statinfo.st_size);
		goto leave0;
	}
	*/
	
    if (_filelength(_fileno(f2send)) > MAX_FILE_SIZE) {
        fprintf(stderr, "File size too big : MAX = %dBytes\n", MAX_FILE_SIZE);
        goto leave0;
    }

	//sprintf(FHeader.filelength, "%d", statinfo.st_size);
	sprintf(FHeader.filelength, "%d", _filelength(_fileno(f2send)));

	ret = -1;   // if return in error clause, ret = -1

	BUF = (unsigned char*)malloc(sizeof(char) * BUF_SIZE + 1);

	printf("Downloader for Dall Shell\n(c) Copyright 2009, ");
	printf("by Minsuk Lee (minsuk@hansung.ac.kr)\n");
	if (connect_nds() < 0) {
		goto leave0;
	}

	if (send_data(MAGIC_DOWNLOAD_STRING, MAGIC_LEN) != MAGIC_LEN) {
		printf("Failed to send Magic String\n");
		goto leave1;
	}

	printf("Magic Download Code Sent\n");

	if (send_data((unsigned char *)&FHeader, sizeof(FHeader)) != sizeof(FHeader)) {
		printf("Failed to send File Header\n");
		goto leave1;
	}
	printf("Header Sent\n");

	switch (Channel) {
	case CHANNEL_SERIAL:
	case CHANNEL_USB:
		if (recv_data_byte((unsigned char *)&FResp, sizeof(FResp)) != sizeof(FResp)) {
			printf("Failed to Recv Reponse\n");
			goto leave1;
		}
		break;
	case CHANNEL_TCP:
		if (recv_data((unsigned char *)&FResp, sizeof(FResp)) != sizeof(FResp)) {
			printf("Failed to Recv Reponse\n");
			goto leave1;
		}
		break;
	}

	if (strncmp(FResp.code, "000", 3)) {
		FResp.code[3] = 0;
		printf("Abort: %s: %s\n", FResp.code, FResp.string);
		goto leave1;
	}

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
			Sleep(0);
			break;
		}
		total_sent += size;
		printf("\b\b\b\b\b\b\b\b\b\\b\b\b\b\b\b\b\b\b\b%dB Sent", total_sent);
		fflush(stdout);
	}
	printf("\n");

	sprintf(BUF, "%d", checksum);
	fprintf(stderr, "checksum: %s\n", BUF);
	if (send_data(BUF, MAX_FILE_LENGTH_LEN) != MAX_FILE_LENGTH_LEN) {
		printf("Failed to send Checksum\n");
		goto leave1;
	}
	printf("Checksum Sent\n");

	switch (Channel) {
	case CHANNEL_SERIAL:
	case CHANNEL_USB:
		if (recv_data_byte((unsigned char *)&FResp, sizeof(FResp)) != sizeof(FResp)) {
			printf("Failed to Recv Reponse\n");
			goto leave1;
		}
		break;
	case CHANNEL_TCP:
		if (recv_data((unsigned char *)&FResp, sizeof(FResp)) != sizeof(FResp)) {
			printf("Failed to Recv Reponse\n");
			goto leave1;
		}
		break;
	}

	if (strncmp(FResp.code, "000", 3)) {
		FResp.code[3] = 0;
		printf("Abort: %s: %s\n", FResp.code, FResp.string);
		goto leave1;
	}

	if (send_data(MAGIC_CLEANUP_STRING, MAGIC_LEN) != MAGIC_LEN) {
		printf("Failed to send Magic String\n");
		goto leave1;
	}
	printf("Magic Cleanup Code Sent\n");

	//printf("Transfer file: '%s' (%dB) Done!\n", FHeader.filename, statinfo.st_size);
	printf("Transfer file: '%s' (%dB) Done!\n", FHeader.filename, _filelength(f2send));

	ret = 0;

leave1:
	free(BUF);
	disconnect_nds();
leave0:
	fclose(f2send);
	return ret;
}

