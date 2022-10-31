#include "headfile.h"

int sector_editor(flash_data_union* data){
    char namelist[][20]={
        "PAR1",
        "PAR2",
        "PAR3",
        "PAR4",
        "PAR5"
    };
    os_ui_edit_list("SecEdit",(char*)namelist,20,data,5);
    return 0;
}