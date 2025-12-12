#include <bits/stdc++.h>
using namespace std;

#define nl '\n'
#define ll long long
#define fixed(n) fixed << setprecision(n)
#define sz(x) (int)(x.size())
#define all(v) v.begin(), v.end()

// "سُبْحَانَكَ لا عِلْمَ لَنَا إِلَّا مَا عَلَّمْتَنَا إِنَّكَ أَنْتَ الْعَلِيمُ الْحَكِيمُ"

#define MAX_SIZE 5
template <typename T> class  Queue
{
    T arr[MAX_SIZE];
    int front, rear;
public:
    Queue()
    {
        front = rear = -1;
    }
    bool isEmpty()
    {
        return front == -1 && rear == -1;
    }
    bool isFull()
    {
        return rear == MAX_SIZE - 1;
    }

    void enqueue(int value)
    {
        if(rear == MAX_SIZE - 1)
        {
            cout << "Queue is full (overflow)\n";
            return;
        }
        else if(isEmpty())
        {
            front = rear = 0;
        }
        else
        {
            rear++;
        }
        arr[rear] = value;
    }

    int dequeue()
    {
        if(isEmpty())
        {  
            throw "The queue is empty (Underflow)";
        }
        else
        {
            int temp  = arr[front];
            if(front == rear)
            {  
                front = rear = -1;
            }
            else
            {
                front++;
            }
            return temp;
        }
    }


    int peek()
    {
        if(isEmpty())
        {  
            throw "Queue is empty (Underflow)\n";
        }
        else
        {
            return arr[front];
        }
    }

    void print()
    {
        if(isEmpty())
        {
            cout << "Queue is Empty\n";
        }
        else
        {
            for(int i = front; i <= rear; i++)
            {
                cout << arr[i] << " ";
            }
            cout << endl;
        }
    }
};

void solve()
{
    Queue <int> q;
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(5);
    q.print();
    cout << "front: " << q.peek() << endl;
    try{
        cout << "dequeue: " <<  q.dequeue() << nl;
        q.print();
        cout << "dequeue: " <<  q.dequeue() << nl;
        q.print();
        cout << "dequeue: " <<  q.dequeue() << nl;
        q.print();
        cout << "dequeue: " <<  q.dequeue() << nl;
    }
    catch(char const *msg)
    {
        cout << msg << endl;
    }
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