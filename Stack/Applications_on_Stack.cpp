#include<Template.h>

const int MAX = 100;

template <typename T>
class Stack
{
private:
    int top;
    T arr[MAX];

public:
    Stack()
    {
        top = -1;
    }

    bool IsEmpty()
    {
        return top == -1;
    }
    // Push
    void Push(T val)
    {
        if(top == MAX - 1)
        {
            cout << "Stack Overflow." << nl;
            return;
        }
        arr[++top] = val;
    }
    // Pop (Always returns a value)
    T Pop()
    {
        if(IsEmpty())
        {
            cout << "Stack UnderFlow." << nl;
            return T();   // return default value of T
        }
        return arr[top--];
    }
    // Peek
    T Peek()
    {
        if(IsEmpty())
        {
            cout << "Stack Underflow." << nl;
            return T();
        }
        return arr[top];
    }
    // Search
    void Search(T val)
    {
        if(IsEmpty())
        {
            cout << "Stack Is Empty." << nl;
            return;
        }
        for(int i = top; i >= 0; i--)
        {
            if(arr[i] == val)
            {
                cout << "Value found at position : " << i << nl;
                return;
            }
        }
        cout << "Value isn't found" << nl;
    }
    //--
    void Print()
    {
        if(IsEmpty())
        {
            cout << "Stack Is Empty." << nl;
            return;
        }
        for(int i = top; i >= 0; i--)
            cout << arr[i] << " ";
        cout << nl;
    }
};


string Reverse(string s)
{
    Stack <char> st;
    string res = "";
    for(int i = 0; i < sz(s); i++)
    {
        st.Push(s[i]);
    }
    while(!st.IsEmpty())
    {
        res += st.Pop();
    }
    return res;
}

bool checkBalance(string s)
{
    Stack <char> st;
    for(int i = 0; i < sz(s); i++)
    {
        if(s[i] == '(' || s[i] == '[' || s[i] == '{') st.Push(s[i]);
        else
        {
            if(st.IsEmpty()) return false;
            else
            {
                if((s[i] == ')' && st.Peek() == '(') || (s[i] == ']' && st.Peek() == '[') || (s[i] == '}' && st.Peek() == '{'))
                {
                    st.Pop();    
                }else return false;
            }    
        }
    } 
    if(st.IsEmpty()) return true;
    else return false;
}

int priority(char op)
{
    if(op == '+' || op == '-') return 1;
    else if(op == '*' || op == '/' || op == '%') return 2;
    else if(op == '^') return 3;
    else return 0;
}

string infix_to_postfix(string exp)
{
    Stack <char> s;
    string result = "";
    for(int i = 0; i < exp.length(); i++)
    {
        if(isspace(exp[i])) { continue; }
        else if(isdigit(exp[i]))
        {
            string num = "";
            while(i < exp.length() && isdigit(exp[i]))
            {
                num += exp[i];
                i++;
            }
            i--; 
            result += num + ' ';
        }
        else if(exp[i] == '(')
        {
            s.Push(exp[i]);
        }
        else if(exp[i] == ')')
        {
            while(s.Peek() != '(')
            {
                result += s.Pop();
                result += ' ';
            }
            s.Pop();
        }
        else
        {
            while(!s.IsEmpty() && priority(exp[i]) <= priority(s.Peek()))
            {
                result += s.Pop();
                result += ' ';
            }
            s.Push(exp[i]);
        }
    }
    while(!s.IsEmpty())
    {
        result += s.Pop();
        result += ' ';
    }
    return result;
}

int evaluate_postfix(string exp)
{
    Stack <char> s;
    for(int i = 0; i < sz(exp); i++)
    {
        if(isspace(exp[i])) {continue;}
        else if(isdigit(exp[i]))
        {
            int num = 0;
            while(i < exp.length() && isdigit(exp[i]))
            {
                num = num * 10 + (exp[i] - '0');  
                i++;
            }
            i--;
            s.Push(num); 
        }
        else 
        {
            int second = s.Pop();
            int first = s.Pop();
            switch(exp[i])
            {
                case '+': s.Push(first + second); break;
                case '-': s.Push(first - second); break;
                case '*': s.Push(first * second); break;
                case '/': s.Push(first / second); break;
            }
        }
    }
    return s.Peek();
}

bool isPalindrome(string s)
{
    Stack<char> st;
    for(int i = 0; i < sz(s); i++) st.Push(s[i]);
    string rev = "";
    while (!st.IsEmpty()) rev += st.Pop();
    return rev == s;
}


string decTobin(int n)
{
    if(n == 0) return "0";
    Stack<int> st;
    while(n > 0)
    {
        st.Push(n % 2);
        n /= 2;
    }
    string bin = "";
    while (!st.IsEmpty()) bin += to_string(st.Pop());
    return bin;
}