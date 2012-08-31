#include "EndTimer.h"

int endTimerFlag = 0;
void (&ETimerHandler)(exitinfo*);
struct itimerval endtimertv;
exitinfo* exitInformation;

int CreateEndTimer(void (*handler)(exitinfo*),int interval,exitinfo* exitInfo)
{
	pthread_t timer_thread;
	int res;

	ETimerHandler = handler;
	exitInformation = exitInfo;
	//SIGALRM이 발생할 때 불려지게될 함수를 등록한다.
	if(signal( SIGALRM,EndTimerHandler ) == SIG_ERR){
			LogMessage("Signal Setting Error.");
			return -1;
	}

	endTimerFlag = 1;

	//시간의 interval를 확인하는 쓰레드를 생성한다.
	//interval 만큼 시간이 되었을 경우 SIGALRM을 발생시킨다.
	res = pthread_create(&timer_thread,NULL,(void *)CheckTimerInterval,(void *)&interval);
	if(res !=0){
		LogMessage("TimerThread creation failed.");
		return -1;
	}
#ifdef DEBUG
	printf("End Timer Start\n");
#endif

	return 0;
}
/**
 * SIGALRM이 발생할 때 불려지게 되는 함수.
 */
void EndTimerHandler()
{
#ifdef DEBUG
	printf("End Timer Exit\n");
#endif
	endTimerFlag = 0;
	ETimerHandler(exitInformation);
}

/**
 * interval 만큼 시간이 되었을 경우 SIGALRM을 발생시키는 함수.
 */
void CheckTimerInterval(void *interval){
	long int timerInterval = *((int*)interval);
	EndTimerStart(timerInterval);
}

void EndTimerStart(long int timerInterval){
	struct timeval *tv;

	tv = &endtimertv.it_interval;
	tv->tv_sec = 0;
	tv->tv_usec = 0;

	EndTimerInterval(&endtimertv.it_value,timerInterval);

	setitimer(ITIMER_REAL,&endtimertv,NULL);
}

void EndTimerInterval(struct timeval *tv,long int timerInterval){

	time_t t1,t2;
	struct timeval tv2;

	gettimeofday(&tv2,NULL);
	t2 = (time_t)tv2.tv_sec;

	t1 = t2 + timerInterval;

	tv->tv_sec = t1 - tv2.tv_sec -1;
	tv->tv_usec = 1000000 - tv2.tv_usec;
	if(tv->tv_usec == 0) tv->tv_sec += 1;
}

/**
 * CalculateTimer가 끝난 여부를 알려준다.
 */
int GetEndTimerFlag(){
	return endTimerFlag;
}

/**
 * 타이머를 중단한다.
 */
void StopEndTimer(){
	endTimerFlag = 0;
	endtimertv.it_value.tv_sec = 0;
	endtimertv.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL,&endtimertv,NULL);
#ifdef DEBUG
	printf("End Timer Stop\n");
#endif
}
