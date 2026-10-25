# Stack in C++: Array, Linked List, and Applications

> Two from-scratch implementations of a stack (one on top of an array, one on top of a linked list), followed by classic problems that a stack solves: reversing, balanced brackets, infix to postfix, postfix evaluation, palindromes, and decimal to binary.

## Table of Contents

1. [Files](#files)
2. [What Is a Stack?](#what-is-a-stack)
3. [Array-Based Stack](#array-based-stack)
4. [Linked-List-Based Stack](#linked-list-based-stack)
5. [Array vs Linked List](#array-vs-linked-list)
6. [Applications](#applications)
7. [Known Issues and Limitations](#known-issues-and-limitations)

---

## Files

| File                        | Description                                                             |
| --------------------------- | ----------------------------------------------------------------------- |
| `Stack_Array.h`             | `Stack` for `int` values, stored in a fixed array of 100 cells          |
| `Stack_Linked_List.h`       | `Stack` for `int` values, stored as a linked list with no fixed capacity |
| `Applications_on_Stack.cpp` | A template `Stack<T>` and the application functions built on top of it  |

The two headers both define a class named `Stack` (and the linked list one also defines `Node`), so include only one of them in the same source file.

`Applications_on_Stack.cpp` is a `.cpp` file with **no `main`**. It carries its own array-based `Stack<T>`, so it does not need the headers. To use it, declare the functions you need and compile it together with your own `main`:

```
g++ main.cpp Applications_on_Stack.cpp -o app
```

---

## What Is a Stack?

A stack is a collection where you can only add and remove at **one end**, called the **top**. The last element pushed is the first one popped. This rule is called **LIFO** (last in, first out), like a pile of plates.

```
Push(10)    Push(20)    Push(30)    Pop()       Pop()
                        [ 30 ] <
            [ 20 ] <    [ 20 ]      [ 20 ] <
[ 10 ] <    [ 10 ]      [ 10 ]      [ 10 ]      [ 10 ] <

< marks the top
```

| Operation | Meaning |
| --- | --- |
| `Push(val)` | Put a value on top |
| `Pop()` | Remove the top value |
| `Peek()` | Look at the top value without removing it |

All three are O(1). A stack is also what the program itself uses for function calls (each call pushes an activation record, and each `return` pops it), which is covered in `02_stack_vs_heap_memory_allocation`.

Both implementations print their results and error messages instead of returning values: `Pop` prints `Poped Element Is : x`, `Peek` prints `Top Element Is : x`, and an empty stack prints `Stack Underflow.`.

---

## Array-Based Stack

The values live in an array of `MAX = 100` cells, and the integer `top` is the index of the top element. `top = -1` means the stack is empty.

```
index:   0    1    2    3    4   ...  99
       [ 10 ][ 20 ][ 30 ][ _ ][ _ ] ... [ _ ]
                      ^
                     top = 2
```

- `Push` increments `top` and writes into `arr[top]` (`arr[++top] = val`).
- `Pop` reads `arr[top]` and decrements `top`. The old value stays in the array, but it is no longer part of the stack.
- Pushing when `top == MAX - 1` prints `Stack Overflow.`.

### Usage

```cpp
#include "Stack_Array.h"

int main()
{
    Stack s;
    s.Push(10);
    s.Push(20);
    s.Push(30);
    s.Print();         // top first
    s.Peek();
    s.Pop();
    s.Search(10);
    s.Print();
}
```

Output:

```
30 20 10
Top Element Is: 30
Poped Element Is : 30
Value found at position : 0
20 10
```

### Functions reference

Let `n` be the number of elements.

| Function | Description | Time |
| --- | --- | --- |
| `Stack()` | Creates an empty stack (`top = -1`) | O(1) |
| `IsEmpty()` | Returns `true` if the stack has no elements | O(1) |
| `Push(val)` | Adds `val` on top, or prints `Stack Overflow.` when full | O(1) |
| `Pop()` | Removes the top element and prints it, or prints `Stack UnderFlow.` | O(1) |
| `Peek()` | Prints the top element, or prints `Stack Underflow.` | O(1) |
| `Search(val)` | Looks from the top down and prints the **array index** where `val` is found | O(n) |
| `Print()` | Prints all elements from the top down | O(n) |

`Search` reports the index counted from the **bottom** of the stack (0-based), so in the example above `10` is at position 0.

The whole stack lives inside the object (404 bytes: 100 `int`s plus `top`), whether it holds one element or a hundred. There is nothing to free, so no destructor is needed.

---

## Linked-List-Based Stack

Each element is a node, and `head` points to the top of the stack. There is no capacity: we allocate one node per push.

```
head (top)
 |
 v
[ 30 | * ]--->[ 20 | * ]--->[ 10 | null ]
```

- `Push` creates a node and makes it the new `head` (it is `AddToHead` of a linked list).
- `Pop` removes the head node, prints its value, and deletes the node.
- The destructor deletes every remaining node.

The usage code is exactly the same as for the array version, with `#include "Stack_Linked_List.h"`. Output:

```
30 20 10
Top Element Is : 30
Poped Element Is : 30
Value found in position : 2
20 10
```

### Functions reference

| Function | Description | Time |
| --- | --- | --- |
| `Stack()` | Creates an empty stack (`head = nullptr`) | O(1) |
| `~Stack()` | Deletes every node | O(n) |
| `isEmpty()` | Returns `true` if the stack has no elements | O(1) |
| `Push(val)` | Adds a node on top | O(1) |
| `Pop()` | Removes the top node and prints its value, or prints `Stack Underflow.` | O(1) |
| `Peek()` | Prints the top element, or prints `Stack Underflow.` | O(1) |
| `Search(val)` | Prints the 1-based position of `val`, counted from the top | O(n) |
| `Print()` | Prints all elements from the top down | O(n) |

Here `Search` counts from the **top** and starts at 1, so in the example `10` is at position 2 (counting `20` as position 1 after the pop). This is different from the array version.

---

## Array vs Linked List

| | Array-based | Linked-list-based |
| --- | --- | --- |
| Capacity | Fixed (100) | Limited only by memory |
| Overflow possible | Yes | No |
| `Push`, `Pop`, `Peek` | O(1) | O(1) |
| Memory | Fixed 404 bytes, used or not | 16 bytes per element (4-byte `int` + padding + 8-byte pointer) |
| Allocation | None while running | One `new` and one `delete` per push and pop |
| Cache behavior | Elements are contiguous | Nodes can be scattered |
| `Search` numbering | Index from the bottom, 0-based | Position from the top, 1-based |
| Needs a destructor | No | Yes |
| Safe to copy | Yes (the array is copied with the object) | No (see Known Issues) |

For a stack whose maximum size is known and small, the array is simpler and faster. When the size is unpredictable, the linked list never overflows.

---

## Applications

`Applications_on_Stack.cpp` contains a `Stack<T>` template (array-based, 100 cells) and the functions below. This stack differs from the header versions in one way: it **returns** values instead of printing them. `Pop()` returns the removed element and `Peek()` returns the top, and on an empty stack both print an underflow message and return `T()` (the default value, such as `0`), so the algorithms can use them inside expressions.

| Function | Result | Time | Extra space |
| --- | --- | --- | --- |
| `Reverse(s)` | The string reversed | O(n) | O(n) |
| `checkBalance(s)` | `true` if every bracket is closed in the right order | O(n) | O(n) |
| `infix_to_postfix(exp)` | The expression converted to postfix | O(n) | O(n) |
| `evaluate_postfix(exp)` | The integer value of a postfix expression | O(n) | O(n) |
| `isPalindrome(s)` | `true` if `s` reads the same in both directions | O(n) | O(n) |
| `decTobin(n)` | The binary representation of `n` | O(log n) | O(log n) |

### Usage

```cpp
#include <iostream>
#include <string>

string Reverse(string s);
bool checkBalance(string s);
string infix_to_postfix(string exp);
int evaluate_postfix(string exp);
bool isPalindrome(string s);
string decTobin(int n);

int main()
{
    cout << Reverse("hello") << '\n';
    cout << checkBalance("{[()]}") << checkBalance("{[(])}") << '\n';

    string post = infix_to_postfix("(1+2)*30");
    cout << post << '\n';
    cout << evaluate_postfix(post) << '\n';

    cout << isPalindrome("level") << isPalindrome("stack") << '\n';
    cout << decTobin(10) << '\n';
}
```

Output:

```
olleh
10
1 2 + 30 *
90
10
1010
```

### Reversing a string

Push every character, then pop them all. Since the last character pushed comes out first, the result is the string backwards.

```
push h, e, l, l, o      stack (top on the right):  h e l l o
pop  o, l, l, e, h      result:                    "olleh"
```

### Balanced brackets

Scan the string. Every opening bracket `(`, `[`, `{` is pushed. Every closing bracket must match the bracket on top of the stack, and if it does we pop it. If the stack is empty when a closing bracket arrives, or the top does not match, the answer is `false`. At the end the stack must be empty.

```
"{[()]}"                         "{[(])}"

{    push   [ { ]                {    push   [ { ]
[    push   [ { [ ]              [    push   [ { [ ]
(    push   [ { [ ( ]            (    push   [ { [ ( ]
)    match  pop -> [ { [ ]       ]    top is "(", not "[" -> false
]    match  pop -> [ { ]
}    match  pop -> [ ]
end: empty -> true
```

### Infix to postfix

Postfix (reverse Polish notation) puts the operator after its operands, so `(1+2)*30` becomes `1 2 + 30 *`. It needs no parentheses and no precedence rules when evaluated. The conversion uses a stack of operators and a priority for each one:

| Operator | Priority |
| --- | --- |
| `+`, `-` | 1 |
| `*`, `/`, `%` | 2 |
| `^` | 3 |
| anything else, including `(` | 0 |

The algorithm scans the expression from left to right:

- A **number** (one or more digits) goes straight to the output.
- `(` is pushed.
- `)` pops operators to the output until the matching `(`, which is then discarded.
- An **operator** first pops every operator on the stack with a priority greater than or equal to its own, sends them to the output, and then is pushed itself.
- At the end, everything left on the stack is popped to the output.

```
Expression: (1+2)*30

token   action                              stack    output
(       push                                (
1       to output                           (        1
+       push                                ( +      1
2       to output                           ( +      1 2
)       pop until "("                                1 2 +
*       push                                *        1 2 +
30      to output                           *        1 2 + 30
end     pop the rest                                 1 2 + 30 *
```

Multi-digit numbers are read as a whole, whitespace is skipped, and every token in the output is followed by a space.

### Evaluating a postfix expression

Scan the postfix string. A number is pushed. An operator pops the two top values, applies the operator, and pushes the result. The first value popped is the **second** operand, so `5 3 -` means `5 - 3`. The answer is what remains on top at the end.

```
Expression: 1 2 + 30 *

1     push                [ 1 ]
2     push                [ 1 2 ]
+     pop 2, 1 -> 1 + 2   [ 3 ]
30    push                [ 3 30 ]
*     pop 30, 3 -> 3 * 30 [ 90 ]

result: 90
```

Supported operators: `+`, `-`, `*`, `/`.

### Palindrome

Push all characters, pop them into a new string (which is the reverse), and compare it with the original.

### Decimal to binary

Repeatedly divide the number by 2 and push each remainder. The remainders come out from the **least** significant bit to the most, so popping them reverses the order into the correct binary string.

```
n = 10

10 % 2 = 0   push 0     n = 5
 5 % 2 = 1   push 1     n = 2
 2 % 2 = 0   push 0     n = 1
 1 % 2 = 1   push 1     n = 0

pop: 1, 0, 1, 0   ->   "1010"
```

`decTobin(0)` returns `"0"`. A negative number returns an empty string.

---

## Known Issues and Limitations

These are confirmed behaviors of the current code.

**`Stack_Array.h`**

- **`Search` prints nothing when the value is not found.** After the loop finishes, `i` is `-1`, so the check `if (i == 0 && arr[i] != val)` is false and the "not found" message is never printed. (The stack inside `Applications_on_Stack.cpp` and the linked list version both print `Value isn't found`.)

**`Stack_Linked_List.h`**

- **No copy constructor and no copy assignment.** Copying a `Stack` copies only the `head` pointer, so both stacks point to the same nodes and both destructors delete them (a double free). Pass stacks by reference and never copy them.

**`Applications_on_Stack.cpp`**

- **`evaluate_postfix` stores numbers in a `Stack<char>`.** A `char` holds only a small range (-128 to 127 on most machines), so any intermediate value above that wraps around. For example, `evaluate_postfix("100 3 *")` returns `44` instead of `300`. Using `Stack<int>` fixes it.
- **`evaluate_postfix` ignores `%` and `^`.** `infix_to_postfix` produces them, but the evaluator pops two operands for any operator and only pushes a result for `+`, `-`, `*`, `/`. For `%` and `^` both operands are lost, so the function prints `Stack Underflow.` and returns `0`.
- **Division by zero is not checked** in `evaluate_postfix`.
- **`checkBalance` expects brackets only.** Any character that is not an opening bracket is treated as a closing bracket, so a string with other characters returns `false`. For example, `checkBalance("(a)")` is `false`.
- **`infix_to_postfix` does not validate its input.** An unmatched `)` makes the loop `while (s.Peek() != '(')` run forever, because `Peek` on an empty stack returns `'\0'`.
- **Fixed capacity.** The `Stack<T>` holds at most 100 elements, so very long expressions print `Stack Overflow.`.