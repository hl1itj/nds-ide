/*
 * wifi.c
 *
 *  Created on: 2010. 7. 6.
 *      Author: 컴퓨터공학과
 */
#include "dall_conf.h"
#include "input.h"
#include "graphic.h"

extern int current_focus;
u64 current_flag;

int selected = 0;
int count = 0;
extern int numAp;

int wifi_searchAP()
{
	int i,k;
	int numAp;
	int temp = -1;
	bool ap_flag = true;

	iprintf("find AP\n");
	PA_InitText(0, 1);
	Wifi_InitDefault(INIT_ONLY);

	while(ap_flag)
	{
		Wifi_ScanMode();
		numAp = Wifi_GetNumAP();
		Stylus.X = 0;
		Stylus.Y = 0;

		count = numAp;

		// Listing
		for(k=0;k<14;k++) {
			for(i=0;i<14;i++) {
				if(i>=numAp) break;
				if(Wifi_GetAPData(i,&ap)==WIFI_RETURN_OK) {
					PA_SetTextTileCol(0,1);
					PA_OutputText(0,4,5+i,"%d = ",i);
					PA_OutputText(0,8,5+i,"%d%",(ap.rssi*100)/0xD0);
					PA_OutputText(0,20,5+i,"%s", ap.ssid);
					PA_OutputText(0,3,5+i,"%s", i == temp ? "*" : " ");
				}
				PA_OutputText(0,8,5+k,"                                                               ");
				PA_OutputText(0,20,5+k,"                                                              ");

				temp = check_location();
				if(temp!=-1) {
					memset(&ap, 0, sizeof(Wifi_AccessPoint));
					Wifi_GetAPData(selected, &ap);
					Wifi_DisableWifi();
					Config.dhcp=0; // ??
					ap_flag = false;
				}
				if(ap_flag == false) break;
			}
			PA_WaitForVBL();
			if(ap_flag == false) break;
		}
		PA_OutputText(0,18,19,"%s",ap.ssid);
	}
	return 0;
}

void wifi_Manualconf()
{
	PA_InitText(0,1);
	iprintf("Manual Configuration\n");

	PA_SetTextTileCol(0,1);
	PA_OutputText(0,10,6,"%s\n", ap.ssid);
	PA_OutputText(0,10,8,"%d.%d.%d.%d\n",(int)(Config.my_ip.s_addr&255),(int)((Config.my_ip.s_addr>>8)&255),(int)((Config.my_ip.s_addr>>16)&255),(int)((Config.my_ip.s_addr>>24)&255));
	PA_OutputText(0,10,10,"%d.%d.%d.%d\n",(int)(Config.gateway.s_addr&255),(int)((Config.gateway.s_addr>>8)&255),(int)((Config.gateway.s_addr>>16)&255),(int)((Config.gateway.s_addr>>24)&255));
	PA_OutputText(0,10,12,"%d.%d.%d.%d\n",(int)(Config.subnet.s_addr&255),(int)((Config.subnet.s_addr>>8)&255),(int)((Config.subnet.s_addr>>16)&255),(int)((Config.subnet.s_addr>>24)&255));
	PA_OutputText(0,10,14,"%d.%d.%d.%d\n",(int)(int)(Config.dns1.s_addr&255),(int)((Config.dns1.s_addr>>8)&255),(int)((Config.dns1.s_addr>>16)&255),(int)((Config.dns1.s_addr>>24)&255));
	PA_OutputText(0,10,16,"%d.%d.%d.%d\n",(int)(int)(Config.dns2.s_addr&255),(int)((Config.dns2.s_addr>>8)&255),(int)((Config.dns2.s_addr>>16)&255),(int)((Config.dns2.s_addr>>24)&255));
	PA_OutputText(0,10,19,"%d\n", (int)ap.channel);
}


