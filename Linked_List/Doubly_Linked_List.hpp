#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include "Template.hpp"

template <typename T>
class Node
{
public:
    T Data;
    Node<T> *next;
    Node<T> *prev;
 
    Node(T val)
    {
        Data = val;
        next = nullptr;
        prev = nullptr;
    }
};
 
// Doubly linked list: every node knows both its next and its previous node.
// FindNode reports positions as 1-based.
template <typename T>
class Doubly_Linked_List
{
private:
    Node<T> *head;
 
public:
    // ---------- Constructors and destructor ----------
    Doubly_Linked_List()
    {
        head = nullptr;
    }
 
    ~Doubly_Linked_List()
    {
        if(!IsEmpty())
        {
            while(head->next != nullptr)
            {
                head = head->next;
                delete head->prev;
                head->prev = nullptr;
            }
            delete head;
            head = nullptr;
        }
    }
 
    // ---------- Utilities ----------
    bool IsEmpty() const
    {
        if(head == nullptr) return true;
        else return false;
    }
 
    void FindNode(T val) const
    {
        if(head == nullptr)
        {
            cout << "List Is Empty." << nl;
            return;
        }
        int idx = 1;
        Node<T> *curr = head;
        while(curr != nullptr)
        {
            if(curr->Data == val)
            {
                cout << "Value Is Found, Value's Position Is : " << idx << nl;
                return;
            }
            else idx++;
            curr = curr->next;
        }
        cout << "Value Isn't Found." << nl;
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
            curr = curr->next;
        }
        cout << nl;
    }
 
    // ---------- Adding ----------
    void AddToHead(T val)
    {
        Node<T> *p = new Node<T>(val);
        if(!IsEmpty()) head->prev = p;
        p->prev = nullptr;
        p->next = head;
        head = p;
    }
 
    void AddToTail(T val)
    {
        Node<T> *p = new Node<T>(val);
        p->next = nullptr;
        if(IsEmpty())
        {
            p->prev = nullptr;
            head = p;
            return;
        }
        Node<T> *curr = head;
        while(curr->next != nullptr)
        {
            curr = curr->next;
        }
        p->prev = curr;
        curr->next = p;
    }
 
    // Adds a new node with `data` after the first node whose value is `val`.
    void InsertAfter(T val, T data)
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << nl;
            return;
        }
        Node<T> *curr = head;
        if(curr->next == nullptr)
        {
            if(curr->Data == val)
            {
                AddToTail(data);
            }
            else cout << "Value Isn't Found" << nl;
            return;
        }
        while(curr != nullptr && curr->Data != val)
        {
            curr = curr->next;
        }
        if(curr == nullptr)
        {
            cout << "Value Isn't Found" << nl;
            return;
        }
        if(curr->next == nullptr)
        {
            AddToTail(data);
            return;
        }
        Node<T> *n = new Node<T>(data);
        n->next = curr->next;
        curr->next->prev = n;
        n->prev = curr;
        curr->next = n;
    }
 
    // ---------- Removing ----------
    void DeleteFromHead()
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(head->next == nullptr)
        {
            delete head;
            head = nullptr;
            return;
        }
        head = head->next;
        delete head->prev;
        head->prev = nullptr;
    }
 
    void DeleteFromTail()
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(head->next == nullptr)
        {
            delete head;
            head = nullptr;
            return;
        }
        Node<T> *curr = head;
        while(curr->next != nullptr)
        {
            curr = curr->next;
        }
        curr->prev->next = nullptr;
        delete curr;
        curr = nullptr;
    }
 
    // Removes the first node whose value is `val`.
    void DeleteAt(T val)
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(head->next == nullptr)
        {
            if(head->Data == val)
            {
                delete head;
                head = nullptr;
            }
            else
            {
                cout << "Value Isn't Found." << nl;
            }
            return;
        }
        Node<T> *curr = head;
        while(curr != nullptr && curr->Data != val)
        {
            curr = curr->next;
        }
        if(curr == nullptr)
        {
            cout << "Value Isn't Found" << nl;
            return;
        }
        if(curr == head)
        {
            DeleteFromHead();
            return;
        }
        if(curr->next == nullptr)
        {
            DeleteFromTail();
            return;
        }
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr;
        curr = nullptr;
    }
 
    // ---------- Reordering ----------
    void reverse()
    {
        if(IsEmpty())
        {
            cout << "List Is Empty." << nl;
            return;
        }
        if(head->next == nullptr) return;
        Node<T> *curr = head;
        Node<T> *temp = nullptr;
        while(curr != nullptr)
        {
            temp = curr->next; // old next
            curr->next = curr->prev;
            curr->prev = temp;
            curr = temp;
        }
        if(temp != nullptr) head = temp->prev;
    }
};

#endif