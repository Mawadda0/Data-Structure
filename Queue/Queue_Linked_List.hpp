#ifndef QUEUELINKEDLIST_H
#define QUEUELINKEDLIST_H

#include "Template.hpp"

template <typename T>
class Node
{
public:
    T data;
    Node<T> *next;
};
 
// Queue stored as a singly linked list: dequeue removes at head, enqueue adds at tail.
template <typename T>
class Queue
{
private:
    Node<T> *head;
    Node<T> *tail;
 
public:
    // ---------- Constructors and destructor ----------
    Queue()
    {
        head = nullptr;
        tail = nullptr;
    }
 
    ~Queue()
    {
        Node<T> *curr = head;
        while(curr != nullptr)
        {
            Node<T> *nxt = curr->next;
            delete curr;
            curr = nxt;
        }
    }
 
    // ---------- Utilities ----------
    bool isEmpty() const
    {
        return head == nullptr;
    }
 
    void print() const
    {
        if(isEmpty())
        {
            cout << "Queue is Empty" << nl;
            return;
        }
 
        Node<T> *curr = head;
        while(curr != nullptr)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << "\n";
    }
 
    // ---------- Queue operations ----------
    void enqueue(T value)
    {
        Node<T> *p = new Node<T>();
        p->data = value;
        p->next = nullptr;
 
        if(isEmpty())
        {
            head = tail = p;
        }
        else
        {
            tail->next = p;
            tail = p;
        }
    }
 
    T dequeue()
    {
        if(isEmpty())
        {
            throw "Queue is empty (Underflow)\n";
        }
 
        Node<T> *temp = head;
        T value = temp->data;
 
        if(head == tail)
        {
            head = tail = nullptr;
        }
        else
        {
            head = head->next;
        }
        delete temp;
        return value;
    }
 
    T peek() const
    {
        if(isEmpty())
        {
            throw "Queue is empty (Underflow)\n";
        }
        return head->data;
    }
};

#endif