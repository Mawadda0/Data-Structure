#ifndef STACKARRAY_H
#define STACKARRAY_H

#include<Template.h>

const int MAX = 100;

class Stack
{
private:
    int top;
    int arr[MAX];
public:
    Stack()
    {
        top = -1;
    }
    //---
    bool IsEmpty()
    {
        return top == -1;
    }
    //---
    void Push(int val)
    {
        if(top == MAX - 1)
        {
            return void(cout << "Stack Overflow." << nl);
        }
        arr[++top] = val;
    }
    //---
    void Pop()
    {
        if(IsEmpty())
        {
            return void(cout << "Stack UnderFlow." << nl);
        }
        int x = arr[top];
        top--;
        cout << "Poped Element Is : " << x << nl;
    }
    //---
    void Peek()
    {
        if(IsEmpty())
        {
            return void(cout << "Stack Underflow." << nl);
        }
        cout << "Top Element Is: " << arr[top] << nl;
    }
    //---
    void Search(int val)
    {
        if(IsEmpty())
        {
            return void(cout << "Stack Is Empty." << nl);    
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
            return void(cout << "Value isn't found" << nl);    
        }
    }
    //---
    void Print()
    {
        if(IsEmpty())
        {
            return void(cout << "Stack Is Empty." << nl);
        }
        for(int i = top; i >= 0; i--) cout << arr[i] << " ";
        cout << nl;
    }
};

#endif