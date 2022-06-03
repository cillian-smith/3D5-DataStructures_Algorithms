#include <stdlib.h>
#include <stdio.h>
#include "queue.h"


Queue *front = NULL, *rear = NULL;

//Adds an item to the queue
void enqueue(int data)
{
    Queue *temp = malloc(sizeof(Queue));
    temp->data = data;
    temp->next = NULL;
    if (isEmpty())
    {
        front = temp;
        rear = temp;
    }
    else
    {
        rear->next = temp;
        rear = temp;
    }
}

//Removes an item from the queue
void dequeue()
{
    if (isEmpty())
    {
        return;
    }
    Queue *temp = front;
    front = front->next;
    free(temp);
}

//Checks if the queue is empty
int isEmpty()
{
    if (front == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//Retrieves data at front of queue
int frontData()
{
    if (front == NULL)
    {
        return -1;
    }
    return front->data;
}