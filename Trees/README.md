# Trees in C++

> A collection of three fundamental tree data structures implemented from scratch in C++. This repository is divided into three folders, each containing a different type of tree. Together, they demonstrate how adding specific rules to a basic tree changes its behavior, use cases, and underlying memory structure.

## Table of Contents

1. [Repository Structure](#repository-structure)
2. [Which Tree Should I Use?](#which-tree-should-i-use)
3. [1. The Binary Tree (Unordered)](#1-the-binary-tree-unordered)
4. [2. The Binary Search Tree (Ordered)](#2-the-binary-search-tree-ordered)
5. [3. The Binary Heap (Priority)](#3-the-binary-heap-priority)
6. [Common Features and Traversals](#common-features-and-traversals)

---

## Repository Structure

The project is organized into three distinct folders. Each folder contains the header file for the data structure and a dedicated `README.md` with in-depth explanations and usage examples.

| Folder | Contents | Data Structure |
| --- | --- | --- |
| `Binary_Tree/` | `Binary_Tree.hpp`, `README.md` | A basic, un-ordered tree built with nodes and pointers. |
| `Binary_Search_Tree/` | `BinarySearchTree.hpp`, `README.md` | A value-ordered tree built with nodes and pointers. |
| `Binary_Heap/` | `BINARYHEAPTREE.HPP`, `README.md` | A complete, priority-ordered tree built on a `std::vector`. |

*Note: All implementations share a dependency on a global `Template.hpp` for standard library inclusions.*

---

## Which Tree Should I Use?

The choice of tree depends entirely on the problem you are trying to solve. Here is a quick comparison of how the three structures manage their data:

| Feature | Binary Tree | Binary Search Tree (BST) | Binary Heap |
| --- | --- | --- | --- |
| **Primary Goal** | Representing hierarchical data exactly as specified. | Fast searching, inserting, and deleting of arbitrary values. | Instantly finding the absolute maximum or minimum value. |
| **Ordering Rule** | None. | `Left < Root <= Right` | `Parent is better than Children` |
| **Insertion** | By exact path (e.g., Left -> Right). | By value (auto-places itself). | By value (auto-places itself). |
| **Search Time** | $\mathcal{O}(n)$ | $\mathcal{O}(h)$ (Fast) | $\mathcal{O}(n)$ (Not supported/intended) |
| **Underlying Memory** | `Node` objects + raw pointers | `Node` objects + raw pointers | Flat array (`std::vector`) |
| **Shape** | Can be anything (even a linked list). | Can be anything (depends on insertion order). | **Always** a complete, perfectly balanced tree. |

---

## 1. The Binary Tree (Unordered)

**Folder:** `Binary_Tree/`

The most basic form of a tree. Every node has up to two children (left and right). Because there is no mathematical rule governing where values go, this structure relies on the user to build the tree manually using directional paths.

*   **How it works:** You insert data by providing a path from the root, such as `{'L', 'R'}` (go left, then right). 
*   **Best for:** When the structure of the tree matters more than the values inside it (e.g., parsing a mathematical expression, building a decision tree, or representing a file system).
*   **Limitation:** Searching for a value requires checking every single node ($\mathcal{O}(n)$).

---

## 2. The Binary Search Tree (Ordered)

**Folder:** `Binary_Search_Tree/`

A binary tree that introduces a strict **ordering rule**: for any given node, all values in its left subtree are strictly smaller, and all values in its right subtree are greater than or equal to it (duplicates are allowed on the right).

*   **How it works:** You simply `insert(value)`. The tree compares the value against the root and walks down the correct path to find the exact spot it belongs.
*   **Best for:** Maintaining a sorted collection of data where you need to frequently search for, insert, or delete specific items efficiently. 
*   **Limitation:** If values are inserted in sorted order (e.g., 1, 2, 3, 4), the tree becomes skewed like a linked list, degrading its performance from $\mathcal{O}(\log n)$ to $\mathcal{O}(n)$.

---

## 3. The Binary Heap (Priority)

**Folder:** `Binary_Heap/`

A highly specialized tree designed to act as a **Priority Queue**. It enforces a strict **shape rule** (it must be filled level-by-level with no gaps) and a localized **order rule** (parents are always smaller/larger than their children, depending on if it's a Min or Max heap).

*   **How it works:** Because there are no gaps, it doesn't need pointers. The entire tree is flattened into a `std::vector`. The parent-child relationships are calculated using simple math (e.g., left child is at index `2i + 1`).
*   **Best for:** Situations where you constantly need the "best" item next, such as pathfinding algorithms (Dijkstra's, A*), task scheduling, or heap sort.
*   **Limitation:** It only cares about the top item. Finding or removing an arbitrary item buried deep in the heap is highly inefficient.

---

## Common Features and Traversals

Despite their differences, all three implementations share a consistent, user-friendly interface.

### Display and Traversals
All three trees support viewing their contents, though the methods differ based on their underlying architecture:

| Traversal | Order | Binary & BST | Heap |
| --- | --- | --- | --- |
| **In-Order** | Left, Root, Right | Supported | N/A (Doesn't make sense for heaps) |
| **Pre-Order** | Root, Left, Right | Supported | N/A |
| **Post-Order** | Left, Right, Root | Supported | N/A |
| **Level-Order** | Level by Level (BFS) | Supported (Uses a `queue`) | Supported (Just prints the array) |

### Memory Management
*   **Pointer-Based Trees (Binary & BST):** These utilize raw pointers (`Node*`). Therefore, they fully implement custom Destructors, Copy Constructors, and Copy Assignment operators (`operator=`) to ensure deep copies and prevent memory leaks. They clean up their nodes using Post-Order traversal.
*   **Vector-Based Tree (Heap):** Because it uses `std::vector`, memory management is entirely automated by the C++ Standard Library. No custom destructors or copy constructors are required.

> **Explore Further:** Navigate into any of the three folders and read their respective `README.md` files for full function references, time complexities, and implementation details.