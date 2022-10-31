#include "headfile.h"

int main(void)
{
    DisableGlobalIRQ();
    board_init();
    
    //硬件初始化
    InitTimer();

    os_preinit();

    //屏幕与按键
    screen_init();
    init_keys();
    mclear();
    mputstr_fmt("JS Car OS",MCONIO_POSTYPE_MID,(MCONIO_MAX_POS_Y>>1)-1);
    os_set_basic_info();
    
    //Flash存储
    mstatus("Flash...");
    flash_init();
    os_storage_load();    

    os_postinit();

    EnableGlobalIRQ(0);
    
     //启动完成
    mstatus("Any key = stop autorun");

    if(wait_key_ms(1000)==K_NONE && PAR_AUTORUN_ID>=0){
        os_app_run_by_idx(PAR_AUTORUN_ID);
    }
    //应用选择器
    while(1){
        int32 idx=os_app_select();
        if(idx<0) break;
        os_app_run_by_idx(idx);
    }

    mclear();
    mstatus("Saving...");
    os_storage_save();
    
    mclear();
    mprintf("Now you can shutdown safely.");

    return 0;
}
