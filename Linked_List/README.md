# Linked Lists in C++: Singly, Circular, and Doubly

> Three from-scratch implementations of a linked list using raw pointers, `new` and `delete`. All of them are built from nodes that live anywhere in the heap and are connected by pointers, instead of sitting side by side in one block like an array.

## Table of Contents

1. [Files](#files)
2. [What Is a Linked List?](#what-is-a-linked-list)
3. [Singly Linked List](#singly-linked-list)
4. [Circular Linked List](#circular-linked-list)
5. [Doubly Linked List](#doubly-linked-list)
6. [Comparison](#comparison)
7. [Known Issues and Limitations](#known-issues-and-limitations)

---

## Files

| File                       | Class                  | Description                                          |
| -------------------------- | ---------------------- | ---------------------------------------------------- |
| `LinkedList.h`             | `Linked_List`          | Singly linked list                                   |
| `Circular_Linked_List.h`   | `Circular_Linked_List` | Singly linked list whose last node points to the head |
| `Doubly_Linked_List.h`     | `Doubly_Linked_List`   | Every node points to both its next and previous node |

All three are header-only and store `int` values. **Each header defines its own `Node` class, so include only one of them in the same source file.**

Most functions report problems by printing a message (for example `List Is Empty.` or `Value Isn't Found.`) instead of returning an error code.

---

## What Is a Linked List?

A linked list is a chain of **nodes**. Each node holds a value and a pointer to the next node. The list itself only remembers one pointer, `head`, which points to the first node. To reach any other node we must start at `head` and follow the pointers one by one.

```cpp
class Node
{
public:
    int Data;
    Node *Next;
};
```

### Linked list vs array

| | Array / `Vector` | Linked list |
| --- | --- | --- |
| Memory layout | One contiguous block | Nodes scattered in the heap |
| Access by index | O(1) | O(n), we must walk from `head` |
| Insert or delete at the front | O(n), everything shifts | O(1) |
| Insert or delete in the middle | O(n), everything shifts | O(1) once we hold the pointer, but finding it is O(n) |
| Growing | Allocate a bigger block and copy everything | Allocate one more node |
| Extra memory per element | None | One or two pointers |
| CPU cache | Very friendly (neighbors in memory) | Unfriendly (nodes can be far apart) |

The memory cost is easy to see with the padding rules from `05_memory_alignment_and_padding`. On a 64-bit machine a node with an `int` (4 bytes, plus 4 bytes of padding) and one pointer (8 bytes) takes **16 bytes**, four times the size of the `int` it stores. A doubly linked node has two pointers and takes **24 bytes**.

---

## Singly Linked List

Each node points only to the next node, and the last node points to `nullptr`.

```
head
 |
 v
[ 5 | * ]--->[ 10 | * ]--->[ 15 | * ]--->[ 20 | null ]
```

### Usage

```cpp
#include "LinkedList.h"

int main()
{
    Linked_List l;
    l.AddToTail(10);
    l.AddToTail(20);
    l.AddToHead(5);        // 5 10 20
    l.AddAfter(10, 15);    // 5 10 15 20
    l.printList();

    l.DelAtVal(10);        // 5 15 20
    l.printList();
    cout << l.getCount() << '\n';
}
```

Output:

```
5 10 15 20
5 15 20
3
Node Has Been Deleted.
```

The last line comes from the destructor, which frees every node and prints a message.

### Constructors and destructor

| Function | Description | Time |
| --- | --- | --- |
| `Linked_List()` | Creates an empty list (`head = nullptr`) | O(1) |
| `~Linked_List()` | Walks the list and deletes every node, then prints `Node Has Been Deleted.` | O(n) |

### Functions reference

Let `n` be the number of nodes. Positions are **1-based**. No function allocates extra memory proportional to `n`, so the extra space is O(1) everywhere.

**Utilities**

| Function | Description | Time |
| --- | --- | --- |
| `isEmpty()` | Prints whether the list is empty | O(1) |
| `getCount()` | Returns the number of nodes (there is no stored size, so it counts them) | O(n) |
| `printList()` | Prints all values separated by spaces | O(n) |

**Adding**

| Function | Description | Time |
| --- | --- | --- |
| `AddToHead(val)` | New node becomes the head | O(1) |
| `AddToTail(val)` | Walks to the last node and links the new one after it | O(n) |
| `AddBefore(val, data)` | Inserts `data` before the first node whose value is `val` | O(n) |
| `AddAfter(val, data)` | Inserts `data` after the first node whose value is `val` | O(n) |
| `AddAtMid(data)` | Inserts so that the new node sits at position `(n + 1) / 2` | O(n) |

**Removing**

| Function | Description | Time |
| --- | --- | --- |
| `DeleteFromHead()` | Removes the first node | O(1) |
| `DeleteFromNTail()` | Removes the last node, tracking the previous node while walking | O(n) |
| `DeleteFromTail2()` | Removes the last node, stopping at the second-to-last node instead | O(n) |
| `DelAtPos(pos)` | Removes the node at position `pos` | O(n), O(1) when `pos == 1` |
| `DelAfter(val)` | Removes the node that comes right after the first node with value `val` | O(n) |
| `DelAtVal(val)` | Removes the first node whose value is `val` | O(n) |
| `DelAtMid()` | Removes the node at position `(n + 1) / 2` | O(n) |

**Updating**

| Function | Description | Time |
| --- | --- | --- |
| `Update(pos, val)` | Overwrites the value of the node at position `pos` | O(n) |

### Notes

**Two ways to delete the tail.** `DeleteFromNTail` and `DeleteFromTail2` do the same job. The first keeps two pointers, `curr` and `pre`, and when `curr` reaches the last node it unlinks it through `pre`. The second keeps one pointer and looks two steps ahead (`curr->Next->Next`), so it stops at the second-to-last node and deletes the one after it. Both are O(n), because a singly linked list cannot step backwards.

**Why `AddToTail` is O(n).** The list only stores `head`, so to find the last node we must walk the whole list. Keeping a second pointer, `tail`, would make it O(1) (this is exactly what the linked list queue in `06_queue` does).

**The middle node.** For `n` nodes the middle position is `(n + 1) / 2` using integer division. For 5 nodes it is 3, and for 4 nodes it is 2 (the first of the two middle nodes).

**Special cases.** Almost every function handles the head separately, because the head is the only node that no other node points to. Deleting or inserting at the head changes `head` itself, while every other node is changed through its predecessor's `Next`.

---

## Circular Linked List

Here the last node points back to the head instead of `nullptr`, so the list forms a ring.

```
head
 |
 v
[ 0 | * ]--->[ 1 | * ]--->[ 2 | * ]--->[ 3 | * ]
   ^                                       |
   +---------------------------------------+
```

There is no `nullptr` at the end, so the loops change. A normal list walks `while (curr != nullptr)`, and a circular list walks until it comes back to where it started, `while (curr != head)`. A list with one node points to itself (`head->next == head`), and almost every function checks for that case first.

### Usage

```cpp
#include "Circular_Linked_List.h"

int main()
{
    Circular_Linked_List c;
    c.AddToTail(1);
    c.AddToTail(2);
    c.AddToTail(3);
    c.AddToHead(0);        // 0 1 2 3
    c.PrintList();

    c.InsertAfter(2, 9);   // 0 1 2 9 3
    c.PrintList();

    c.DeleteNode(1);       // 0 2 9 3
    c.PrintList();
    c.getCount();
}
```

Output:

```
0 1 2 3
0 1 2 9 3
0 2 9 3
List Size Is : 4
List Has Been Deleted
```

### Functions reference

| Function | Description | Time |
| --- | --- | --- |
| `Circular_Linked_List()` | Creates an empty list | O(1) |
| `~Circular_Linked_List()` | Deletes every node, then prints `List Has Been Deleted` | O(n) |
| `IsEmpty()` | Returns `true` if the list has no nodes | O(1) |
| `getCount()` | Prints `List Size Is : n` | O(n) |
| `PrintList()` | Prints all values starting from `head` | O(n) |
| `AddToHead(val)` | Adds a node and makes it the new head | O(n) |
| `AddToTail(val)` | Adds a node just before the head | O(n) |
| `InsertAfter(val, data)` | Inserts `data` after the first node whose value is `val` | O(n) |
| `DeleteFromHead()` | Removes the head, the next node becomes the head | O(n) |
| `DeleteFromTail()` | Removes the last node | O(n) |
| `DeleteNode(val)` | Removes the first node whose value is `val` | O(n) |

### Notes

**Why adding or deleting at the head is O(n).** The last node must always point to the head. When the head changes, the last node has to be updated too, and the list only stores `head`, so the only way to reach the last node is to walk the whole ring. If the list stored a `tail` pointer instead (with `tail->next` being the head), `AddToHead`, `AddToTail` and `DeleteFromHead` would all become O(1).

**Where a circular list is useful.** Anything that cycles: round-robin scheduling between processes, turn-taking in games, repeating playlists, and circular buffers.

---

## Doubly Linked List

Every node stores two pointers, `next` and `prev`, so we can move in both directions. The `prev` of the first node and the `next` of the last node are `nullptr`.

```
head
 |
 v
null <-[ 0 ]<->[ 1 ]<->[ 2 ]<->[ 3 ]-> null
```

### Usage

```cpp
#include "Doubly_Linked_List.h"

int main()
{
    Doubly_Linked_List d;
    d.AddToTail(1);
    d.AddToTail(2);
    d.AddToTail(3);
    d.AddToHead(0);        // 0 1 2 3
    d.printList();

    d.InsertAfter(1, 7);   // 0 1 7 2 3
    d.printList();

    d.DeleteAt(2);         // 0 1 7 3
    d.printList();
    d.FindNode(3);
}
```

Output:

```
0 1 2 3
0 1 7 2 3
0 1 7 3
Value Is Found, Value's Position Is : 4
Node Has Been Deleted.
```

### Functions reference

| Function | Description | Time |
| --- | --- | --- |
| `Doubly_Linked_List()` | Creates an empty list | O(1) |
| `~Doubly_Linked_List()` | Deletes every node, then prints `Node Has Been Deleted.` | O(n) |
| `IsEmpty()` | Returns `true` if the list has no nodes | O(1) |
| `FindNode(val)` | Prints the 1-based position of the first node with value `val` | O(n) |
| `printList()` | Prints all values from head to tail | O(n) |
| `AddToHead(val)` | New node becomes the head | O(1) |
| `AddToTail(val)` | Walks to the last node and links the new one after it | O(n) |
| `InsertAfter(val, data)` | Inserts `data` after the first node whose value is `val` | O(n) to find, O(1) to link |
| `DeleteFromHead()` | Removes the first node | O(1) |
| `DeleteFromTail()` | Removes the last node | O(n) |
| `DeleteAt(val)` | Removes the first node whose value is `val` (by value, not by position) | O(n) to find, O(1) to unlink |
| `reverse()` | Swaps `next` and `prev` in every node | O(n) |

### Notes

**What the extra pointer buys us.** Once we hold a pointer to a node, we can remove it without searching for its predecessor, because the node already knows it:

```cpp
curr->prev->next = curr->next;
curr->next->prev = curr->prev;
delete curr;
```

A singly linked list needs a second pointer, `pre`, tracked during the walk, to do the same thing. The price is an extra 8 bytes per node and more pointers to keep consistent: every insert and delete must update both directions, and forgetting one leaves a broken list.

**`AddToTail` and `DeleteFromTail` are still O(n).** The list stores only `head`, so reaching the tail means walking the list. With a `tail` pointer, both would be O(1), and the `prev` pointer would let `DeleteFromTail` find the new last node immediately.

**The idea behind `reverse()`.** Reversing a doubly linked list does not need to move any data. Every node swaps its `next` and `prev`, and the old tail becomes the new head. (See the known issue below about the last step.)

---

## Comparison

| | Singly | Circular | Doubly |
| --- | --- | --- | --- |
| Pointers per node | 1 | 1 | 2 |
| Node size (64-bit, `int` data) | 16 bytes | 16 bytes | 24 bytes |
| End of the list | `nullptr` | Points back to `head` | `nullptr` at both ends |
| `AddToHead` | O(1) | O(n) | O(1) |
| `AddToTail` | O(n) | O(n) | O(n) |
| `DeleteFromHead` | O(1) | O(n) | O(1) |
| `DeleteFromTail` | O(n) | O(n) | O(n) |
| Walk backwards | No | No | Yes |
| Delete a node when you already hold a pointer to it | Needs the previous node | Needs the previous node | O(1) |
| Typical use | Stacks, simple lists | Round-robin, buffers | Editors, undo/redo, browsers' back and forward |

All three share the same structure: `n` nodes, O(1) extra space per operation, and O(n) to look up anything by value. Storing a `tail` pointer would make adding at the tail O(1) in all three lists. Removing the tail would only become O(1) in the doubly linked list, because it is the only one where the last node can reach its predecessor.

---

## Known Issues and Limitations

These are confirmed behaviors of the current code.

**`Linked_List`**

- `Update(pos, val)` does not validate `pos`. A position below 1 skips the loop and overwrites the **first** node, while `DelAtPos` rejects it with `Invalid Position.`.

**`Circular_Linked_List`**

- **The destructor assumes the list is not empty.** It reads `head->next` first, so destroying a list that is empty (never filled, or whose nodes were all deleted) dereferences a null pointer and crashes. Always keep at least one node, or guard the destructor with `if (head == nullptr)`.
- **`InsertAfter` leaks a node when the value is not found.** The new node is allocated before the search, and it is not deleted when the search fails.

**`Doubly_Linked_List`**

- **`reverse()` does not update `head`.** After the loop, `temp` is always `nullptr`, so the line `if (temp != nullptr) head = temp->prev;` never runs. `head` still points to the old first node, which is now the last one, so the list shows a single element and the rest becomes unreachable:

  ```
  list:           1 2 3 4 5
  after reverse:  1
  ```

  The fix is to remember the last node visited inside the loop and assign it to `head` after the loop.

**All three classes**

- **No copy constructor and no copy assignment.** Copying a list copies only the `head` pointer, so both lists point to the same nodes and both destructors delete them (a double free). Pass lists by reference and never copy them.
- **No stored size.** `getCount` has to walk the list. A `size` member would make it O(1).
- **Output is printed, not returned.** Functions like `getCount` (circular) and `FindNode` print their result, which makes them hard to reuse from other code.