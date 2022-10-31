#include "mecanum.h"

mecanum_info mecanum;

//停车
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

//定时中断内调用该函数
void mecanum_callback(){
    //超声波数据应当在主函数内更新
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

    //编码器
    if(FORCE_RUN) RUNNING=true;
    if(RUNNING) {
        motor_callback(MTR_LF);
        motor_callback(MTR_RF);
        motor_callback(MTR_LB);
        motor_callback(MTR_RB);
    }else{
        mecanum_stop();
    }
    int32 dx=MTR_LB->ecdval-MTR_RB->ecdval+MTR_RF->ecdval-MTR_LF->ecdval;
    int32 dy=MTR_LB->ecdval+MTR_LF->ecdval+MTR_RB->ecdval+MTR_RF->ecdval;
    CUR_MILE_POS.x+=dx;CUR_MILE_POS.y+=dy;
    if(RUNNING){
        if(CAR_MODE==MODE_MILEAGE){
            dx=DST_MILE_POS.x-CUR_MILE_POS.x;dy=DST_MILE_POS.y-CUR_MILE_POS.y;
            double r1=sqrt((double)dx*dx+(double)dy*dy);
            if(r1<=MILE_POS_ERR*max((CAR_SPD-500)/100,1) && !FORCE_RUN){
                mecanum_stop();
            }else{
                int32 wx=CAR_SPD*dx/r1,wy=CAR_SPD*dy/r1,w1=wy-wx,w2=wy+wx;
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
                mecanum_stop();
            }else{
                int32 mspd=CAR_SPD;
                if(r1<APPROACH_THRESHOLD_MM) mspd=mspd*r1/APPROACH_THRESHOLD_MM;
                if(mspd<MIN_SPD) mspd=MIN_SPD;
                int32 wx=mspd*dx/r1,wy=mspd*dy/r1,w1=wy-wx,w2=wy+wx;
                mecanum_set_wheels_spd(w1-rot_mod*5,w2+rot_mod*5,w2-rot_mod*5,w1+rot_mod*5);
            }
        }
    }
    if(FORCE_RUN>0) --FORCE_RUN;
}

/* 移动到指定位置
 * mode: 位置测量方式
 * spd:  速度
 * (val1,val2): 
 *      mode==MILEAGE时，分别表示x,y坐标（x坐标向左为正，y坐标向前为正）
 *      mode==USOUND_FR，分别表示与前面和右边的超声波距离
 *      mode==USOUND_RB，分别表示与右边和后面的超声波距离
 *      mode==USOUND_BL，分别表示与后面和左边的超声波距离
 *      mode==USOUND_LF，分别表示与左边和前面的超声波距离
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

//清空状态并停车
void mecanum_clear(){
    int i;
    memset(&mecanum,0,sizeof(mecanum));
    for(i=0;i<4;i++){motor_get_encoder(motors+i);motors[i].ecdval=0;}
}

int32 mecanum_get_spd(){
    int32 w1=(MTR_LF->ecdval+MTR_RB->ecdval),w2=(MTR_RF->ecdval+MTR_LB->ecdval);
    int32 wx=w2-w1,wy=w2+w1;
    return sqrt(wx*wx+wy*wy);
}

point mecanum_get_pos(){
    return CUR_MILE_POS;
}

bool mecanum_is_running(){
    return RUNNING;
}