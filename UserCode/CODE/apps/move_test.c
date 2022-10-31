#include "minesweeper.h"

#define sqr(x) ((x)*(x))

run_data mrd;
seg_info cur_seg;

int32 dist[4];
bool meet_mine=false;

int32 get_num(char *beg,int32 *ret){
    int32 f=1,num=0,len=0;
    if(*beg=='-') {f=-1;++beg;++len;}
    while(*beg>='0' && *beg<='9') {
        num=(num<<3)+(num<<1)+(*beg)-'0';
        ++beg;++len;
    }
    *ret=num;
    return len;
}

void openmv_recv_eh(queue *que,uint8 dat){
    if(dat==';'){
        char str[MAX_QUEUE_SIZE];
        int32 len=qSize(que);
        qPopAll(que,(uint8*)str);str[len]='\0';
        if(str[len-1]!=';' || str[0]!='O') {
            return;
        }
        int32 x,y,l2;
        l2=get_num(str+1,&x);
        if(str[l2+1]!=','){
            return;
        }
        l2+=get_num(str+l2+2,&y);
        if(l2+3!=len){
            return;
        }
        //add_pts(x,y);
        meet_mine=true;
    }
}

void ehF(queue *que,uint8 dat){
    if(meet_mine || cur_seg.cam!=FRONT_CAM){
        qClear(que);
    }else{
        openmv_recv_eh(que,dat);
    }
}

void ehB(queue *que,uint8 dat){
    if(meet_mine || cur_seg.cam!=BACK_CAM){
        qClear(que);
    }else{
        openmv_recv_eh(que,dat);
    }
}

void ehL(queue *que,uint8 dat){
    if(meet_mine || cur_seg.cam!=LEFT_CAM){
        qClear(que);
    }else{
        openmv_recv_eh(que,dat);
    }
}

void ehR(queue *que,uint8 dat){
    if(meet_mine || cur_seg.cam!=RIGHT_CAM){
        qClear(que);
    }else{
        openmv_recv_eh(que,dat);
    }
}

int32 get_dist(point a,point b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

void my_print(){
    int32 i;
    for(i=0;i<4;i++){
        msetcur(1,i);
        mprintf("PWM:%6d ECD:%6d",motors[i].duty,motors[i].ecdval);
    }
    msetcur(1,4);
    mprintf("SPD:(%4d,%4d,%4d,%4d))\n CUR:(%4d,%4d,%4d,%4d)\nSPD:%5d",
        (int)MTR_LF->pid.target,(int)MTR_RF->pid.target,
        (int)MTR_LB->pid.target,(int)MTR_RB->pid.target,
        CUR_USD[0],CUR_USD[1],CUR_USD[2],CUR_USD[3],mecanum_get_spd());
}

int move_test_inner(int mode){
    if(mode==1){
        setRunData1(&mrd);
    }else{
        setRunData2(&mrd);
    }
    int32 i,ptr=0,cnt=mrd.seg_cnt;
    serial *serB=serial_init(USART_1,UART1_TX_B12,UART1_RX_B13,ehB);
    serial *serL=serial_init(USART_5,UART5_TX_C28,UART5_RX_C29,ehL);
    serial *serF=serial_init(USART_4,UART4_TX_C16,UART4_RX_C17,ehF);
    serial *serR=serial_init(USART_8,UART8_TX_D16,UART8_RX_D17,ehR);
    mecanum_clear();
    cur_seg=mrd.path[ptr++];
    mecanum_move_to(cur_seg.mode,cur_seg.spd,cur_seg.dist1,cur_seg.dist2);
    AddTimer(mecanum_callback,PID_PERIOD_MS,"mt");
    while(!key_down(K_YES)){
        usound_get_dist_certain(CUR_USD,cur_seg.mode>>1);
        my_print();
        if(meet_mine && RUNNING){
            mdelay_ms(14000/mecanum_get_spd());
            mecanum_stop();
            BuzzerOn();
            mdelay_ms(1800);
            BuzzerOff();
            mecanum_move_to(cur_seg.mode,cur_seg.spd,cur_seg.dist1,cur_seg.dist2);
            for(i=0;i<2;i++){
                usound_get_dist_certain(CUR_USD,cur_seg.mode>>1);
                my_print();
            }
            meet_mine=false;
        }
        if(!RUNNING && ptr<cnt){
            mdelay_ms(80);
            cur_seg=mrd.path[ptr++];
            meet_mine=false;
            mecanum_move_to(cur_seg.mode,cur_seg.spd,cur_seg.dist1,cur_seg.dist2);
        }
        if(!RUNNING && ptr>=cnt){
            mecanum_stop();
            break;
        }
        mdelay_ms(5);
    }
    for(i=0;i<4;i++) motor_set_pwm(motors+i,0);
    RemoveTimer("mt");
    serial_close(serF);
    serial_close(serB);
    return 0;
}

int move_test(flash_data_union *dat){
    return move_test_inner(1);
}

int move_test2(flash_data_union *dat){
    return move_test_inner(2);
}