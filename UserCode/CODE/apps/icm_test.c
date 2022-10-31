#include "headfile.h"

int icm_test(flash_data_union* data){
    icm_data idat,*pidat=&idat;
    int32 rot=0;
    icm_auto_calib();
    AddTimer(mecanum_callback,PID_PERIOD_MS,"mt");
    RUNNING=true;
    while(!key_down(K_YES)){
        icm_get(pidat);
        if(abs(pidat->wz)>5) rot+=pidat->wz;
        mprintf("%10d\n",rot);
        mdelay_ms(10);
    }
    return 0;
}