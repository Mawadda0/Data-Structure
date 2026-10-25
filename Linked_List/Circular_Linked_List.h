#ifndef CIRCULARLINKEDLIST_H
#define CIRCULARLINKEDLIST_H

#include "Template.hpp"

class Node
{
public:
    int Data;
    Node *next;
 
    Node(int val)
    {
        Data = val;
        next = nullptr;
    }
};
 
// Circular singly linked list: the last node points back to head.
class Circular_Linked_List
{
private:
    Node *head;
 
public:
    // ---------- Constructors and destructor ----------
    Circular_Linked_List()
    {
        head = nullptr;
    }
 
    ~Circular_Linked_List()
    {
        Node *curr = head->next;
        Node *n = curr->next;
        while(curr != head)
        {
            n = curr->next;
            delete curr;
            curr = n;
        }
        delete head;
        head = nullptr;
        cout << "List Has Been Deleted" << '\n';
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
            cout << "List Size Is : 0" << '\n';
            return;
        }
        int idx = 1;
        Node *curr = head->next;
        while(curr != head)
        {
            curr = curr->next;
            idx++;
        }
        cout << "List Size Is : " << idx << '\n';
    }
 
    void PrintList() const
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << '\n';
            return;
        }
        cout << head->Data;
        Node *curr = head->next;
        while(curr != head)
        {
            cout << " " << curr->Data;
            curr = curr->next;
        }
        cout << '\n';
    }
 
    // ---------- Adding ----------
    void AddToHead(int val)
    {
        Node *n = new Node(val);
        if(head == nullptr)
        {
            n->next = n;
            head = n;
        }
        n->next = head;
        Node *curr = head;
        while(curr->next != head)
        {
            curr = curr->next;
        }
        curr->next = n;
        head = n;
    }
 
    void AddToTail(int val)
    {
        Node *n = new Node(val);
        if(head == nullptr)
        {
            n->next = n;
            head = n;
        }
        Node *curr = head;
        while(curr->next != head)
        {
            curr = curr->next;
        }
        curr->next = n;
        n->next = head;
    }
 
    // Adds a new node with `data` after the first node whose value is `val`.
    void InsertAfter(int val, int data)
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << '\n';
            return;
        }
        Node *no = new Node(data);
        if(head->Data == val)
        {
            no->next = head->next;
            head->next = no;
            return;
        }
        Node *curr = head->next;
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
        Node *curr = head;
        head = head->next;
        Node *last = head;
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
        Node *curr = head;
        while(curr->next->next != head)
        {
            curr = curr->next;
        }
        delete curr->next;
        curr->next = head;
    }
 
    // Removes the first node whose value is `val`.
    void DeleteNode(int val)
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
        Node *curr = head->next;
        Node *pre = head;
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
