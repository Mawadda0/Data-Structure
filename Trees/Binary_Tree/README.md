# Binary Tree in C++

> A from-scratch binary tree built with nodes and raw pointers: every node has at most two children, nodes are placed by giving a path from the root, and the tree supports the four classic traversals and the basic measurements (height, number of nodes, number of leaves, search).

## Table of Contents

1. [Files and Usage](#files-and-usage)
2. [What Is a Binary Tree?](#what-is-a-binary-tree)
3. [Building the Tree](#building-the-tree)
4. [Traversals](#traversals)
5. [Measurements](#measurements)
6. [Constructors, Destructor, and Copying](#constructors-destructor-and-copying)
7. [Functions Reference](#functions-reference)
8. [Why the Height Matters](#why-the-height-matters)
9. [Design Notes and Limitations](#design-notes-and-limitations)

---

## Files and Usage

| File               | Description                                           |
| ------------------ | ----------------------------------------------------- |
| `Binary_Tree.hpp`  | The class templates `Binary_Tree<T>` and `Node<T>`    |
| `README.md`        | This document                                         |

Every header starts with `#include "Template.hpp"`, the shared header of the repository that provides the standard library includes, `using namespace std;` and the `nl` macro (a newline). This is why the examples use `cout` and `nl` without the `std::` prefix.

```cpp
#include "Binary_Tree.hpp"

int main()
{
    Binary_Tree<int> t(1);
    t.addByPath({'L'}, 2);
    t.addByPath({'R'}, 3);
    t.addByPath({'L', 'L'}, 4);
    t.addByPath({'L', 'R'}, 5);
    t.addByPath({'R', 'R'}, 6);

    t.display_inOrder();         // 4 2 5 1 3 6
    t.display_preOrder();        // 1 2 4 5 3 6
    t.display_postOrder();       // 4 5 2 6 3 1
    t.display_LevelByLevel();    // 1 2 3 4 5 6

    cout << t.TreeHeight() << " " << t.TreeCount() << " " << t.LeavesCount() << nl;   // 3 6 3
    cout << t.TreeSearch(5) << t.TreeSearch(9) << nl;                                  // 10
}
```

Output:

```
4 2 5 1 3 6
1 2 4 5 3 6
4 5 2 6 3 1
1 2 3 4 5 6
3 6 3
10
```

### What `T` must support

| Needed by | Requirement on `T` |
| --- | --- |
| The whole tree | Default-constructible and copy-assignable (a node first creates `Data`, then assigns the value to it) |
| `TreeSearch` | `operator==` |
| The four `display_` functions | `operator<<` for `ostream` |

---

## What Is a Binary Tree?

A tree is made of **nodes** connected by pointers, with no cycles. In a **binary** tree every node has at most two children, called `left` and `right`. A missing child is `nullptr`.

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

The tree itself only remembers one pointer, `root`, the node at the top. Every other node is reached by following `left` and `right` pointers from the root, just as every node of a linked list is reached from `head`. A linked list is actually a tree where every node has one child.

The tree built in the usage example above looks like this:

```
          1            <- root, level 1
         / \
        2   3          <- level 2
       / \   \
      4   5   6        <- leaves, level 3
```

| Term | Meaning | In the example |
| --- | --- | --- |
| Root | The top node, the only one without a parent | `1` |
| Leaf | A node with no children | `4`, `5`, `6` |
| Subtree | A node together with everything below it | The subtree of `2` is `2`, `4`, `5` |
| Level | The distance from the root, counted from 1 | `1` is level 1, `2` and `3` are level 2 |
| Height | The number of levels of the tree | 3 |

Unlike a binary **search** tree, a binary tree has **no ordering rule** between the values. Where a value goes is decided by the caller, not by comparing it with other values.

---

## Building the Tree

Since there is no ordering, the tree needs another way to know where a new node goes. `addByPath` takes a **path**: a list of directions starting at the root, where `'L'` means go left and `'R'` means go right.

```
{'L', 'R'}   ->   from the root go left, then right   ->   the node holding 5
```

`setRoot(item)` creates the root, or overwrites its value if the tree already has one.

`addByPath(path, item)` walks the path and sets the value of the **last** node on it:

- The last node is **created** if it is missing, or its value is **overwritten** if it already exists.
- Every node **before** the last one must already exist. A node on the way would need a value, and we only have one value to give, so the function refuses instead of inventing nodes.
- Directions are not case sensitive (`'l'` and `'L'` are the same).

When something is wrong, nothing is changed and a message is printed:

| Situation | Message |
| --- | --- |
| The tree has no root yet | `Tree Is Empty.` |
| The path is empty, or contains a character other than `L` or `R` | `Invalid Path.` |
| A node before the last one does not exist | `Path Doesn't Exist.` |

```cpp
#include "Binary_Tree.hpp"

int main()
{
    Binary_Tree<int> t;
    t.addByPath({'L'}, 2);         // the tree has no root yet
    t.setRoot(1);
    t.addByPath({'L', 'L'}, 4);    // the node on the way, "L", does not exist
    t.addByPath({'X'}, 4);         // not a direction
    t.addByPath({'l'}, 2);         // lowercase works too
    t.addByPath({'L'}, 20);        // an existing node is overwritten
    t.display_preOrder();
}
```

Output:

```
Tree Is Empty.
Path Doesn't Exist.
Invalid Path.
1 20
```

Because the nodes are placed by path and not by value, building a tree is the caller's job. A binary search tree will insert by value instead and will not need paths at all.

---

## Traversals

A traversal visits every node exactly once. There are two families.

**Depth-First (DFS)** goes as deep as possible before moving sideways. It is written with recursion, so it uses the call stack. The three versions differ only in **when the node itself is printed**, relative to its two subtrees:

| Traversal | Order | Result for the example |
| --- | --- | --- |
| `display_inOrder` | left, **root**, right | `4 2 5 1 3 6` |
| `display_preOrder` | **root**, left, right | `1 2 4 5 3 6` |
| `display_postOrder` | left, right, **root** | `4 5 2 6 3 1` |

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

**Breadth-First (BFS)**, also called level order, visits the tree level by level, from left to right. It uses a **queue** instead of recursion: take a node from the front, print it, and push its children at the back.

```
queue: [1]          print 1   push 2, 3
queue: [2, 3]       print 2   push 4, 5
queue: [3, 4, 5]    print 3   push 6
queue: [4, 5, 6]    print 4
queue: [5, 6]       print 5
queue: [6]          print 6

result: 1 2 3 4 5 6
```

### When each one is used

| Traversal | Typical use |
| --- | --- |
| In-order | On a binary search tree it visits the values in sorted order |
| Pre-order | Copying a tree (parents are created before their children), saving a tree to a file |
| Post-order | Deleting a tree (children are deleted before their parent), which is what the destructor does |
| Level order | Anything that works level by level, such as the array layout of a heap |

On an empty tree every `display_` function prints `Tree Is Empty.`.

---

## Measurements

Each of these follows the same idea: solve the problem for the left subtree, solve it for the right subtree, and combine the two answers.

| Function | Rule | Example tree |
| --- | --- | --- |
| `TreeHeight()` | `0` for an empty tree, otherwise `1 + max(height of left, height of right)` | 3 |
| `TreeCount()` | `0` for an empty tree, otherwise `1 + count of left + count of right` | 6 |
| `LeavesCount()` | `0` for an empty tree, `1` for a node with no children, otherwise `leaves of left + leaves of right` | 3 |
| `TreeSearch(item)` | `true` if the root holds `item`, otherwise search the left subtree, then the right subtree | `5` found, `9` not found |

The height counts **levels** (nodes on the longest path), so a single node has height 1 and an empty tree has height 0. Some books count edges instead, which gives a value smaller by one.

`TreeSearch` stops as soon as it finds the value, but when the value is missing it must look at every node. Without an ordering there is no shortcut, so it is O(n). A binary search tree makes this O(h) by deciding at each node whether to go left or right.

---

## Constructors, Destructor, and Copying

| Function | Description | Time |
| --- | --- | --- |
| `Binary_Tree()` | Creates an empty tree (`root = nullptr`) | O(1) |
| `Binary_Tree(T rootVal)` | Creates a tree with one node, the root | O(1) |
| `Binary_Tree(const Binary_Tree &other)` | Creates a **deep copy** of `other` | O(n) |
| `~Binary_Tree()` | Deletes every node | O(n) |
| `operator=` | Replaces the contents with a deep copy of `other` | O(n) |
| `Clear()` | Deletes every node and leaves an empty tree | O(n) |

**The destructor deletes in post-order.** A node must be deleted **after** its children. If we deleted the parent first, we would lose the pointers to its children and could never delete them.

**Copying is deep.** Without a copy constructor, the compiler would copy only the `root` pointer, so two trees would share the same nodes and both destructors would delete them (a double free). Here the copy is built recursively in pre-order, creating a new node for each original node, so the two trees are independent.

```cpp
#include "Binary_Tree.hpp"

int main()
{
    Binary_Tree<int> a(1);
    a.addByPath({'L'}, 2);

    Binary_Tree<int> b = a;        // copy constructor: a deep copy
    b.addByPath({'R'}, 3);

    a.display_preOrder();
    b.display_preOrder();
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

Let `n` be the number of nodes, `h` the height of the tree, and `L` the length of a path.

### Utilities

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `IsEmpty()` | Returns `true` if the tree has no nodes | O(1) | O(1) |
| `Clear()` | Deletes every node | O(n) | O(h) |
| `TreeHeight()` | Number of levels | O(n) | O(h) |
| `TreeCount()` | Number of nodes | O(n) | O(h) |
| `LeavesCount()` | Number of nodes with no children | O(n) | O(h) |
| `TreeSearch(item)` | `true` if `item` is in the tree | O(n) | O(h) |

### Building

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `setRoot(item)` | Creates the root, or overwrites its value | O(1) | O(1) |
| `addByPath(path, item)` | Sets the value of the last node on the path, creating it if missing | O(L), and `L <= h` for a valid path | O(1) |

### Display

| Function | Description | Time | Extra space |
| --- | --- | --- | --- |
| `display_inOrder()` | left, root, right | O(n) | O(h) |
| `display_preOrder()` | root, left, right | O(n) | O(h) |
| `display_postOrder()` | left, right, root | O(n) | O(h) |
| `display_LevelByLevel()` | Level by level, left to right | O(n) | O(w), where `w` is the width of the widest level |

The extra space for the DFS functions is the call stack, which is as deep as the tree is high. For level order it is the queue, which holds at most one level at a time.

### Private helpers

| Function | Description | Time |
| --- | --- | --- |
| `clear(node)` | Deletes a subtree in post-order | O(n) |
| `copy(node)` | Builds a copy of a subtree in pre-order | O(n) |
| `inOrder`, `preOrder`, `postOrder`, `levelOrder` | The traversals themselves | O(n) |
| `height`, `countNodes`, `countLeaves`, `searchValue` | The recursive versions of the measurements | O(n) |

---

## Why the Height Matters

Every function that visits the whole tree takes O(n) time, but the **recursion depth** and the cost of `addByPath` depend on the height `h`, and `h` depends on the **shape** of the tree:

| Shape | Height | Recursion depth |
| --- | --- | --- |
| Balanced (levels are almost full) | about log2(n) | small: 20 levels for a million nodes |
| Skewed (every node has one child, like a linked list) | n | as large as the number of nodes |

```
Balanced, n = 7, h = 3          Skewed, n = 4, h = 4

        4                        1
       / \                        \
      2   6                        2
     / \ / \                        \
    1  3 5  7                        3
                                      \
                                       4
```

A skewed tree with 20,000 levels works with this implementation (it was tested), but the recursion uses one stack frame per level, so a skewed tree with a very large number of levels can overflow the call stack.

This is the reason balanced trees exist. The AVL tree later keeps the height close to log2(n) by rebalancing after every insertion, and that is why `TreeHeight` is part of this class.

---

## Design Notes and Limitations

- **The values are not ordered**, so searching is O(n). A binary search tree fixes this.
- **Nodes are placed only by path.** There is no insertion by value, and the nodes on the way to the target must already exist.
- **`addByPath` overwrites an existing last node.** Use `TreeSearch` first if you want to avoid replacing a value.
- **Messages are printed, not returned.** Errors such as `Invalid Path.` are printed with `cout`, and the `display_` functions print the tree instead of returning it.
- **Left out on purpose.** Functions such as the maximum value, the left and right boundaries, the diameter, and the perfect-tree check are algorithms that run **on top of** a tree. They are not operations of the data structure itself, so they are not part of this class.
- **`Binary_Tree(T rootVal)` is not `explicit`.** That means `Binary_Tree<int> t = 5;` compiles and builds a tree whose root is 5. Marking it `explicit` prevents such accidental conversions.
- **No move semantics.** There is no move constructor or move assignment, so returning or reassigning large trees always copies. Adding them is a natural next step (the rule of five).
- **Single-threaded only.** There is no synchronization.