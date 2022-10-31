#ifndef MLIB__MOTOR_H_
#define MLIB__MOTOR_H_

#include "headfile.h"

#define DUTY_MAX 50000

#define MOVE_MODE_FRONT 0
#define MOVE_MODE_LEFT 1
#define MOVE_MODE_RIGHT 2
#define MOVE_MODE_BACK 3
#define MOVE_MODE_ROTATE_CLOCKWISE 4
#define MOVE_MODE_ROTATE_ANTICLOCKWISE 5

typedef struct{
    PWMCH_enum pwm;
    PIN_enum dir;
    int32 duty;
    QTIMERN_enum ecd;
    QTIMER_PIN_enum ecdp1,ecdp2;
    int32 ecdval;
    bool ecdflip;
    PID pid;
}motor;

extern motor motors[4];

#define MTR_LF motors
#define MTR_RF (motors+1)
#define MTR_LB (motors+2)
#define MTR_RB (motors+3)

#define PID_PERIOD_MS 10

void motor_init(motor *mtr, PWMCH_enum pwmch, PIN_enum dirpin,
    QTIMERN_enum qch,QTIMER_PIN_enum qpin1,QTIMER_PIN_enum qpin2,bool flip,
    float kp,float ki,float kd);
int32 motor_get_encoder(motor *mtr);
void motor_set_pwm(motor *mtr,int32 _duty);
void motor_set_spd(motor *mtr,int32 spd);
void motor_callback(motor *mtr);


#endif /*motor.h*/