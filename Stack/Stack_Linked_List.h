#ifndef STACKLINKEDLIST_H
#define STACKLINKEDLIST_H

#include "Template.hpp"

class Node
{
public:
    int data;
    Node *next;
};

class Stack
{
    Node *head;
public:
    Stack()
    {
        head = NULL;
    }
    //---
    bool isEmpty()
    { 
        return head == NULL; 
    }
    //---
    void Push(int value)
    {
        Node *p = new Node;
        p->data = value;
        p->next = head;
        head = p;
    }
    //---
    void Pop()
    { 
        if(isEmpty())
        {
            return void(cout << "Stack Underflow." << nl);
        }
        Node *temp = head;
        head = head->next;
        int value = temp->data;
        delete temp;
        cout << "Poped Element Is : " << value << nl;
    }
    //---
    void Peek()
    {
        if(isEmpty())
        {
            return void(cout << "Stack Underflow." << nl);
        }
        cout << "Top Element Is : " << head->data << nl;
    }
    //---
    void Search(int val)
    {
        if(isEmpty())
        {
            return void(cout << "Stack is empty." << nl);
        }
        Node* curr = head;
        int pos = 1;
        while(curr != NULL)
        {
            if(curr->data == val)
            {
                return void(cout << "Value found in position : " << pos << nl);
            }
            pos++;
            curr = curr->next;
        }
        if(curr == NULL)
        {
            return void(cout << "Value isn't found" << nl);
        }
    }
    //---
    void Print()
    {
        if(isEmpty())
        { 
            return void(cout << "Stack Is Empty." << nl); 
        }
        Node *curr = head;
        while(curr != NULL)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << nl;
    }
    //---
    ~Stack()
    {
        Node * curr = head;
        while(!isEmpty())
        {
            head = head->next;
            delete curr;
            curr = head;
        }
    }
};

#endif