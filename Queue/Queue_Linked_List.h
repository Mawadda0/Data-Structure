#ifndef QUEUELINKEDLIST_H
#define QUEUELINKEDLIST_H

#include<Template.h>

template <typename T>
class Node
{
public:
    T data;
    Node<T>* next;
};

template <typename T>
class Queue 
{
    Node<T>* head;
    Node<T>* tail;
public:
    Queue()
    {
        head = NULL;
        tail = NULL;
    }
    bool isEmpty()
    {
        return head == NULL;
    }

    void enqueue(T value)
    {
        Node<T>* p = new Node<T>();
        p->data = value;
        p->next = NULL;

        if(isEmpty())
        {
            head = tail = p;
        }else
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

        Node<T>* temp = head;
        T value = temp->data;

        if(head == tail)
        {
            head = tail = NULL;
        }else
        {
            head = head->next;
        }
        delete temp;
        return value;
    }

    T peek()
    {
        if(isEmpty())
        {
            throw "Queue is empty (Underflow)";
        }
        return head->data;
    }

    void print()
    {
        if(isEmpty())
        {
            cout << "Queue is Empty\n";
            return;
        }

        Node<T>* curr = head;
        while(curr != NULL)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << "\n";
    }

    ~Queue()
    {
        Node<T>* curr = head;
        while (curr != NULL)
        {
            Node<T>* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
    }
};

#endif