# Stack vs Heap Memory Allocation

> How a program is laid out in memory, what happens on the stack when functions call each other, and why we need the heap for dynamic memory.

## Table of Contents

1. [How a Program Runs](#how-a-program-runs)
2. [Memory Allocation](#memory-allocation)
3. [Static Memory Allocation (Stack)](#static-memory-allocation-stack)
4. [Function Calls and Activation Records](#function-calls-and-activation-records)
5. [Dynamic Memory Allocation (Heap)](#dynamic-memory-allocation-heap)
6. [Accessing the Heap Through Pointers](#accessing-the-heap-through-pointers)
7. [Stack vs Heap](#stack-vs-heap)

---

## How a Program Runs

The disk on our machine stores two things: the **program code** and the **data** of the program.

Our program is made of instructions, so who executes those instructions? The **CPU**. To do that, the CPU takes the code and loads it into memory, and it also takes the data and loads it into memory. So now the memory holds the program instructions and the program data. But how is this memory organized?

Memory is divided into three main parts:

```
┌────────────────────────┐
│          Code          │  program instructions
├────────────────────────┤
│          Stack         │  local variables, function calls
├────────────────────────┤
│          Heap          │  dynamically allocated memory
└────────────────────────┘
```

> A real program also has a data segment for global and static variables. It is left out here to keep the picture simple.

---

## Memory Allocation

Every piece of data in our program has a data type, like `int` or `float`. And every data type takes a known amount of memory: an `int` takes 4 bytes, a `double` takes 8 bytes, and so on.

**Memory allocation** means reserving a place in memory with the size of the data we want to store.

---

## Static Memory Allocation (Stack)

What about the data we declare inside `main`? These are primitive data types, which means their sizes are known in advance. We know exactly how much memory each one needs. Their size is decided at **compile time**, which is the stage the program is in before it runs, and they are stored in the **stack**. That is why this is called **static memory allocation**.

```cpp
int main() {
    int a = 5;        // 4 bytes on the stack
    double b = 2.5;   // 8 bytes on the stack
}
```

The compiler knows the exact size of every variable, so the space is reserved automatically when the function starts and released automatically when it ends. We don't write anything to manage it.

---

## Function Calls and Activation Records

What if our program is more complex, and functions call other functions?

The stack is the first place where our variables are stored. The block of memory that holds the variables of a function (its parameters, its local variables, and where to return to) is called an **activation record**, or a **stack frame**.

If `main` calls a function, we create an activation record for that function and push it on the stack. If that function calls another one, another activation record is pushed on top of it. This repeats for every call.

```
 Stack (grows upward)

 ┌──────────────────┐
 │  funcB variables │  ◄── top: runs first
 ├──────────────────┤
 │  funcA variables │
 ├──────────────────┤
 │  main variables  │  ◄── bottom
 └──────────────────┘
```

When the program executes, it starts from the function at the top of the stack, which is the last one that was called. When a function returns, its activation record is removed from the stack and that space is freed. This continues until we reach `main`, and when `main` finishes, the program ends.

```cpp
void funcB() { int z = 3; }

void funcA() {
    int y = 2;
    funcB();       // push funcB's frame
}                  // funcB's frame is popped when it returns

int main() {
    int x = 1;
    funcA();       // push funcA's frame
}
```

The stack works as **last in, first out**: the last frame pushed is the first one removed.

---

## Dynamic Memory Allocation (Heap)

If that was static memory allocation, what is dynamic memory allocation?

Here we use the keyword **`new`**, which is made to allocate memory, but this time in the **heap**, not in the stack.

Why do we need it? Imagine we have an array in our program. How do we know its size, 5 or 10? What if we want to give the user the freedom to choose the size? If the user asks for 5 elements, `new` goes to the heap and reserves space for exactly 5 elements.

```cpp
int n;
cin >> n;                 // the size is known only at run time
int* arr = new int[n];    // allocated on the heap
```

This is called **dynamic memory allocation**, and it is dynamic because the size is not decided until **run time**.

Since the heap is not managed automatically, we are responsible for giving the memory back when we are done with it:

```cpp
delete[] arr;
arr = nullptr;
```

If we forget, the memory stays reserved until the program ends. This is called a **memory leak**. (How `new` and `delete` work in detail is covered in the next document.)

---

## Accessing the Heap Through Pointers

Our code in `main` cannot access the heap directly, because heap memory has no variable name. So how do we reach it? Through **pointers**.

The pointer itself is a normal variable, so it is stored in the **stack**. What it holds is the **address** of the memory in the **heap**. Through that pointer we can access any element of the array, or a single variable, or anything else we allocated.

```cpp
int* arr = new int[3];   // arr lives on the stack
arr[0] = 10;             // the data lives on the heap
```

```
        Stack                        Heap
   ┌─────────────┐             ┌────┬────┬────┐
   │ arr = 0x5A0 │ ──────────► │ 10 │  ? │  ? │
   └─────────────┘             └────┴────┴────┘
                                   0x5A0
```

If we lose that pointer (for example, by overwriting it) before calling `delete`, there is no way to reach that heap memory again, and it leaks.

---

## Stack vs Heap

| | Stack | Heap |
| --- | --- | --- |
| Allocation type | Static | Dynamic |
| Size known | At compile time | At run time |
| How we allocate | Declare a variable | `new` |
| Who frees it | Automatic, when the function returns | The programmer, with `delete` |
| Speed | Faster | Slower |
| Size limit | Small, a fixed size | Much larger |
| Access | Directly by variable name | Only through a pointer |
| Main risk | Stack overflow (for example, very deep recursion) | Memory leaks and dangling pointers |
