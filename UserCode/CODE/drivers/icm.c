//Gyroscope driver

#include "icm.h"

icm_data __calib;

void __icm_cpy(icm_data *p){
    p->wx=icm_gyro_x;p->wy=icm_gyro_y;p->wz=icm_gyro_z;
    p->ax=icm_acc_x; p->ay=icm_acc_y; p->az=icm_acc_z;
}

void icm_get_raw(icm_data *dat_ptr){
    icm_gyro();icm_acc();
    __icm_cpy(dat_ptr);
}

void icm_get(icm_data *dat_ptr){
    icm_get_raw(dat_ptr);
    icm_data_calib(dat_ptr,&__calib);
}

void icm_data_calib(icm_data *dat_ptr,icm_data *calib){
    dat_ptr->ax-=calib->ax;dat_ptr->ay-=calib->ay;dat_ptr->az-=calib->az;
    dat_ptr->wx-=calib->wx;dat_ptr->wy-=calib->wy;dat_ptr->wz-=calib->wz;
}

void icm_data_clear(icm_data *dat_ptr){
    memset(dat_ptr,0,sizeof(icm_data));
}

void icm_auto_calib(){
    icm_data dat;
    int32 ax=0,ay=0,az=0,wx=0,wy=0,wz=0;
    for(uint8 i=0;i<100;i++){
        icm_get_raw(&dat);
        ax+=dat.ax;ay+=dat.ay;az+=dat.az;
        wx+=dat.wx;wy+=dat.wy;wz+=dat.wz;
        mdelay_ms(5);
    }
    __calib.ax=(int16)(ax*0.01);__calib.ay=(int16)(ay*0.01);__calib.az=(int16)(az*0.01);
    __calib.wx=(int16)(wx*0.01);__calib.wy=(int16)(wy*0.01);__calib.wz=(int16)(wz*0.01);
}
