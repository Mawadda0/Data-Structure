#ifndef QUEUECIRCULAR_H
#define QUEUECIRCULAR_H

#include "Template.hpp"

#define MAX_SIZE 4

template <typename T> class CircularQueue
{
    T arr[MAX_SIZE];
    int front, rear;
public:
    CircularQueue()
    {
        front = rear = -1;
    }
    bool isEmpty()
    {
        return front == -1 && rear == -1;
    }
    bool isFull()
    {
        return front == (rear + 1) % MAX_SIZE;
    }

    void enqueue(int value)
    {
        if(isFull())
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
            rear = (rear + 1) % MAX_SIZE;
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
            if(front == rear){  
                front = rear = -1;
            }
            else
            {
                front = (front + 1) % MAX_SIZE;
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
            for(int i = front; i != rear; i = (i + 1) % MAX_SIZE)
            {
                cout << arr[i] << " ";
            }
            cout << arr[rear] << nl;
        }
    }

    int Size()
    {
        if(isEmpty()) return 0;
        int count = 1;
        for(int i = front; i != rear; i = (i + 1) % MAX_SIZE)  count++;
        return count;
    }

    void enqueueN(int n)
    {
        cout << "Enter " << n << " values:\n";
        for(int i = 0; i < n; i++)
        {
            T x;
            cin >> x;
            enqueue(x);
        }
    }

    void dequeueN(int n)
    {
        for(int i = 0; i < n; i++)
        {
            try
            {
                cout << "Removed: " << dequeue() << "\n";
            }
            catch(const char* msg)
            {
                cout << msg << "\n";
                return;
            }
        }
    }

    int search(T value)
    {
        if(isEmpty()) return -1;
        int pos = 0;
        for(int i = front; ; i = (i + 1) % MAX_SIZE)
        {
            if(arr[i] == value) return pos;
            if(i == rear) break;
            pos++;
        }
        return -1;
    }
};

#endif