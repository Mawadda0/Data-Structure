# Data Structures in C++

> A comprehensive, from-scratch implementation of fundamental data structures in C++. This repository is designed for deep learning, focusing on raw memory management, algorithmic efficiency, and clean, readable code. Every structure is built without relying on the C++ Standard Template Library (STL) containers, relying instead on raw pointers, dynamic arrays, and custom nodes.

## Table of Contents

1. [About The Project](#about-the-project)
2. [Linear Data Structures](#linear-data-structures)
3. [Non-Linear Data Structures](#non-linear-data-structures)
4. [Memory & Core Concepts](#memory--core-concepts)
5. [Global Utilities](#global-utilities)
6. [Design Philosophy](#design-philosophy)

---

## About The Project

This repository serves as both a library and an educational resource. Rather than dumping all files into a single directory, the project is highly modular. **Every major data structure has its own dedicated folder**, complete with its implementation header files and a specific `README.md` that explains the theory, time complexity, and usage of that structure.

---

## Linear Data Structures

These structures organize data in a sequential manner. Elements are accessed in a specific, predictable order (e.g., First-In-First-Out, Last-In-First-Out, or by index).

| Folder | Implementations | Description |
| :--- | :--- | :--- |
| **`Vector/`** | `Vector.hpp` | A dynamically resizing array that automatically manages its own memory block. |
| **`Linked_List/`** | `Linked_List.hpp`<br>`Doubly_Linked_List.hpp`<br>`Circular_Linked_List.hpp` | Node-based sequences where each element points to the next (and optionally the previous or the head). |
| **`Stack/`** | `Stack_Array.hpp`<br>`Stack_Linked_List.hpp`<br>`Applications_on_Stack.cpp` | LIFO (Last-In, First-Out) data structure. Implemented using both fixed/dynamic arrays and linked nodes. Includes practical algorithmic applications. |
| **`Queue/`** | `Queue_Linear.hpp`<br>`Queue_Circular.hpp`<br>`Queue_Linked_List.hpp` | FIFO (First-In, First-Out) data structure. Features a standard linear queue, a memory-efficient circular queue, and a dynamic linked queue. |
| **`Deque/`** | `Deque.hpp` | A Double-Ended Queue allowing insertion and deletion from both the front and the back in O(1) time. |

---

## Non-Linear Data Structures

These structures organize data hierarchically, allowing for more complex relationships and vastly improved search/retrieval times compared to linear structures.

| Folder | Implementations | Description |
| :--- | :--- | :--- |
| **`Trees/`** | `Binary_Tree/`<br>`Binary_Search_Tree/`<br>`Binary_Heap/` | **[Master Folder]** Contains sub-folders for unordered Binary Trees, ordered Binary Search Trees (BST), and array-based Priority Queues (Min/Max Heaps). See the `Trees/README.md` for a full breakdown. |

---

## Memory & Core Concepts

Understanding data structures requires a solid grasp of how memory works under the hood. The **`Memory/`** folder acts as an educational handbook containing markdown notes on C++ memory architecture.

| Concept Note | Description |
| :--- | :--- |
| `1_pointers_and_dynamic_memory.md` | The fundamentals of raw pointers, addresses, and dynamic allocation. |
| `2_stack_vs_heap_memory_allocation.md` | Architectural differences between the Call Stack and the Heap. |
| `3_how_to_allocate_dynamic_memory.md` | Practical guide to using `new` and `delete`. |
| `4_pass_by_value_vs_pass_by_reference.md` | How data is passed to functions and the cost of copying. |
| `5_memory_alignment_and_padding.md` | Advanced hardware-level memory packing and struct alignment. |

---

## Global Utilities

To keep the implementations clean and focused strictly on the data structure logic, the repository uses a shared configuration file at the root.

| File | Description |
| :--- | :--- |
| `Template.hpp` | A global header included by every implementation in this repository. It contains essential C++ standard library includes (like `<iostream>`, `<vector>`, `<queue>`), `using namespace std;`, and helpful macros like `#define nl '\n'`. |

---

## Design Philosophy

- **No STL Containers:** With the deliberate exception of building the Heap (which sits on a `std::vector`), all core data structures are built entirely from scratch to demonstrate how things like `std::vector` or `std::list` actually work under the hood.
- **Deep Copying & Memory Safety:** All pointer-based classes feature custom Destructors, Copy Constructors, and Assignment Operators (The Rule of Three) to prevent memory leaks and shallow-copy double-free crashes.
- **Unified Interface:** Operations are named consistently across structures (e.g., `push()`, `pop()`, `size()`, `empty()`, `clear()`, `display()`), making it easy to swap one structure for another when testing algorithms.
- **Detailed Documentation:** Every folder contains a comprehensive `README.md` detailing the "Why" and "How" of the structure, not just the code.

> **Dive In:** Navigate to any of the folders above and read their specific `README.md` to get started!