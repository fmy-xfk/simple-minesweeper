#include "motor.h"

//速度的数值等于轮子旋转100ms的编码器值!!!
motor motors[4];

void motor_init(motor *mtr, PWMCH_enum pwmch, PIN_enum dirpin,
    QTIMERN_enum qch,QTIMER_PIN_enum qpin1,QTIMER_PIN_enum qpin2,bool flip,
    float kp,float ki,float kd){
    memset(mtr,0,sizeof(motor));
    mtr->pwm=pwmch;
    pwm_init(pwmch,400,0);
    mtr->dir=dirpin;
    gpio_init(dirpin,GPO,GPIO_LOW,GPIO_PIN_CONFIG);
    mtr->ecd=qch;
    mtr->ecdp1=qpin1;
    mtr->ecdp2=qpin2;
    mtr->ecdflip=flip;
    PIDInit(&mtr->pid,kp,ki,kd);
    qtimer_quad_init(qch,qpin1,qpin2);
}

//获得编码器值
int32 motor_get_encoder(motor *mtr){
    mtr->ecdval=(int32)qtimer_quad_get(mtr->ecd,mtr->ecdp1);
    qtimer_quad_clear(mtr->ecd,mtr->ecdp1);
    if(mtr->ecdflip) mtr->ecdval=-mtr->ecdval;
    return mtr->ecdval;
}

//设置电机占空比
void motor_set_pwm(motor *mtr,int32 _duty){
    //assert(_duty<=DUTY_MAX && _duty>=-DUTY_MAX);
    if(_duty>DUTY_MAX) _duty=DUTY_MAX;
    else if(_duty<-DUTY_MAX) _duty=-DUTY_MAX;
    mtr->duty=_duty;
    gpio_set(mtr->dir,_duty>=0);
    pwm_duty(mtr->pwm,abs(_duty));
}

//设置速度
void motor_set_spd(motor *mtr,int32 spd){
    PIDSet(&mtr->pid,spd*PID_PERIOD_MS/100);
}

void motor_callback(motor *mtr){
    motor_get_encoder(mtr);
    motor_set_pwm(mtr,mtr->duty+PIDCalc(&mtr->pid,mtr->ecdval,mtr->duty));
}
