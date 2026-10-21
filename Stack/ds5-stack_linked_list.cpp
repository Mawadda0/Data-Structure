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
    int data;
    Node *next;
};

class Stack
{
    Node *head;
public:
    Stack()
    {
        head = NULL;
    }
    //---
    bool isEmpty()
    { 
        return head == NULL; 
    }
    //---
    void Push(int value)
    {
        Node *p = new Node;
        p->data = value;
        p->next = head;
        head = p;
    }
    //---
    void Pop()
    { 
        if(isEmpty())
        {
            return void(cout << "Stack Underflow." << nl);
        }
        Node *temp = head;
        head = head->next;
        int value = temp->data;
        delete temp;
        cout << "Poped Element Is : " << value << nl;
    }
    //---
    void Peek()
    {
        if(isEmpty())
        {
            return void(cout << "Stack Underflow." << nl);
        }
        cout << "Top Element Is : " << head->data << nl;
    }
    //---
    void Search(int val)
    {
        if(isEmpty())
        {
            return void(cout << "Stack is empty." << nl);
        }
        Node* curr = head;
        int pos = 1;
        while(curr != NULL)
        {
            if(curr->data == val)
            {
                return void(cout << "Value found in position : " << pos << nl);
            }
            pos++;
            curr = curr->next;
        }
        if(curr == NULL)
        {
            return void(cout << "Value isn't found" << nl);
        }
    }
    //---
    void Print()
    {
        if(isEmpty())
        { 
            return void(cout << "Stack Is Empty." << nl); 
        }
        Node *curr = head;
        while(curr != NULL)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << nl;
    }
    //---
    ~Stack()
    {
        Node * curr = head;
        while(!isEmpty())
        {
            head = head->next;
            delete curr;
            curr = head;
        }
    }
};


void solve()
{
   Stack s;
    s.Push(3);
    s.Push(4);
    s.Push(5);
    s.Print();

    s.Peek();
    s.Pop();
    s.Print();
    s.Search(4);
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