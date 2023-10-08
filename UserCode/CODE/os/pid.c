#include "pid.h"

PID LPID,RPID;

/* PID清空
 * pid: PID结构体指针q
 */
void PIDClear(PID* pid){
    pid->__isum=0;pid->__last_err=0.0f;pid->__last2_err=0.0f;
}

/* PID初始化
 * pid: PID结构体指针
 * kp:比例系数
 * ki:积分系数
 * kd:微分系数
 */
void PIDInit(PID* pid, float kp, float ki, float kd){
    memset(pid,0,sizeof(PID));
    pid->kd=kd;pid->ki=ki;pid->kp=kp;
    pid->__isum=0;pid->__last_err=0.0f;pid->__last2_err=0.0f;
}
void DRPIDInit(PID* pid, float KP, float WC){
    pid->kd=KP;pid->ki=KP*WC*WC;pid->kp=2*KP*WC;
    pid->__isum=0;pid->__last_err=0.0f;pid->__last2_err=0.0f;
}
void DRPIInit(PID* pid, float KP, float WC){
    pid->kd=0;pid->ki=KP*WC;pid->kp=KP;
    pid->__isum=0;pid->__last_err=0.0f;pid->__last2_err=0.0f;
}

/* PID设置
 * pid: PID结构体指针
 * target: 目标值
 */
void PIDSet(PID* pid, float target){
    pid->target = target;
}

/* PID计算（增量式，误差较小）
 * pid: PID结构体指针
 * input: 本次输入值
 * return: PID计算增量值
 */
float PIDCalc(PID* pid, float input_enc, float input_pwm){
    float err = pid->target - input_enc, ret;
    if(input_pwm>PWM_DUTY_MAX){
        if(err<0) pid->__isum += err;
    }else if(input_pwm<-PWM_DUTY_MAX){
        if(err>0) pid->__isum += err;
    }else{
        pid->__isum += err;
    }
    ret = pid->kp * (err - pid->__last_err) +
          pid->ki * err +
          pid->kd * (err - 2*(pid->__last_err)+pid->__last2_err);

    pid->__last2_err = pid->__last_err;
    pid->__last_err = err;
    return ret;
}
