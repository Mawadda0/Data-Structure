#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Template.hpp"

template <typename T>
class Node
{
public:
    T Data;
    Node<T> *Next;
 
    Node(T val)
    {
        Data = val;
        Next = nullptr;
    }
};
 
// Singly linked list. Positions used by DelAtPos and Update are 1-based.
template <typename T>
class Linked_List
{
private:
    Node<T> *head;
 
public:
    // ---------- Constructors and destructor ----------
    Linked_List()
    {
        head = nullptr;
    }
 
    ~Linked_List()
    {
        Node<T> *curr = head;
        while(curr != nullptr)
        {
            head = head->Next;
            delete curr;
            curr = head;
        }
    }
 
    // ---------- Utilities ----------
    void isEmpty() const
    {
        if(head == nullptr) cout << "List Is Empty." << nl;
        else cout << "List Isn't Empty." << nl;
    }
 
    int getCount() const
    {
        if(head == nullptr)
        {
            return 0;
        }
        int c = 0;
        Node<T> *curr = head;
        while(curr != nullptr)
        {
            c++;
            curr = curr->Next;
        }
        return c;
    }
 
    void printList() const
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        Node<T> *curr = head;
        while(curr != nullptr)
        {
            cout << curr->Data << " ";
            curr = curr->Next;
        }
        cout << nl;
    }
 
    // ---------- Adding ----------
    void AddToHead(T val)
    {
        Node<T> *p = new Node<T>(val);
        p->Next = head;
        head = p;
    }
 
    void AddToTail(T val)
    {
        Node<T> *p = new Node<T>(val);
        if(head == nullptr)
        {
            head = p;
        }
        else
        {
            Node<T> *curr = head;
            while(curr->Next != nullptr)
            {
                curr = curr->Next;
            }
            curr->Next = p;
        }
    }
 
    // Adds a new node with `data` before the first node whose value is `val`.
    void AddBefore(T val, T data)
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(head->Data == val)
        {
            AddToHead(data);
            return;
        }
        Node<T> *newNode = new Node<T>(data);
        Node<T> *curr = head;
        while(curr->Next != nullptr && curr->Next->Data != val)
        {
            curr = curr->Next;
        }
        if(curr->Next == nullptr)
        {
            delete newNode;
            cout << "Value Isn't Found." << nl;
            return;
        }
        newNode->Next = curr->Next;
        curr->Next = newNode;
    }
 
    // Adds a new node with `data` after the first node whose value is `val`.
    void AddAfter(T val, T data)
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        Node<T> *curr = head;
        while(curr != nullptr && curr->Data != val)
        {
            curr = curr->Next;
        }
 
        if(curr == nullptr)
        {
            cout << "Value Isn't Found." << nl;
            return;
        }
 
        Node<T> *newNode = new Node<T>(data);
        newNode->Next = curr->Next;
        curr->Next = newNode;
    }
 
    void AddAtMid(T data)
    {
        int n = getCount();
        if(n == 0)
        {
            head = new Node<T>(data);
            return;
        }
 
        int mid_idx = (n + 1) / 2;
 
        Node<T> *newNode = new Node<T>(data);
        if(mid_idx == 1)
        {
            newNode->Next = head;
            head = newNode;
            return;
        }
 
        Node<T> *curr = head;
        for(int i = 1; curr != nullptr && i < mid_idx - 1; i++)
        {
            curr = curr->Next;
        }
        newNode->Next = curr->Next;
        curr->Next = newNode;
    }
 
    // ---------- Removing ----------
    void DeleteFromHead()
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        Node<T> *curr = head;
        head = head->Next;
        delete curr;
        curr = nullptr;
    }
 
    // Removes the last node (keeps track of the previous node while walking).
    void DeleteFromNTail()
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        else if(head->Next == nullptr)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            Node<T> *pre = nullptr;
            Node<T> *curr = head;
            while(curr->Next != nullptr)
            {
                pre = curr;
                curr = curr->Next;
            }
            delete curr;
            pre->Next = nullptr;
        }
    }
 
    // Removes the last node (stops at the second-to-last node instead).
    void DeleteFromTail2()
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        else if(head->Next == nullptr)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            Node<T> *curr = head;
            while(curr->Next->Next != nullptr)
            {
                curr = curr->Next;
            }
            delete curr->Next;
            curr->Next = nullptr;
        }
    }
 
    void DelAtPos(int pos)
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(pos < 1)
        {
            cout << "Invalid Position." << nl;
            return;
        }
        if(pos == 1)
        {
            Node<T> *x = head;
            head = head->Next;
            delete x;
            x = nullptr;
            return;
        }
 
        Node<T> *curr = head;
        Node<T> *prev = nullptr;
        int idx = 1;
        while(curr != nullptr && idx < pos)
        {
            prev = curr;
            curr = curr->Next;
            idx++;
        }
 
        if(curr == nullptr)
        {
            cout << "Position Out Of Range." << nl;
            return;
        }
        prev->Next = curr->Next;
        delete curr;
        curr = nullptr;
    }
 
    // Removes the node that comes right after the first node whose value is `val`.
    void DelAfter(T val)
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
 
        Node<T> *curr = head;
        while(curr != nullptr && curr->Data != val)
        {
            curr = curr->Next;
        }
 
        if(curr == nullptr)
        {
            cout << "Value Isn't Found." << nl;
            return;
        }
 
        if(curr->Next == nullptr)
        {
            cout << "No Node Exists After This Value." << nl;
            return;
        }
 
        Node<T> *x = curr->Next;
        curr->Next = x->Next;
        delete x;
        x = nullptr;
    }
 
    // Removes the first node whose value is `val`.
    void DelAtVal(T val)
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(head->Data == val)
        {
            Node<T> *x = head;
            head = head->Next;
            delete x;
            x = nullptr;
            return;
        }
        Node<T> *curr = head;
        while(curr->Next != nullptr && curr->Next->Data != val)
        {
            curr = curr->Next;
        }
 
        if(curr->Next == nullptr)
        {
            cout << "Value Isn't Found." << nl;
            return;
        }
 
        Node<T> *x = curr->Next;
        curr->Next = x->Next;
        delete x;
        x = nullptr;
    }
 
    void DelAtMid()
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        int n = getCount();
        if(n == 0)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(n == 1)
        {
            delete head;
            head = nullptr;
            return;
        }
        int mid_idx = (n + 1) / 2;
        DelAtPos(mid_idx);
    }
 
    // ---------- Updating ----------
    void Update(int pos, T val)
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        Node<T> *curr = head;
        int idx = 1;
        while(curr != nullptr && idx < pos)
        {
            curr = curr->Next;
            idx++;
        }
        if(curr == nullptr)
        {
            cout << "Position Out Of Range." << nl;
            return;
        }
        curr->Data = val;
    }
};

#endif