# Pass by Value vs Pass by Reference

> The ways a C++ function can receive its parameters, and how each one behaves in memory.

## Table of Contents

1. [Introduction](#introduction)
2. [Pass by Value](#pass-by-value)
3. [Pass by Reference](#pass-by-reference)
4. [Pass by Pointer](#pass-by-pointer)
5. [Comparison](#comparison)
6. [What About C?](#what-about-c)

---

## Introduction

When we write functions in C++, we often need to pass data from one function to another. Sometimes we want the function to **use** the data without changing it. Other times we want the function to **modify** the original data.

How we pass the parameters decides which one happens.

---

## Pass by Value

In pass by value, the function receives a **copy** of the variable's value.

- Any change made to the parameter inside the function does **not** affect the original variable in the caller.
- Two separate copies exist in memory: the original variable in the caller, and the function's local copy.

```cpp
void addTen(int x) {
    x = x + 10;     // changes only the local copy
}

int main() {
    int a = 5;
    addTen(a);
    cout << a;      // 5, unchanged
}
```

```
   main                 addTen
┌─────────┐          ┌─────────┐
│ a = 5   │  copy ─► │ x = 5   │   two separate memory locations
└─────────┘          └─────────┘
```

---

## Pass by Reference

In pass by reference, the parameter is declared with `&`, so it becomes an **alias** for the variable that was passed in. The function works on the original variable directly, with no copy made.

- The function can modify the original variable in the caller.
- Only one memory location exists for the variable, so changes inside the function affect the original.

```cpp
void addTen(int& x) {
    x = x + 10;     // changes the original
}

int main() {
    int a = 5;
    addTen(a);
    cout << a;      // 15
}
```

```
   main                 addTen
┌─────────┐
│ a = 5   │ ◄──── x (alias for a)       one memory location, two names
└─────────┘
```

A common example is a swap function:

```cpp
void swapByValue(int a, int b)  { int t = a; a = b; b = t; }   // does nothing to the caller
void swapByRef(int& a, int& b)  { int t = a; a = b; b = t; }   // swaps the caller's variables
```

Passing by reference is also useful for **large objects** (like a `string` or a `vector`), because it avoids copying them. If we don't want the function to change the object, we pass it by `const` reference:

```cpp
void print(const string& s);   // no copy, and no modification allowed
```

---

## Pass by Pointer

We can also give the function the **address** of the variable, using a pointer. The function then reaches the original variable by dereferencing the pointer. It has the same effect as pass by reference, but the syntax is different: we pass `&a` when we call it, and we use `*x` inside the function.

```cpp
void addTen(int* x) {
    *x = *x + 10;
}

int main() {
    int a = 5;
    addTen(&a);
    cout << a;      // 15
}
```

Unlike a reference, a pointer parameter can be `nullptr`, so we should check it before dereferencing when that is possible.

---

## Comparison

| | Pass by Value | Pass by Reference | Pass by Pointer |
| --- | --- | --- | --- |
| What is passed | A copy of the value | An alias of the variable | The address of the variable |
| Original variable | Not affected | Changes are reflected | Changes are reflected |
| Memory | Two separate locations (original + copy) | One shared location | One shared location (plus the pointer itself) |
| Parameter type | Normal variable | Reference (`&`) | Pointer (`*`) |
| Inside the function | `x` | `x` | `*x` |
| Can be null | No | No | Yes |
| Use when | We want to protect the original data | We want to modify the original, or avoid copying large objects | We want to modify the original and `nullptr` is a valid input |
| Example | `void func(int x)` | `void func(int& x)` | `void func(int* x)` |

---

## What About C?

C does not have pass by reference like C++. Every argument in C is passed by value. To modify a variable of the caller, we pass its address using a pointer, which is exactly the pass by pointer form above. The pointer itself is copied, but the address it holds is the same, so the function can reach the original variable.
