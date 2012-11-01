#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SV_IP "127.0.0.1"
#define SV_PORT 49193
#define MSG_FAIL "Failed"
#define MSG_OK	 "OK"