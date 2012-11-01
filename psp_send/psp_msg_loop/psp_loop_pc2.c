//create sock1
//
//create sock2
//connect to SV(sock2)
//recv(sock2, IP)
//
//loop:
//
//port = random();
//listen(sock1, port)
//print(IP, port)
//
//send(sock2, port)
//
//recv(sock2, buf)
//if(buf == fail)
//{
//	close(sock1)
//	goto loop
//}

#include "psp_loop.h"

#include <pthread.h>
#include <time.h>

#define LOCAL_PORT_MIN 50000
#define LOCAL_PORT_MAX (0xFFFF - LOCAL_PORT_MIN)

int server_socket;
int local_socket;
int target_socket;
int local_port;

struct sockaddr_in server_addr;
struct sockaddr_in local_addr;
struct sockaddr_in target_addr;
struct sockaddr_in temp_addr;

char buf[BUF_SIZE] = "Hello World!\n";

void *t_function() {
	if (-1 == listen(local_socket, 5)) {
		fprintf(stderr, "listen() error!\n");
		//close sequence
		exit(1);
	}

	//make sAddr2
	int sockaddr_size = sizeof(target_addr);
	memset(&target_addr, 0x00, sizeof(target_addr));

	target_socket = accept(local_socket, (struct sockaddr*) &target_addr,
			&sockaddr_size);
	if (-1 == target_socket) {
		fprintf(stderr, "target accept() error!\n");
		close(server_socket);
		close(target_socket);
		exit(1);
	}

	printf("target Accept Complete\n");
	printf("IP	: %s\n", inet_ntoa(target_addr.sin_addr));
	printf("port	: %d\n", target_addr.sin_port);

	pthread_exit(0);
}

int main(int argc, char **argv) {

	srand( time(NULL));

	pthread_t p_thread;
	int thr_id = 1;
	int tmp;

	if (argc != 1) {
		fprintf(stderr, "Usage : %s ", argv[0]);
		exit(1);
	}
	//create sock1
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		fprintf(stderr, "socket() error!\n");
		exit(1);
	}
	//connect to server(sock1)
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SV_IP);
	server_addr.sin_port = htons(SV_PORT);

	if (-1 == (connect(server_socket, (struct sockaddr*) &server_addr,
			sizeof(server_addr)))) {
		fprintf(stderr, "connect() error!\n");
		close(server_socket);
		exit(1);
	}
	printf("Connect to Server\n");

	memset(&temp_addr, 0x00, sizeof(temp_addr));
	read(server_socket, &temp_addr.sin_addr, sizeof(temp_addr.sin_addr));
	printf("My IP : %s\n", inet_ntoa(temp_addr.sin_addr));

///////////////////////////////////
	do {
		memset(&local_addr, 0x00, sizeof(local_addr));
		local_addr.sin_family = AF_INET;
		local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		local_port = (rand() % LOCAL_PORT_MAX) + LOCAL_PORT_MIN;
		local_addr.sin_port = htons(local_port);

		printf("Listen port : %d\n", local_addr.sin_port);
		if (-1 == bind(local_socket, (struct sockaddr*) &local_addr,
				sizeof(local_addr))) {
			fprintf(stderr, "bind() error!\n");
			close(local_socket);
			exit(1);
		}
		thr_id = pthread_create(&p_thread, NULL, t_function, (void*) tmp);
		
		write(server_socket, &local_addr.sin_port, sizeof(local_addr.sin_port));

		memset(&buf, 0x00, sizeof(buf));
		read(server_socket, &buf, sizeof(buf));
		printf("Recv MSG : %s", buf);
		
		close(local_socket);
		pthread_cancel(p_thread);

	} while (buf != MSG_OK);
	pthread_join(p_thread, (void*) &tmp);
	
///////////////////
	printf("Connect to Target!\n");

	//socket test
	memset(&buf, 0x00, sizeof(buf));
	strcpy(buf, "Hello world!!!\n");
	write(target_socket, buf, sizeof(buf));

	memset(&buf, 0x00, sizeof(buf));
	read(target_socket, &buf, sizeof(buf));
	printf("Recv MSG : %s\n", buf);

	close(server_socket);
	close(target_socket);
	close(local_socket);

	return 0;
}