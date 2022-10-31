/*
 * keys.h
 *
 *  Created on: 2022Äê3ÔÂ3ÈÕ
 *      Author: xff20
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "syshead.h"

#define K_NONE 0

void init_keys();
void enter_BIOS();
bool key_down_immediate(uint8 keyid);
bool key_down(uint8 keyid);
uint8 wait_key();
uint8 wait_key_ms(uint32 ms);
uint8 get_key_immediate();

#endif /* KEYS_H_ */
