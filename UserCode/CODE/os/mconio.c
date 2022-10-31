#include <stdarg.h>
#include "mconio.h"

//My Console IO for IPS200/114 图形化界面库
//Mingyu Fang 2022

int8 posx = 0;
int8 posy = 0;

/*-----------------------
 * 设置光标横坐标
 *-----------------------*/
void msetcurx(int8 x){
    assert(x >= 0);
    assert(x < MCONIO_MAX_POS_X);
    posx = x;
}

/*-----------------------
 * 设置光标纵坐标
 *-----------------------*/
void msetcury(int8 y){
    assert(y >= 0);
    assert(y < MCONIO_MAX_POS_Y);
    posy = y;
}

/*-----------------------
 * 设置光标横纵坐标
 *-----------------------*/
void msetcur(int8 x, int8 y){
    msetcurx(x); msetcury(y);
}

/*-----------------------
 * 清空屏幕
 *-----------------------*/
void mclear(){
    screen_clear(IPS114_BGCOLOR);
    msetcur(0,0);
}

/*-----------------------
 * 清空行
 *-----------------------*/
void mclearline(){
    uint8 tposy=posy;
    msetcurx(0);
    for(int i=0;i<MCONIO_MAX_POS_X;i++){
        mputchar(' ');
    }
    msetcur(0,tposy);
}

/*-----------------------
 * 换行
 *-----------------------*/
void mendl(){
    posx = 0; posy++;
    if(posy >= MCONIO_MAX_POS_Y){
        posy = 0;
        mclear();
    }
}

/*-----------------------
 * 退格
 *-----------------------*/
void mprevpos() {
    posx--;
   if(posx<0) {
       posx=MCONIO_MAX_POS_Y-1;
       posy--;
       if(posy < 0) {
           posy = 0;
           posx = 0;
       }
   }
}

/*-----------------------
 * 下一个
 *-----------------------*/
void mnextpos() {
    if(posx >= MCONIO_MAX_POS_X) {
        mendl();
    }
}

/*-----------------------
 * 打印字符
 *-----------------------*/
void mputchar(const char c){
    if(c == '\b') {
        mprevpos();
        return;
    }else if(c == '\r') {
        posx = 0;
        return;
    }else if(c == '\n') {
        mendl();
        return;
    }else{
        screen_show_char(posx * MCONIO_CHAR_WIDTH, posy * MCONIO_CHAR_HEIGHT, c);
        posx++;
        mnextpos();
    }
}

/*-----------------------
 * 打印字符串
 *-----------------------*/
int32 mputstr(const char dat[]){
    uint16_t j = 0;
    while(dat[j] != '\0')
    {
        mputchar(dat[j]);
        j++;
    }
    return j;
}

/*-----------------------
 * 打印空格串
 *-----------------------*/
int32 mputspace(uint16 cnt){
    int32 ret=cnt;
    while(cnt--)
    {
        mputchar(' ');
    }
    return ret;
}

void mputstr_ali(const char dat[], uint8 pos_type){
    uint16_t cnt = strlen(dat);
    if(cnt <= MCONIO_MAX_POS_X){
        switch(pos_type) {
        case MCONIO_POSTYPE_LEFT:
            msetcurx(0);break;
        case MCONIO_POSTYPE_MID:
            msetcurx((MCONIO_MAX_POS_X - cnt)>>1);break;
        case MCONIO_POSTYPE_RIGHT:
            msetcurx(MCONIO_MAX_POS_X - cnt);break;
        }
    }
    mputstr(dat);
}

void mputstr_fmt(const char dat[], uint8 pos_type, uint8 line){
    assert(line >= 0 && line < MCONIO_MAX_POS_Y);
    msetcury(line);
    mputstr_ali(dat,pos_type);
}

int32 mputint(int64 x){
    char s[16]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
    uint8 pos = 14;
    if(x == 0) {
        mputchar('0');
        return 1;
    }
    if(x < 0){
        mputchar('-');
        x=-x;
    }
    while(x){
        s[pos--]=(x%10)+'0';
        x/=10;
    }
    mputstr(s + pos + 1);
    return 14-pos;
}

int32 mputint2(int64 x,uint8 digit){
    char s[16]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
    uint8 pos = 14,neg=0;
    if(x == 0) {
        s[pos--]='0';
    }else if(x < 0){
        neg=1;
        x=-x;
    }
    while(x){
        s[pos--]=(x%10)+'0';
        x/=10;
    }
    if(neg)s[pos--]='-';
    while(pos>14-digit){
        s[pos--]=' ';
    }
    mputstr(s + pos + 1);
    return 14-pos;
}

uint8 mdigitcnt(int32 val){
    uint8 cnt=0;
    if(val==0) return 1;
    if(val<0) {cnt++;val=-val;}
    while(val){
        cnt++;
        val/=10;
    }
    return cnt;
}

int32 mprintf(const char *format, ...){
    va_list arg;
    va_start(arg, format);
    int cnt=0;
    while (*format) {
        int8 ret = *format;
        if ('%' == ret) {
            int8 width=0;
            format++;
            while(*format>='0' && *format<='9'){
                width=width*10+(*format)-'0';
                format++;
            }
            switch (*format){
            case 'c':{// 一个字符
                int8 ch = (int8)va_arg(arg, uint32);
                mputchar((char)ch);++cnt;
            }break;
            case 'd':case 'i':{// 有符号十进制整数
                int32 ival = (int32)va_arg(arg, int32);
                if(width>0){
                    cnt+=mputint2(ival,width);
                }else{
                    cnt+=mputint(ival);
                }
            }break;
            case 's':{// 字符串
                int8 *pc = va_arg(arg, int8 *);
                int32 len=0;
                if(width>0 && width>(len=strlen((const char*)pc))){
                    cnt+=mputspace(width-len);
                }
                cnt+=mputstr((const char*)pc);
            }break;
            case 'f':{// double
                double fval = (double)va_arg(arg, double);
                if(width>0){
                    cnt+=mputdouble(fval,width);
                }else{
                    cnt+=mputdouble(fval,4);
                }
            }break;
            case '%':// 输出字符%
                mputchar('%');++cnt;break;
            default:
                break;
            }
        }else{
            mputchar((char)(*format));++cnt;
        }
        format++;
    }
    va_end(arg);
    return cnt;
}

int32 mputdouble(double d,uint8 digit){
    int32 len=0;
    if(d<0){mputchar('-');len++;d=-d;}
    int x=(int)d;
    len+=mputint(x);d-=x;
    mputchar('.');len++;
    while(digit--){
        d*=10;x=(int)d;d-=x;
        len+=mputint(x);
    }
    return len;
}

void mtitle(const char *dat){
    uint16_t cnt = strlen(dat),beforesp=(MCONIO_MAX_POS_X - cnt)>>1;
    msetcur(0,0);
    mputspace(beforesp);mputspace(MCONIO_MAX_POS_X-beforesp-mputstr(dat));
}

void mstatus(const char *dat) {
    msetcur(0, MCONIO_MAX_POS_Y-1);
    mputspace(MCONIO_MAX_POS_X-mputstr(dat)-1);
}