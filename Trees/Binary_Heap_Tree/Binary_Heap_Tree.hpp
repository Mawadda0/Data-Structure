#ifndef BINARYHEAPTREE_HPP
#define BINARYHEAPTREE_HPP

#include "Template.hpp"
// Binary Heap stored in an array (a vector), so no nodes and no pointers are needed.
// It is a Min Heap by default (the smallest value is always on top).
// Use Heap<T, false> for a Max Heap (the largest value is always on top).
//
// The tree is always complete: it is filled level by level, from left to right.
// For the node at index i:  left child = 2 * i + 1,  right child = 2 * i + 2,  parent = (i - 1) / 2.
// For every node, its value is better than or equal to the values of its children.
// Only operator< is required from T (a Max Heap just flips the arguments).
template <typename T, bool MIN = true>
class Heap
{
private:
    vector<T> nums;
 
    // ---------- Private helpers ----------
    // Each index helper returns -1 when the node doesn't exist.
    int left(int idx) const
    {
        int i = 2 * idx + 1;
        return (i < size() ? i : -1);
    }
 
    int right(int idx) const
    {
        int i = 2 * idx + 2;
        return (i < size() ? i : -1);
    }
 
    int parent(int idx) const
    {
        return (idx == 0 ? -1 : (idx - 1) / 2);
    }
 
    // true if x must be above y
    bool better(const T &x, const T &y) const
    {
        if(MIN) return x < y; // Min Heap
        return y < x;         // Max Heap
    }
 
    // Moves a node up while it is better than its parent (used after adding at the end).
    void HeapfiyUp(int idx)
    {
        while(idx > 0 && better(nums[idx], nums[parent(idx)]))
        {
            swap(nums[idx], nums[parent(idx)]);
            idx = parent(idx);
        }
    }
 
    // Moves a node down while one of its children is better than it
    // (used after replacing the root, and when building the heap from a vector).
    void HeapfiyDown(int idx)
    {
        while(true)
        {
            int l = left(idx);
            int r = right(idx);
            int best = idx;
 
            if(l != -1 && better(nums[l], nums[best])) best = l;
            if(r != -1 && better(nums[r], nums[best])) best = r;
 
            if(best == idx) break; // already in the correct place
            swap(nums[idx], nums[best]);
            idx = best;
        }
    }
 
public:
    // ---------- Constructors ----------
    // No destructor or copy functions are needed: the vector manages its own memory,
    // so copying a Heap is already a deep copy.
    Heap() {}
 
    // Builds a heap from a vector in O(n): heapify down every node that has a child,
    // starting from the last one.
    Heap(const vector<T> &v)
    {
        nums = v;
        for(int i = size() / 2 - 1; i >= 0; i--)
            HeapfiyDown(i);
    }
 
    // ---------- Utilities ----------
    bool empty() const
    {
        return nums.empty();
    }
 
    int size() const
    {
        return (int)nums.size();
    }
 
    void clear()
    {
        nums.clear();
    }
 
    // The root: the minimum in a Min Heap, the maximum in a Max Heap.
    T top() const
    {
        if(nums.empty())
        {
            cout << "Heap Is Empty." << nl;
            return T();
        }
        return nums[0];
    }
 
    // ---------- Adding and removing ----------
    // Adds the item as the last leaf, then moves it up to its place.
    void push(const T & item)
    {
        nums.push_back(item);
        HeapfiyUp(size() - 1);
    }
 
    // Removes the root. Deleting it directly would leave a hole, so the last leaf
    // takes its place (the tree stays complete) and is then moved down to its place.
    void pop()
    {
        if(nums.empty())
        {
            cout << "Heap Is Empty." << nl;
            return;
        }
        nums[0] = nums.back();
        nums.pop_back();
        HeapfiyDown(0);
    }
 
    // ---------- Display ----------
    // The array of a heap is already its level order.
    void display_LevelByLevel() const
    {
        if(nums.empty())
        {
            cout << "Heap Is Empty." << nl;
            return;
        }
        for(const T & x : nums) cout << x << " ";
        cout << nl;
    }
};

#endif