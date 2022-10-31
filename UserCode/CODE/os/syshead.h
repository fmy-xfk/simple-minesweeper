#ifndef __SYSHEAD_H__
#define __SYSHEAD_H__

#include <stdarg.h>
#include <math.h>
#include <zf_headfile.h>

#define min(a,b) (((a)>(b))?(b):(a))
#define max(a,b) (((a)>(b))?(a):(b))

//��Ҫ�û��޸ĵĲ��֣���ʼ
//��Ļ����
#define MCONIO_SCREEN_WIDTH 240
#define MCONIO_SCREEN_HEIGHT 135 //320//135
#define MCONIO_CHAR_WIDTH 8
#define MCONIO_CHAR_HEIGHT 19//20//19

//�������
#define screen_show_char ips114_showchar
#define screen_clear ips114_clear
#define screen_init ips114_init

//���в���
#define MAX_QUEUE_SIZE 129

//��ť����
#define KEYS_COUNT  4
extern uint32 ALL_KEYS[KEYS_COUNT];

//���������ð������ţ�������syshead.c�е�ALL_KEYS����
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

//��Ҫ�û��޸ĵĲ��֣�����

void init_key(uint32 pin);
bool is_key_pressed(uint32 pin);

void mdelay_ms(uint32 ms);
void mdelay_us(uint32 us);

#endif