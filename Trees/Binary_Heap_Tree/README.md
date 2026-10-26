# Binary Heap in C++

> A from-scratch binary heap stored in an array (a `vector`): no nodes and no pointers, the position of every value inside the array is its position in the tree. The smallest value (Min Heap, the default) or the largest value (Max Heap) is always on top, and both adding a value and removing the top cost only O(log n).

## Table of Contents

1. [Files and Usage](#files-and-usage)
2. [Why a Heap?](#why-a-heap)
3. [What Is a Binary Heap?](#what-is-a-binary-heap)
4. [From Array to Tree](#from-array-to-tree)
5. [Adding a Value](#adding-a-value)
6. [Removing the Top](#removing-the-top)
7. [Building a Heap from a Vector](#building-a-heap-from-a-vector)
8. [Min Heap and Max Heap](#min-heap-and-max-heap)
9. [Constructors and Copying](#constructors-and-copying)
10. [Functions Reference](#functions-reference)
11. [Why the Height Matters](#why-the-height-matters)
12. [Design Notes and Limitations](#design-notes-and-limitations)

---

## Files and Usage

| File         | Description                                  |
| ------------ | -------------------------------------------- |
| `Heap.hpp`   | The class template `Heap<T, MIN>`            |
| `README.md`  | This document                                |

Every header starts with `#include "Template.hpp"`, the shared header of the repository that provides the standard library includes, `using namespace std;` and the `nl` macro (a newline). This is why the examples use `cout` and `nl` without the `std::` prefix.

```cpp
#include "Heap.hpp"

int main()
{
    Heap<int> h;
    for(int x : {5, 3, 8, 1, 9, 2})
        h.push(x);

    h.display_LevelByLevel();    // 1 3 2 5 9 8
    cout << h.top() << " " << h.size() << nl;    // 1 6

    while(!h.empty())            // the values come out in sorted order
    {
        cout << h.top() << " ";
        h.pop();
    }
    cout << nl;                  // 1 2 3 5 8 9
}
```

Output:

```
1 3 2 5 9 8
1 6
1 2 3 5 8 9
```

### What `T` must support

| Needed by | Requirement on `T` |
| --- | --- |
| The whole heap | `operator<` only (a Max Heap just flips the arguments, so it does not need `operator>`), and copy-assignable |
| `top` | Default-constructible (an empty heap returns `T()`) |
| `display_LevelByLevel` | `operator<<` for `ostream` |

---

## Why a Heap?

Imagine you keep a collection of values and you repeatedly need **the smallest one** (or the largest), while new values keep arriving.

| Structure | Add a value | Get the smallest | Remove the smallest |
| --- | --- | --- | --- |
| Unsorted array | O(1) | O(n) | O(n) |
| Sorted array | O(n), the elements after the right place must be shifted | O(1) | O(n) from the front |
| Sorted linked list | O(n), the list must be traversed to find the place | O(1) | O(1) |
| **Heap** | **O(log n)** | **O(1)** | **O(log n)** |

A sorted array is slow to insert into, and a linked list is slow to search for the right place. The heap keeps just enough order to always know the smallest value, which is much cheaper than keeping everything sorted. This is the data structure behind a **priority queue**.

---

## What Is a Binary Heap?

A binary heap is a binary tree with two rules:

- **Shape rule:** the tree is **complete**. Every level is full, except maybe the last one, and the last level is filled from **left to right**.
- **Order rule:** for every node, its value is **better than or equal to** the values of its children. In a Min Heap "better" means smaller, so every parent is smaller than or equal to its children.

The heap built in the usage example above looks like this:

```
          1            <- root, level 1
        /   \
       3     2         <- level 2
      / \   /
     5   9 8           <- level 3
```

| Term | Meaning | In the example |
| --- | --- | --- |
| Root (top) | The first node, always the minimum in a Min Heap | `1` |
| Leaf | A node with no children | `5`, `9`, `8` |
| Level | The distance from the root, counted from 1 | `1` is level 1, `3` and `2` are level 2 |
| Height | The number of levels of the tree | 3 |

Only the **parent and its children** are compared. Nothing is said about the two children, or about a node and its cousins: in the example `3` is on the left of `2` and bigger than it, and this is fine. This is why a heap is **not** a sorted structure, and why it is cheaper to keep than one.

---

## From Array to Tree

Because the tree is always complete, there are no holes, so it fits in an array exactly in **level order**: the root first, then the second level from left to right, and so on. No pointers are needed, the position of a node tells where its relatives are.

```
index:    0  1  2  3  4  5
value:  [ 1, 3, 2, 5, 9, 8 ]
```

| Relative of the node at index `i` | Formula |
| --- | --- |
| Left child | `2 * i + 1` |
| Right child | `2 * i + 2` |
| Parent | `(i - 1) / 2` |

```
Node 3 is at index 1:   left child  = 2 * 1 + 1 = 3  -> 5
                        right child = 2 * 1 + 2 = 4  -> 9
                        parent      = (1 - 1) / 2 = 0 -> 1
```

```cpp
int left(int idx) const
{
    int i = 2 * idx + 1;
    return (i < size() ? i : -1);
}
```

The helpers `left`, `right`, and `parent` return `-1` when the node does not exist (an index outside the array, or the parent of the root), so the callers only need to compare with `-1`.

An array printed as it is **is** the level-order traversal of the heap. That is exactly what `display_LevelByLevel` does.

---

## Adding a Value

`push(item)` has two steps:

1. Put the value at the **end** of the array. This is the next free place in the last level, so the tree stays **complete**.
2. The new value may be better than its parent, so the order rule may be broken. `HeapfiyUp` fixes it: while the value is better than its parent, **swap** them and continue from the parent's place.

```
Push 2 into [1, 3, 8, 5, 9]     (a Min Heap)

step 1: put it at the end        [1, 3, 8, 5, 9, 2]     index 5
step 2: parent of 5 is 2 (value 8), and 2 < 8, so swap
                                 [1, 3, 2, 5, 9, 8]     index 2
        parent of 2 is 0 (value 1), and 2 is not < 1, stop
```

```cpp
void HeapfiyUp(int idx)
{
    while(idx > 0 && better(nums[idx], nums[parent(idx)]))
    {
        swap(nums[idx], nums[parent(idx)]);
        idx = parent(idx);
    }
}
```

The value moves up at most one level at a time, so it moves at most `h` times: **O(log n)**.

---

## Removing the Top

`pop()` removes the root. Removing it directly would leave a **hole** at the top and break the shape rule. The solution has three steps:

1. Copy the **last** value of the array into the root. Now the old top is gone and the tree is still complete (the last place is released).
2. Remove the last place of the array.
3. The value now at the root is probably in a wrong place, so `HeapfiyDown` fixes it: compare it with both children, and while one of them is better, **swap** it with the **better** child and continue from there.

```
Pop from [1, 3, 2, 5, 9, 8]

step 1-2: 8 replaces the root, last place removed
                                 [8, 3, 2, 5, 9]
step 3:   children of index 0 are 3 and 2, the better is 2, and 2 < 8, so swap
                                 [2, 3, 8, 5, 9]
          index 2 has no children, stop
```

```cpp
void HeapfiyDown(int idx)
{
    while(true)
    {
        int l = left(idx);
        int r = right(idx);
        int best = idx;

        if(l != -1 && better(nums[l], nums[best])) best = l;
        if(r != -1 && better(nums[r], nums[best])) best = r;

        if(best == idx) break;   // already in the correct place
        swap(nums[idx], nums[best]);
        idx = best;
    }
}
```

It is important to swap with the **better** of the two children: the child that moves up becomes the parent of the other one, so it must be better than it, or the order rule would break there. The value moves down at most `h` times: **O(log n)**.

`top()` just returns `nums[0]`, so it is **O(1)**.

---

## Building a Heap from a Vector

The constructor `Heap(const vector<T> &v)` copies the vector and then fixes the order. Pushing the values one by one costs O(n log n), but there is a faster way, **O(n)**:

- A node with no children is already a valid heap by itself, so the **leaves** need no work. The leaves are the second half of the array.
- Go over the other nodes **from the last one back to the root** and call `HeapfiyDown` on each. When a node is processed, both of its subtrees are already heaps, so moving that one node down is enough.

```cpp
for(int i = size() / 2 - 1; i >= 0; i--)
    HeapfiyDown(i);
```

`size() / 2 - 1` is the index of the last node that has a child.

```
Vector [5, 3, 8, 1, 9, 2]      n = 6, start at index 6 / 2 - 1 = 2

i = 2:  8 has the child 2 (index 5), 2 < 8, swap     [5, 3, 2, 1, 9, 8]
i = 1:  3 has the children 1 and 9, the better is 1, swap
                                                     [5, 1, 2, 3, 9, 8]
i = 0:  5 has the children 1 and 2, the better is 1, swap
        then 5 (now index 1) has the children 3 and 9, the better is 3, swap
                                                     [1, 3, 2, 5, 9, 8]
```

```cpp
#include "Heap.hpp"

int main()
{
    Heap<int> h(vector<int>{5, 3, 8, 1, 9, 2});
    h.display_LevelByLevel();
    cout << h.top() << nl;
}
```

Output:

```
1 3 2 5 9 8
1
```

---

## Min Heap and Max Heap

The second template parameter chooses the kind of heap. It is `true` (a **Min Heap**) by default.

```cpp
Heap<int> a;           // Min Heap: the smallest value is on top
Heap<int, false> b;    // Max Heap: the largest value is on top
```

The whole difference is in one function, `better`, which says whether a value must be **above** another one:

```cpp
bool better(const T &x, const T &y) const
{
    if(MIN) return x < y; // Min Heap
    return y < x;         // Max Heap
}
```

The Max Heap flips the arguments instead of using `operator>`, so `T` only needs `operator<`, the same as the binary search tree. `MIN` is a template parameter, so the `if` is decided at compile time and costs nothing.

```cpp
#include "Heap.hpp"

int main()
{
    Heap<int, false> h;
    for(int x : {4, 9, 1, 7})
        h.push(x);

    cout << h.top() << " " << h.size() << nl;
    h.pop();
    cout << h.top() << nl;
}
```

Output:

```
9 4
7
```

### Empty heap

`top` and `pop` on an empty heap print a message and change nothing. `top` also returns `T()`, which can be confused with a real value, so check `empty()` first.

```cpp
#include "Heap.hpp"

int main()
{
    Heap<int> h;
    h.pop();
    h.top();
    h.display_LevelByLevel();
}
```

Output:

```
Heap Is Empty.
Heap Is Empty.
Heap Is Empty.
```

---

## Constructors and Copying

| Function | Description | Time |
| --- | --- | --- |
| `Heap()` | Creates an empty heap | O(1) |
| `Heap(const vector<T> &v)` | Builds a heap from the values of `v` | O(n) |
| `clear()` | Removes every value | O(n) |

There is no destructor, copy constructor, or `operator=` in the class, and none is needed. The only data is a `vector`, which manages its own memory, so the copy that the compiler generates is already a **deep copy**. This is a big difference from the tree classes, where raw pointers made these functions necessary.

```cpp
#include "Heap.hpp"

int main()
{
    Heap<int> a;
    a.push(2);
    a.push(1);

    Heap<int> b = a;        // a deep copy
    b.push(0);

    cout << a.top() << " " << a.size() << nl;
    cout << b.top() << " " << b.size() << nl;
}
```

Output:

```
1 2
0 3
```

---

## Functions Reference

Let `n` be the number of values in the heap.

### Utilities

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `empty()` | Returns `true` if the heap has no values | O(1) | O(1) |
| `size()` | Number of values | O(1) | O(1) |
| `clear()` | Removes every value | O(n) | O(1) |
| `top()` | The minimum (Min Heap) or the maximum (Max Heap) | O(1) | O(1) |

### Adding and removing

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `push(item)` | Adds `item` at the end and moves it up | O(log n) | O(1) |
| `pop()` | Removes the top: the last value replaces it and moves down | O(log n) | O(1) |

### Display

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `display_LevelByLevel()` | Prints the values in level order, which is the array order | O(n) | O(1) |

### Private helpers

| Function | Description | Time |
| --- | --- | --- |
| `left(idx)`, `right(idx)`, `parent(idx)` | The index formulas, `-1` when the node does not exist | O(1) |
| `better(x, y)` | `true` if `x` must be above `y` (depends on `MIN`) | O(1) |
| `HeapfiyUp(idx)` | Moves a node up while it is better than its parent | O(log n) |
| `HeapfiyDown(idx)` | Moves a node down while a child is better than it | O(log n) |

All the loops are iterative, not recursive, so there is no call stack to worry about.

---

## Why the Height Matters

Both `HeapfiyUp` and `HeapfiyDown` walk along **one path** of the tree, so their cost is the height `h`. In a binary search tree `h` depends on the insertion order, and a bad order makes the tree a linked list. In a heap this **cannot happen**: the shape rule forces the tree to be complete, so the height is always the smallest possible.

| Values in the heap | Height |
| --- | --- |
| 7 | 3 |
| 1,000 | 10 |
| 1,000,000 | 20 |

A heap of `n` values always has `floor(log2(n)) + 1` levels, whatever order the values were added in. Sorted input is not a problem here:

```
Pushed 1, 2, 3, 4, 5, 6, 7 (already sorted)

        1
      /   \
     2     3
    / \   / \
   4   5 6   7        still 3 levels, so every push is O(1) here
```

The price of this guarantee is that the order rule is **weaker** than the one of a binary search tree: a heap knows its minimum immediately, but it cannot find an arbitrary value without looking at everything.

---

## Design Notes and Limitations

- **Only the top is available.** There is no search for an arbitrary value, no removal of an arbitrary value, and no way to change a value inside the heap. Searching is O(n) because there is no ordering between the nodes.
- **Duplicates are allowed.** Equal values are not "better" than each other, so they simply stay where they are.
- **The heap is not stable.** Equal values do not come out in the order they were added.
- **Only `operator<` is used** to compare values. There is no custom comparator, so the order of a type is the one defined by its `operator<`.
- **Messages are printed, not returned.** `Heap Is Empty.` is printed with `cout`, and `display_LevelByLevel` prints the array instead of returning it. `top()` on an empty heap returns `T()`, so check `empty()` first.
- **The heap printed is the array, not a sorted list.** `display_LevelByLevel` shows the internal order, for example `1 3 2 5 9 8`. To get the values in sorted order, call `top()` and `pop()` until the heap is empty (this is **heap sort**, O(n log n)).
- **`Heap(const vector<T> &v)` is not `explicit`.** That means a `vector<int>` converts to a `Heap<int>` silently. Marking it `explicit` prevents such accidental conversions.
- **Sizes are `int`.** Like the trees in this repository, `size()` returns an `int`, so a heap cannot hold more than about two billion values.
- **No move semantics.** The compiler-generated move operations of `vector` are used, so moving a heap is already cheap, but nothing is written explicitly.
- **Single-threaded only.** There is no synchronization.