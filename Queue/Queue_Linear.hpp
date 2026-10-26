#ifndef QUEUELINEAR_HPP
#define QUEUELINEAR_HPP

#include "Template.hpp"
 
// Queue stored in a fixed-size array. front and rear only move forward,
// so space freed by dequeue is not reused until the queue becomes empty.
template <typename T>
class Queue
{
private:
    static constexpr int MAX_SIZE = 5;
 
    T arr[MAX_SIZE];
    int front, rear;
 
public:
    // ---------- Constructor ----------
    Queue()
    {
        front = rear = -1;
    }
 
    // ---------- Utilities ----------
    bool isEmpty() const
    {
        return front == -1 && rear == -1;
    }
 
    bool isFull() const
    {
        return rear == MAX_SIZE - 1;
    }
 
    void print() const
    {
        if(isEmpty())
        {
            cout << "Queue is Empty" << nl;
        }
        else
        {
            for(int i = front; i <= rear; i++)
            {
                cout << arr[i] << " ";
            }
            cout << '\n';
        }
    }
 
    // ---------- Queue operations ----------
    void enqueue(int value)
    {
        if(rear == MAX_SIZE - 1)
        {
            cout << "Queue is full (overflow)" << nl;
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
            throw "The queue is empty (Underflow)\n";
        }
        else
        {
            int temp = arr[front];
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
 
    int peek() const
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
};

#endif