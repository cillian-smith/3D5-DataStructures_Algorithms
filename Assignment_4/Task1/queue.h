#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdlib.h>

typedef struct Queue Queue;
struct Queue
{
    int data;
    Queue *next;
};

void enqueue(int data);
void dequeue();
int isEmpty();
int frontData();

#endif