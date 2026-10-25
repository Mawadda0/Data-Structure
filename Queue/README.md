# Queue in C++: Linear, Circular, and Linked List

> Three from-scratch implementations of a queue. Each one fixes a problem of the one before it: the linear array queue wastes space, the circular queue reuses it but has a fixed capacity, and the linked list queue has no capacity limit at all.

## Table of Contents

1. [Files](#files)
2. [What Is a Queue?](#what-is-a-queue)
3. [Error Handling](#error-handling)
4. [Linear Queue](#linear-queue)
5. [Circular Queue](#circular-queue)
6. [Linked List Queue](#linked-list-queue)
7. [Comparison](#comparison)
8. [Known Issues and Limitations](#known-issues-and-limitations)

---

## Files

| File                    | Class              | Description                                              |
| ----------------------- | ------------------ | -------------------------------------------------------- |
| `Queue_Linear.h`        | `Queue<T>`         | Array queue where `front` and `rear` only move forward (capacity 5) |
| `Queue_Circular.h`      | `CircularQueue<T>` | Array queue where `front` and `rear` wrap around (capacity 4)       |
| `Queue_Linked_List.h`   | `Queue<T>`         | Queue stored as a linked list, no capacity limit                    |

All three are header-only templates. The capacity of the two array queues is a constant named `MAX_SIZE` inside the class. **`Queue_Linear.h` and `Queue_Linked_List.h` both define a class named `Queue`, so include only one of them in the same source file.**

---

## What Is a Queue?

A queue is a collection where you add at one end, the **rear**, and remove from the other end, the **front**. The first element added is the first one removed. This rule is called **FIFO** (first in, first out), like people waiting in a line.

```
enqueue --> [ rear ]  [ 30 ]  [ 20 ]  [ 10 ] [ front ] --> dequeue
```

| Operation | Meaning |
| --- | --- |
| `enqueue(value)` | Add a value at the rear |
| `dequeue()` | Remove and return the value at the front |
| `peek()` | Return the front value without removing it |

All three operations are O(1) in every implementation here. Queues appear in task scheduling, printer spoolers, breadth-first search, and buffering data between two processes that run at different speeds.

---

## Error Handling

Adding to a full array queue prints a message and does nothing:

```
Queue is full (overflow)
```

Removing from or peeking at an empty queue **throws** a string literal, so it must be caught as `const char *`:

```cpp
try
{
    q.dequeue();
}
catch(const char *msg)
{
    std::cout << msg << '\n';   // Queue is empty (Underflow)
}
```

The linked list queue never overflows, so only `dequeue` and `peek` can fail.

---

## Linear Queue

The values live in an array of `MAX_SIZE = 5` cells. `front` and `rear` are the indices of the first and last element, and both are `-1` when the queue is empty.

- `enqueue` moves `rear` one cell forward and writes the value (the first element sets both to 0).
- `dequeue` returns `arr[front]` and moves `front` one cell forward. When `front == rear` the queue becomes empty and both are reset to `-1`.
- The queue is full when `rear == MAX_SIZE - 1`.

```
After enqueue(1) ... enqueue(5):

index:   0    1    2    3    4
       [ 1 ][ 2 ][ 3 ][ 4 ][ 5 ]
         ^                   ^
       front               rear
```

### The wasted space problem

`front` and `rear` never move backwards, so the cells that `dequeue` frees are never used again, unless the queue becomes completely empty:

```
After dequeue(), dequeue():

index:   0    1    2    3    4
       [ _ ][ _ ][ 3 ][ 4 ][ 5 ]
                   ^         ^
                 front     rear

enqueue(6)  ->  rear is already at the last index, so it prints
                "Queue is full (overflow)" even though cells 0 and 1 are free.
```

This is the reason the circular queue exists.

### Usage

```cpp
#include "Queue_Linear.h"

int main()
{
    Queue<int> q;
    for (int i = 1; i <= 5; i++) q.enqueue(i);
    q.dequeue();
    q.dequeue();
    q.print();         // 3 4 5
    q.enqueue(6);      // rejected: rear is at the end, even though 2 cells are free
}
```

Output:

```
3 4 5
Queue is full (overflow)
```

### Functions reference

| Function | Description | Time |
| --- | --- | --- |
| `Queue()` | Creates an empty queue | O(1) |
| `isEmpty()` | Returns `true` if the queue has no elements | O(1) |
| `isFull()` | Returns `true` if `rear` is at the last cell | O(1) |
| `enqueue(value)` | Adds at the rear, or prints `Queue is full (overflow)` | O(1) |
| `dequeue()` | Removes and returns the front, or throws `The queue is empty (Underflow)` | O(1) |
| `peek()` | Returns the front, or throws `Queue is empty (Underflow)` | O(1) |
| `print()` | Prints the elements from front to rear | O(n) |

---

## Circular Queue

The fix is to treat the array as a circle. When `front` or `rear` reaches the last cell, the next step wraps back to cell 0 using the modulo operator:

```cpp
rear = (rear + 1) % MAX_SIZE;
front = (front + 1) % MAX_SIZE;
```

The queue is full when moving `rear` one step forward would land on `front`:

```cpp
front == (rear + 1) % MAX_SIZE
```

Since an empty queue is represented by `front == rear == -1` (and not by `front == rear`), **all `MAX_SIZE` cells can be used**.

```
MAX_SIZE = 4. After enqueue(1..4), dequeue(), dequeue(), enqueue(5), enqueue(6):

index:   0    1    2    3
       [ 5 ][ 6 ][ 3 ][ 4 ]
              ^    ^
            rear front

order from front:  3 -> 4 -> 5 -> 6
```

`rear` wrapped around to index 1 while `front` is at index 2, so the elements run from index 2 to the end of the array and continue from index 0.

### Usage

```cpp
#include "Queue_Circular.h"

int main()
{
    CircularQueue<int> q;
    for (int i = 1; i <= 4; i++) q.enqueue(i);
    q.dequeue();
    q.dequeue();
    q.enqueue(5);      // reuses the freed cells
    q.enqueue(6);
    q.print();         // 3 4 5 6
    std::cout << q.Size() << " " << q.isFull() << " " << q.search(5) << '\n';
}
```

Output:

```
3 4 5 6
4 1 2
```

The same sequence fails in the linear queue.

### Functions reference

| Function | Description | Time |
| --- | --- | --- |
| `CircularQueue()` | Creates an empty queue | O(1) |
| `isEmpty()` | Returns `true` if the queue has no elements | O(1) |
| `isFull()` | Returns `true` if `front == (rear + 1) % MAX_SIZE` | O(1) |
| `Size()` | Returns the number of elements (counted by walking from `front` to `rear`) | O(n) |
| `print()` | Prints the elements from front to rear | O(n) |
| `search(value)` | Returns the 0-based position of `value` counted from the front, or `-1` | O(n) |
| `enqueue(value)` | Adds at the rear, or prints `Queue is full (overflow)` | O(1) |
| `dequeue()` | Removes and returns the front, or throws `The queue is empty (Underflow)` | O(1) |
| `peek()` | Returns the front, or throws `Queue is empty (Underflow)` | O(1) |
| `enqueueN(n)` | Prints `Enter n values:`, reads `n` values from `cin`, and enqueues each one | O(n) |
| `dequeueN(n)` | Dequeues up to `n` times, printing `Removed: x` each time, and stops at the first underflow | O(n) |

---

## Linked List Queue

The queue is a singly linked list with two pointers: `head` is the front and `tail` is the rear.

```
head (front)                        tail (rear)
 |                                    |
 v                                    v
[ 10 | * ]--->[ 20 | * ]--->[ 30 | null ]
```

- `enqueue` creates a node and links it after `tail`, then moves `tail` to it. The `tail` pointer is what makes this O(1). In the plain singly linked list in `04_linked_list`, adding at the tail is O(n) because it only stores `head`.
- `dequeue` removes the head node, so it is O(1) as well. When the last node is removed, both pointers become `nullptr`.
- There is no capacity, so there is no overflow and no wasted space. Every element costs one `new`, and the destructor frees all remaining nodes.

### Usage

```cpp
#include "Queue_Linked_List.h"

int main()
{
    Queue<int> q;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.print();                           // 10 20 30
    std::cout << q.dequeue() << '\n';    // 10
    std::cout << q.peek() << '\n';       // 20

    try
    {
        Queue<int> empty;
        empty.dequeue();
    }
    catch(const char *msg)
    {
        std::cout << msg << '\n';
    }
}
```

Output:

```
10 20 30
10
20
Queue is empty (Underflow)
```

### Functions reference

| Function | Description | Time |
| --- | --- | --- |
| `Queue()` | Creates an empty queue | O(1) |
| `~Queue()` | Deletes every remaining node | O(n) |
| `isEmpty()` | Returns `true` if the queue has no elements | O(1) |
| `enqueue(value)` | Adds a node at the tail | O(1) |
| `dequeue()` | Removes and returns the front, or throws `Queue is empty (Underflow)` | O(1) |
| `peek()` | Returns the front, or throws `Queue is empty (Underflow)` | O(1) |
| `print()` | Prints the elements from front to rear | O(n) |

---

## Comparison

| | Linear | Circular | Linked list |
| --- | --- | --- | --- |
| Capacity | 5 | 4 | Limited only by memory |
| Overflow possible | Yes | Yes | No |
| Reuses freed space | Only when the queue becomes empty | Yes | Yes (nodes are deleted) |
| `enqueue`, `dequeue`, `peek` | O(1) | O(1) | O(1) |
| Memory | Fixed array | Fixed array | One node (data + pointer) per element |
| Needs a destructor | No | No | Yes |
| Safe to copy | Yes | Yes | No (see Known Issues) |
| Index arithmetic | `front++`, `rear++` | `% MAX_SIZE` | None, only pointers |

When the maximum number of elements is known, the circular queue is the best choice: no allocation and no wasted cells. When it is not, use the linked list queue.

---

## Known Issues and Limitations

These are confirmed behaviors of the current code.

**`Queue_Linear.h` and `Queue_Circular.h`**

- **`enqueue`, `dequeue` and `peek` use `int`, not `T`.** The array holds `T`, but the functions take and return `int`, so any other element type is converted. For example, `Queue<double>` followed by `enqueue(2.75)` stores and prints `2`. Using `T` in those signatures fixes it.
- **`dequeueN` prints `Removed: ` before the underflow message.** The output expression starts printing before `dequeue()` throws, so the last line reads `Removed: The queue is empty (Underflow)`.
- **The capacities are tiny** (5 and 4) because they are meant for demonstration.

**`Queue_Linked_List.h`**

- **No copy constructor and no copy assignment.** Copying a queue copies only the `head` and `tail` pointers, so both queues point to the same nodes and both destructors delete them (a double free). Pass queues by reference and never copy them.
- **Elements are default-initialized, not constructed with the value.** `enqueue` creates the node with `new Node<T>()` and then assigns `data`, so `T` must be default-constructible and assignable.