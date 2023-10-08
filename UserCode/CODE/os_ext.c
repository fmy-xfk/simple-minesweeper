#include "os/os_dev.h"
#include "headfile.h"
#include "apps/__applist.h"

/********************
*  Delay functions
*********************/
void mdelay_ms(uint32 ms){
    systick_delay_ms(ms);
}

void mdelay_us(uint32 us){
    systick_delay_us(us);
}

/********************
*  Keys
*********************/
uint32 ALL_KEYS[KEYS_COUNT]={C31,C27,C26,C4};

//Indicate how to initialize the key
void init_key(uint32 pin){
    gpio_init(pin, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
}

//Indicate how to determine a key is pressed
bool is_key_pressed(uint32 pin){
    return !gpio_get(pin);
}

/********************
*  System
*********************/
//Basic parameters of the system
system_data SYSMAIN={
    //Built-in applications
    {
        {"Normal Run",move_test},       //Main program
        {"Quick Run",move_test2},       //Main program
        {"USound Test",usound_test},    //Checker
        {"ICM Test",icm_test},          //Checker
        {"PWM Checker",pwm_checker},    //Checker
        {"Speed Test",spd_mea},         //Obsolete!
        {"Sector Editor",sector_editor},//Obsolete!
    },
    //Number of applications
    7,
    //Reserved value. You should not change it!
    {0},
    //Index of sector where system settings are stored
    1023
};

//Initialization before the system boots
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

//Initialization after the system boots
void os_postinit(){
    icm_init();
    icm_auto_calib();
}

