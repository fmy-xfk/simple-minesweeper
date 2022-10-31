#include "os/os_dev.h"
#include "headfile.h"
#include "apps/__applist.h"

/********************
*  ��ʱ����
*********************/
void mdelay_ms(uint32 ms){
    systick_delay_ms(ms);
}

void mdelay_us(uint32 us){
    systick_delay_us(us);
}

/********************
*  ��������
*********************/
uint32 ALL_KEYS[KEYS_COUNT]={C31,C27,C26,C4};

//ָ��������ʼ����ʽ
void init_key(uint32 pin){
    gpio_init(pin, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
}

//ָ�����������Ƿ��µ��жϷ�ʽ
bool is_key_pressed(uint32 pin){
    return !gpio_get(pin);
}

/********************
*  ϵͳ����
*********************/
//ָ��ϵͳ��������
system_data SYSMAIN={
    //ָ��ϵͳӦ��
    {
        {"Normal Run",move_test},
        {"Quick Run",move_test2},
        {"USound Test",usound_test},
        {"ICM Test",icm_test},
        {"PWM Checker",pwm_checker},
        {"Speed Test",spd_mea},
        {"Sector Editor",sector_editor},
    },
    //ָ��ϵͳӦ������
    7,
    //���������û�ָ��
    {0},
    //ָ��ϵͳ����ʹ�õ��������
    1023
};

//ָ��ϵͳ����ǰ��Ҫ��ʼ��������
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

//ָ��ϵͳ�������ٳ�ʼ��������
void os_postinit(){
    icm_init();
    icm_auto_calib();
}

