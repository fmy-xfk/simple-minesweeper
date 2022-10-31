/*
 * queue.c ���ݽṹ������
 *
 *  Created on: Jul 18, 2022
 *      Author: xff20
 */

#include "queue.h"

/*
 * ��ն���
 */
void qClear(queue *q) {
    q->front = q->rear = 0;
}

/*
 * �ж������Ƿ�Ϊ��
 */
bool qEmpty(queue *q) {
    return q->front == q->rear;
}

/*
 * ��ö�����Ԫ�ظ���
 */
int32 qSize(queue *q) {
    return (q->rear - q->front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
}

/*
 * ��ö���Ԫ��
 */
int32 qFront(queue *q) {
    if (q->front == q->rear)
        return -1;
    return q->base[q->front];
}

/*
 * �������
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
 * ��һ������һ�μ������
 */
void qPushArray(queue *q, uint8 *dat, int32 len){
    uint8 *p=dat,*pend=dat+len;
    while(p<pend){
        qPush(q,*p);
        p++;
    }
}

/*
 * ��������
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
 * �������е�Ԫ�ص���������
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
 * �������е�����Ԫ�ص���������
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
