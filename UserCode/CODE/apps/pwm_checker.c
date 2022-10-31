#include "headfile.h"

int pwm_checker(flash_data_union* data){
    int32 mtr_id=0,i;
    for(i=0;i<4;i++) motor_set_pwm(motors+i,5000);
    mputchar('>');
    mstatus("1:INC 2:DEC 3:SW 4:ESC");
    while(!key_down(K_YES)){
        if(key_down(K_DOWN)){
            motors[mtr_id].duty-=500;
            motor_set_pwm(motors+mtr_id,motors[mtr_id].duty);
        }
        if(key_down(K_UP)){
            motors[mtr_id].duty+=500;
            motor_set_pwm(motors+mtr_id,motors[mtr_id].duty);
        }
        if(key_down(K_EDIT)){
            msetcur(0,mtr_id);
            mputchar(' ');
            mtr_id=(mtr_id+1)%4;
            msetcur(0,mtr_id);
            mputchar('>');
        }
        for(i=0;i<4;i++){
            motor_get_encoder(motors+i);
            msetcur(1,i);
            mprintf("PWM:%6d ECD:%6d",motors[i].duty,motors[i].ecdval);
        }
        mdelay_ms(10);
    }
    for(i=0;i<4;i++) motor_set_pwm(motors+i,0);
    return 0;
}