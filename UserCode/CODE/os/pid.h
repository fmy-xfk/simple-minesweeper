#ifndef PID_H_
#define PID_H_

#include "syshead.h"

typedef struct{
    float kp,ki,kd,target;
    float __isum,__last_err,__last2_err;
}PID;

void PIDInit(PID* ret, float kp,float ki,float kd);
void DRPIDInit(PID* pid, float KP, float WC);
void DRPIInit(PID* pid, float KP, float WC);
void PIDClear(PID* pid);
void PIDSet(PID* pid, float target);
float PIDCalc(PID* pid, float input_enc, float input_pwm);

#endif /* PID_H_ */
