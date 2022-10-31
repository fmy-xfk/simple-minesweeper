/*
 * util.c ÆäËûº¯Êı
 *
 *  Created on: Aug 1, 2022
 *      Author: xff20
 */

#include "util.h"

float fsgn(float x){
    if(x>1e-6){
        return 1.0f;
    }else if(x<-1e-6){
        return -1.0f;
    }else{
        return 0;
    }
}

bool inrange(int x,int r){
    if(r<0) r=-r;
    return -r<=x && x<=r;
}

int32 get_true_count(int32 n,...){
    va_list arg;int32 cnt=0;
    va_start(arg, n);
    for(int i=0;i<n;i++){
        bool b=(bool)va_arg(arg,uint32);
        if(b) cnt++;
    }
    va_end(arg);
    return cnt;
}