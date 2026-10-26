#ifndef QUEUECIRCULAR_HPP
#define QUEUECIRCULAR_HPP

#include "Template.hpp"

// Queue stored in a fixed-size circular array. front and rear wrap around
// with the modulo operator, so freed space is reused.
// size() and search() report positions counted from the front (0-based).
template <typename T>
class CircularQueue
{
private:
    static constexpr int MAX_SIZE = 4;
 
    T arr[MAX_SIZE];
    int front, rear;
 
public:
    // ---------- Constructor ----------
    CircularQueue()
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
        return front == (rear + 1) % MAX_SIZE;
    }
 
    int Size() const
    {
        if(isEmpty()) return 0;
        int count = 1;
        for(int i = front; i != rear; i = (i + 1) % MAX_SIZE) count++;
        return count;
    }
 
    void print() const
    {
        if(isEmpty())
        {
            cout << "Queue is Empty" << nl;
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
 
    int search(T value) const
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
 
    // ---------- Queue operations ----------
    void enqueue(int value)
    {
        if(isFull())
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
            rear = (rear + 1) % MAX_SIZE;
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
                front = (front + 1) % MAX_SIZE;
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
 
    // ---------- Bulk operations ----------
    void enqueueN(int n)
    {
        cout << "Enter " << n << " values:" << nl;
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
                cout << "Removed: " << dequeue() << nl;
            }
            catch(const char *msg)
            {
                cout << msg << nl;
                return;
            }
        }
    }
};

#endif