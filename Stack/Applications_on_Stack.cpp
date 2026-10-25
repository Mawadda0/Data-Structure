#include "Stack_Array.hpp"
// ---------- Applications ----------
string Reverse(string s)
{
    Stack<char> st;
    string res = "";
    for(int i = 0; i < (int)s.size(); i++)
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
    Stack<char> st;
    for(int i = 0; i < (int)s.size(); i++)
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
                }
                else return false;
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
    Stack<char> s;
    string result = "";
    for(int i = 0; i < (int)exp.length(); i++)
    {
        if(isspace(exp[i])) { continue; }
        else if(isdigit(exp[i]))
        {
            string num = "";
            while(i < (int)exp.length() && isdigit(exp[i]))
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
    Stack<char> s;
    for(int i = 0; i < (int)exp.size(); i++)
    {
        if(isspace(exp[i])) { continue; }
        else if(isdigit(exp[i]))
        {
            int num = 0;
            while(i < (int)exp.length() && isdigit(exp[i]))
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
    for(int i = 0; i < (int)s.size(); i++) st.Push(s[i]);
    string rev = "";
    while(!st.IsEmpty()) rev += st.Pop();
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
    while(!st.IsEmpty()) bin += to_string(st.Pop());
    return bin;
}