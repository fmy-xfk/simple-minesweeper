/*
 * queue.c 数据结构：队列
 *
 *  Created on: Jul 18, 2022
 *      Author: xff20
 */

#include "queue.h"

/*
 * 清空队列
 */
void qClear(queue *q) {
    q->front = q->rear = 0;
}

/*
 * 判定队列是否为空
 */
bool qEmpty(queue *q) {
    return q->front == q->rear;
}

/*
 * 获得队列中元素个数
 */
int32 qSize(queue *q) {
    return (q->rear - q->front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
}

/*
 * 获得队首元素
 */
int32 qFront(queue *q) {
    if (q->front == q->rear)
        return -1;
    return q->base[q->front];
}

/*
 * 加入队列
 */
int32 qPush(queue *q, uint8 e) {
    int32 ret=-1;
    if ((q->rear + 1) % MAX_QUEUE_SIZE == q->front)
        ret=qPop(q);
    q->base[q->rear] = e;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    return ret;
}

/*
 * 将一个数组一次加入队列
 */
void qPushArray(queue *q, uint8 *dat, int32 len){
    uint8 *p=dat,*pend=dat+len;
    while(p<pend){
        qPush(q,*p);
        p++;
    }
}

/*
 * 弹出队列
 */
int32 qPop(queue *q) {
    int32 ret=-1;
    if (q->front == q->rear)
        return -1;
    ret = q->base[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return ret;
}

/*
 * 将队列中的元素弹出到数组
 */
int32 qPopArray(queue *q, uint8 *dat, int32 maxlen) {
    int32 len=0;
    while(len<maxlen){
        int32 d=qPop(q);
        if(d>=0){
            dat[len++]=d;
        }else{
            break;
        }
    }
    return len;
}

/*
 * 将队列中的所有元素弹出到数组
 */
int32 qPopAll(queue *q, uint8 *dat) {
    int32 len=0;
    while(q->front != q->rear){
        *dat=q->base[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        ++len;++dat;
    }
    return len;
}
