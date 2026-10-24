#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include<Template.h>

class Node
{
public:
    int Data;
    Node *next;
    Node *prev;

    Node(int val)
    {
        Data = val;
        next = NULL;
        prev = NULL;
    }
};

class Doubly_Linked_List
{
    Node *head;
     
public:
    Doubly_Linked_List()
    {
        head = NULL;
    }
    bool IsEmpty()
    {
        if(head == NULL) return true;
        else return false;
    }
    //---
    void AddToHead(int val)
    {
        Node *p = new Node(val);
        if(!IsEmpty()) head->prev = p;
        p->prev = NULL;
        p->next = head;
        head = p;
    }
    //---
    void AddToTail(int val)
    {
        Node *p = new Node(val);
        p->next = NULL;
        if(IsEmpty())
        {
            p->prev = NULL;
            head = p;
            return;
        }
        Node *curr = head;
        while(curr->next != NULL)
        {
            curr = curr->next;
        }
        p->prev = curr;
        curr->next = p;
    }
    //---
    void DeleteFromHead()
    {
        if(IsEmpty())
        {
            return void(cout << "List Is Empty." << nl);
        }
        if(head->next == NULL)
        {
                delete head;
                head = NULL;
                return;
        }
        head = head->next;
        delete head->prev;
        head->prev = NULL;
    }
    //---
    void DeleteFromTail()
    {
        if(IsEmpty())
        {
                return void(cout << "List Is Empty." << nl);
        }
        if(head->next == NULL)
        {
                delete head;
                head = NULL;
                return;
        }
        Node *curr = head;
        while(curr->next != NULL)
        {
                curr = curr->next;
        }
        curr->prev->next = NULL;
        delete curr;
        curr = NULL;
    }
    //---
    void DeleteAt(int val)
    {
        if(IsEmpty())
        {
            return void(cout << "List Is Empty." << nl);
        }
        if(head->next == NULL)
        {
            if(head->Data == val)
            {
                delete head;
                head = NULL;
            }
            else
            {
                cout << "Value Isn't Found." << nl;
            }
            return;
        }
        Node *curr = head;
        while(curr != NULL && curr->Data != val)
        {
            curr = curr->next;
        }
        if(curr == NULL)
        {
            return void(cout << "Value Isn't Found" << nl);
        }
        if(curr == head)
        {
            DeleteFromHead();
            return;  
        }
        if(curr->next == NULL)
        {
            DeleteFromTail();
            return;
        }
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr;
        curr = NULL;
    }
    //---
    void FindNode(int val) // based 1.
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }
        int idx = 1;
        Node *curr = head;
        while(curr != NULL)
        {
            if(curr->Data == val)
            {
                cout << "Value Is Found, Value's Position Is : " << idx << nl;
                return;
            }else idx++;
            curr = curr->next;
        }
        cout << "Value Isn't Found." << nl;
    }
    //---
    void InsertAfter(int val, int data)
    {
        if(IsEmpty())
        {
            return void(cout << "List Is Empty." << nl);    
        }
        Node *curr = head;
        if(curr->next == NULL)
        {
            if(curr->Data == val)
            {
                AddToTail(data);
            }
            else cout << "Value Isn't Found" << nl;
            return;
        }
        while(curr != NULL && curr->Data != val)
        {
            curr = curr->next;
        }
        if(curr == NULL)
        {
            return void(cout << "Value Isn't Found" << nl);
        }
        if(curr->next == NULL)
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
    //---
    void reverse()
    {
        if(IsEmpty())
        {
            return void(cout << "List Is Empty." << nl);    
        }
        if(head->next == NULL) return;
        Node *curr = head;
        Node *temp = NULL;
        while (curr != NULL)
        {
            temp = curr->next; // old next
            curr->next = curr->prev;
            curr->prev = temp;
            curr = temp;
        }
        if(temp != NULL) head = temp->prev;
    }
    //---
    void printList()
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }
        Node *curr = head;
        while(curr != NULL)
        {
            cout << curr->Data << " ";
            curr = curr->next;
        }
        cout << nl;
    }
    //---
    ~Doubly_Linked_List()
    {
        if(!IsEmpty())
        {
            while(head->next != NULL)
            {
                head = head->next;
                delete head->prev;
                head->prev = NULL;
            }
            delete head;
            head = NULL;
        }
        cout << "Node Has Been Deleted." << nl;
    }
};

#endif
