/*
 ============================================================================
 Name        : psp_loop.c
 Author      : Hansung_EmbLab
 Version     :
 Copyright   : Free!
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "psp_loop.h"

int main(int argc, char **argv){
		
	int server_socket;
	int target_socket;
	struct sockaddr_in server_addr;
	struct sockaddr_in target_addr;
		
	char buf[BUF_SIZE] = "Hello World!\n";
/*
	if(argc != 2){
		fprintf(stderr, "Usage : %s [File to send]", argv[0]);
		exit(1);
	}
*/
	//create sock1
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(server_socket == -1){
		fprintf(stderr, "socket() error!\n");
		exit(1);
	}
	
//connect to server(sock1)
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	
	server_addr.sin_addr.s_addr = inet_addr(SV_IP);
	server_addr.sin_port = htons(SV_PORT);
	
	if(-1 == (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))))
	{
		fprintf(stderr, "connect() error!\n");
		close(server_socket);
		exit(1);
	}
	printf("Connect to Server\n");

	memset(&target_addr, 0x00, sizeof(target_addr));
	target_addr.sin_family = AF_INET;
	read(server_socket, &target_addr.sin_addr.s_addr, sizeof(target_addr.sin_addr.s_addr));
	printf("Target IP   : %s\n", inet_ntoa(target_addr.sin_addr));
//create sock2
	if(target_socket = socket(PF_INET, SOCK_STREAM, 0) == -1){
		fprintf(stderr, "socket() error!\n");
		close(server_socket);
		close(target_socket);
		exit(1);
	}

loop:
//receive pc2_data
	read(server_socket, &target_addr.sin_port, sizeof(target_addr.sin_port));
	printf("Target port : %d\n", target_addr.sin_port);
//connect to pc2(sock2)
	printf("Connecting...");
	if(-1 == (connect(target_socket, (struct sockaddr*)&target_addr, sizeof(target_addr))))
	{
		printf("Connect Failed!... ");
		memset(&buf, 0x00, sizeof(buf));
		strcpy(buf, MSG_FAIL);
				
		write(server_socket, buf, strlen(buf));
		printf("Send MSG : %s\n", buf);
		goto loop;
	}
	memset(&buf, 0x00, sizeof(buf));
	strcpy(buf, MSG_OK);
	write(server_socket, buf, strlen(buf));
	printf("Connect to Target! Send MSG : %s\n", buf);
	
//file_send(sock2) start
	memset(&buf, 0x00, sizeof(buf));
	read(target_socket, &buf, sizeof(buf));
	printf("Recv MSG : %s\n", buf);
	
	memset(&buf, 0x00, sizeof(buf));
	strcpy(buf, "Hello world!\n");
	write(target_socket, buf, strlen(buf));

}

//connect to server(sock1)
//create sock2
//
//loop:
//receive pc2_data
//connect to pc2(sock2)
//if(connect failed)
//{
//	write(sock1, "Fail")
//	goto loop
//}
//write(sock1, "OK")
//file_send(sock2) start
