/*
 * keys.c
 *
 *  Created on: 2022Äê3ÔÂ3ÈÕ
 *      Author: xff20
 */
#include "keys.h"

void init_keys() {
    int i;
    for(i=0;i<KEYS_COUNT;++i){
        init_key(ALL_KEYS[i]);
    }
}

bool key_down_immediate(uint8 keyid) {
    uint8 cnt=0;
    if(is_key_pressed(keyid)){
        for(uint8 i=0;i<20;i++) {
            if(is_key_pressed(keyid)) cnt++;
            mdelay_us(10);
        }
    }
    return cnt==20;
}

bool key_down(uint8 keyid) {
    int i,cnt=0;
    if(key_down_immediate(keyid)){
        for(i=0;i<10;i++){
            if(key_down_immediate(keyid)){
                cnt++;
            }
            mdelay_ms(10);
        }
        return cnt>8;
    }else{
        return 0;
    }
}

uint8 wait_key() {
    while(1){
        int i;
        for(i=0;i<KEYS_COUNT;++i){
            if(key_down(ALL_KEYS[i])) return ALL_KEYS[i];
        }
        mdelay_ms(10);
    }
}

uint8 wait_key_ms(uint32 ms) {
    uint32 cnt=0;
    while(cnt<=ms){
        int i;
        for(i=0;i<KEYS_COUNT;++i){
            if(key_down_immediate(ALL_KEYS[i])) return ALL_KEYS[i];
        }
        mdelay_ms(99);
        cnt+=100;
    }
    return K_NONE;
}

uint8 get_key_immediate() {
    int i;
    for(i=0;i<KEYS_COUNT;++i){
        if(key_down(ALL_KEYS[i])) return ALL_KEYS[i];
    }
    return K_NONE;
}
