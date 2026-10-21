#include <bits/stdc++.h>
using namespace std;

#define nl '\n'
#define ll long long
#define fixed(n) fixed << setprecision(n)
#define sz(x) (int)(x.size())
#define all(v) v.begin(), v.end()

// "سُبْحَانَكَ لا عِلْمَ لَنَا إِلَّا مَا عَلَّمْتَنَا إِنَّكَ أَنْتَ الْعَلِيمُ الْحَكِيمُ"

#define MAX_SIZE 4

template <typename T> class CircularQueue
{
    T arr[MAX_SIZE];
    int front, rear;
public:
    CircularQueue()
    {
        front = rear = -1;
    }
    bool isEmpty()
    {
        return front == -1 && rear == -1;
    }
    bool isFull()
    {
        return front == (rear + 1) % MAX_SIZE;
    }

    void enqueue(int value)
    {
        if(isFull())
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
            rear = (rear + 1) % MAX_SIZE;
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
            if(front == rear){  
                front = rear = -1;
            }
            else
            {
                front = (front + 1) % MAX_SIZE;
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
            for(int i = front; i != rear; i = (i + 1) % MAX_SIZE)
            {
                cout << arr[i] << " ";
            }
            cout << arr[rear] << nl;
        }
    }

    int Size()
    {
        if(isEmpty()) return 0;
        int count = 1;
        for(int i = front; i != rear; i = (i + 1) % MAX_SIZE)  count++;
        return count;
    }

    void enqueueN(int n)
    {
        cout << "Enter " << n << " values:\n";
        for(int i = 0; i < n; i++)
        {
            T x;
            cin >> x;
            enqueue(x);
        }
    }

    void dequeueN(int n)
    {
        for(int i = 0; i < n; i++)
        {
            try
            {
                cout << "Removed: " << dequeue() << "\n";
            }
            catch(const char* msg)
            {
                cout << msg << "\n";
                return;
            }
        }
    }

    int search(T value)
    {
        if(isEmpty()) return -1;
        int pos = 0;
        for(int i = front; ; i = (i + 1) % MAX_SIZE)
        {
            if(arr[i] == value) return pos;
            if(i == rear) break;
            pos++;
        }
        return -1;
    }
};

void solve()
{
   CircularQueue <int> q;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);

    q.print();
    cout << "front: " << q.peek() << nl;
    q.dequeue();
    q.dequeue();
    q.enqueue(50);
    q.enqueue(50);
    q.print(); 

    cout << "Size = " << q.Size() << nl;
    int x = 20;
    cout << "Position of x = " << q.search(20) << nl;
    q.dequeueN(2);
    cout << "Size after dequeue = " << q.Size() << nl;
    q.enqueueN(2);
    q.print();
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