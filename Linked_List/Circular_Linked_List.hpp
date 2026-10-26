#ifndef CIRCULARLINKEDLIST_HPP
#define CIRCULARLINKEDLIST_HPP

#include "Template.hpp"

template <typename T>
class Node
{
public:
    T Data;
    Node<T> *next;
 
    Node(T val)
    {
        Data = val;
        next = nullptr;
    }
};
 
// Circular singly linked list: the last node points back to head.
template <typename T>
class Circular_Linked_List
{
private:
    Node<T> *head;
 
public:
    // ---------- Constructors and destructor ----------
    Circular_Linked_List()
    {
        head = nullptr;
    }
 
    ~Circular_Linked_List()
    {
        Node<T> *curr = head->next;
        Node<T> *n = curr->next;
        while(curr != head)
        {
            n = curr->next;
            delete curr;
            curr = n;
        }
        delete head;
        head = nullptr;
    }
 
    // ---------- Utilities ----------
    bool IsEmpty() const
    {
        if(head == nullptr) return true;
        else return false;
    }
 
    void getCount() const
    {
        if(head == nullptr)
        {
            cout << "List Size Is : 0" << nl;
            return;
        }
        int idx = 1;
        Node<T> *curr = head->next;
        while(curr != head)
        {
            curr = curr->next;
            idx++;
        }
        cout << "List Size Is : " << idx << nl;
    }
 
    void PrintList() const
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << '\n';
            return;
        }
        cout << head->Data;
        Node<T> *curr = head->next;
        while(curr != head)
        {
            cout << " " << curr->Data;
            curr = curr->next;
        }
        cout << '\n';
    }
 
    // ---------- Adding ----------
    void AddToHead(T val)
    {
        Node<T> *n = new Node<T>(val);
        if(head == nullptr)
        {
            n->next = n;
            head = n;
        }
        n->next = head;
        Node<T> *curr = head;
        while(curr->next != head)
        {
            curr = curr->next;
        }
        curr->next = n;
        head = n;
    }
 
    void AddToTail(T val)
    {
        Node<T> *n = new Node<T>(val);
        if(head == nullptr)
        {
            n->next = n;
            head = n;
        }
        Node<T> *curr = head;
        while(curr->next != head)
        {
            curr = curr->next;
        }
        curr->next = n;
        n->next = head;
    }
 
    // Adds a new node with `data` after the first node whose value is `val`.
    void InsertAfter(T val, T data)
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << '\n';
            return;
        }
        Node<T> *no = new Node<T>(data);
        if(head->Data == val)
        {
            no->next = head->next;
            head->next = no;
            return;
        }
        Node<T> *curr = head->next;
        while(curr != head && curr->Data != val)
        {
            curr = curr->next;
        }
        if(curr == head)
        {
            cout << "Value Isn't Found." << '\n';
            return;
        }
        no->next = curr->next;
        curr->next = no;
    }
 
    // ---------- Removing ----------
    void DeleteFromHead()
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << '\n';
            return;
        }
        if(head->next == head)
        {
            delete head;
            head = nullptr;
            return;
        }
        Node<T> *curr = head;
        head = head->next;
        Node<T> *last = head;
        while(last->next != curr)
        {
            last = last->next;
        }
        last->next = head;
        delete curr;
        curr = nullptr;
    }
 
    void DeleteFromTail()
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << '\n';
            return;
        }
        if(head->next == head)
        {
            delete head;
            head = nullptr;
            return;
        }
        Node<T> *curr = head;
        while(curr->next->next != head)
        {
            curr = curr->next;
        }
        delete curr->next;
        curr->next = head;
    }
 
    // Removes the first node whose value is `val`.
    void DeleteNode(T val)
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << '\n';
            return;
        }
        if(head->next == head)
        {
            if(val == head->Data)
            {
                delete head;
                head = nullptr;
                return;
            }
            else
            {
                cout << "Value Isn't Found." << '\n';
                return;
            }
        }
        if(head->Data == val)
        {
            DeleteFromHead();
            return;
        }
        Node<T> *curr = head->next;
        Node<T> *pre = head;
        while(curr != head && val != curr->Data)
        {
            pre = curr;
            curr = curr->next;
        }
        if(curr == head)
        {
            cout << "Value Isn't Found." << '\n';
            return;
        }
        pre->next = curr->next;
        delete curr;
    }
};

#endif