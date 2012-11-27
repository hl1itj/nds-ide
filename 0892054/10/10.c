#include <unistd.h>
#include <signal.h>
#include <stdio.h>


void timeover(int sigunum){
	printf("\n time over! \n");
}

main(){

	char buf[1024];
	char *alpha = "abcdefghijklmnopqrstuvwxyz";

	int timelimit;
	struct sigaction act;

	act.sa_handler = timeover;
	sigaction(SIGALRM, &act, NULL);

	printf("input sec \n");
	scanf("%d",&timelimit);

	alarm(timelimit);

	printf("START! \n > ");
	scanf("%s",buf);

	if(!strcmp(buf,alpha))
		printf("ok");
	else
		printf("fail");
}

