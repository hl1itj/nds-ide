#ifndef ENDTIMER_H_
#define ENDTIMER_H_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include "../util/constant.h"

int CreateEndTimer(void (*)(), int, exitinfo*);
void CheckTimerInterval(void*);
void EndTimerHandler();
void EndTimerStart(long int);
void EndTimerInterval(struct timeval*, long int);
int GetEndTimerFlag();
void StopEndTimer(0;

extern void LogMessage(char*);
