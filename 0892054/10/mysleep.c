#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void tmp(){
}

main(){
	struct sigaction act;
	int time;

	sigfillset(&(act.sa_mask));
	act.sa_handler = tmp;
	sigaction(SIGALRM, &act, NULL);

	printf("input sec \n");
	scanf("%d",&time);
	alarm(time);
	pause();
	printf("%d초 잠\n", time);
}
