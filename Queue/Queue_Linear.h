#ifndef QUEUELINEAR_H
#define QUEUELINEAR_H

#include<Template.h>

#define MAX_SIZE 5
template <typename T> class  Queue
{
    T arr[MAX_SIZE];
    int front, rear;
public:
    Queue()
    {
        front = rear = -1;
    }
    bool isEmpty()
    {
        return front == -1 && rear == -1;
    }
    bool isFull()
    {
        return rear == MAX_SIZE - 1;
    }

    void enqueue(int value)
    {
        if(rear == MAX_SIZE - 1)
        {
            cout << "Queue is full (overflow)\n";
            return;
        }
        else if(isEmpty())
        {
            front = rear = 0;
        }
        else
        {
            rear++;
        }
        arr[rear] = value;
    }

    int dequeue()
    {
        if(isEmpty())
        {  
            throw "The queue is empty (Underflow)";
        }
        else
        {
            int temp  = arr[front];
            if(front == rear)
            {  
                front = rear = -1;
            }
            else
            {
                front++;
            }
            return temp;
        }
    }


    int peek()
    {
        if(isEmpty())
        {  
            throw "Queue is empty (Underflow)\n";
        }
        else
        {
            return arr[front];
        }
    }

    void print()
    {
        if(isEmpty())
        {
            cout << "Queue is Empty\n";
        }
        else
        {
            for(int i = front; i <= rear; i++)
            {
                cout << arr[i] << " ";
            }
            cout << endl;
        }
    }
};


#endif