void wifi_APconn()
{
	iprintf("Wifi AP Connecting\n");

	if(Config.dhcp == 1)
		Wifi_InitDefault(INIT_ONLY);
	PA_InitText(0,1);

	int ret;
	int j = 0;
	int oldstatus;

	PA_SetTextTileCol(0,1);

	if(Config.dhcp==0) // DHCP에서 IP할당
	{
		//Wifi_SetIP(Config.my_ip.s_addr,Config.gateway.s_addr,Config.subnet.s_addr,Config.dns1.s_addr,Config.dns2.s_addr);
		Wifi_SetIP(0,0,0,0,0);
	}
	else //if(Config.dhcp==1) // 전부 수동으로 입력
	{
		Wifi_SetIP(Config.my_ip.s_addr,Config.gateway.s_addr,Config.subnet.s_addr,Config.dns1.s_addr,Config.dns2.s_addr);
	}
	// DHCP 수정여부 확인 해보기

	ret = Wifi_ConnectAP(&ap,WEPMODE_NONE,0,0);


	if(ret != -1)
	{
		while(j != ASSOCSTATUS_ASSOCIATED && j!= ASSOCSTATUS_CANNOTCONNECT)
		{
			oldstatus = j;
			j = Wifi_AssocStatus();
			if(oldstatus != j) {
				PA_OutputText(0,6,11,"                             ");
				PA_OutputText(0,6,11,"%s\n",ASSOCSTATUS_STRINGS[j]);
				if(j == ASSOCSTATUS_ASSOCIATED )
				{
					if(Config.dhcp == 0)
						Config.my_ip = Wifi_GetIPInfo(&Config.gateway, &Config.subnet, &Config.dns1, &Config.dns2);
				}
			}
		}
	}
}

void wifi_Test_Dns() {
	iprintf("Test DNS\n");

	PA_InitText(0,1);
	PA_SetTextTileCol(0,1);
	int i;

	int check_num=-1;
	// Let's send a simple HTTP request to a server and print the results!
	// store the HTTP request for later
	const char * request_text =  "GET / HTTP/1.1\r\n\r\n";
	//        "Host: www.akkit.org\r\n"
	//        "User-Agent: Nintendo DS\r\n\r\n";

	// Find the IP address of the server, with gethostbyname
	// DNS를 이용해서 Name으로 IP를 얻는다.
	PA_OutputSimpleText(0,2,6, "DNS Resolve Start\n" );
	struct hostent * myhost = gethostbyname( "www.hansung.ac.kr" );
	PA_OutputText(0,2,8, "Found IP Address!\n[www.hansung.ac.kr]\n",myhost->h_addr_list[0] );

	// Tell the socket to connect to the IP address we found, on port 80 (HTTP)
	struct sockaddr_in sain;
	sain.sin_family = AF_INET;
	sain.sin_port = htons(80);
	sain.sin_addr.s_addr = inet_addr("128.134.165.1");

	// Create a TCP socket
	int my_socket;
	fd_set readfd;
	fd_set tempfd;
	struct timeval stTime;
	struct timeval stTempTime;
	int iRet;

	stTime.tv_sec = 100; // request time
	stTime.tv_usec = 0;

	my_socket = socket( AF_INET, SOCK_STREAM, 0 );

	PA_OutputSimpleText(0,2,10, "Try To Connect\n" );
	check_num = connect( my_socket,(struct sockaddr *)&sain, sizeof(sain));
	if(check_num)
	{
		PA_OutputSimpleText(0,10,10, "                                    ");
		PA_OutputSimpleText(0,10,10, "Connect Error!");
		shutdown(my_socket,0); // good practice to shutdown the socket.
	}
	else {
		PA_OutputSimpleText(0,10,10, "                                    ");
		PA_OutputSimpleText(0,10,10, "Connected to server!\n");

		// send our request
		send( my_socket, request_text, strlen(request_text), 0 );
		PA_OutputSimpleText(0,2,10, "                                    ");
		PA_OutputSimpleText(0,2,10, "Sent our request!\n");

		// Print incoming data
		PA_OutputSimpleText(0,2,12, "Printing incoming data:\n");

		int recvd_len;
		char incoming_buffer[256];

		PA_OutputSimpleText(0,2,14, "Recv Start\n");
		//FD_ZERO( &readfd );
		FD_SET( my_socket, &readfd );
		while( 1 )
		{
			tempfd = readfd;
			stTempTime = stTime;
			iRet = select( my_socket + 1, &tempfd, NULL, NULL, &stTempTime );
			if( iRet > 0 )
			{
				recvd_len = recv( my_socket, incoming_buffer, 255, 0 );
				PA_OutputText(0,2,16, "Recv End Size[%d]\n", recvd_len );
				if( recvd_len > 0 )
				{ // data was received!
					incoming_buffer[ recvd_len ] = 0; // null-terminate
					for(i=6;i<16;i++)
						PA_OutputSimpleText(0,2,i,"                                         ");
					PA_OutputSimpleText(0,2,6, incoming_buffer );
				}
				break;
			}
			// Time Expired
			else if( iRet == 0 )
			{
				PA_OutputSimpleText(0,2,18, "Time Expired If You Press B, Exit Receiving Process\n" );
				break;
			}
			else
			{
				PA_OutputSimpleText(0,2,18, "Error~~!!\n" );
				break;
			}
		}
		PA_OutputSimpleText(0,2,18, "Other side closed connection!\n");
		shutdown(my_socket,0); // good practice to shutdown the socket.
		close(my_socket); // remove the socket.
	}

}

