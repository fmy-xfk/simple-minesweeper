#include "usound.h"

USound *cur_usound=NULL;

void usound_handler(){
    if(cur_usound==NULL) {
        pit_close(PIT_CH2);
        return;
    }
    ++cur_usound->wktime;
    if(cur_usound->wktime>=MAX_USOUND_TIME_US || !gpio_get(cur_usound->echo)){
        pit_close(PIT_CH2);
        cur_usound->working=false;
        cur_usound->fl_time=cur_usound->fl_time_buff;
        cur_usound=NULL;
    }else{
        cur_usound->fl_time_buff++;
    }
}

void usound_init(USound *usd,PIN_enum trig,PIN_enum echo){
    int i;
    usd->trigger=trig;usd->echo=echo;
    gpio_init(trig, GPO, 0, GPIO_PIN_CONFIG);
    gpio_init(echo, GPI, 0, GPIO_PIN_CONFIG);
}

bool usound_is_working(){
    return cur_usound!=NULL;
}

bool usound_send(USound *usd){
    if(cur_usound!=NULL) return false;
    gpio_set(usd->trigger, 1);
    mdelay_us(10);
    gpio_set(usd->trigger, 0);
    usd->fl_time_buff=0;
    usd->wktime=0;
    usd->working=true;
    cur_usound=usd;
    while(!gpio_get(cur_usound->echo));
    pit_interrupt_us(PIT_CH2,1);
    return true;
}

//声速公式v=331+0.61t，t为摄氏温度
int32 usound_get(USound *usd){
    return max((int32)(0.25736*(double)usd->fl_time-36.4414),0);
}

USound __usd[4];

void usound_init_all(PIN_enum trig1,PIN_enum echo1,
    PIN_enum trig2,PIN_enum echo2,
    PIN_enum trig3,PIN_enum echo3,
    PIN_enum trig4,PIN_enum echo4){
    usound_init(__usd  ,trig1,echo1);
    usound_init(__usd+1,trig2,echo2);
    usound_init(__usd+2,trig3,echo3);
    usound_init(__usd+3,trig4,echo4);
}

//请注意，该函数是阻塞的，运行约需200ms
void usound_get_dist_all(int32 *dist){
    int32 i;
    for(i=0;i<4;i++){
        usound_send(__usd+i);
        while(usound_is_working());
        dist[i]=usound_get(__usd+i);
    }
}

//mode 2^0:Front  2^1:Right  2^2:Back  2^3:Left
void usound_get_dist_certain(int32 *dist,int32 mode){
    int32 i;
    for(i=0;i<4;i++){
        if(mode&1) {
            usound_send(__usd+i);
            while(usound_is_working());
            dist[i]=usound_get(__usd+i);
            mdelay_ms(5);
        }else{
            dist[i]=-1;
        }
        mode>>=1;
    }
}