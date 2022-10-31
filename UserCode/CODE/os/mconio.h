/*
 * mconio.h
 *
 *  Created on: 2022Äê3ÔÂ3ÈÕ
 *      Author: xff20
 */

#ifndef MLIB__MCONIO_H_
#define MLIB__MCONIO_H_

#include "syshead.h"

#define MCONIO_MAX_POS_X (MCONIO_SCREEN_WIDTH/MCONIO_CHAR_WIDTH)
#define MCONIO_MAX_POS_Y (MCONIO_SCREEN_HEIGHT/MCONIO_CHAR_HEIGHT)
#define MCONIO_POSTYPE_AUTO 0
#define MCONIO_POSTYPE_LEFT 1
#define MCONIO_POSTYPE_MID 2
#define MCONIO_POSTYPE_RIGHT 3

void msetpencolor(uint16 color);
void msetbgcolor(uint16 color);
void msetcolor(uint16 pen,uint16 bg);
void mswapcolor();
void msetcurx(int8 x);
void msetcury(int8 y);
void msetcur(int8 x, int8 y);
void mclear();
void mclear_color(uint16 color);
void mclearline();
void mendl();
void mprevpos();
void mnextpos();
void mputchar(const char c);
int32 mputstr(const char dat[]);
int32 mputspace(uint16 cnt);
void mputstr_ali(const char dat[], uint8 pos_type);
void mputstr_fmt(const char dat[], uint8 pos_type, uint8 line);
int32 mputint(int64 x);
int32 mputint2(int64 x,uint8 digit);
uint8 mdigitcnt(int32 val);
int32 mprintf(const char *format, ...);
void mtitle(const char *dat);
void mstatus(const char *dat);
int32 mputdouble(double d,uint8 digit);
void my_double_to_str(char *str, double number, uint8 point_bit);

#define mputstr_mid(s) mputstr_ali((s), MCONIO_POSTYPE_MID)

#endif /* MCONIO_H_ */
