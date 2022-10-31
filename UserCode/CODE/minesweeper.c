#include "minesweeper.h"

seg_info path1[]={
    {240,   240,    NULL_CAM,   MODE_USOUND_RB, 1000},
    {240,   240,    FRONT_CAM,  MODE_USOUND_FR, 1000},
    {240,   240,    LEFT_CAM,   MODE_USOUND_LF, 1000},
    {240,   240,    BACK_CAM,   MODE_USOUND_BL, 1000},
    {550,   240,    RIGHT_CAM,  MODE_USOUND_RB, 1000},
    {550,   550,    FRONT_CAM,  MODE_USOUND_FR, 1000},
    {550,   550,    LEFT_CAM,   MODE_USOUND_LF, 1000},
    {550,   550,    BACK_CAM,   MODE_USOUND_BL, 1000},
    {900,   550,    RIGHT_CAM,  MODE_USOUND_RB, 1000},
    {900,   900,    FRONT_CAM,  MODE_USOUND_FR, 1000},
    {900,   900,    LEFT_CAM,   MODE_USOUND_LF, 1000},
    {900,   900,    BACK_CAM,   MODE_USOUND_BL, 1000},
    {1200,  900,    RIGHT_CAM,  MODE_USOUND_RB, 1000},
    {1200,  1200,   FRONT_CAM,  MODE_USOUND_FR, 1000},
    {1225,  1200,   LEFT_CAM,   MODE_USOUND_LF, 1000},
    {1225,  1225,   BACK_CAM,   MODE_USOUND_BL, 1000},
    {1500,  1225,   RIGHT_CAM,  MODE_USOUND_RB, 1000},
    {240,   240,    NULL_CAM,   MODE_USOUND_RB, 1000},
};

seg_info path2[]={
    {260,   260,    NULL_CAM,   MODE_USOUND_RB, 1200},
    {280,   280,    FRONT_CAM,  MODE_USOUND_FR, 1200},
    {280,   280,    LEFT_CAM,   MODE_USOUND_LF, 1200},
    {280,   280,    BACK_CAM,   MODE_USOUND_BL, 1200},
    {550,   300,    RIGHT_CAM,  MODE_USOUND_RB, 1200},
    {550,   550,    FRONT_CAM,  MODE_USOUND_FR, 1200},
    {550,   550,    LEFT_CAM,   MODE_USOUND_LF, 1200},
    {550,   550,    BACK_CAM,   MODE_USOUND_BL, 1200},
    {900,   550,    RIGHT_CAM,  MODE_USOUND_RB, 1200},
    {900,   900,    FRONT_CAM,  MODE_USOUND_FR, 1100},
    {900,   900,    LEFT_CAM,   MODE_USOUND_LF, 1100},
    {900,   900,    BACK_CAM,   MODE_USOUND_BL, 1100},
    {1200,  900,    RIGHT_CAM,  MODE_USOUND_RB, 1100},
    {1225,  1200,   FRONT_CAM,  MODE_USOUND_FR, 1000},
    {1225,  1225,   LEFT_CAM,   MODE_USOUND_LF, 1000},
    {1225,  1225,   BACK_CAM,   MODE_USOUND_BL, 1000},
    {1500,  1225,   RIGHT_CAM,  MODE_USOUND_RB, 1000},
    {260,   260,    NULL_CAM,   MODE_USOUND_RB, 1300},
};

void setRunData1(run_data* rdt){
    rdt->path=path1;
    rdt->seg_cnt=sizeof(path1)/sizeof(seg_info);
}

void setRunData2(run_data* rdt){
    rdt->path=path2;
    rdt->seg_cnt=sizeof(path2)/sizeof(seg_info);
}