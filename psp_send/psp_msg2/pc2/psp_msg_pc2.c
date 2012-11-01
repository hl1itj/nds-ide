#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
	int client_socket;
	int target_socket;
	struct sockaddr_in server_addr;
	struct sockaddr_in target_addr;
	struct sockaddr_in my_addr;
	
	char buf[BUF_SIZE] = "Hello World!\n";

	if (argc != 3) {
		fprintf(stderr, "Usage : %s [server's IP] [server's port]", argv[0]);
		exit(1);
	}

	//socket1
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		fprintf(stderr, "socket() error!\n");
		exit(1);
	}

	//connect() socket1 to SV
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	//=> socket1(sAddr1(SV))
	if (-1 == (connect(client_socket, (struct sockaddr*) &server_addr,
			sizeof(server_addr)))) {
		fprintf(stderr, "connect() error!\n");
		close(client_socket);
		exit(1);
	}
	//	memset(&buf, 0x00, sizeof(buf));
	printf("Connect to Server Success!\n");
	printf("Server's IP	: %s\n", inet_ntoa(server_addr.sin_addr));
	printf("Server's port	: %d\n", server_addr.sin_port);

	//receive sockaddr from SV => sAddr2(PC1)
	memset(&target_addr, 0x00, sizeof(target_addr));
	read(client_socket, &target_addr, sizeof(target_addr));

	printf("Receive IP	: %s\n", inet_ntoa(target_addr.sin_addr));
	printf("Receive port	: %d\n", target_addr.sin_port);
	
	//receive sockaddr from SV => sAddr2(PC2)	
	memset(&my_addr, 0x00, sizeof(my_addr));
	read(client_socket, &my_addr, sizeof(my_addr));

	printf("My IP\t\t: %s\n", inet_ntoa(my_addr.sin_addr));
	printf("My port\t\t: %d\n", my_addr.sin_port);

	close(client_socket);

	//make socket2
	target_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		fprintf(stderr, "socket() error!\n");
		exit(1);
	}
	
	sleep(2);
	if (-1 == (connect(target_socket, (struct sockaddr*) &target_addr,
			sizeof(target_addr)))) {
		fprintf(stderr, "connect() error!\n");
		close(target_socket);
		exit(1);
	}
	/*
	//bind socket2 to sAddr2(PC1)
	if (-1 == bind(target_socket, (struct sockaddr*) &target_addr, sizeof(target_addr))) 
	{
		close(client_socket);
		close(target_socket);

		fprintf(stderr, "bind() Error!\n");
		exit(1);
	}
*/

	printf("Connect OK!\n");
	
	sleep(1);
	//receive message to socket2
	memset(&buf, 0x00, sizeof(buf));
	read(target_socket, &buf, BUF_SIZE);
	printf("Recv Msg : %s\n", buf);

	printf("Program END\n");
	
	close(target_socket);

	return 0;
}
