#ifndef __USOUND_H__
#define __USOUND_H__

#include "os/os_dev.h"
#define MAX_USOUND_TIME_US 50000

typedef struct{
    PIN_enum trigger,echo;
    int32 fl_time,fl_time_buff,wktime;
    bool working;
}USound;

void usound_handler();

void usound_init(USound *usd,PIN_enum trig,PIN_enum echo);
bool usound_send(USound *usd);
int32 usound_get(USound *usd);

void usound_init_all(PIN_enum trig1,PIN_enum echo1,
    PIN_enum trig2,PIN_enum echo2,
    PIN_enum trig3,PIN_enum echo3,
    PIN_enum trig4,PIN_enum echo4);
void usound_get_dist_all(int32 *dist);
void usound_get_dist_certain(int32 *dist,int32 mode);

#endif