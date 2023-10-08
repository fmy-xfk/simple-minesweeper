#include "serial.h"

extern LPUART_Type * UARTN[];
uint8 serial_cnt=0;
serial serials[4];

void uart_callback_inner(serial *ser, LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData) {
    if(kStatus_LPUART_RxIdle == status){
        qPush(&ser->uart_data,ser->rx_buffer);
        if(ser->eh!=NULL) ser->eh(&ser->uart_data,ser->rx_buffer);
    }
    handle->rxDataSize = ser->receivexfer.dataSize;
    handle->rxData = ser->receivexfer.data;
}

void uart_callback0(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData) {
    uart_callback_inner(serials,base,handle,status,userData);
}

void uart_callback1(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData) {
    uart_callback_inner(serials+1,base,handle,status,userData);
}

void uart_callback2(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData) {
    uart_callback_inner(serials+2,base,handle,status,userData);
}

void uart_callback3(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData) {
    uart_callback_inner(serials+3,base,handle,status,userData);
}

lpuart_transfer_callback_t callbacks[4]={uart_callback0,uart_callback1,uart_callback2,uart_callback3};

/* Serial port initialization
 * num: Number of the serial port
 * tx: TX pin
 * rx: RX pin
 * eh: NULL to disable event-driven mode; pointer of the event handler to enable event-driven mode
 */
serial* serial_init(UARTN_enum num,UARTPIN_enum tx,UARTPIN_enum rx,char_recv_event_handler eh){
    serial *ser=serials+serial_cnt;
    ser->num=num;
    uart_init(num,115200,tx,rx);
    ser->receivexfer.dataSize = 1;
    ser->receivexfer.data = &ser->rx_buffer; 
    qClear(&ser->uart_data);
    ser->eh=eh;
    if(eh!=NULL){
        uart_rx_irq(num,1);
        uart_set_handle(num, &ser->g_lpuartHandle, callbacks[serial_cnt++], NULL, 0, ser->receivexfer.data, 1);
    }
    NVIC_SetPriority(LPUART1_IRQn,3);
    return ser;
}

void serial_write(serial* ser,int32 num,uint8 *dat){
    uart_putbuff(ser->num,dat,num);
}

void serial_write_byte(serial* ser,uint8 dat){
    uart_putchar(ser->num,dat);
}

int32 serial_read(serial*ser,int32 num,uint8 *dat){
    return qPopArray(&ser->uart_data,dat,num);
}

int32 serial_read_byte(serial* ser){
    return qPop(&ser->uart_data);
}

void serial_close(serial *ser){
    uart_tx_irq(ser->num,0);
    uart_rx_irq(ser->num,0);
    LPUART_Deinit(UARTN[ser->num]);   
}