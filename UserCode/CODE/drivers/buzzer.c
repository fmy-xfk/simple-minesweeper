/*
 * buzzer.c
 *
 *  Created on: 2022Äê5ÔÂ13ÈÕ
 *      Author: xff20
 */

#include <headfile.h>

uint32 __buzzer_last_time=0,__buzzer_cnt=0;

void BuzzerInit(){
    gpio_init(B11, GPO, GPIO_LOW, GPIO_PIN_CONFIG);
    gpio_init(C30, GPO, GPIO_LOW, GPIO_PIN_CONFIG);
    pit_interrupt_ms(PIT_CH1,1);
    pit_close(PIT_CH1);
}

void BuzzerOn(){
    gpio_set(B11, GPIO_HIGH);
    gpio_set(C30, GPIO_HIGH);
}

void BuzzerOff(){
    gpio_set(B11, GPIO_LOW);
    gpio_set(C30, GPIO_LOW);
}

void BuzzerBeep(uint32 last_time){
    __buzzer_last_time=last_time;
    __buzzer_cnt=0;
    BuzzerOn();
    pit_start(PIT_CH1);
}
