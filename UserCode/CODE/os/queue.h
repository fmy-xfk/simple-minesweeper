#ifndef MLIB_QUEUE_H_
#define MLIB_QUEUE_H_

#include "syshead.h"

typedef struct {
    uint8 base[MAX_QUEUE_SIZE+1];
    volatile int32 front,rear;
} queue;

void qClear(queue *q);
bool qEmpty(queue *q);
int32 qSize(queue *q);
int32 qFront(queue *q);
int32 qPush(queue *q, uint8 e);
void qPushArray(queue *q, uint8 *dat, int32 len);
int32 qPop(queue *q);
int32 qPopArray(queue *q, uint8 *dat, int32 maxlen);
int32 qPopAll(queue *q, uint8 *dat);


#endif /* MLIB_QUEUE_H_ */
