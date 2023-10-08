#include "mecanum.h"

mecanum_info mecanum;

//Stop moving
void mecanum_stop(){
    int i;
    RUNNING=false;
    for(i=0;i<4;++i) {
        motor_get_encoder(motors+i);
        motor_set_pwm(motors+i,0);
    }
}

#define APPROACH_THRESHOLD_MM (500*CAR_SPD/1000)
#define MIN_SPD 200
#define ROT_THRE 1500
#define ROT_K 500
#define ROT_LIM 20

void mecanum_set_wheels_spd(int32 lf,int32 rf,int32 lb,int32 rb){
    motor_set_spd(MTR_LF,lf);
    motor_set_spd(MTR_RF,rf);
    motor_set_spd(MTR_LB,lb);
    motor_set_spd(MTR_RB,rb);
}

//Call this function in the interrupt of timer!
void mecanum_callback(){
    //Since ultrasound function blocks, their data should be updated in the main function, not here.

    //Get data of gyroscope to determine the yaw of the car
    icm_get(ICM_BUF);
    if(abs(ICM_BUF->wz)>5) CAR_DIR+=ICM_BUF->wz;
    int32 rot_mod=0;
    if(CAR_DIR>ROT_THRE) {
        rot_mod=(CAR_DIR-ROT_THRE)/ROT_K;
        if(rot_mod>ROT_LIM) rot_mod=ROT_LIM;
    }else if(CAR_DIR<-ROT_THRE){
        rot_mod=(CAR_DIR+ROT_THRE)/ROT_K;
        if(rot_mod<-ROT_LIM) rot_mod=-ROT_LIM;
    }

    //Calc the motor callback to update the value of encoder and use PID algorithm to maintain the speed of every wheel
    if(FORCE_RUN) RUNNING=true;
    if(RUNNING) {
        motor_callback(MTR_LF);
        motor_callback(MTR_RF);
        motor_callback(MTR_LB);
        motor_callback(MTR_RB);
    }else{
        mecanum_stop();
    }

    //Determine the position of the car
    int32 dx=MTR_LB->ecdval-MTR_RB->ecdval+MTR_RF->ecdval-MTR_LF->ecdval;
    int32 dy=MTR_LB->ecdval+MTR_LF->ecdval+MTR_RB->ecdval+MTR_RF->ecdval;
    CUR_MILE_POS.x+=dx;CUR_MILE_POS.y+=dy;

    //Continue to move the given position
    if(RUNNING){
        if(CAR_MODE==MODE_MILEAGE){
            dx=DST_MILE_POS.x-CUR_MILE_POS.x;dy=DST_MILE_POS.y-CUR_MILE_POS.y;
            double r1=sqrt((double)dx*dx+(double)dy*dy);
            if(r1<=MILE_POS_ERR*max((CAR_SPD-500)/100,1) && !FORCE_RUN){
                mecanum_stop();
            }else{
                int32 wx=CAR_SPD*dx/r1,wy=CAR_SPD*dy/r1,w1=wy-wx,w2=wy+wx;
                //Prevent the car from rotating by add bias determined by current yaw
                mecanum_set_wheels_spd(w1-rot_mod*5,w2+rot_mod*5,w2-rot_mod*5,w1+rot_mod*5);
            }
        }else{
            if(CAR_MODE==MODE_USOUND_FR){
                dx=DST_USD[1]-CUR_USD[1];dy=CUR_USD[0]-DST_USD[0];
            }else if(CAR_MODE==MODE_USOUND_RB){
                dx=DST_USD[1]-CUR_USD[1];dy=DST_USD[2]-CUR_USD[2];
            }else if(CAR_MODE==MODE_USOUND_BL){
                dx=CUR_USD[3]-DST_USD[3];dy=DST_USD[2]-CUR_USD[2];
            }else{//CAR_MODE==MODE_USOUND_LR
                dx=CUR_USD[3]-DST_USD[3];dy=CUR_USD[0]-DST_USD[0];
            }
            double r1=sqrt((double)dx*dx+(double)dy*dy);
            if(r1<=USD_POS_ERR*max((CAR_SPD-400)/100-3,1) && !FORCE_RUN){
                //Stop moving when the car is near enough to the given position, considering the inertia of the car
                mecanum_stop();
            }else{
                int32 mspd=CAR_SPD;
                if(r1<APPROACH_THRESHOLD_MM) mspd=mspd*r1/APPROACH_THRESHOLD_MM;
                if(mspd<MIN_SPD) mspd=MIN_SPD;
                int32 wx=mspd*dx/r1,wy=mspd*dy/r1,w1=wy-wx,w2=wy+wx;
                //Prevent the car from rotating by add bias determined by current yaw
                mecanum_set_wheels_spd(w1-rot_mod*5,w2+rot_mod*5,w2-rot_mod*5,w1+rot_mod*5);
            }
        }
    }
    if(FORCE_RUN>0) --FORCE_RUN;
}

/* Move to the given position
 * mode: distance metrics, selecting from {MILEAGE,USOUND_FR,USOUND_RB,USOUND_BL,USOUND_LF}
 * spd:  speed
 * (val1,val2): the given position
 *      When mode==MILEAGE, represents the coordinate (x,y) measured by encoder of motor.
 *          This mode is deprecated since it is not accurate. (x: left is positive; y: front is positive)
 *      When mode==USOUND_FR, represents the distance to the FRONT and to the RIGHT respectively
 *      When mode==USOUND_RB, represents the distance to the RIGHT and to the BACK respectively
 *      When mode==USOUND_BL, represents the distance to the BACK and to the LEFT respectively
 *      When mode==USOUND_LF, represents the distance to the LEFT and to the FRONT respectively
 */
void mecanum_move_to(car_mode_enum mode,int32 spd,int32 val1,int32 val2){
    CAR_MODE=mode;
    FORCE_RUN=20;
    RUNNING=true;
    CAR_SPD=spd;
    if(mode==MODE_MILEAGE){
        DST_MILE_POS.x=val1;DST_MILE_POS.y=val2;
    }else if(mode==MODE_USOUND_FR){
        DST_USD[0]=val1;DST_USD[1]=val2;
    }else if(mode==MODE_USOUND_RB){
        DST_USD[1]=val1;DST_USD[2]=val2;
    }else if(mode==MODE_USOUND_BL){
        DST_USD[2]=val1;DST_USD[3]=val2;
    }else{
        DST_USD[3]=val1;DST_USD[0]=val2;
    }
}

//Clear the status and stop moving.
void mecanum_clear(){
    int i;
    memset(&mecanum,0,sizeof(mecanum));
    for(i=0;i<4;i++){motor_get_encoder(motors+i);motors[i].ecdval=0;}
}

//Get current speed of the car measured by motor encoders.
int32 mecanum_get_spd(){
    int32 w1=(MTR_LF->ecdval+MTR_RB->ecdval),w2=(MTR_RF->ecdval+MTR_LB->ecdval);
    int32 wx=w2-w1,wy=w2+w1;
    return sqrt(wx*wx+wy*wy);
}

//Get current position measured by motor encoders.
point mecanum_get_pos(){
    return CUR_MILE_POS;
}

//Get whether the car is running
bool mecanum_is_running(){
    return RUNNING;
}