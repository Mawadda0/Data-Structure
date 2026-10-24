#ifndef VECTOR_H
#define VECTOR_H

#include<Template.h>

template <typename T>
class Vector
{
private:
    static constexpr int MIN_CAP = 10;
 
    int size = 0;
    int cap = MIN_CAP;
    T *arr = nullptr;
 
    // Allocates a new array, copies the elements, frees the old one.
    void reallocate(int newCap)
    {
        T *newArr = new T[newCap]();
        for (int i = 0; i < size; i++)
        {
            newArr[i] = arr[i];    
        }
        delete[] arr;
        arr = newArr;
        cap = newCap;
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
 
public:
    // ---------- Constructors and destructor ----------
    Vector()
    {
        arr = new T[cap]();
    }
 
    Vector(int size)
    {
        if(size < 0) size = 0;
        this->size = size;
        this->cap = size + MIN_CAP;
        arr = new T[cap]();
    }
 
    Vector(const Vector & other)
    {
        size = other.size;
        cap = other.cap;
        arr = new T[cap]();
        for(int i = 0; i < size; i++)
        {
            arr[i] = other.arr[i];    
        }
    }
 
    ~Vector()
    {
        delete[] arr;
        arr = nullptr;
    }
 
    // ---------- Access ----------
    void set(int idx, T val)
    {
        if(idx < 0 || idx >= size) return;
        arr[idx] = val;
    }
 
    T get(int idx) const
    {
        if(idx < 0 || idx >= size) return T();
        return arr[idx];
    }
 
    T Front() const
    {
        if(size == 0) return T();
        return arr[0];
    }
 
    T Back() const
    {
        if(size == 0) return T();
        return arr[size - 1];
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
    }
 
    // ---------- Adding and removing ----------
    void pushBack(T val)
    {
        if(size == cap) expandCap();
        arr[size++] = val;
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
            arr[i + 1] = arr[i];
        }
        arr[idx] = val;
        size++;
    }
 
    void deleteAt(int idx)
    {
        if(idx < 0 || idx >= size) return;
 
        for(int i = idx; i + 1 < size; i++)
        {
            arr[i] = arr[i + 1];
        }
        size--;
        shrinkIfSparse();
    }
 
    // ---------- Utilities ----------
    void Print() const
    {
        for(int i = 0; i < size; i++)
        {
            cout << arr[i] << " ";    
        }
        cout << nl;
    }
 
    int find(T val) const
    {
        for(int i = 0; i < size; i++)
        {
            if(arr[i] == val) return i;    
        }
        return -1;
    }
 
    T getMax() const
    {
        if(size == 0) return T();
        T maxi = arr[0];
        for(int i = 1; i < size; i++)
        {
            maxi = max(maxi, arr[i]);    
        }
        return maxi;
    }
 
    T getMin() const
    {
        if(size == 0) return T();
        T mini = arr[0];
        for(int i = 1; i < size; i++)
        {
            mini = min(mini, arr[i]);    
        }
        return mini;
    }
 
    void Sort()
    {
        sort(arr, arr + size);
    }
 
    void Reverse()
    {
        reverse(arr, arr + size);
    }
 
    // ---------- Iterators ----------
    T *begin()
    {
        return arr;
    }
 
    T *end()
    {
        return arr + size;
    }
 
    const T *begin() const
    {
        return arr;
    }
 
    const T *end() const
    {
        return arr + size;
    }
 
    // ---------- Operators ----------
    Vector & operator=(const Vector<T> & other)
    {
        if(this != &other)
        {
            T *newArr = new T[other.cap]();
            for (int i = 0; i < other.size; i++)
            {
                newArr[i] = other.arr[i];
            } 
            delete[] arr;
            arr = newArr;
            size = other.size;
            cap = other.cap;
        }
        return *this;
    }
 
    T &operator[](int idx)
    {
        assert(idx >= 0 && idx < size);
        return arr[idx];
    }
 
    const T &operator[](int idx) const
    {
        assert(idx >= 0 && idx < size);
        return arr[idx];
    }
};

#endif
