#ifndef STACKARRAY_H
#define STACKARRAY_H

#include "Template.hpp"

// Stack with a fixed capacity, stored in an array.
// Search reports the index counted from the bottom of the stack (0-based).
template <typename T>
class Stack
{
private:
    static constexpr int MAX = 100;
 
    int top;
    T arr[MAX];
 
public:
    // ---------- Constructor ----------
    Stack()
    {
        top = -1;
    }
 
    // ---------- Utilities ----------
    bool IsEmpty() const
    {
        return top == -1;
    }
 
    void Search(T val) const
    {
        if(IsEmpty())
        {
            cout << "Stack Is Empty." << nl;
            return;
        }
        int i;
        for(i = top; i >= 0; i--)
        {
            if(arr[i] == val)
            {
                cout << "Value found at position : " << i << nl;
                return;
            }
        }
        if(i == 0 && arr[i] != val)
        {
            cout << "Value isn't found" << nl;
            return;
        }
    }
 
    void Print() const
    {
        if(IsEmpty())
        {
            cout << "Stack Is Empty." << nl;
            return;
        }
        for(int i = top; i >= 0; i--) cout << arr[i] << " ";
        cout << nl;
    }
 
    // ---------- Stack operations ----------
    void Push(T val)
    {
        if(top == MAX - 1)
        {
            cout << "Stack Overflow." << nl;
            return;
        }
        arr[++top] = val;
    }
 
    T Pop()
    {
        if(IsEmpty())
        {
            cout << "Stack UnderFlow." << nl;
            return;
        }
        T x = arr[top];
        top--;
        return x;
    }
 
    T Peek() const
    {
        if(IsEmpty())
        {
            cout << "Stack Underflow." << nl;
            return;
        }
        return arr[top];
    }
};

#endif