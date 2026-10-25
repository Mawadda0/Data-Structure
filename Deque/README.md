# Deque: A Double-Ended Queue Using a Dynamic Circular Array

> A from-scratch implementation of a deque in C++ where adding and removing at **both** ends is O(1) amortized. It uses a resizable circular array, so no element is ever shifted when we push or pop at the front.

## Table of Contents

1. [Files and Usage](#files-and-usage)
2. [The Problem with a Plain Array](#the-problem-with-a-plain-array)
3. [How the Circular Array Works](#how-the-circular-array-works)
4. [Resizing](#resizing)
5. [Constructors and Destructor](#constructors-and-destructor)
6. [Operators](#operators)
7. [Functions Reference](#functions-reference)
8. [Amortized Time Complexity](#amortized-time-complexity)
9. [Deque vs Vector](#deque-vs-vector)
10. [Design Notes and Limitations](#design-notes-and-limitations)

---

## Files and Usage

| File        | Description                     |
| ----------- | ------------------------------- |
| `Deque.hpp`   | The class template, header-only |
| `README.md` | This document                   |

Every header starts with `#include "Template.hpp"`, the shared header of the repository that provides the standard library includes, `using namespace std;` and the `nl` macro (a newline). This is why the examples use `cout` and `string` without the `std::` prefix.

```cpp
#include "Deque.hpp"

int main()
{
    Deque<int> d;
    d.pushBack(2);
    d.pushBack(3);
    d.pushFront(1);
    d.pushFront(0);       // 0 1 2 3
    d.Print();

    d.popFront();
    d.popBack();          // 1 2
    d.Print();

    d.pushFront(9);
    d.pushBack(5);        // 9 1 2 5
    d.Sort();             // 1 2 5 9

    for (int i = 0; i < d.Size(); i++)
        cout << d[i] << " ";
    cout << '\n';

    cout << d.Front() << " " << d.Back() << " " << d[2] << '\n';
}
```

Output:

```
0 1 2 3
1 2
1 2 5 9
1 9 5
```

The element type `T` must be default-constructible and copy-assignable. `Sort`, `find`, `getMax` and `getMin` additionally need `<`, `==` and `<` on `T` respectively.

---

## The Problem with a Plain Array

If we build a deque on top of a normal dynamic array (like our `Vector`), `pushBack` and `popBack` are fast, but `pushFront` and `popFront` are not. To add an element at index 0, every existing element has to move one place to the right to make room:

```
pushFront(5) on [10, 20, 30]:

before:        [10, 20, 30, _ ]
after shift:   [ _, 10, 20, 30]     all n elements moved
after write:   [ 5, 10, 20, 30]
```

That is O(n) per operation. A deque is supposed to be fast at **both** ends, so we need a different layout.

---

## How the Circular Array Works

The idea: instead of moving the elements, we move the **starting point**. The class keeps four pieces of data:

| Member | Meaning                                                   |
| ------ | --------------------------------------------------------- |
| `arr`  | Pointer to a block of memory allocated on the heap        |
| `size` | The number of elements that are actually stored           |
| `cap`  | The number of elements the block can hold (capacity)      |
| `head` | The **physical** index in `arr` where the first element lives |

The array is treated as a circle. When we walk past the last cell, we continue from the first cell.

### Logical vs physical index

The user thinks in **logical** indices: index 0 is the front of the deque, index `size - 1` is the back. The array uses **physical** indices. The private helper `phys(i)` converts one to the other:

```
physical index = (head + logical index) % cap
```

The code avoids the `%` operator and uses a single comparison instead, since `head + i` can never reach `2 * cap`:

```cpp
int phys(int i) const
{
    int p = head + i;
    return p >= cap ? p - cap : p;
}
```

### Example

We start with `cap = 10`, then call `pushBack(1)`, `pushBack(2)`, `pushBack(3)`, and finally `pushFront(0)`.

```
After the three pushBack calls:    head = 0, size = 3

physical:   0    1    2    3    4    5    6    7    8    9
          [ 1 ][ 2 ][ 3 ][ _ ][ _ ][ _ ][ _ ][ _ ][ _ ][ _ ]
            ^
          head
```

`pushFront(0)` moves `head` one cell **backwards**. Since `head` is 0, it wraps around to the last cell, `cap - 1 = 9`, and we write the value there. Nothing else moves.

```
After pushFront(0):                head = 9, size = 4

physical:   0    1    2    3    4    5    6    7    8    9
          [ 1 ][ 2 ][ 3 ][ _ ][ _ ][ _ ][ _ ][ _ ][ _ ][ 0 ]
                                                          ^
                                                        head

logical order:  0  1  2  3
                (physical 9, 0, 1, 2)
```

To read logical index 1, we compute `phys(1) = 9 + 1 = 10`, which is `>= cap`, so we subtract `cap` and get 0. And indeed `arr[0]` holds 1.

### How each operation works

| Operation       | What it does                                                           |
| --------------- | ---------------------------------------------------------------------- |
| `pushFront(v)`  | `head` moves one cell back (wrapping to `cap - 1`), write `v`, `size++` |
| `pushBack(v)`   | Write `v` at `phys(size)`, `size++`                                    |
| `popFront()`    | `head` moves one cell forward (wrapping to 0), `size--`                |
| `popBack()`     | `size--`                                                               |

All four are a few assignments, with no loop. The expensive part only happens when the array is full or too empty, which is the resizing case.

---

## Resizing

### Growing

When `pushFront`, `pushBack` or `insertAt` is called and `size == cap`, there is no room left. The deque allocates a new array with **double** the capacity, copies the elements, and frees the old one.

The copy goes in **logical order**, not physical order. This matters because the elements may be wrapped around the end of the old array. After copying, they are laid out contiguously from index 0, so `head` is reset to 0:

```
Old array (full, wrapped):  cap = 5, head = 3

physical:   0    1    2    3    4
          [ c ][ d ][ e ][ a ][ b ]
                          ^
                        head
logical order:  a  b  c  d  e

New array (cap = 10):       head = 0

physical:   0    1    2    3    4    5    6    7    8    9
          [ a ][ b ][ c ][ d ][ e ][ _ ][ _ ][ _ ][ _ ][ _ ]
```

### Shrinking

When `popFront`, `popBack` or `deleteAt` leaves the deque sparse, it gives memory back. It halves the capacity only when **`size <= cap / 4`** and the new capacity would not go below the minimum of 10.

Shrinking at one quarter (and not at one half) leaves a gap after every resize. Otherwise a `popBack` followed by a `pushBack` at the boundary would shrink and immediately grow again, making each operation O(n).

The same `reallocate` function performs both, so the shrink also copies in logical order and resets `head` to 0.

---

## Constructors and Destructor

| Constructor  | Signature                       | What it does                                                                                                           | Time                                                      |
| ------------ | ------------------------------- | ---------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------- |
| Default      | `Deque()`                       | Creates an empty deque with `size = 0`, `cap = 10`, `head = 0`                                                         | O(1)                                                      |
| Parametrized | `Deque(int size)`               | Creates a deque that already holds `size` elements, all value-initialized (`0` for numbers). Capacity is `size + 10`. A negative size is treated as 0 | O(n)                                                      |
| Copy         | `Deque(const Deque &other)`     | Creates a new deque that is a **deep copy** of `other`. The copy is laid out contiguously from index 0                 | O(n)                                                      |
| Destructor   | `~Deque()`                      | Frees the heap memory                                                                                                  | O(1) for primitive types, O(n) for types with destructors |

### Differences between them

**Default vs parametrized.** `Deque<int> a;` is empty (`Size() == 0`), while `Deque<int> b(5);` already holds 5 zeros (`Size() == 5`, `Cap() == 15`). To get an empty deque with room reserved, use the default constructor.

**The copy constructor is a deep copy.** Without it, the compiler would copy only the pointer, and two deques would share one block, so changing one would change the other and the same memory would be freed twice. Our copy constructor allocates its own block. It also reads the source in logical order, so it works correctly even when the source is wrapped around its array.

**The destructor** calls `delete[]` so the memory is not leaked.

---

## Operators

### Assignment operator `=`

`operator=` is used when the object on the left **already exists**. It allocates and fills the new block first, and only then frees the old one. It also checks `this != &other`, so `d = d` is safe. After assignment the deque is laid out from index 0 (`head = 0`).

Note that `Deque<int> c = a;` is **not** `operator=`. It creates a new object, so it calls the copy constructor.

### Subscript operator `[]`

| Version                            | Returns                    | Used when                 | Can modify?         |
| ---------------------------------- | -------------------------- | ------------------------- | ------------------- |
| `T &operator[](int)`               | A reference to the element | The deque is not `const`  | Yes: `d[2] = 5;`    |
| `const T &operator[](int) const`   | A const reference          | The deque is `const`      | No, read only       |

Both convert the logical index with `phys()` and use `assert`, so an invalid index stops the program in a debug build.

### `[]` vs `get` and `set`

| | `d[i]` | `d.get(i)` and `d.set(i, x)` |
| --- | --- | --- |
| Invalid index | `assert` fails (the program stops) | Silently returns `T()` or does nothing |
| Read and write | Both through one operator | Separate functions |

### Looping over the elements

`Deque` has no `begin()` and `end()`, so the range-based `for` loop is not available. The elements are not necessarily contiguous in memory (they can wrap around the end of the array), so a plain pointer cannot walk them. Use an index loop instead, which goes through `operator[]` and handles the wrap-around:

```cpp
for (int i = 0; i < d.Size(); i++)
    cout << d[i] << " ";

for (int i = 0; i < d.Size(); i++)   // modify in place
    d[i] *= 2;
```

Algorithms that need iterators, such as `sort`, cannot be used directly either. That is why `Sort` and `Reverse` are member functions.

---

## Functions Reference

Let `n` be the current size of the deque.

### Access

| Function            | Description                                                    | Time | Extra space |
| ------------------- | -------------------------------------------------------------- | ---- | ----------- |
| `get(idx)`          | Returns the element at logical `idx`, or `T()` if invalid      | O(1) | O(1)        |
| `set(idx, val)`     | Overwrites the element at `idx`. Does nothing if invalid       | O(1) | O(1)        |
| `Front()`           | Returns the first element, or `T()` if empty                   | O(1) | O(1)        |
| `Back()`            | Returns the last element, or `T()` if empty                    | O(1) | O(1)        |
| `operator[]`        | Returns a reference to the element, asserts on an invalid index | O(1) | O(1)        |

### Capacity

| Function  | Description                                                     | Time | Extra space |
| --------- | --------------------------------------------------------------- | ---- | ----------- |
| `Size()`  | Number of stored elements                                       | O(1) | O(1)        |
| `Cap()`   | Current capacity                                                | O(1) | O(1)        |
| `Empty()` | Returns `true` if `size == 0`                                   | O(1) | O(1)        |
| `Clear()` | Sets `size = 0` and `head = 0`. The memory is kept, not released | O(1) | O(1)        |

### Adding and removing

| Function              | Description                                                                                            | Time                            | Extra space                  |
| --------------------- | ------------------------------------------------------------------------------------------------------ | ------------------------------- | ---------------------------- |
| `pushFront(val)`      | Adds at the front by moving `head` back. Doubles the capacity if full                                  | O(1) amortized, O(n) worst case | O(1), or O(n) while resizing |
| `pushBack(val)`       | Adds at the back. Doubles the capacity if full                                                         | O(1) amortized, O(n) worst case | O(1), or O(n) while resizing |
| `popFront()`          | Removes the front by moving `head` forward. May halve the capacity                                     | O(1) amortized, O(n) worst case | O(1), or O(n) while resizing |
| `popBack()`           | Removes the back. May halve the capacity                                                               | O(1) amortized, O(n) worst case | O(1), or O(n) while resizing |
| `insertAt(idx, val)`  | Inserts at logical `idx` and shifts everything after it one place to the right. `idx == size` is allowed | O(n)                            | O(1)                         |
| `deleteAt(idx)`       | Removes the element at `idx` and shifts everything after it one place to the left                      | O(n)                            | O(1)                         |

`insertAt` and `deleteAt` stay O(n) because they shift elements. This is true for any array-based deque. Only the two ends are special.

### Searching and utilities

| Function     | Description                                                       | Time       | Extra space |
| ------------ | ----------------------------------------------------------------- | ---------- | ----------- |
| `find(val)`  | Linear search. Returns the logical index of the first match, or `-1` | O(n)       | O(1)        |
| `getMax()`   | Largest element, or `T()` if empty                                | O(n)       | O(1)        |
| `getMin()`   | Smallest element, or `T()` if empty                               | O(n)       | O(1)        |
| `Sort()`     | Makes the storage contiguous, then sorts ascending with `sort` | O(n log n) | O(log n)    |
| `Reverse()`  | Makes the storage contiguous, then reverses in place              | O(n)       | O(1)        |
| `Print()`    | Prints all elements from front to back, separated by spaces       | O(n)       | O(1)        |

### Private helpers

| Function              | Description                                                                                         | Time                          |
| --------------------- | --------------------------------------------------------------------------------------------------- | ----------------------------- |
| `phys(i)`             | Converts a logical index to a physical index in `arr`                                               | O(1)                          |
| `reallocate(newCap)`  | Allocates a block of `newCap`, copies the elements in logical order, frees the old block, sets `head = 0` | O(n)                          |
| `expandCap()`         | Calls `reallocate(cap * 2)`                                                                         | O(n)                          |
| `shrinkCap()`         | Calls `reallocate(cap / 2)`                                                                         | O(n)                          |
| `shrinkIfSparse()`    | Shrinks only if `size <= cap / 4` and the new capacity stays at least 10                            | O(1), or O(n) when it shrinks |
| `linearize()`         | Rotates the storage in place so the elements start at index 0 and are contiguous                    | O(n)                          |

### Why `Sort` and `Reverse` call `linearize()`

`sort` and `reverse` expect the elements in one straight run of memory. If the elements are wrapped, they would also include the empty cells in the middle.

`linearize()` fixes this with a single `rotate(arr, arr + head, arr + cap)`. The empty cells always sit right after the last element (going around the circle), so rotating the whole array left by `head` places the elements first and the empty cells last:

```
cap = 10, head = 8, size = 5

before:   0    1    2    3    4    5    6    7    8    9
        [ c ][ d ][ e ][ _ ][ _ ][ _ ][ _ ][ _ ][ a ][ b ]
                                                  ^
                                                head

after rotate by head:
        [ a ][ b ][ c ][ d ][ e ][ _ ][ _ ][ _ ][ _ ][ _ ]    head = 0
```

---

## Amortized Time Complexity

A single `pushFront` or `pushBack` can cost O(n) when the array is full and everything has to be copied. But we **double** the capacity each time, so the expensive copies happen after 10, 20, 40, 80, ... insertions. For `n` pushes starting from capacity 10, the total number of copied elements is at most:

```
10 + 20 + 40 + ... + n/2  <  n
```

So the total work for `n` pushes is less than `2n`, which is O(1) per push on average, no matter which end we push to. The same argument applies to the pops with the shrink-at-a-quarter rule.

This is the key point: the circular layout removes the shifting, and doubling removes the cost of resizing. Together they give O(1) amortized at both ends.

---

## Deque vs Vector

| | `Vector` | `Deque` (this implementation) |
| --- | --- | --- |
| `pushBack` / `popBack` | O(1) amortized | O(1) amortized |
| `pushFront` / `popFront` | O(n) | O(1) amortized |
| Random access `[i]` | O(1) | O(1), with one extra addition and comparison |
| `insertAt` / `deleteAt` | O(n) | O(n) |
| Memory layout | Always contiguous | Can wrap around the end of the array |
| Range-based `for` | Yes (raw pointers) | No, use an index loop |
| `Sort` / `Reverse` | Directly on the array | After `linearize()`, which costs O(n) |

The only structural differences are the `head` index and the wrap-around. Everything else follows the same design as `Vector`.

### Comparison with `deque`

The standard library's `deque` uses a different design: it stores elements in many fixed-size blocks and keeps a table of pointers to them. Growing it never moves existing elements, and references to elements stay valid after a `push` at either end. The cost is a slightly slower index lookup (two steps instead of one).

The circular array used here is simpler and has faster indexing, but a resize copies every element, so those references and pointers are invalidated. Both designs give O(1) amortized operations at the two ends.

---

## Design Notes and Limitations

- **No automatic bounds errors in `get` and `set`.** They return a default value or do nothing on an invalid index. Use `operator[]` when you want invalid access to be caught.
- **`Clear()` does not free memory.** It only resets the size and `head`, so the capacity stays where it was.
- **`insertAt` and `deleteAt` always shift the elements after the index.** A possible improvement is to shift whichever side (before or after the index) is shorter, which halves the work on average, but it is still O(n).
- **The constructor `Deque(int size)` is not `explicit`.** That means `Deque<int> d = 5;` compiles and builds a deque with 5 elements. Marking it `explicit` prevents such accidental conversions.
- **Deep copies are expensive.** Pass a `Deque` by `const Deque<T> &` instead of by value.
- **No move semantics.** There is no move constructor or move assignment, so returning or reassigning large deques always copies.
- **Elements are default-constructed.** The `new T[cap]()` expression constructs `cap` objects up front, so `T` must have a default constructor.
- **References are invalidated by resizing.** Any `push`, `pop`, `insertAt` or `deleteAt` that triggers a resize moves the elements to a new block, so references and pointers to them become dangling.
- **Single-threaded only.** There is no synchronization.