int wifi_SaveConf() {
	iprintf("Save Configuration\n");

	PA_InitText(0,1);
	fatInitDefault(); // Initialize for Using FAT
	// ip를 받는 부분의 문자열 배열 크기 늘림. (11.08.29)
	char sav_ip[42],sav_gateway[42],sav_subnet[42],sav_dns1[42],sav_dns2[42];

	FILE *fp = fopen("DallShell_Conf.txt","wb");

	if(fp==NULL)
	{
		iprintf("File open error");
		return -1;
	}
	sprintf(sav_ip,"%d.%d.%d.%d",(int)(Config.my_ip.s_addr&255),(int)((Config.my_ip.s_addr>>8)&255),(int)((Config.my_ip.s_addr>>16)&255),(int)((Config.my_ip.s_addr>>24)&255));
	sprintf(sav_gateway,"%d.%d.%d.%d",(int)(Config.gateway.s_addr&255),(int)((Config.gateway.s_addr>>8)&255),(int)((Config.gateway.s_addr>>16)&255),(int)((Config.gateway.s_addr>>24)&255));
	sprintf(sav_subnet,"%d.%d.%d.%d",(int)(Config.subnet.s_addr&255),(int)((Config.subnet.s_addr>>8)&255),(int)((Config.subnet.s_addr>>16)&255),(int)((Config.subnet.s_addr>>24)&255));
	sprintf(sav_dns1,"%d.%d.%d.%d",(int)(int)(Config.dns1.s_addr&255),(int)((Config.dns1.s_addr>>8)&255),(int)((Config.dns1.s_addr>>16)&255),(int)((Config.dns1.s_addr>>24)&255));
	sprintf(sav_dns2,"%d.%d.%d.%d",(int)(int)(Config.dns2.s_addr&255),(int)((Config.dns2.s_addr>>8)&255),(int)((Config.dns2.s_addr>>16)&255),(int)((Config.dns2.s_addr>>24)&255));

	// Save for data in .txt file
	fprintf(fp,"%s\n%d\n%d\n%s\n%s\n%s\n%s\n%s",
			ap.ssid,
			ap.ssid_len,
			ap.channel,
			sav_ip,
			sav_gateway,
			sav_subnet,
			sav_dns1,
			sav_dns2);
	fclose(fp);
	return 0;
}


