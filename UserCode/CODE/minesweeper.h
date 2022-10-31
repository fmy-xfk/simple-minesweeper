#ifndef __MINESWEEPER_H__
#define __MINESWEEPER_H__

#include "headfile.h"

#define NULL_CAM 0
#define FRONT_CAM 1
#define BACK_CAM 2
#define LEFT_CAM 3
#define RIGHT_CAM 4

typedef struct{
    int32 dist1,dist2;
    uint8 cam;
    car_mode_enum mode;
    int32 spd;
}seg_info;

typedef struct{
    int32 seg_cnt;
    seg_info *path;
}run_data;

void setRunData1(run_data* rdt);
void setRunData2(run_data* rdt);

#endif