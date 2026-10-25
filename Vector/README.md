# Vector: A Custom Dynamic Array in C++

> A from-scratch implementation of a resizable array (like `vector`) using a raw pointer, `new[]` and `delete[]`. It covers capacity management, resizing, deep copying, and amortized time complexity.

## Table of Contents

1. [Files and Usage](#files-and-usage)
2. [How It Works](#how-it-works)
3. [Constructors and Destructor](#constructors-and-destructor)
4. [Operators](#operators)
5. [Functions Reference](#functions-reference)
6. [Amortized Time Complexity](#amortized-time-complexity)
7. [Design Notes and Limitations](#design-notes-and-limitations)

---

## Files and Usage

| File       | Description                           |
| ---------- | ------------------------------------- |
| `Vector.hpp` | The class template, header-only       |
| `README.md`| This document                         |

Every header starts with `#include "Template.hpp"`, the shared header of the repository that provides the standard library includes, `using namespace std;` and the `nl` macro (a newline). This is why the examples use `cout` and `string` without the `std::` prefix.

```cpp
#include "Vector.hpp"

int main()
{
    Vector<int> v;
    v.pushBack(30);
    v.pushBack(10);
    v.pushBack(20);
    v.insertAt(1, 99);   // 30 99 10 20
    v.Sort();            // 10 20 30 99
    v.Print();

    for (int x : v)
        cout << x << " ";
    cout << '\n';

    Vector<int> copy = v;   // copy constructor
    copy[0] = -1;           // does not affect v
    cout << v[0] << " " << copy[0] << '\n';
}
```

Output:

```
10 20 30 99
10 20 30 99
10 -1
```

The element type `T` must be default-constructible and copy-assignable. `Sort`, `find`, `getMax` and `getMin` additionally need `<`, `==` and `<` on `T` respectively.

---

## How It Works

The vector keeps three pieces of data:

| Member | Meaning                                                 |
| ------ | ------------------------------------------------------- |
| `arr`  | Pointer to a block of memory allocated on the heap      |
| `size` | The number of elements that are actually stored         |
| `cap`  | The number of elements the block can hold (capacity)    |

**Size and capacity are different things.** The size is how many elements we have, and the capacity is how much room we reserved. We always have `size <= cap`. The extra room is what allows `pushBack` to be fast most of the time.

```
cap = 10, size = 4

index:   0    1    2    3    4    5    6    7    8    9
       [ 10 | 20 | 30 | 99 |  _ |  _ |  _ |  _ |  _ |  _ ]
        └──── used ──────┘  └────── free space ──────────┘
```

### Growing

When `pushBack` or `insertAt` is called and `size == cap`, there is no room left. We cannot extend the existing block, because the memory right after it may already be used by something else. So the vector:

1. Allocates a new block with **double** the capacity.
2. Copies all the elements into it.
3. Frees the old block.
4. Makes `arr` point to the new block.

### Shrinking

When `popBack` or `deleteAt` makes the vector sparse, it gives memory back. The vector shrinks (halves the capacity) only when **`size <= cap / 4`** and the new capacity would not go below the minimum capacity of 10.

Why a quarter and not a half? If we shrank as soon as the vector was half full, then a `popBack` followed by a `pushBack` right at that boundary would shrink and then immediately grow again, and each of those costs O(n). Alternating the two would make every operation O(n). Shrinking at one quarter leaves a gap, so after a resize we need many more operations before the next one.

```
cap = 80, size = 20 after a popBack   ->  size <= cap / 4, shrink to cap = 40
cap = 40, size = 20                   ->  half full, plenty of room in both directions
```

---

## Constructors and Destructor

| Constructor | Signature | What it does | Time |
| --- | --- | --- | --- |
| Default | `Vector()` | Creates an empty vector with `size = 0` and `cap = 10` | O(1) |
| Parametrized | `Vector(int size)` | Creates a vector that already contains `size` elements, all value-initialized (`0` for numbers). Capacity is `size + 10`. A negative size is treated as 0 | O(n) |
| Copy | `Vector(const Vector &other)` | Creates a new vector that is a **deep copy** of `other` | O(n) |
| Destructor | `~Vector()` | Frees the heap memory | O(1) for primitive types, O(n) for types with destructors |

### Differences between them

**Default vs parametrized.** `Vector<int> a;` is empty (`Size() == 0`), while `Vector<int> b(5);` is not empty. It already has 5 elements, all zeros, and `Size() == 5`, `Cap() == 15`. If you want an empty vector with some room reserved, use the default constructor and `pushBack`.

**Why the copy constructor matters.** Without it, the compiler generates a copy that copies only the pointer (a **shallow copy**). Then two vectors would point to the same block, changing one would change the other, and when both are destroyed the same memory would be freed twice, which crashes the program. Our copy constructor allocates a new block and copies every element (a **deep copy**), so the two vectors are independent.

**Why the destructor matters.** The elements live on the heap, so they are not freed automatically when the vector goes out of scope. The destructor calls `delete[]` to prevent a memory leak.

---

## Operators

### Assignment operator `=`

```cpp
Vector<int> a;
Vector<int> b;
b = a;              // calls operator=
Vector<int> c = a;  // calls the COPY CONSTRUCTOR, not operator=
```

Even though `Vector<int> c = a;` contains an `=`, it creates a new object, so it is the copy constructor. `operator=` is used when the object on the left **already exists**.

| Copy constructor | `operator=` |
| --- | --- |
| Builds a new object | Overwrites an object that already exists |
| No old memory to worry about | Must free the old block (no memory leak) |
| No self-assignment problem | Must check `this != &other` for `a = a` |
| Returns nothing | Returns `*this` so assignments can be chained (`a = b = c`) |

In our implementation, `operator=` allocates and fills the new block first, and only then frees the old one.

### Subscript operator `[]`

Two versions exist, and the compiler picks one depending on whether the vector is `const`:

| Version | Returns | Used when | Can modify? |
| --- | --- | --- | --- |
| `T &operator[](int)` | A reference to the element | The vector is not `const` | Yes: `v[2] = 5;` |
| `const T &operator[](int) const` | A const reference | The vector is `const` | No, read only |

Both use `assert`, so an invalid index stops the program in a debug build.

### `[]` vs `get` and `set`

| | `v[i]` | `v.get(i)` and `v.set(i, x)` |
| --- | --- | --- |
| Invalid index | `assert` fails (the program stops) | Silently returns `T()` or does nothing |
| Syntax | Like a normal array | Function calls |
| Read and write | Both through one operator | Separate functions |

The tradeoff is simple: `[]` makes a bug visible immediately, while `get` and `set` never crash but can hide a bug, because `T()` (for example `0`) looks like a valid value.

### Iterators `begin()` and `end()`

`begin()` returns a pointer to the first element, and `end()` returns a pointer one past the last element. Having these two (in lowercase) is what makes range-based `for` loops and algorithms like `sort` work:

```cpp
for (int x : v)
    cout << x << " ";
```

---

## Functions Reference

Let `n` be the current size of the vector.

### Access

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `get(idx)` | Returns the element at `idx`, or `T()` if the index is invalid | O(1) | O(1) |
| `set(idx, val)` | Overwrites the element at `idx`. Does nothing if the index is invalid | O(1) | O(1) |
| `Front()` | Returns the first element, or `T()` if empty | O(1) | O(1) |
| `Back()` | Returns the last element, or `T()` if empty | O(1) | O(1) |
| `operator[]` | Returns a reference to the element, asserts on an invalid index | O(1) | O(1) |
| `begin()`, `end()` | Pointers to the first element and one past the last | O(1) | O(1) |

### Capacity

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `Size()` | Number of stored elements | O(1) | O(1) |
| `Cap()` | Current capacity | O(1) | O(1) |
| `Empty()` | Returns `true` if `size == 0` | O(1) | O(1) |
| `Clear()` | Sets `size = 0`. The memory is kept, not released | O(1) | O(1) |

### Adding and removing

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `pushBack(val)` | Appends at the end. Doubles the capacity if full | O(1) amortized, O(n) worst case | O(1), or O(n) while resizing |
| `popBack()` | Removes the last element. May halve the capacity | O(1) amortized, O(n) worst case | O(1), or O(n) while resizing |
| `insertAt(idx, val)` | Inserts at `idx` and shifts everything after it to the right. `idx == size` is allowed (insert at the end) | O(n) | O(1) |
| `deleteAt(idx)` | Removes the element at `idx` and shifts everything after it to the left | O(n) | O(1) |

`insertAt` and `deleteAt` are O(n) because of the shifting. At the very end of the array they shift nothing, so they behave like `pushBack` and `popBack`. At the front they are the worst case, because all `n` elements move.

### Searching and utilities

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `find(val)` | Linear search. Returns the index of the first match, or `-1` | O(n) | O(1) |
| `getMax()` | Largest element, or `T()` if empty | O(n) | O(1) |
| `getMin()` | Smallest element, or `T()` if empty | O(n) | O(1) |
| `Sort()` | Sorts ascending using `sort` | O(n log n) | O(log n) |
| `Reverse()` | Reverses the elements in place | O(n) | O(1) |
| `Print()` | Prints all elements separated by spaces | O(n) | O(1) |

### Private helpers

| Function | Description | Time |
| --- | --- | --- |
| `reallocate(newCap)` | Allocates a block of `newCap`, copies the elements, frees the old block | O(n) |
| `expandCap()` | Calls `reallocate(cap * 2)` | O(n) |
| `shrinkCap()` | Calls `reallocate(cap / 2)` | O(n) |
| `shrinkIfSparse()` | Shrinks only if `size <= cap / 4` and the new capacity stays at least 10 | O(1), or O(n) when it shrinks |

---

## Amortized Time Complexity

A single `pushBack` can cost O(n), when the array is full and everything has to be copied. But that happens rarely. Because we **double** the capacity every time, the expensive copies happen after 10, 20, 40, 80, ... insertions.

Suppose we do `n` pushes starting from capacity 10. The total number of element copies caused by resizing is at most:

```
10 + 20 + 40 + ... + n/2  <  n
```

Add the `n` normal writes, and the total work for `n` pushes is less than `2n`. Dividing by `n` operations gives a constant cost per push, which is **O(1) amortized**.

```
push #:    1  2  3 ... 10 | 11 ... 20 | 21 ... 40 | 41 ...
cost:      1  1  1 ...  1 | 11 ...  1 | 21 ...  1 | 41 ...
                            ^ resize     ^ resize    ^ resize
```

If we grew by a fixed amount instead (for example `cap + 10`), the number of resizes would grow linearly with `n`, and `pushBack` would become O(n) amortized. Multiplying the capacity by a constant factor is what makes it constant.

The same argument applies to `popBack` with the shrink-at-a-quarter rule.

---

## Design Notes and Limitations

- **No automatic bounds errors in `get` and `set`.** They return a default value or do nothing on an invalid index. Use `operator[]` when you want invalid access to be caught.
- **`Clear()` does not free memory.** It only resets the size, so the capacity stays where it was. A vector that once held a million elements keeps its large block.
- **Deep copies are expensive.** Passing a `Vector` by value copies every element, so prefer `const Vector<T> &` for function parameters.
- **No move semantics.** There is no move constructor or move assignment, so returning or reassigning large vectors always copies. Adding them is a natural next step (the rule of five).
- **Elements are default-constructed.** The `new T[cap]()` expression constructs `cap` objects up front, so `T` must have a default constructor, unlike `vector`, which constructs elements only when they are inserted.
- **The constructor `Vector(int size)` is not `explicit`.** That means `Vector<int> v = 5;` compiles and builds a vector with 5 elements. Marking it `explicit` prevents such accidental conversions.
- **Single-threaded only.** There is no synchronization.