#include <bits/stdc++.h>
using namespace std;

#define nl '\n'
#define ll long long
#define fixed(n) fixed << setprecision(n)
#define sz(x) (int)(x.size())
#define all(v) v.begin(), v.end()

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
    void getMax()
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }
        int maxi = -1e9;
        Node *curr = head;
        while(curr != NULL)
        {
            if(curr->Data > maxi) maxi = curr->Data;
            curr = curr->Next;
        }
        cout << "Maximum Number Is : " << maxi << nl;
    }
    //---
    void getMin()
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }
        int mini = 1e9;
        Node *curr = head;
        while(curr != NULL)
        {
            if(curr->Data < mini) mini = curr->Data;
            curr = curr->Next;
        }
        cout << "Minimum Number Is : " << mini << nl;
    }
    //---
    void getCount()
    {
        if(head == NULL)
        {
            return void(cout << "List Is Empty." << nl);
        }
        int c = 0;
        Node *curr = head;
        while(curr != NULL)
        {
            c++;
            curr = curr->Next;
        }
        cout << "Number of Nodes is " << c << nl;
    }
    //---
    void CopyList(Linked_List & li)
    {
        Node *curr = head;
        while(curr != NULL)
        {
            li.AddToHead(curr->Data);
            curr = curr->Next;
        }
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
                cout << "Value Is Found, " << "Value's Position Is : " << idx << nl;
                return;
            }else idx++;
            curr = curr->Next;
        }
        cout << "Value Isn't Found." << nl;
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
    //check if is empty
    li.isEmpty();
    // check the count of elements before adding
    li.getCount();
    // adding numbers
    li.AddToHead(4);
    li.AddToHead(5);
    li.AddToHead(6);
    li.AddToHead(7);
    li.AddToHead(8);
    //check if is empty after adding elements
    li.isEmpty();
    // check the count of elements after adding
    li.getCount();
    // print list
    li.printList();
    // get max number
    li.getMax();
    //get min number
    li.getMin();
    // check on exist nide
    li.FindNode(4);
    // check on non-exist node
    li.FindNode(10);
    // check from user
    int x; cin >> x;
    li.FindNode(x);
}

signed main()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    //cin >> tc;
    while (tc--)
    {
        solve();
        if(tc) cout << nl;
    }
    return 0;
}