#ifndef STACKARRAY_H
#define STACKARRAY_H

#include "Template.hpp"

// Stack with a fixed capacity, stored in an array.
// Search reports the index counted from the bottom of the stack (0-based).
class Stack
{
private:
    static constexpr int MAX = 100;
 
    int top;
    int arr[MAX];
 
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
 
    void Search(int val) const
    {
        if(IsEmpty())
        {
            cout << "Stack Is Empty." << '\n';
            return;
        }
        int i;
        for(i = top; i >= 0; i--)
        {
            if(arr[i] == val)
            {
                cout << "Value found at position : " << i << '\n';
                return;
            }
        }
        if(i == 0 && arr[i] != val)
        {
            cout << "Value isn't found" << '\n';
            return;
        }
    }
 
    void Print() const
    {
        if(IsEmpty())
        {
            cout << "Stack Is Empty." << '\n';
            return;
        }
        for(int i = top; i >= 0; i--) cout << arr[i] << " ";
        cout << '\n';
    }
 
    // ---------- Stack operations ----------
    void Push(int val)
    {
        if(top == MAX - 1)
        {
            cout << "Stack Overflow." << '\n';
            return;
        }
        arr[++top] = val;
    }
 
    void Pop()
    {
        if(IsEmpty())
        {
            cout << "Stack UnderFlow." << '\n';
            return;
        }
        int x = arr[top];
        top--;
        cout << "Poped Element Is : " << x << '\n';
    }
 
    void Peek() const
    {
        if(IsEmpty())
        {
            cout << "Stack Underflow." << '\n';
            return;
        }
        cout << "Top Element Is: " << arr[top] << '\n';
    }
};

#endif