#include "os/os_dev.h"
#include "headfile.h"
#include "apps/__applist.h"

/********************
*  延时部分
*********************/
void mdelay_ms(uint32 ms){
    systick_delay_ms(ms);
}

void mdelay_us(uint32 us){
    systick_delay_us(us);
}

/********************
*  按键部分
*********************/
uint32 ALL_KEYS[KEYS_COUNT]={C31,C27,C26,C4};

//指定按键初始化方式
void init_key(uint32 pin){
    gpio_init(pin, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
}

//指定按键引脚是否按下的判断方式
bool is_key_pressed(uint32 pin){
    return !gpio_get(pin);
}

/********************
*  系统部分
*********************/
//指定系统基本参数
system_data SYSMAIN={
    //指定系统应用
    {
        {"Normal Run",move_test},
        {"Quick Run",move_test2},
        {"USound Test",usound_test},
        {"ICM Test",icm_test},
        {"PWM Checker",pwm_checker},
        {"Speed Test",spd_mea},
        {"Sector Editor",sector_editor},
    },
    //指定系统应用数量
    7,
    //此项无需用户指定
    {0},
    //指定系统配置使用的扇区编号
    1023
};

//指定系统启动前需要初始化的内容
void os_preinit(){
    //Motor
    const float kp=50.0f,ki=5.0f,kd=0.0f;
    motor_init(MTR_LF,PWM1_MODULE0_CHA_D12,D14,QTIMER_3,QTIMER3_TIMER2_B18,QTIMER3_TIMER3_B19,true,kp,ki,kd);
    motor_init(MTR_RF,PWM1_MODULE0_CHB_D13,D15,QTIMER_2,QTIMER2_TIMER0_C3,QTIMER2_TIMER3_C25,false,kp,ki,kd);
    motor_init(MTR_LB,PWM2_MODULE3_CHA_D2,D0,QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1,true,kp,ki,kd);
    motor_init(MTR_RB,PWM2_MODULE3_CHB_D3,D1,QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24,false,kp,ki,kd);
    //Buzzer
    BuzzerInit();
    BuzzerOn();
    mdelay_ms(100);
    BuzzerOff();
    //USound
    usound_init_all(C9,C8,C13,C12,C15,C14,C11,C10);
}

//指定系统启动后再初始化的内容
void os_postinit(){
    icm_init();
    icm_auto_calib();
}

