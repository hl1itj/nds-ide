//create sock1
//
//listen(sock1) local
//fork
//{
//	if pid=0{
//		accept(sock2)
//		{
//		send(sock2, pc2_IP);
//		recv(sock2, pc2_port);
//		}
//		accept(sock3)
//		send(sock3, port);
//	loop:
//		recv(sock3, buf);
//		if(buf == fail){
//			send(sock2, "fail")
//			goto loop
//		}else if(buf == OK){
//			END sequence start
//		}		
//	}
//}

#include "psp_loop.h"

int main(int argc, char **argv) {
	int pc1_socket, pc2_socket, server_socket;
	int sockaddr_size;

	unsigned short tmp_port;

	struct sockaddr_in pc1_addr, pc2_addr, server_addr;

	char buf[BUF_SIZE];

	//make sAddr1
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == server_socket) {
		fprintf(stderr, "socket() error!\n");
		exit(1);
	}
	//set sAddr1 about SV(itself) == sAddr1(SV)
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SV_PORT);
	//bind socket1 to sAddr1(SV) == socket1(sAddr1(SV))
	if (-1 == bind(server_socket, (struct sockaddr*) &server_addr,
			sizeof(server_addr))) {
		fprintf(stderr, "bind() error!\n");
		close(server_socket);
		exit(1);
	}

	while (1) {
		//listen() socket1(SV)
		if (-1 == listen(server_socket, 5)) {
			fprintf(stderr, "listen() error!\n");
			exit(1);
		}
		printf("Ready to Connect!\n");
		printf("port : %d\n", server_addr.sin_port);
		//make sAddr2
		sockaddr_size = sizeof(pc2_addr);
		memset(&pc2_addr, 0x00, sizeof(pc2_addr));

		pc2_socket = accept(server_socket, (struct sockaddr*) &pc2_addr,
				&sockaddr_size);
		if (-1 == pc2_socket) {
			fprintf(stderr, "PC2 accept() error!\n");
			close(server_socket);
			close(pc2_socket);
			exit(1);
		}
		printf("PC2's Accept Complete\n");
		printf("IP	: %s\n", inet_ntoa(pc2_addr.sin_addr));
		printf("port	: %d\n", pc2_addr.sin_port);


		//make sAddr3
		sockaddr_size = sizeof(pc1_addr);
		memset(&pc1_addr, 0x00, sizeof(pc1_addr));
		//accept socket1 from PC1 to sAddr3
		//=> socket3(sAddr3(PC1))
		pc1_socket = accept(server_socket, (struct sockaddr*) &pc1_addr,
				&sockaddr_size);
		if (-1 == pc2_socket) {
			fprintf(stderr, "PC2 accept() error!\n");
			close(server_socket);
			close(pc2_socket);
			exit(1);
		}
		printf("PC1's Accept Complete\n");
		printf("IP	: %s\n", inet_ntoa(pc1_addr.sin_addr));
		printf("port	: %d\n\n", pc1_addr.sin_port);

		printf("send PC2's Addr to PC1, PC2\n");
		write(pc2_socket, &pc2_addr.sin_addr, sizeof(pc2_addr.sin_addr));
		write(pc1_socket, &pc2_addr.sin_addr, sizeof(pc2_addr.sin_addr));

		do {
			printf("Transmission PC2's port to PC1\n");
			memset(&buf, 0x00, sizeof(buf));
			read(pc2_socket, &tmp_port, sizeof(buf));
			printf("Recv PC2's port : %d\n", tmp_port);
			write(pc1_socket, &tmp_port, sizeof(buf));

			memset(&buf, 0x00, sizeof(buf));
			read(pc1_socket, buf, sizeof(buf));
			printf("recv MSG from PC1 : %s\n", buf);
			write(pc2_socket, buf, sizeof(buf));

		} while (buf != MSG_OK);
		close(pc1_socket);
		close(pc2_socket);
		printf("PC1-PC2 Connect END\n\n\n");
	}
	return 0;
}
