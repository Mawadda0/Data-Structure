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
            std::cout << "Queue is Empty\n";
            return;
        }
 
        Node<T> *curr = head;
        while(curr != nullptr)
        {
            std::cout << curr->data << " ";
            curr = curr->next;
        }
        std::cout << "\n";
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
            throw "Queue is empty (Underflow)";
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
            throw "Queue is empty (Underflow)";
        }
        return head->data;
    }
};

#endif