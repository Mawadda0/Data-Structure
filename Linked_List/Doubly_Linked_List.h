#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "Template.hpp"
class Node
{
public:
    int Data;
    Node *next;
    Node *prev;
 
    Node(int val)
    {
        Data = val;
        next = nullptr;
        prev = nullptr;
    }
};
 
// Doubly linked list: every node knows both its next and its previous node.
// FindNode reports positions as 1-based.
class Doubly_Linked_List
{
private:
    Node *head;
 
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
        std::cout << "Node Has Been Deleted." << '\n';
    }
 
    // ---------- Utilities ----------
    bool IsEmpty() const
    {
        if(head == nullptr) return true;
        else return false;
    }
 
    void FindNode(int val) const
    {
        if(head == nullptr)
        {
            std::cout << "List Is Empty." << '\n';
            return;
        }
        int idx = 1;
        Node *curr = head;
        while(curr != nullptr)
        {
            if(curr->Data == val)
            {
                std::cout << "Value Is Found, Value's Position Is : " << idx << '\n';
                return;
            }
            else idx++;
            curr = curr->next;
        }
        std::cout << "Value Isn't Found." << '\n';
    }
 
    void printList() const
    {
        if(head == nullptr)
        {
            std::cout << "List Is Empty." << '\n';
            return;
        }
        Node *curr = head;
        while(curr != nullptr)
        {
            std::cout << curr->Data << " ";
            curr = curr->next;
        }
        std::cout << '\n';
    }
 
    // ---------- Adding ----------
    void AddToHead(int val)
    {
        Node *p = new Node(val);
        if(!IsEmpty()) head->prev = p;
        p->prev = nullptr;
        p->next = head;
        head = p;
    }
 
    void AddToTail(int val)
    {
        Node *p = new Node(val);
        p->next = nullptr;
        if(IsEmpty())
        {
            p->prev = nullptr;
            head = p;
            return;
        }
        Node *curr = head;
        while(curr->next != nullptr)
        {
            curr = curr->next;
        }
        p->prev = curr;
        curr->next = p;
    }
 
    // Adds a new node with `data` after the first node whose value is `val`.
    void InsertAfter(int val, int data)
    {
        if(IsEmpty())
        {
            std::cout << "List Is Empty." << '\n';
            return;
        }
        Node *curr = head;
        if(curr->next == nullptr)
        {
            if(curr->Data == val)
            {
                AddToTail(data);
            }
            else std::cout << "Value Isn't Found" << '\n';
            return;
        }
        while(curr != nullptr && curr->Data != val)
        {
            curr = curr->next;
        }
        if(curr == nullptr)
        {
            std::cout << "Value Isn't Found" << '\n';
            return;
        }
        if(curr->next == nullptr)
        {
            AddToTail(data);
            return;
        }
        Node *n = new Node(data);
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
            std::cout << "List Is Empty." << '\n';
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
            std::cout << "List Is Empty." << '\n';
            return;
        }
        if(head->next == nullptr)
        {
            delete head;
            head = nullptr;
            return;
        }
        Node *curr = head;
        while(curr->next != nullptr)
        {
            curr = curr->next;
        }
        curr->prev->next = nullptr;
        delete curr;
        curr = nullptr;
    }
 
    // Removes the first node whose value is `val`.
    void DeleteAt(int val)
    {
        if(IsEmpty())
        {
            std::cout << "List Is Empty." << '\n';
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
                std::cout << "Value Isn't Found." << '\n';
            }
            return;
        }
        Node *curr = head;
        while(curr != nullptr && curr->Data != val)
        {
            curr = curr->next;
        }
        if(curr == nullptr)
        {
            std::cout << "Value Isn't Found" << '\n';
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
            std::cout << "List Is Empty." << '\n';
            return;
        }
        if(head->next == nullptr) return;
        Node *curr = head;
        Node *temp = nullptr;
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