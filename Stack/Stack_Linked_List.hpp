#ifndef STACKLINKEDLIST_H
#define STACKLINKEDLIST_H

#include "Template.hpp"

class Node
{
public:
    int data;
    Node *next;
};
 
// Stack stored as a singly linked list: head is the top of the stack.
// Search reports the position counted from the top of the stack (1-based).
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
 
    void Search(int val) const
    {
        if(isEmpty())
        {
            cout << "Stack is empty." << '\n';
            return;
        }
        Node *curr = head;
        int pos = 1;
        while(curr != nullptr)
        {
            if(curr->data == val)
            {
                cout << "Value found in position : " << pos << '\n';
                return;
            }
            pos++;
            curr = curr->next;
        }
        if(curr == nullptr)
        {
            cout << "Value isn't found" << '\n';
            return;
        }
    }
 
    void Print() const
    {
        if(isEmpty())
        {
            cout << "Stack Is Empty." << '\n';
            return;
        }
        Node *curr = head;
        while(curr != nullptr)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << '\n';
    }
 
    // ---------- Stack operations ----------
    void Push(int value)
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
            cout << "Stack Underflow." << '\n';
            return;
        }
        Node *temp = head;
        head = head->next;
        int value = temp->data;
        delete temp;
        cout << "Poped Element Is : " << value << '\n';
    }
 
    void Peek() const
    {
        if(isEmpty())
        {
            cout << "Stack Underflow." << '\n';
            return;
        }
        cout << "Top Element Is : " << head->data << '\n';
    }
};

#endif