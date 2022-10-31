/*
 * buzzer.h
 *
 *  Created on: 2022Äê5ÔÂ13ÈÕ
 *      Author: xff20
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include <headfile.h>

extern uint32 __buzzer_last_time,__buzzer_cnt;

void BuzzerInit();
void BuzzerOn();
void BuzzerOff();
void BuzzerBeep(uint32 last_time);
#endif /* BUZZER_H_ */
