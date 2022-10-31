#ifndef __SYSHEAD_H__
#define __SYSHEAD_H__

#include <stdarg.h>
#include <math.h>
#include <zf_headfile.h>

#define min(a,b) (((a)>(b))?(b):(a))
#define max(a,b) (((a)>(b))?(a):(b))

//需要用户修改的部分：开始
//屏幕部分
#define MCONIO_SCREEN_WIDTH 240
#define MCONIO_SCREEN_HEIGHT 135 //320//135
#define MCONIO_CHAR_WIDTH 8
#define MCONIO_CHAR_HEIGHT 19//20//19

//输出函数
#define screen_show_char ips114_showchar
#define screen_clear ips114_clear
#define screen_init ips114_init

//队列部分
#define MAX_QUEUE_SIZE 129

//按钮引脚
#define KEYS_COUNT  4
extern uint32 ALL_KEYS[KEYS_COUNT];

//在这里设置按键引脚，并加入syshead.c中的ALL_KEYS数组
#define K_UP        C31
#define K_DOWN      C27
#define K_LEFT      
#define K_RIGHT     
#define K_PRESS     

#define K_PREV      C31
#define K_NEXT      C27
#define K_YES       C4
#define K_NO        C26
#define K_CANCEL    C26

#define K_EDIT      C26
#define K_SAVE      
#define K_UNDO      
#define K_REDO      

//需要用户修改的部分：结束

void init_key(uint32 pin);
bool is_key_pressed(uint32 pin);

void mdelay_ms(uint32 ms);
void mdelay_us(uint32 us);

#endif