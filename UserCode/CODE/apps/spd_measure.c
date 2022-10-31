#include "headfile.h"

void leastSquareLinearFit(float x[],float y[],const int num,float *a,float *b)
{
    float sum_x2=0.0f,sum_y=0.0f,sum_x=0.0f,sum_xy=0.0f;
    for(int i=0;i<num;++i){
        sum_x2+=x[i]*x[i];
        sum_y+=y[i];
        sum_x+=x[i];
        sum_xy+=x[i]*y[i];
    }
    *a=(num*sum_xy-sum_x*sum_y)/(num*sum_x2-sum_x*sum_x);
    *b=(sum_x2*sum_y-sum_x*sum_xy)/(num*sum_x2-sum_x*sum_x);
}

#define ST 6
#define ED 14
#define NUM (ED-ST+1)

int spd_mea(flash_data_union* data){
    int32 target_spd=600,i,j;
    float y[4][NUM],x[4][NUM],k,b;
    for(i=0;i<4;i++){
        k=MY_PAR_F(i*2);b=MY_PAR_F(i*2+1);
        mprintf("y%d=%fx%d+%f\n",i,k,i,b);
    }
    for(i=0;i<4;i++) motor_get_encoder(motors+i);
    for(j=ST;j<=ED;j++){
        mprintf("%d ",j);
        for(i=0;i<4;i++) {
            motor_set_pwm(motors+i,j*500);
        }
        mdelay_ms(1000);
        for(i=0;i<4;i++) {
            y[i][j-ST]=motor_get_encoder(motors+i)*0.1f;
            motor_set_pwm(motors+i,0);
            x[i][j-ST]=j*500;
        }
    }
    mclear();
    for(i=0;i<4;i++){
        leastSquareLinearFit(x[i],y[i],NUM,&k,&b);
        MY_PAR_F(i*2)=k;MY_PAR_F(i*2+1)=b;
        mprintf("y%d=%fx%d+%f\n",i,k,i,b);
    }
    os_storage_save();
    wait_key();
    return 0;
}