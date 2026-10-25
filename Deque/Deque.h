#ifndef DEQUE_H
#define DEQUE_H

#include "Template.hpp"

template <typename T>
class Deque
{
private:
    static constexpr int MIN_CAP = 10;
 
    int size = 0;
    int cap = MIN_CAP;
    int head = 0;       // physical index of the first element
    T *arr = nullptr;
 
    // Converts a logical index (0 = front) to a physical index in arr.
    // Valid for 0 <= i < cap.
    int phys(int i) const
    {
        int p = head + i;
        return p >= cap ? p - cap : p;
    }
 
    // Allocates a new array, copies the elements in logical order,
    // frees the old one. After this, head is always 0.
    void reallocate(int newCap)
    {
        T *newArr = new T[newCap]();
        for(int i = 0; i < size; i++)
        {
            newArr[i] = arr[phys(i)];
        }
        delete[] arr;
        arr = newArr;
        cap = newCap;
        head = 0;
    }
 
    void expandCap()
    {
        reallocate(cap * 2);
    }
 
    void shrinkCap()
    {
        reallocate(cap / 2);
    }
 
    // Shrinks only when the array is a quarter full (never below MIN_CAP).
    void shrinkIfSparse()
    {
        if(cap / 2 >= MIN_CAP && size <= cap / 4)
        {
            shrinkCap();
        }
    }
 
    // Rotates the storage in place so the elements are contiguous from index 0.
    // Needed before algorithms that expect a plain array (sort, reverse).
    void linearize()
    {
        if(head == 0) return;
        rotate(arr, arr + head, arr + cap);
        head = 0;
    }
 
public:
    // ---------- Constructors and destructor ----------
    Deque()
    {
        arr = new T[cap]();
    }
 
    Deque(int size)
    {
        if(size < 0) size = 0;
        this->size = size;
        this->cap = size + MIN_CAP;
        arr = new T[cap]();
    }
 
    Deque(const Deque & other)
    {
        size = other.size;
        cap = other.cap;
        arr = new T[cap]();
        for(int i = 0; i < size; i++)
        {
            arr[i] = other.arr[other.phys(i)];
        }
    }
 
    ~Deque()
    {
        delete[] arr;
        arr = nullptr;
    }
 
    // ---------- Access ----------
    void set(int idx, T val)
    {
        if(idx < 0 || idx >= size) return;
        arr[phys(idx)] = val;
    }
 
    T get(int idx) const
    {
        if(idx < 0 || idx >= size) return T();
        return arr[phys(idx)];
    }
 
    T Front() const
    {
        if(size == 0) return T();
        return arr[head];
    }
 
    T Back() const
    {
        if(size == 0) return T();
        return arr[phys(size - 1)];
    }
 
    int Size() const
    {
        return size;
    }
 
    int Cap() const
    {
        return cap;
    }
 
    bool Empty() const
    {
        return size == 0;
    }
 
    void Clear()
    {
        size = 0;
        head = 0;
    }
 
    // ---------- Adding and removing ----------
    void pushFront(T val)
    {
        if(size == cap) expandCap();
        head = (head == 0) ? cap - 1 : head - 1;
        arr[head] = val;
        size++;
    }
 
    void pushBack(T val)
    {
        if(size == cap) expandCap();
        arr[phys(size)] = val;
        size++;
    }
 
    void popFront()
    {
        if(size == 0) return;
        head = (head + 1 == cap) ? 0 : head + 1;
        size--;
        shrinkIfSparse();
    }
 
    void popBack()
    {
        if(size == 0) return;
        size--;
        shrinkIfSparse();
    }
 
    void insertAt(int idx, T val)
    {
        if(idx < 0 || idx > size) return;
        if(size == cap) expandCap();
 
        for(int i = size - 1; i >= idx; i--)
        {
            arr[phys(i + 1)] = arr[phys(i)];
        }
        arr[phys(idx)] = val;
        size++;
    }
 
    void deleteAt(int idx)
    {
        if(idx < 0 || idx >= size) return;
 
        for(int i = idx; i + 1 < size; i++)
        {
            arr[phys(i)] = arr[phys(i + 1)];
        }
        size--;
        shrinkIfSparse();
    }
 
    // ---------- Utilities ----------
    void Print() const
    {
        for(int i = 0; i < size; i++)
        {
            cout << arr[phys(i)] << " ";
        }
        cout << nl;
    }
 
    int find(T val) const
    {
        for(int i = 0; i < size; i++)
        {
            if(arr[phys(i)] == val) return i;
        }
        return -1;
    }
 
    T getMax() const
    {
        if(size == 0) return T();
        T maxi = arr[head];
        for(int i = 1; i < size; i++)
        {
            maxi = max(maxi, arr[phys(i)]);
        }
        return maxi;
    }
 
    T getMin() const
    {
        if(size == 0) return T();
        T mini = arr[head];
        for(int i = 1; i < size; i++)
        {
            mini = min(mini, arr[phys(i)]);
        }
        return mini;
    }
 
    void Sort()
    {
        linearize();
        sort(arr, arr + size);
    }
 
    void Reverse()
    {
        linearize();
        reverse(arr, arr + size);
    }
 
    // ---------- Operators ----------
    Deque & operator=(const Deque<T> & other)
    {
        if(this != &other)
        {
            T *newArr = new T[other.cap]();
            for(int i = 0; i < other.size; i++)
            {
                newArr[i] = other.arr[other.phys(i)];
            }
            delete[] arr;
            arr = newArr;
            size = other.size;
            cap = other.cap;
            head = 0;
        }
        return *this;
    }
 
    T &operator[](int idx)
    {
        assert(idx >= 0 && idx < size);
        return arr[phys(idx)];
    }
 
    const T &operator[](int idx) const
    {
        assert(idx >= 0 && idx < size);
        return arr[phys(idx)];
    }
};

#endif