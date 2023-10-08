#include "timer.h"

char __TimerName[10][20];
Timer __Timers[10];
uint8_t __TimerCount=0;

void TimerLoop(){
    static uint32 Period = 0,timer_cnt=0;
    Period++;timer_cnt=GetTimerCount();
    for(int i=0;i<timer_cnt;++i){
        Timer *t=GetTimerByID(i);
        if(t->Enabled && !(Period%t->Period)) t->Func();
    }
}

void InitTimer(){
    pit_init();
    //NVIC_SetPriority(PIT_IRQn,3);
}

bool AddTimer(TimerFunc func,uint16_t Period,const char* name){
    if(__TimerCount>=10 || FindTimer(name)>=0) {
        return 0;
    }
    __Timers[__TimerCount].Func=func;
    __Timers[__TimerCount].Enabled=1;
    __Timers[__TimerCount].Period=Period;
    strcpy(__Timers[__TimerCount++].name,name);
    return 1;
}

int FindTimer(const char* name){
    for(int i=0;i<__TimerCount;i++){
        if(strcmp(__Timers[i].name,name)==0){
            return i;
        }
    }
    return -1;
}

bool RemoveTimer(const char* name){
    int pos=FindTimer(name);
    if(pos>=0) {
        for(int i=pos;i<__TimerCount-1;i++){
            __Timers[i]=__Timers[i+1];
        }
        return 1;
    }
    return 0;
}

bool DisableTimer(const char* name){
    int pos=FindTimer(name);
    if(pos>=0) {
        __Timers[pos].Enabled=0;
        return 1;
    }
    return 0;
}

bool EnableTimer(const char* name){
    int pos=FindTimer(name);
    if(pos>=0) {
        __Timers[pos].Enabled=1;
        return 1;
    }
    return 0;
}

int GetTimerCount(){
    return __TimerCount;
}

void ClearTimer(){
    __TimerCount=0;
}

Timer* GetTimer(const char* name){
    int pos=FindTimer(name);
    if(pos>=0) {
        return __Timers+pos;
    }
    return NULL;
}

Timer* GetTimerByID(int id){
    assert(id>=0 && id<__TimerCount);
    return __Timers+id;
}
