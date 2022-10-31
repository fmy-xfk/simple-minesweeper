/*
 * os.c 迷你操作系统
 *
 *  Created on: Jun 28, 2022
 *      Author: xff20
 */

#include "os_dev.h"

/********************************************
 * UI部分
 ********************************************/
//确认运行提示
bool os_ui_confirm(const char dat[]){
    uint8 key;
    mclear();
    mprintf("You are going to:\n%s\nProceed?\nK4=Yes Other=No",dat);
    key=wait_key();
    return key==K_YES;
}

//错误提示
void os_ui_err_handler(bool res, const char dat[]){
    if(!res){
        mclear();
        mprintf("ERROR! Fail to:\n%s\nPress any key to proceed.",dat);
        wait_key();
    }
}

//致命错误提示
void os_ui_fatal(bool res, const char dat[]){
    if(!res){
        mclear();
        mprintf("FATAL ERROR! Fail to:\n%s\nPress RST to restart.",dat);
        while(1);
    }
}

void os_assert(bool condition){
    os_ui_err_handler(condition,"ASSERT");
}

//输出一行
void os_ui_println(uint8 line, const char* text, bool selected){
    msetcur(0,line);
    mputchar(selected?'>':' ');
    mputspace(MCONIO_MAX_POS_X-mputstr(text));
}

//输出一行参数
void os_ui_println2(uint8 line, const char* text, int32 val, bool selected){
    msetcur(0,line);
    mputchar(selected?'>':' ');
    uint8 len=MCONIO_MAX_POS_X-mputstr(text)-mdigitcnt(val)-1;
    mputspace(len);mputint(val);
}

//编辑一个参数
bool os_ui_edit_one(uint8 line, const char* text, int32* val_ptr){
    bool exit_pressed=false,save=true;
    int32 val=*val_ptr;uint8 key;
    mstatus("1:+ 2:- 3:Esc 4:Save");
    while(!exit_pressed){
        msetcur(0,line);
        mputchar('*');
        uint8 len=MCONIO_MAX_POS_X-mputstr(text)-mdigitcnt(val)-1;
        mputspace(len);mputint(val);
        key=wait_key();
        if(key==K_UP)       {val++;}
        else if(key==K_DOWN){val--;}
        else if(key==K_YES) {exit_pressed=true;save=true;}
        else if(key==K_NO)  {exit_pressed=true;save=false;}
    }
    if(save) *val_ptr=val;
    return save;
}

#define ITH_ELEM(list,item_size,i) ((list)+(item_size)*(i))
//编辑若干参数
void os_ui_edit_list(const char* title, char* namelist, uint32 max_name_length, flash_data_union data[], uint32 item_count){
    int32 i=0,cur_item=0,begin_item=0;
    uint8 key; bool callexit=false;
    mclear();
    mtitle(title);
    mstatus("1:Prev 2:Next 3:Edit 4:Esc");
    while(!callexit){
        for(i=0;i<MCONIO_MAX_POS_Y-2 && i+begin_item<item_count;i++){
            os_ui_println2(i+1, ITH_ELEM(namelist,max_name_length,(i+begin_item)), data[i+begin_item].int32_type, i+begin_item==cur_item);
        }
        key = wait_key();
        switch(key){
        case K_PREV:
            cur_item--;if(cur_item<0) cur_item=0;break;
        case K_NEXT:
            cur_item++;if(cur_item>=item_count) cur_item=item_count-1;break;
        case K_EDIT:
            os_ui_edit_one(cur_item-begin_item+1,ITH_ELEM(namelist,max_name_length,cur_item),(int32*)(data+cur_item));
            mstatus("1:Prev 2:Next 3:Edit 4:Esc");
            break;
        case K_YES:
            callexit=true;
            break;
        }
        while(cur_item-begin_item>MCONIO_MAX_POS_Y-3)begin_item++;
        while(begin_item>cur_item)begin_item--;
    }
    os_storage_save();
}

//从列表中选择一项
int os_ui_select(const char* title, char* namelist, uint32 max_name_length, uint32 item_count, uint32 initial_item,bool allow_exit){
    int32 i=0,cur_item=0,begin_item=initial_item;
    uint8 key; bool callexit=false;
    mclear();
    mtitle(title);
    if(allow_exit){
        mstatus("1:Prev 2:Next 3:Esc 4:Sel");
    }else{
        mstatus("1:Prev 2:Next 4:Sel");
    }
    while(!callexit){
        for(i=0;i<MCONIO_MAX_POS_Y-2 && i+begin_item<item_count;i++){
            os_ui_println(i+1, ITH_ELEM(namelist,max_name_length,(i+begin_item)), i+begin_item==cur_item);
        }
        key = wait_key();
        switch(key){
        case K_PREV:
            cur_item--;if(cur_item<0) cur_item=0;break;
        case K_NEXT:
            cur_item++;if(cur_item>=item_count) cur_item=item_count-1;break;
        case K_NO:
            if(allow_exit) return -1;
        case K_YES:
            callexit=true;
            break;
        }
        while(cur_item-begin_item>MCONIO_MAX_POS_Y-3)begin_item++;
        while(begin_item>cur_item)begin_item--;
    }
    return cur_item;
}


/********************************************
 *存储管理器
 ********************************************/
//读取存储
void os_storage_load(){
    uint32 elem_cnt=FLASH_PAGE_SIZE/sizeof(flash_data_union);
    uint8 i;
    for(i=0;i<16;++i) {
        flash_read_page(STORAGE_SECTOR,i,(uint32*)__PAR+elem_cnt*i,elem_cnt);
    }
}

//写入存储
void os_storage_save(){
    uint32 elem_cnt=FLASH_PAGE_SIZE/sizeof(flash_data_union);
    uint8 i;
    os_ui_fatal(!flash_erase_sector(STORAGE_SECTOR),"Erase storage");
    for(i=0;i<16;++i) {
        os_ui_fatal(!flash_page_program(STORAGE_SECTOR,i,(uint32*)__PAR+elem_cnt*i,elem_cnt),"Write storage");
    }
}

/********************************************
 *应用管理器
 ********************************************/
//按名称运行应用
int os_app_run_by_name(const char name[]){
    int i;
    for(i=0;i<APP_CNT;i++){
        if(strcmp(name,SYSMAIN.apps[i].name)==0){
            return os_app_run_by_idx(i);
        }
    }
    os_ui_err_handler(0, name);
    return -1;
}

//按编号运行应用，编号从0开始
int os_app_run_by_idx(int idx){
    int exit_code=-1,i;
    mclear();
    if(idx>=0 && idx<APP_CNT){
        CUR_APPID=idx;
        pit_interrupt_ms(PIT_CH3, 1);
        exit_code=SYSMAIN.apps[idx].app(MY_PAR_BEG);
        pit_close(PIT_CH3);
        ClearTimer();
        os_storage_save();
    }else{
        os_ui_err_handler(0, "Bad APP Index");
    }
    return exit_code;
}

//显示应用列表并供用户选择
int os_app_select(){
    return os_ui_select("Applications",(char*)APPS,sizeof(app_info),APP_CNT,0,true);
}

bool os_app_add(const char* name, app_entry_point entry){
    if(APP_CNT>=20) return false;
    strcpy(APPS[APP_CNT].name,name);
    APPS[APP_CNT].app=entry;
    ++APP_CNT;
    return true;
}

void os_set_basic_info(){
    memset(__PAR,0,sizeof(__PAR));
}