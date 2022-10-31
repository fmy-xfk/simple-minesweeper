/*
 * os.h
 *
 *  Created on: Jun 28, 2022
 *      Author: xff20
 */

#ifndef OS_H_
#define OS_H_

#include "syshead.h"

typedef union __flash_data_union{
    int32 int32_type;
    uint32 uint32_type;
    float float_type;
} flash_data_union;

//函数原型：int app_main(flash_data_union* storage)
typedef int(* app_entry_point)(flash_data_union*);

typedef struct{
    char name[20];          //应用名称
    app_entry_point app;    //函数入口点
}app_info;

typedef struct{
    app_info apps[20];
    uint32 apps_count;
    flash_data_union storage[1024];
    int32 storage_sector_id;  //作为数据存储使用的扇区编号，0表示不使用，有效范围1~20
    int32 running_app; //正在运行的app编号，-1为无
}system_data;

extern system_data SYSMAIN;
#define SYS_PAR_OFFSET 64
#define PAR_COUNT_PER_APP 64
#define CUR_APPID SYSMAIN.running_app
#define __PAR SYSMAIN.storage
#define SYS_PAR(i) SYSMAIN.storage[i].int32_type
#define MY_PAR_BEG  (__PAR+SYS_PAR_OFFSET+CUR_APPID*PAR_COUNT_PER_APP)
#define MY_PAR_I(i) __PAR[SYS_PAR_OFFSET+CUR_APPID*PAR_COUNT_PER_APP+(i)].int32_type
#define MY_PAR_U(i) __PAR[SYS_PAR_OFFSET+CUR_APPID*PAR_COUNT_PER_APP+(i)].uint32_type
#define MY_PAR_F(i) __PAR[SYS_PAR_OFFSET+CUR_APPID*PAR_COUNT_PER_APP+(i)].float_type
#define GLOBE_PAR_BEG(appid) (__PAR+SYS_PAR_OFFSET+appid*PAR_COUNT_PER_APP)
#define GLOBE_PAR_I(appid,i) __PAR[SYS_PAR_OFFSET+appid*PAR_COUNT_PER_APP+(i)].int32_type
#define GLOBE_PAR_U(appid,i) __PAR[SYS_PAR_OFFSET+appid*PAR_COUNT_PER_APP+(i)].uint32_type
#define GLOBE_PAR_F(appid,i) __PAR[SYS_PAR_OFFSET+appid*PAR_COUNT_PER_APP+(i)].float_type
#define STORAGE_SECTOR SYSMAIN.storage_sector_id
#define APPS SYSMAIN.apps
#define APP_CNT SYSMAIN.apps_count

#define VERIFY_CODE     SYS_PAR(0)
#define PAR_DEVICE_ID   SYS_PAR(1)
#define PAR_AUTORUN_ID  SYS_PAR(2)

bool os_ui_confirm(const char dat[]);
void os_ui_err_handler(bool res, const char dat[]);
void os_assert(bool condition);
void os_ui_println(uint8 line, const char* text, bool selected);
void os_ui_println2(uint8 line, const char* text, int32 val, bool selected);
bool os_ui_edit_one(uint8 line, const char* text, int32* val_ptr);
void os_ui_edit_list(const char* title, char* namelist, uint32 max_name_length, flash_data_union data[], uint32 item_count);
int  os_ui_select(const char* title, char* namelist, uint32 max_name_length, uint32 item_count, uint32 initial_item,bool allow_exit);

int  os_app_run_by_name(const char name[]);
int  os_app_run_by_idx(int idx);
int  os_app_select();

void os_set_basic_info();
void os_storage_load();
void os_storage_save();

void os_preinit();
void os_postinit();

#endif /* OS_H_ */
