#include <stdio.h>
#include <signal.h>


static void unused_func(void)
{
	printf("%s\n", __FUNCTION__);
}

static void sighandler(int sig)
{
	printf("%s=n", __FUNCTION__);
}

int main(void)
{
	struct sigaction sa;

	sa.sa_handler = sighandler;
	sigaction(SIGUSR1, &sa, NULL);

	printf("before raise()\n");
	raise(SIGUSR1);
	printf("after raise()\n");

	return 0;
}
