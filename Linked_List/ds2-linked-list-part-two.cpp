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
    Node *Next;

    Node(int val)
    {
        Data = val;
        Next = NULL;
    }
};


class Linked_List
{
    Node *head;
public:
    Linked_List()
    {
        head = NULL;
    }
    //---
    void isEmpty()
    {
        if(head == NULL) cout << "List Is Empty." << nl;
        else cout << "List Isn't Empty." << nl;
    }
    //---
    void AddToHead(int val)
    {
        Node *p = new Node(val);
        p->Next = head;
        head = p;
    }
    //---
    void AddToTail(int val)
    {
        Node *p = new Node(val);
        if(head == NULL)
        {
            head = p;
        }else
        {
            Node *curr = head;
            while(curr->Next != NULL)
            {
                curr = curr->Next;
            }
            curr->Next = p;
        }
    }
    //---
    int getCount()
    {
        if(head == NULL)
        {
            return 0;
        }
        int c = 0;
        Node *curr = head;
        while(curr != NULL)
        {
            c++;
            curr = curr->Next;
        }
        return c;
    }
    //----
    void DeleteFromHead()
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }
        Node *curr = head;
        head = head->Next;
        delete curr;
        curr = NULL;
    }
    //---
    void DeleteFromNTail()
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }else if(head->Next == NULL)
        {
            delete head;
            head = NULL;
        }else
        {
            Node *pre = NULL;
            Node *curr = head;
            while(curr->Next != NULL)
            {
                pre = curr;
                curr = curr->Next;
            }
            delete curr;
            pre->Next = NULL;
        }
    }
    //---
    void DeleteFromTail2()
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }else if(head->Next == NULL)
        {
            delete head;
            head = NULL;
        }else
        {
            Node *curr = head;
            while(curr->Next->Next != NULL)
            {
                curr = curr->Next;
            }
            delete curr->Next;
            curr->Next = NULL;
        }
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
            curr = curr->Next;
        }
        cout << nl;
    }
    //---
    void DelAtPos(int pos) //done
    {
        if(head == NULL)
        {
            cout << "List Is Empty." << endl;
            return;
        }
        if(pos < 1)
        {
            cout << "Invalid Position." << endl;
            return;
        }
        if(pos == 1)
        {
            Node *x = head;
            head = head->Next;
            delete x;
            x = NULL;
            return;
        }

        Node *curr = head;
        Node *prev = NULL;
        int idx = 1;
        while(curr != NULL && idx < pos)
        {
            prev = curr;
            curr = curr->Next;
            idx++;
        }

        if(curr == NULL)
        {
            cout << "Position Out Of Range." << endl;
            return;
        }
        prev->Next = curr->Next;
        delete curr;
        curr = NULL;
    }
    //---
    void DelAfter(int val) //done
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);    
        }

        Node *curr = head;
        while(curr != NULL && curr->Data != val)
        {
            curr = curr->Next;
        }

        if(curr == NULL)
        {
            return void(cout << "Value Isn't Found." << nl);
        }

        if(curr->Next == NULL)
        {
            return void(cout << "No Node Exists After This Value." << nl);
        }

        Node* x = curr->Next;
        curr->Next = x->Next;
        delete x;
        x = NULL;
    }
    //---
    void AddBefore(int val, int data) //done
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);    
        }
        if(head->Data == val)
        {
            AddToHead(data);
            return;
        }
        Node *newNode = new Node(data);
        Node* curr = head;
        while(curr->Next != NULL && curr->Next->Data != val)
        {
            curr = curr->Next;
        }
        if(curr->Next == NULL)
        {
            delete newNode;
            return void(cout << "Value Isn't Found." << nl);
        }
        newNode->Next = curr->Next;
        curr->Next = newNode;
    }
    //---
    void Update(int pos, int val) //done
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);    
        }
        Node *curr = head;
        int idx = 1;
        while(curr != NULL && idx < pos)
        {
            curr = curr->Next;
            idx++;
        }
        if(curr == NULL)
        {
            return void(cout << "Position Out Of Range." << nl);
        }
        curr->Data = val;
    }
    //---
    void DelAtMid() //done
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);    
        }
        int n = getCount();
        if(n == 0) return void(cout << "List Is Empty." << nl);
        if(n == 1)
        {
            delete head;
            head = NULL;
            return;
        }
        int mid_idx = (n + 1) / 2;
        DelAtPos(mid_idx);
     }
    //---
    void AddAtMid(int data) //done
    {
        int n = getCount();
        if(n == 0)
        {
            head = new Node(data);
            return;
        }

        int mid_idx = (n + 1) / 2;

        Node *newNode = new Node(data);
        if(mid_idx == 1)
        {
            newNode->Next = head;
            head = newNode;
            return;
        }

        Node *curr = head;
        for(int i = 1; curr != NULL && i < mid_idx - 1; i++)
        {
            curr = curr->Next;    
        }
        newNode->Next = curr->Next;
        curr->Next = newNode;
    }
    // in case u need to remove a specific val
    void DelAtVal(int val) //done
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);    
        }
        if(head->Data == val)
        {
            Node *x = head;
            head = head->Next;
            delete x;
            x = NULL;
            return;
        }
        Node *curr = head;
        while(curr->Next != NULL && curr->Next->Data != val)
        {
            curr = curr->Next;
        }

        if(curr->Next == NULL)
        {
            return void(cout << "Value Isn't Found." << nl);
        }

        Node *x = curr->Next;
        curr->Next = x->Next;
        delete x;
        x = NULL;
    }
    // in case u need to add after a specific val
    void AddAfter(int val, int data) //done
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);    
        }
        Node *curr = head;
        while(curr != NULL && curr->Data != val)
        {
            curr = curr->Next;
        }

        if(curr == NULL)
        {
            return void(cout << "Value Isn't Found." << nl);    
        }

        Node *newNode = new Node(data);
        newNode->Next = curr->Next;
        curr->Next = newNode;
    }
    //---
    ~Linked_List()
    {
        Node *curr = head;
        while(curr != NULL)
        {
            head = head->Next;
            delete curr;
            curr = head;
        }
        cout << "Node Has Been Deleted." << nl;
    }

};


void solve()
{
    Linked_List li;
    li.DelAtPos(3);
}

signed main()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    solve();
    return 0;
}