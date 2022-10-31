#ifndef __MECANUM_H__
#define __MECANUM_H__

#include "headfile.h"

typedef struct{
    int32 x,y;
}point;

//LOW            HIGH
//MILE  F    R    B    L
//2^0   2^1  2^2  2^3  2^4

typedef enum{
    MODE_UNDEFINED=0x00,
    MODE_MILEAGE=0x01,
    MODE_USOUND_F=0x02,
    MODE_USOUND_R=0x04,
    MODE_USOUND_B=0x08,
    MODE_USOUND_L=0x10,
    MODE_USOUND_FR=0x06,
    MODE_USOUND_RB=0x0C,
    MODE_USOUND_BL=0x18,
    MODE_USOUND_LF=0x12,
}car_mode_enum;

typedef struct{
    bool running;
    point mile_cur,mile_dst;
    int32 usd_cur[4],usd_dst[4];
    int32 spd,force_run_rounds;
    car_mode_enum mode;
    icm_data icm_buf;
    int32 rot;
}mecanum_info;

extern mecanum_info mecanum;

#define CAR_MODE mecanum.mode
#define RUNNING mecanum.running
#define CUR_MILE_POS mecanum.mile_cur
#define DST_MILE_POS mecanum.mile_dst
#define CAR_SPD mecanum.spd
#define MILE_POS_ERR 500
#define USD_POS_ERR 20
#define FORCE_RUN mecanum.force_run_rounds
#define CUR_USD mecanum.usd_cur
#define DST_USD mecanum.usd_dst
#define ICM_BUF (&mecanum.icm_buf)
#define CAR_DIR mecanum.rot

void mecanum_callback();
void mecanum_move_to(car_mode_enum mode,int32 spd,int32 val1,int32 val2);
void mecanum_clear();
void mecanum_stop();
point mecanum_get_pos();
int32 mecanum_get_spd();
bool mecanum_is_running();

#endif