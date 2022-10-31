#include "headfile.h"

int usound_test(flash_data_union* data){
    mtitle("USound");
    int32 buf[4]={0,0,0,0};
    while(!key_down(K_YES)){
        usound_get_dist_all(buf);
        mprintf("%6d %6d %6d %6d\n",buf[0],buf[1],buf[2],buf[3]);
        //wait_key();
    }
    return 0;
}