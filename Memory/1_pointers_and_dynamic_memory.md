# Pointers in C++

> What a pointer really is, how to use it, the types of pointers you will run into, and how a pointer differs from a reference.

## Table of Contents

1. [What Is a Pointer?](#what-is-a-pointer)
2. [Declaring and Dereferencing](#declaring-and-dereferencing)
3. [Size of a Pointer](#size-of-a-pointer)
4. [Types of Pointers](#types-of-pointers)
5. [Pointer Arithmetic](#pointer-arithmetic)
6. [Pointer to Pointer](#pointer-to-pointer)
7. [Function Pointers](#function-pointers)
8. [Smart Pointers](#smart-pointers)
9. [Pointer vs Reference](#pointer-vs-reference)

---

## What Is a Pointer?

A pointer is a special kind of variable. Instead of storing a normal value, it stores the **memory address of another variable**.

Take a normal variable:

```cpp
int x = 10;
```

`x` is an `int`, and the value stored inside it is 10. The block of memory that `x` occupies also has an address in the machine. To see that address, we put the **address-of operator** `&` before the variable:

```cpp
cout << &x;   // prints the address of x
```

Now what if we want a variable that holds that address, `&x`, inside it? That variable is a **pointer**. It is called a pointer because it *points to* another variable. Working with pointers gives us much more control over memory and helps us understand how it really works.

```
        x                    p
   ┌─────────┐          ┌─────────┐
   │   10    │ ◄─────── │  &x     │
   └─────────┘          └─────────┘
   address: 0x1000      holds the address 0x1000
```

When we access a pointer directly, we get the address it holds, not the data stored at that address.

---

## Declaring and Dereferencing

A pointer always points to data of a specific type, so when we declare it we must declare it with that type. A pointer to an `int` is an `int*`, a pointer to a `char` is a `char*`, a pointer to a `string` is a `string*`, and so on. We write the `*` as part of the declaration.

```cpp
int x = 10;
int* p = &x;      // p holds the address of x

cout << p;        // prints the address stored in p (same as &x)
cout << *p;       // prints 10, the value p points to
```

The `*` has two jobs, depending on where it appears:

- In a **declaration** (`int* p`), it says that `p` is a pointer and not a normal variable.
- In an **expression** (`*p`), it is the **dereference operator**. It follows the address and gives us the value stored there.

The process of accessing the value at the memory address a pointer holds is called **dereferencing**.

```cpp
*p = 20;          // changes x through the pointer
cout << x;        // prints 20
```

A pointer can also be changed to point to another variable, as long as that variable has the same type.

```cpp
int y = 30;
p = &y;           // p now points to y
```

---

## Size of a Pointer

The size of a pointer is the same for every pointer, no matter what type of data it points to. That is because in the end a pointer only stores a memory address. It does not depend on the data type. It depends on the operating system and the CPU architecture:

| System | Pointer size |
| ------ | ------------ |
| 64-bit | 8 bytes      |
| 32-bit | 4 bytes      |

The logic is simple. The maximum width of a memory address is determined by the CPU architecture, so on a 64-bit CPU an address is always 64 bits wide. We can verify this with the `sizeof` operator:

```cpp
cout << sizeof(int*);      // 8
cout << sizeof(double*);   // 8
cout << sizeof(char*);     // 8
```

All of them have the same size. This is a 64-bit system, so the size is 8 bytes.

---

## Types of Pointers

### 1. Wild Pointer

A pointer that is declared without being given any address. It holds a random address that may or may not be valid.

```cpp
int* p;      // wild pointer
*p = 5;      // dangerous
```

Dereferencing it can lead to errors such as segmentation faults, so it is always recommended to initialize a pointer when we declare it.

### 2. Null Pointer

A pointer with an empty value that does not point to anything valid in memory. It is used to initialize a pointer when we don't want it to point to any object yet.

```cpp
int* p = nullptr;
```

In modern C++ we use `nullptr`. `NULL` is the old C-style macro (it is just 0), and `nullptr` is safer because it has its own pointer type.

### 3. Void Pointer

A pointer of type `void*` has no data type attached to it. It can hold the address of any data type, which makes it useful for generic programming.

But because the type is unknown, the compiler does not know how many bytes to read or how to interpret them. That is why a void pointer **cannot be dereferenced directly**. We must first cast it to the right pointer type so the compiler knows what it points to.

```cpp
int x = 10;
void* vp = &x;

// cout << *vp;                      // error
cout << *static_cast<int*>(vp);      // 10
```

### 4. Dangling Pointer

A pointer that points to memory that has already been freed or is no longer valid. It usually happens in two cases:

- The pointer points to a local variable that has gone out of scope.
- Dynamically allocated memory was freed with `delete`, but the pointer still holds the old address.

```cpp
int* p = new int(5);
delete p;
// p is now dangling, it still holds the old address

p = nullptr;   // good habit: reset it right after delete
```

Dereferencing a dangling pointer leads to **undefined behavior**, and it is a common source of bugs that are hard to find.

---

## Pointer Arithmetic

C++ allows some arithmetic operations on pointers:

- Incrementing and decrementing (`p++`, `p--`)
- Adding an integer (`p + n`)
- Subtracting an integer (`p - n`)
- Subtracting two pointers of the same type
- Comparing pointers, including comparing with `nullptr`

The important point is that arithmetic moves by the **size of the type**, not by one byte. `p + 1` on an `int*` moves 4 bytes forward, because the next `int` starts 4 bytes later.

```cpp
int arr[3] = {10, 20, 30};
int* p = arr;

cout << *p;         // 10
cout << *(p + 1);   // 20
cout << *(p + 2);   // 30
cout << (p + 2) - p;   // 2 (the number of elements between them)
```

This is exactly why `arr[i]` and `*(arr + i)` mean the same thing.

---

## Pointer to Pointer

A pointer can also point to another pointer. This is called a **double pointer**. It is useful when working with multi-level data structures, like arrays of pointers, or with dynamic memory management.

```cpp
int x = 10;
int* p = &x;
int** pp = &p;

cout << **pp;   // 10
```

```
   pp             p             x
┌───────┐     ┌───────┐     ┌───────┐
│  &p   │ ──► │  &x   │ ──► │  10   │
└───────┘     └───────┘     └───────┘
```

---

## Function Pointers

Just as there are pointers to variables, there are pointers to functions. A function pointer holds the location of the function's code in memory. This lets us call a function without calling it by its name, or pass it to another function as a parameter, as we do with a custom comparator in sorting.

```cpp
bool descending(int a, int b) { return a > b; }

void applySort(int* arr, int n, bool (*cmp)(int, int)) {
    sort(arr, arr + n, cmp);
}

int nums[4] = {3, 1, 4, 2};
applySort(nums, 4, descending);   // 4 3 2 1
```

The syntax `bool (*cmp)(int, int)` reads as: `cmp` is a pointer to a function that takes two `int`s and returns a `bool`.

---

## Smart Pointers

A smart pointer is a **wrapper class** around a normal (raw) pointer. It overloads operators like `*` and `->` so the object behaves exactly like a raw pointer. Its big advantage is that it deletes the memory by itself as soon as it goes out of scope, which protects us from memory leaks caused by forgetting to write `delete`.

```cpp
#include <memory>

unique_ptr<int> a = make_unique<int>(5);   // single owner
shared_ptr<int> b = make_shared<int>(7);   // shared ownership

cout << *a;   // 5
// no delete needed, memory is freed automatically
```

---

## Pointer vs Reference

A **pointer** is a variable that stores the address of another variable. When we define it, we can give it a value or leave it without one, we can set it to `nullptr`, and we can change what it points to later at any time.

A **reference** is different. It is not a separate variable holding a copy of the value. It is an **alias**, another name for an existing variable. We write `&` before its name, and both names refer to the **same variable in the same memory location**.

```cpp
int x = 10;

int* p = &x;   // pointer: stores the address of x
int& r = x;    // reference: another name for x

r = 99;        // changes x
cout << x;     // 99
```

A reference has rules that a pointer does not have:

- It must be initialized on the same line where it is declared.
- Once it is bound to a variable, it can never be bound to another one.
- It cannot be `null`.
- Changing its value changes the original variable.

Compilers usually implement a reference as a constant pointer behind the scenes, but in our code it behaves like a plain variable name.

| Pointer                                | Reference                                |
| -------------------------------------- | ---------------------------------------- |
| Stores the address of a variable       | Is an alias for a variable               |
| Can be `nullptr`                       | Cannot be null                           |
| Can be declared without a value        | Must be initialized when declared        |
| Can be changed to point elsewhere      | Cannot be rebound after initialization   |
| Needs `*` to reach the value           | Is used like a normal variable           |
| Has its own address and its own size   | Refers to the same address as the original |
