#include <bits/stdc++.h>
using namespace std;

#define nl '\n'
#define ll long long
#define fixed(n) fixed << setprecision(n)
#define sz(x) (int)(x.size())
#define all(v) v.begin(), v.end()

// "سُبْحَانَكَ لا عِلْمَ لَنَا إِلَّا مَا عَلَّمْتَنَا إِنَّكَ أَنْتَ الْعَلِيمُ الْحَكِيمُ"

class Node
{
public:
    int Data;
    Node *next;

    Node(int val)
    {
        Data = val;
        next = NULL;
    }
};

class Circular_Linked_List
{
    Node *head;
public:
    Circular_Linked_List()
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
        Node *n = new Node(val);
        if(head == NULL)
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
    //---
    void AddToTail(int val)
    {
        Node *n = new Node(val);
        if(head == NULL)
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
    //---
    void getCount()
    {
        if(head == NULL)
        {
            return void(cout << "List Size Is : 0" << nl);    
        }
        int idx = 1;
        Node *curr = head->next;
        while(curr != head)
        {
            curr = curr->next;
            idx++;
        }
        cout << "List Size Is : " << idx << nl;
    }
    //---
    void DeleteFromHead()
    {
        if(IsEmpty()) return void(cout << "List Is Empty." << nl);
        if(head->next == head)
        {
            delete head;
            head = NULL;
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
        curr = NULL;
    }
    //---
    void DeleteFromTail()
    {
        if(IsEmpty()) return void(cout << "List Is Empty." << nl);
        if(head->next == head)
        {
            delete head;
            head = NULL;
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
    //---
    void DeleteNode(int val)
    {
        if(IsEmpty()) return void(cout << "List Is Empty." << nl);
        if(head->next == head)
        {
            if(val == head->Data)
            {
                delete head;
                head = NULL;
                return;
            }else
            {
                cout << "Value Isn't Found." << nl;
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
            cout << "Value Isn't Found." << nl;
            return;
        }
        pre->next = curr->next;
        delete curr;
    }
    //---
    void InsertAfter(int val, int data)
    {
        if(IsEmpty()) return void(cout << "List Is Empty." << nl);
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
            cout << "Value Isn't Found." << nl;
            return;
        }
        no->next = curr->next;
        curr->next = no;
    }
    //---
    void PrintList()
    {
        if(IsEmpty())
        {
            return void(cout << "List Is Empty." << nl);
        }
        cout << head->Data;
        Node *curr = head->next;
        while(curr != head)
        {
            cout << " " << curr->Data;
            curr = curr->next;
        }
        cout << nl;
    }
    //---
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
        head = NULL;
        cout << "List Has Been Deleted" << nl;
    }
};
void solve()
{
    Circular_Linked_List li;
    li.AddToHead(0);
    li.AddToHead(10);
    li.AddToHead(20);
    li.AddToTail(30);

    //printing list
    li.PrintList();

    //insert after a specific value
    li.InsertAfter(10, 50);
    li.PrintList();

    //delete from head
    li.DeleteFromHead();
    li.PrintList();

    //delete from tail
    li.DeleteFromTail();
    li.PrintList();

    //delete a specific node
    li.DeleteNode(10);
    li.PrintList();

    //return number of nodes
    li.getCount();
    li.PrintList();
}

signed main()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    solve();
    return 0;
}