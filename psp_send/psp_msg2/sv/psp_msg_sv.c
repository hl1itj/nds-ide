#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char **argv)
{
	int pc1_socket, pc2_socket, server_socket;
	int sockaddr_size;

	struct sockaddr_in pc1_addr, pc2_addr, server_addr;

	char buf[BUF_SIZE];

//make sAddr1
	server_socket  = socket( PF_INET, SOCK_STREAM, 0);
	if( -1 == server_socket)
	{
		fprintf(stderr, "socket() error!\n");
		exit( 1);
	}
//set sAddr1 about SV(itself) == sAddr1(SV)
	memset( &server_addr, 0x00, sizeof(server_addr) );
	server_addr.sin_family	= AF_INET;
	server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	server_addr.sin_port	= htons(4232);
//bind socket1 to sAddr1(SV) == socket1(sAddr1(SV))
	if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		fprintf(stderr, "bind() error!\n");
		close(server_socket);
		exit(1);
	}

	while(1)
	{
//listen() socket1(SV)
		if( -1 == listen(server_socket, 5))
		{
			fprintf(stderr, "listen() error!\n");
			exit(1);
		}
		printf("Ready to Connect!\n");
		printf("port : %d\n", server_addr.sin_port);
//make sAddr2
		sockaddr_size = sizeof(pc2_addr);
		memset( &pc2_addr, 0x00, sizeof(pc2_addr) );
//accept socket1 from PC2 to sAddr2 == sAddr2(PC2)
//	 and bind socket2-sAddr2(PC2)
//=> socket2 = accept(socket1, (struct sockaddr*)&sAddr2, &sAddr_size);
//=> socket2(sAddr2(PC2))
		pc2_socket = accept(server_socket, (struct sockaddr*)&pc2_addr, &sockaddr_size);
		if( -1 == pc2_socket )
		{
			fprintf(stderr, "PC2 accept() error!\n");
			close(server_socket);
			close(pc2_socket);
			exit(1);
		}
		printf("PC2's Accept Complete\n");
		printf("IP	: %s\n", inet_ntoa(pc2_addr.sin_addr));
		printf("port	: %d\n", pc2_addr.sin_port);

//listen() socket1(SV)
/*
		if( -1 == listen(server_socket, 5))
		{
			fprintf(stderr, "listen() error!\n");
			exit(1);
		}
*/
//make sAddr3
		sockaddr_size = sizeof(pc1_addr);
		memset( &pc1_addr, 0x00, sizeof(pc1_addr) );
//accept socket1 from PC1 to sAddr3
//=> socket3(sAddr3(PC1))
		pc1_socket = accept(server_socket, (struct sockaddr*)&pc1_addr, &sockaddr_size);
		if( -1 == pc2_socket )
		{
			fprintf(stderr, "PC2 accept() error!\n");
			close(server_socket);
			close(pc2_socket);
			exit(1);
		}
		printf("PC1's Accept Complete\n");
		printf("IP	: %s\n", inet_ntoa(pc1_addr.sin_addr));
		printf("port	: %d\n\n", pc1_addr.sin_port);


//send sAddr2, 3(PC2) to PC1
		printf("send PC2's Addr to PC1, PC2\n");
		write(pc1_socket, &pc2_addr, sizeof(pc2_addr));
		write(pc1_socket, &pc1_addr, sizeof(pc1_addr));
		
//send sAddr2, 3(PC1) to PC2
		printf("send PC1's Addr to PC1, PC2\n");		
		write(pc2_socket, &pc1_addr, sizeof(pc1_addr));
		write(pc2_socket, &pc2_addr, sizeof(pc2_addr));

//END
		close(pc1_socket);
		close(pc2_socket);
		printf("PC1-PC2 Connect END\n\n\n");
	}
	return 0;
}
