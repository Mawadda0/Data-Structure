# How to Allocate Dynamic Memory

> Using `new` and `delete` in C++: creating space on the heap, reaching it through a pointer, freeing it, and resizing a dynamic array.

## Table of Contents

1. [Two Ways to Allocate Memory](#two-ways-to-allocate-memory)
2. [The Two Steps of Dynamic Allocation](#the-two-steps-of-dynamic-allocation)
3. [Allocating Space with `new`](#allocating-space-with-new)
4. [Accessing Dynamically Created Space](#accessing-dynamically-created-space)
5. [Deallocation with `delete`](#deallocation-with-delete)
6. [What If We Forget to Deallocate?](#what-if-we-forget-to-deallocate)
7. [Example: Resizing a Dynamic Array](#example-resizing-a-dynamic-array)

---

## Two Ways to Allocate Memory

There are two ways memory gets allocated for storing data.

**1. Compile-time (static) allocation**

- The compiler allocates memory for named variables.
- The exact size and type of the storage must be known at compile time.
- This is why the size of a standard array declaration has to be a constant.

**2. Dynamic allocation**

- Memory is allocated "on the fly" while the program is running.
- The space is usually taken from a part of memory called the **heap** (also called the **free store**).
- The compiler does not need to know the exact amount of space, or the number of items, in advance.
- Pointers are essential here, as we will see next.

---

## The Two Steps of Dynamic Allocation

We can allocate storage while the program is running, but we **cannot create new variable names on the fly**. A variable name is something the compiler must know before the program runs.

So dynamic allocation always takes two steps:

1. Create the dynamic space.
2. Store its **address** in a **pointer**, so we can reach the space later.

To allocate memory dynamically in C++ we use the **`new`** operator, and to free it we use the **`delete`** operator.

Deallocation means cleaning up the space used by variables or other data storage. Compile-time variables are deallocated automatically when they go out of scope. For dynamically created space, it is the **programmer's job**.

---

## Allocating Space with `new`

We write `new` followed by the type we want to allocate:

```cpp
new int;       // allocates one int
new double;    // allocates one double
```

To create an array, we put the size in brackets after the type. The size can be a variable:

```cpp
new int[40];        // an array of 40 ints
new double[size];   // an array of `size` doubles
```

By themselves these statements are not useful, because the allocated space has no name. But `new` returns the **starting address** of the space it allocated, so we can store that address in a pointer:

```cpp
int* p;
p = new int;         // allocate an int and store its address in p

double* d;
d = new double;      // allocate a double and store its address in d

// or in a single line
int x = 40;
int* list = new int[x];
float* numbers = new float[x + 10];
```

This is one more way of initializing a pointer to a valid target, and it is the most important one.

---

## Accessing Dynamically Created Space

Once the space is allocated, how do we use it?

**Single items:** we go through the pointer, and dereference it to reach the value.

```cpp
int* p = new int;   // a dynamic int, pointed to by p

*p = 10;            // assigns 10 to the dynamic int
cout << *p;         // prints 10
```

**Arrays:** we can treat the pointer as the array name and use bracket notation, or we can use pointer-offset notation.

```cpp
double* numList = new double[size];

for (int i = 0; i < size; i++)
    numList[i] = 0;          // initialize all elements to 0

numList[5] = 20;             // bracket notation
*(numList + 7) = 15;         // pointer-offset notation, same as numList[7]
```

---

## Deallocation with `delete`

To free memory that was created with `new`, we use `delete` on a pointer that holds the address of that memory:

```cpp
int* ptr = new int;
// ...
delete ptr;          // frees the int that ptr points to
```

Notice that the pointer `ptr` **still exists** after `delete`. It is a normal named variable, so its lifetime is decided at compile time by its scope. Only the memory it pointed to was freed. That means we can reuse it:

```cpp
ptr = new int[10];   // now points to a brand new array
```

To free a dynamic array, we must use the bracket form `delete[]`:

```cpp
int* list = new int[40];
// ...
delete[] list;       // frees the whole array
list = nullptr;      // reset the pointer
```

Two rules to remember:

- Memory allocated with `new` is freed with `delete`, and memory allocated with `new[]` is freed with `delete[]`. Mixing them causes undefined behavior.
- After deallocating, it is a good idea to reset the pointer to `nullptr`, unless we point it at another valid target right away. Otherwise it becomes a dangling pointer.

---

## What If We Forget to Deallocate?

If we never call `delete`, the memory stays reserved for the whole run of the program, even though we can't use it anymore. This is called a **memory leak**.

In a small program it may go unnoticed. In a program that runs for a long time, or that allocates inside a loop, the leaks add up until the program slows down or runs out of memory. This is why deallocation is important.

---

## Example: Resizing a Dynamic Array

Suppose we have an array and we want to make it bigger. We can't just append new cells to it. Arrays are stored in **consecutive memory**, and we never know if the memory right after the array is already used for something else.

So we do it in four steps. Let's say this is our original array, and we want room for 5 more numbers because the old one is full:

```cpp
int* list = new int[size];
```

**1. Create an entirely new array of the new size.** We need another pointer for it.

```cpp
int* temp = new int[size + 5];
```

**2. Copy the data from the old array into the new one**, keeping the same positions.

```cpp
for (int i = 0; i < size; i++)
    temp[i] = list[i];
```

**3. Delete the old array.** We don't need it anymore.

```cpp
delete[] list;
```

**4. Change the pointer.** We still want to call the array `list`, so we make the `list` pointer hold the new address.

```cpp
list = temp;
```

```
Before:   list ──► [ 1 | 2 | 3 ]

Step 1:   list ──► [ 1 | 2 | 3 ]
          temp ──► [ ? | ? | ? | ? | ? | ? | ? | ? ]

Step 2:   list ──► [ 1 | 2 | 3 ]
          temp ──► [ 1 | 2 | 3 | ? | ? | ? | ? | ? ]

Step 3:   (old array freed)
          temp ──► [ 1 | 2 | 3 | ? | ? | ? | ? | ? ]

Step 4:   list ──► [ 1 | 2 | 3 | ? | ? | ? | ? | ? ]
```

The `list` array is now 5 elements larger and it holds the same data as before, with room for 5 more items. As a function:

```cpp
int* resize(int* list, int size, int extra) {
    int* temp = new int[size + extra];
    for (int i = 0; i < size; i++)
        temp[i] = list[i];
    delete[] list;
    return temp;
}

int* list = new int[3]{1, 2, 3};
list = resize(list, 3, 5);
```

This is the same idea behind a dynamic array (`vector`), which is the topic of a later folder.
