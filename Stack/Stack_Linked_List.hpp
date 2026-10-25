#ifndef STACKLINKEDLIST_H
#define STACKLINKEDLIST_H

#include "Template.hpp"

template <typename T>

class Node
{
public:
    int data;
    Node *next;
};
 
// Stack stored as a singly linked list: head is the top of the stack.
// Search reports the position counted from the top of the stack (1-based).
template <typename T>
class Stack
{
private:
    Node *head;
 
public:
    // ---------- Constructors and destructor ----------
    Stack()
    {
        head = nullptr;
    }
 
    ~Stack()
    {
        Node *curr = head;
        while(!isEmpty())
        {
            head = head->next;
            delete curr;
            curr = head;
        }
    }
 
    // ---------- Utilities ----------
    bool isEmpty() const
    {
        return head == nullptr;
    }
 
    void Search(T val) const
    {
        if(isEmpty())
        {
            cout << "Stack is empty." << nl;
            return;
        }
        Node *curr = head;
        int pos = 1;
        while(curr != nullptr)
        {
            if(curr->data == val)
            {
                cout << "Value found in position : " << pos << nl;
                return;
            }
            pos++;
            curr = curr->next;
        }
        if(curr == nullptr)
        {
            cout << "Value isn't found" << nl;
            return;
        }
    }
 
    void Print() const
    {
        if(isEmpty())
        {
            cout << "Stack Is Empty." << nl;
            return;
        }
        Node *curr = head;
        while(curr != nullptr)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << nl;
    }
 
    // ---------- Stack operations ----------
    void Push(T value)
    {
        Node *p = new Node;
        p->data = value;
        p->next = head;
        head = p;
    }
 
    void Pop()
    {
        if(isEmpty())
        {
            cout << "Stack Underflow." << nl;
            return;
        }
        Node *temp = head;
        head = head->next;
        T value = temp->data;
        delete temp;
        cout << "Poped Element Is : " << value << nl;
    }
 
    void Peek() const
    {
        if(isEmpty())
        {
            cout << "Stack Underflow." << nl;
            return;
        }
        cout << "Top Element Is : " << head->data << nl;
    }
};

#endif