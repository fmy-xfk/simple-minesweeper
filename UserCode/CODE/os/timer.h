#ifndef TIMER_H_
#define TIMER_H_

#include "syshead.h"

typedef void(* TimerFunc)(void);

typedef struct{
    TimerFunc Func;
    bool Enabled;
    uint16_t Period;
    char name[20];
}Timer;

void TimerLoop();
void InitTimer();
bool AddTimer(TimerFunc func,uint16_t Period,const char* name);
int FindTimer(const char* name);
bool RemoveTimer(const char* name);
bool DisableTimer(const char* name);
bool EnableTimer(const char* name);
int GetTimerCount();
void ClearTimer();
Timer* GetTimer(const char* name);
Timer* GetTimerByID(int id);

#endif /* TIMER_H_ */
