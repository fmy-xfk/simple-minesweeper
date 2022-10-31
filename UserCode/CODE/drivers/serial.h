#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "headfile.h"

typedef void (*char_recv_event_handler)(queue *dat_que,uint8 this_char);

typedef struct{
    UARTN_enum num;
    uint8 rx_buffer;
    lpuart_transfer_t   receivexfer;
    lpuart_handle_t     g_lpuartHandle;
    queue uart_data;
    char_recv_event_handler eh;
}serial;

extern uint8 serial_cnt;
extern serial serials[4];

void uart_callback0(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
void uart_callback1(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
void uart_callback2(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
void uart_callback3(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
extern lpuart_transfer_callback_t callbacks[4];

serial* serial_init(UARTN_enum num,UARTPIN_enum tx,UARTPIN_enum rx,char_recv_event_handler eh);
void serial_write(serial* ser,int32 num,uint8 *dat);
void serial_write_byte(serial* ser,uint8 dat);
int32 serial_read(serial* ser,int32 num,uint8 *dat);
int32 serial_read_byte(serial* ser);
void serial_close(serial* ser);

#endif