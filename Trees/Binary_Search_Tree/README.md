# Binary Search Tree in C++

> A from-scratch binary search tree built with nodes and raw pointers: every value is placed by comparing it with the values already in the tree, so searching, inserting, and deleting only follow one path from the root. It supports duplicates, the four classic traversals, and the basic measurements (height, number of nodes, number of leaves, minimum, maximum).

## Table of Contents

1. [Files and Usage](#files-and-usage)
2. [What Is a Binary Search Tree?](#what-is-a-binary-search-tree)
3. [Searching](#searching)
4. [Inserting](#inserting)
5. [Deleting](#deleting)
6. [Traversals](#traversals)
7. [Measurements](#measurements)
8. [Constructors, Destructor, and Copying](#constructors-destructor-and-copying)
9. [Functions Reference](#functions-reference)
10. [Why the Height Matters](#why-the-height-matters)
11. [Design Notes and Limitations](#design-notes-and-limitations)

---

## Files and Usage

| File                     | Description                                   |
| ------------------------ | --------------------------------------------- |
| `Binary_Search_Tree.hpp` | The class templates `BST<T>` and `Node<T>`    |
| `README.md`              | This document                                 |

Every header starts with `#include "Template.hpp"`, the shared header of the repository that provides the standard library includes, `using namespace std;` and the `nl` macro (a newline). This is why the examples use `cout` and `nl` without the `std::` prefix.

```cpp
#include "Binary_Search_Tree.hpp"

int main()
{
    BST<int> t;
    for(int x : {50, 30, 70, 20, 40, 60, 80})
        t.insert(x);

    t.display_inOrder();         // 20 30 40 50 60 70 80
    t.display_preOrder();        // 50 30 20 40 70 60 80
    t.display_postOrder();       // 20 40 30 60 80 70 50
    t.display_LevelByLevel();    // 50 30 70 20 40 60 80

    cout << t.TreeHeight() << " " << t.TreeCount() << " " << t.LeavesCount() << nl;   // 3 7 4
    cout << t.TreeMin() << " " << t.TreeMax() << nl;                                  // 20 80
    cout << t.TreeSearch(40) << t.TreeSearch(90) << nl;                               // 10
}
```

Output:

```
20 30 40 50 60 70 80
50 30 20 40 70 60 80
20 40 30 60 80 70 50
50 30 70 20 40 60 80
3 7 4
20 80
10
```

### What `T` must support

| Needed by | Requirement on `T` |
| --- | --- |
| The whole tree | Default-constructible and copy-assignable (a node first creates `Data`, then assigns the value to it) |
| `insert`, `Delete`, `TreeSearch` | `operator<` only. Equality is never used: two values are equal when neither is smaller than the other |
| The four `display_` functions | `operator<<` for `ostream` |
| `TreeMin`, `TreeMax` | Default-constructible (an empty tree returns `T()`) |

---

## What Is a Binary Search Tree?

A binary search tree (BST) is a binary tree with an **ordering rule**. For every node:

- every value in its **left** subtree is **smaller** than the node,
- every value in its **right** subtree is **greater than or equal to** the node.

```cpp
template <typename T>
class Node
{
public:
    T Data;
    Node<T> *left;
    Node<T> *right;
};
```

The tree itself only remembers one pointer, `root`, the node at the top. A missing child is `nullptr`.

The tree built in the usage example above looks like this:

```
          50            <- root, level 1
        /    \
      30      70        <- level 2
     /  \    /  \
    20  40  60  80      <- leaves, level 3
```

Look at the node `70`: everything on its left (`60`) is smaller, everything on its right (`80`) is greater, and the whole subtree is greater than the root `50`. This rule holds at **every** node, not only at the root.

| Term | Meaning | In the example |
| --- | --- | --- |
| Root | The top node, the only one without a parent | `50` |
| Leaf | A node with no children | `20`, `40`, `60`, `80` |
| Subtree | A node together with everything below it | The subtree of `30` is `30`, `20`, `40` |
| Level | The distance from the root, counted from 1 | `50` is level 1, `30` and `70` are level 2 |
| Height | The number of levels of the tree | 3 |

Unlike a plain binary tree, where the caller chooses where each value goes, here **the values decide their own place**. This is what makes the tree useful: at every node, one comparison tells us which half of the tree can be ignored.

---

## Searching

To find a value, compare it with the current node and go to the side where it must be:

```
Search 60 in the example tree

at 50:  50 < 60, so go right
at 70:  60 < 70, so go left
at 60:  neither is smaller than the other, found
```

```cpp
bool searchValue(Node<T> *node, T item) const
{
    if(node == nullptr) return false;
    if(item < node->Data) return searchValue(node->left, item);
    if(node->Data < item) return searchValue(node->right, item);
    return true;
}
```

Only one child is visited at each step, so the cost is the number of levels it walks, which is **O(h)**. A search for a missing value ends when it reaches a `nullptr`.

`TreeMin()` is the **leftmost** node (keep going left), and `TreeMax()` is the **rightmost** node (keep going right). Both are O(h). On an empty tree they print `Tree Is Empty.` and return `T()`.

---

## Inserting

`insert(item)` follows the same path a search would take, and puts a new leaf where the search would have ended:

```
Insert 65 in the example tree

at 50:  go right
at 70:  go left
at 60:  go right, which is nullptr, so the new node goes here

          50
        /    \
      30      70
     /  \    /  \
    20  40  60  80
              \
               65
```

A new value is always added as a **leaf**; no existing node is moved. The cost is **O(h)**.

### Duplicates

Equal values go to the **right**. So `insert` accepts duplicates, and the rule "left is smaller, right is greater than or equal" stays true.

```cpp
#include "Binary_Search_Tree.hpp"

int main()
{
    BST<int> t;
    for(int x : {5, 3, 5, 8, 5})
        t.insert(x);

    t.display_inOrder();
    cout << t.TreeCount() << nl;

    t.Delete(5);                  // removes only one of the three 5s
    t.display_inOrder();
    cout << t.TreeCount() << nl;
}
```

Output:

```
3 5 5 5 8
5
3 5 5 8
4
```

---

## Deleting

`Delete(item)` removes **one** node holding `item`. If the value is not in the tree, nothing is changed. There are three cases, depending on how many children the node has:

| Case | What happens |
| --- | --- |
| **No children** (a leaf) | The node is deleted, and its parent points to `nullptr` |
| **One child** | The node is deleted, and its parent points to the child instead |
| **Two children** | The node cannot simply be removed. Its value is replaced with its **successor**, the smallest value of its right subtree, and then the successor's node is deleted |

The successor is the right choice because it is greater than everything on the left and not greater than anything on the right, so the ordering rule is kept. It is also the leftmost node of that subtree, so it has **no left child**, and deleting it is one of the two easy cases.

```
Delete 50 (two children)

        50                    60
      /    \                /    \
    30      70     ->     30      70
   /  \    /  \          /  \       \
  20  40  60  80        20  40       80

successor = smallest of the right subtree = 60
50 is replaced by 60, then the old node of 60 (a leaf) is deleted
```

```cpp
#include "Binary_Search_Tree.hpp"

int main()
{
    BST<int> t;
    for(int x : {50, 30, 70, 20, 40, 60, 80})
        t.insert(x);

    t.Delete(20);       // a leaf
    t.Delete(30);       // one child (40)
    t.Delete(50);       // two children (the root)
    t.Delete(99);       // not in the tree

    t.display_preOrder();
    t.display_inOrder();
}
```

Output:

```
Value Isn't Found.
60 40 70 80
40 60 70 80
```

| Situation | Message |
| --- | --- |
| The tree is empty | `Tree Is Empty.` |
| The value is not in the tree | `Value Isn't Found.` |

`Delete` first checks with a search, so a missing value never changes the tree. The whole operation is **O(h)**.

---

## Traversals

A traversal visits every node exactly once. There are two families.

**Depth-First (DFS)** goes as deep as possible before moving sideways. It is written with recursion, so it uses the call stack. The three versions differ only in **when the node itself is printed**, relative to its two subtrees:

| Traversal | Order | Result for the example |
| --- | --- | --- |
| `display_inOrder` | left, **root**, right | `20 30 40 50 60 70 80` |
| `display_preOrder` | **root**, left, right | `50 30 20 40 70 60 80` |
| `display_postOrder` | left, right, **root** | `20 40 30 60 80 70 50` |

```cpp
void inOrder(Node<T> *node) const
{
    if(node == nullptr) return;
    inOrder(node->left);
    cout << node->Data << " ";
    inOrder(node->right);
}
```

Moving the `cout` line to the top gives pre-order, and moving it to the bottom gives post-order.

**In-order is special in a BST: it prints the values in sorted order.** This is a direct result of the ordering rule (everything smaller first, then the node, then everything greater).

**Breadth-First (BFS)**, also called level order, visits the tree level by level, from left to right. It uses a **queue** instead of recursion: take a node from the front, print it, and push its children at the back.

```
queue: [50]                print 50   push 30, 70
queue: [30, 70]            print 30   push 20, 40
queue: [70, 20, 40]        print 70   push 60, 80
queue: [20, 40, 60, 80]    print 20, 40, 60, 80 (no children)

result: 50 30 70 20 40 60 80
```

### When each one is used

| Traversal | Typical use |
| --- | --- |
| In-order | Getting the values in sorted order |
| Pre-order | Copying a tree (parents are created before their children), saving a tree to a file and rebuilding the same shape later |
| Post-order | Deleting a tree (children are deleted before their parent), which is what the destructor does |
| Level order | Anything that works level by level |

On an empty tree every `display_` function prints `Tree Is Empty.`.

---

## Measurements

Each of these follows the same idea: solve the problem for the left subtree, solve it for the right subtree, and combine the two answers.

| Function | Rule | Example tree |
| --- | --- | --- |
| `TreeHeight()` | `0` for an empty tree, otherwise `1 + max(height of left, height of right)` | 3 |
| `TreeCount()` | `0` for an empty tree, otherwise `1 + count of left + count of right` | 7 |
| `LeavesCount()` | `0` for an empty tree, `1` for a node with no children, otherwise `leaves of left + leaves of right` | 4 |
| `TreeMin()` | The leftmost node | 20 |
| `TreeMax()` | The rightmost node | 80 |

The height counts **levels** (nodes on the longest path), so a single node has height 1 and an empty tree has height 0. Some books count edges instead, which gives a value smaller by one.

Height, count, and leaves must look at every node, so they are O(n). Min and max only follow one side, so they are O(h).

---

## Constructors, Destructor, and Copying

| Function | Description | Time |
| --- | --- | --- |
| `BST()` | Creates an empty tree (`root = nullptr`) | O(1) |
| `BST(const BST &other)` | Creates a **deep copy** of `other` | O(n) |
| `~BST()` | Deletes every node | O(n) |
| `operator=` | Replaces the contents with a deep copy of `other` | O(n) |
| `Clear()` | Deletes every node and leaves an empty tree | O(n) |

**The destructor deletes in post-order.** A node must be deleted **after** its children. If we deleted the parent first, we would lose the pointers to its children and could never delete them.

**Copying is deep.** Without a copy constructor, the compiler would copy only the `root` pointer, so two trees would share the same nodes and both destructors would delete them (a double free). Here the copy is built recursively in pre-order, creating a new node for each original node, so the two trees are independent and have the same shape.

```cpp
#include "Binary_Search_Tree.hpp"

int main()
{
    BST<int> a;
    a.insert(2);
    a.insert(1);

    BST<int> b = a;        // copy constructor: a deep copy
    b.insert(3);

    a.display_inOrder();
    b.display_inOrder();
}
```

Output:

```
1 2
1 2 3
```

`operator=` builds the new copy first and only then deletes the old nodes, and it checks `this != &other`, so `a = a` is safe.

---

## Functions Reference

Let `n` be the number of nodes and `h` the height of the tree.

### Utilities

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `IsEmpty()` | Returns `true` if the tree has no nodes | O(1) | O(1) |
| `Clear()` | Deletes every node | O(n) | O(h) |
| `TreeHeight()` | Number of levels | O(n) | O(h) |
| `TreeCount()` | Number of nodes | O(n) | O(h) |
| `LeavesCount()` | Number of nodes with no children | O(n) | O(h) |
| `TreeSearch(item)` | `true` if `item` is in the tree | O(h) | O(h) |
| `TreeMin()` | The smallest value | O(h) | O(h) |
| `TreeMax()` | The largest value | O(h) | O(h) |

### Adding and removing

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `insert(item)` | Adds `item` as a new leaf (equal values go right) | O(h) | O(h) |
| `Delete(item)` | Removes one node holding `item` | O(h) | O(h) |

### Display

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `display_inOrder()` | left, root, right (sorted order) | O(n) | O(h) |
| `display_preOrder()` | root, left, right | O(n) | O(h) |
| `display_postOrder()` | left, right, root | O(n) | O(h) |
| `display_LevelByLevel()` | Level by level, left to right | O(n) | O(w), where `w` is the width of the widest level |

The extra space for the recursive functions is the call stack, which is as deep as the tree is high. For level order it is the queue, which holds at most one level at a time.

### Private helpers

| Function | Description | Time |
| --- | --- | --- |
| `clear(node)` | Deletes a subtree in post-order | O(n) |
| `copy(node)` | Builds a copy of a subtree in pre-order | O(n) |
| `insertNode`, `deleteNode`, `searchValue` | The recursive versions of the main operations | O(h) |
| `minValue`, `maxValue` | Follow the left (or right) side to the end | O(h) |
| `inOrder`, `preOrder`, `postOrder`, `levelOrder` | The traversals themselves | O(n) |
| `height`, `countNodes`, `countLeaves` | The recursive versions of the measurements | O(n) |

---

## Why the Height Matters

Search, insert, delete, min, and max all cost **O(h)**, and `h` depends on the **shape** of the tree, which depends on the **order the values were inserted**:

| Shape | Height | Search / insert / delete |
| --- | --- | --- |
| Balanced (levels are almost full) | about log2(n) | about 20 steps for a million nodes |
| Skewed (every node has one child, like a linked list) | n | as slow as a linked list, O(n) |

```
Balanced, n = 7, h = 3          Skewed, n = 4, h = 4
(inserted 4, 2, 6, 1, 3, 5, 7)  (inserted 1, 2, 3, 4)

        4                        1
       / \                        \
      2   6                        2
     / \ / \                        \
    1  3 5  7                        3
                                      \
                                       4
```

Inserting values that are **already sorted** is the worst case: every new value is the largest so far and goes to the right, and the tree becomes a linked list. The recursion also uses one stack frame per level, so a very skewed tree with a very large number of levels can overflow the call stack.

This is the reason balanced trees exist. The AVL tree later keeps the height close to log2(n) by rebalancing after every insertion and deletion, and that is why `TreeHeight` is part of this class.

---

## Design Notes and Limitations

- **The tree is not self-balancing.** Its speed depends on the insertion order. Sorted input gives O(n) operations.
- **Duplicates are allowed** and go to the right. `Delete` removes only **one** of them, and `TreeSearch` only tells whether at least one exists.
- **Only `operator<` is used** to compare values, so a custom type does not need `==`, `>`, or any other operator.
- **`Delete` copies the successor's value** into the node instead of relinking the nodes. This is simple, but it means a pointer to a node's value may point to a different value after a deletion.
- **Messages are printed, not returned.** Errors such as `Value Isn't Found.` are printed with `cout`, and the `display_` functions print the tree instead of returning it.
- **`TreeMin()` and `TreeMax()` on an empty tree** print `Tree Is Empty.` and return `T()`, which can be confused with a real value, so check `IsEmpty()` first.
- **No move semantics.** There is no move constructor or move assignment, so returning or reassigning large trees always copies. Adding them is a natural next step (the rule of five).
- **Single-threaded only.** There is no synchronization.