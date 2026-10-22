# Memory Alignment and Structure Padding in C/C++

> How the compiler lays out a `struct` in memory, why it adds "wasted" bytes on purpose, and how to control it.


---

## The Problem

A `struct` lets us group variables of different data types in one place, for example a `char`, an `int`, and a `double` inside a single unit. So logically, if I have a `char` (1 byte) and an `int` (4 bytes), the struct should take 5 bytes in memory.

But if we print its size, we find that it does not. This is not a bug, and the compiler is not wasting space by mistake. It does this on purpose, and the reason is **speed**.

---

## Data Alignment

The CPU does not read memory one byte at a time. It reads memory in **blocks**, usually 4 or 8 bytes on modern hardware. So for the CPU to read a variable in a single memory cycle, the variable must start at an address that is a multiple of its own size.

| Type     | Size    | Must start at an address divisible by |
| -------- | ------- | ------------------------------------- |
| `char`   | 1 byte  | 1                                     |
| `int`    | 4 bytes | 4                                     |
| `double` | 8 bytes | 8                                     |

### Aligned variable

An `int` stored at address 4 (a multiple of 4) sits entirely inside one block, so the CPU performs one memory fetch and reads all 4 bytes at once.

```
Address:   0  1  2  3 | 4  5  6  7 | 8  9 10 11
Blocks:    [ block 0 ] [ block 1 ] [ block 2 ]
                        └─ int ──┘
                        1 read
```

### Unaligned variable

Now suppose the same `int` is stored across bytes 3, 4, 5, and 6. It is split between two blocks.

```
Address:   0  1  2  3 | 4  5  6  7 | 8  9 10 11
Blocks:    [ block 0 ] [ block 1 ] [ block 2 ]
                     └──── int ───┘
                     2 reads
```

The CPU has to:

1. Read the first block and extract byte 3.
2. Read the second block and extract bytes 4, 5, and 6.
3. Merge both parts into a single register.

That is two memory cycles instead of one. The operation takes double the time, and that is exactly what hurts the performance of the program.

**Data alignment** is storing data at regular memory addresses that are multiples of the variable's size, so the CPU can read it at maximum speed.

---

## Structure Padding

What happens if we don't let the compiler arrange the data this way? The CPU would have to read from memory twice and combine the data, which slows the program down a lot.

So to achieve data alignment, the compiler inserts empty, unused bytes between the variables inside the struct. These bytes are called **padding**, and they guarantee that every variable starts at the right address for the CPU. The compiler adds two types of padding.

### 1. Internal Padding

Padding placed **between** the members of the struct. Its goal is to make sure every member starts at an address suitable for its size.

For example, if a `char` (1 byte) is followed by an `int` (4 bytes), the `char` takes address 0, and if the `int` started right after it at address 1, it would be unaligned. So the compiler inserts 3 empty bytes between them, and the `int` starts at address 4.

```
Address:   0    1    2    3  |  4  5  6  7
           [a] [pad][pad][pad] [     b     ]
```

### 2. Trailing Padding

Padding placed at the **end** of the struct. Its goal is to make the total size of the struct a multiple of the largest alignment requirement among its members.

Why does the compiler need this? Consider:

```c
struct Node {
    int a;   // 4 bytes
    char b;  // 1 byte
};
```

The actual data is 4 + 1 = 5 bytes, and the largest member is the `int` (4 bytes). If the struct were left at 5 bytes and we created an array of it:

- `arr[0]` starts at address 0, so its `int` is aligned.
- `arr[1]` starts at address 5, so its `int` starts at address 5, which is not a multiple of 4. The CPU needs 2 cycles to read it.

To prevent this, the compiler adds 3 bytes of trailing padding at the end, making the size 8 bytes. Now `arr[1]` starts at address 8, which is a multiple of 4, and every element in the array stays aligned.

```
Address:   0  1  2  3 | 4    5    6    7  | 8  9 10 11 | 12 ...
           [    a    ] [b] [pad][pad][pad]  [    a    ]  [b] ...
           └────────── arr[0] ───────────┘  └──── arr[1] ────
```

---

## Does Member Order Matter?

Yes, it does. The compiler walks through the members one by one in the order you declared them. Every time it finds a member that is not at a suitable address, it inserts padding before it to fix that. It does not reorder the members for you, so the order you write is the layout you get.

This means that if we arrange the members from largest to smallest, the big members fill the space naturally, the small ones pack together at the end, and the compiler needs little or no extra padding.

### Bad order

The `char a` takes address 0, then the compiler adds 3 bytes of padding so the `int b` can start at address 4. Then `b` takes bytes 4 to 7, `c` takes address 8, and finally 3 bytes of trailing padding are added to make the size a multiple of 4.

```c
struct Bad {
    char a;   // 1 byte  (+ 3 bytes padding before the int)
    int b;    // 4 bytes
    char c;   // 1 byte  (+ 3 bytes trailing padding)
};
// Total size = 12 bytes (6 of them are padding)
```

```
Address:   0    1    2    3  |  4  5  6  7  |  8    9   10   11
           [a] [pad][pad][pad] [     b     ]  [c] [pad][pad][pad]
```

### Good order

After reordering the variables, `int b` starts at address 0, which is already aligned. Then `a` and `c` sit right next to each other at addresses 4 and 5, and only 2 bytes of trailing padding are needed to complete the size to 8.

```c
struct Good {
    int b;    // 4 bytes
    char a;   // 1 byte
    char c;   // 1 byte  (+ 2 bytes trailing padding)
};
// Total size = 8 bytes
```

```
Address:   0  1  2  3  |  4    5    6    7
           [     b     ] [a]  [c] [pad][pad]
```

Same data, same speed, and we saved about **33%** of the memory just by reordering the members.

---

## Packed Structures

What if we want to save even more memory? A packed structure is a struct where we explicitly ask the compiler to cancel all padding completely, so the members sit right next to each other with no empty bytes at all. The size becomes exactly the real size of the data. For example, 1 + 4 + 1 = 6 bytes instead of 12.

It is done with a single compiler directive. In GCC and Clang, we write it like this:

```c
struct __attribute__((packed)) Packed {
    char a;
    int b;
    char c;
};
// Total size = 6 bytes
```

```
Address:   0    1  2  3  4    5
           [a] [     b     ] [c]
                └─ crosses the block boundary at address 4
```

But this comes at a price. Since the members are no longer aligned, fetching the data becomes much slower for the CPU. This is called the **memory access penalty**, and it is the same double-cycle problem we saw in the alignment section. So we gain memory, but we lose speed.

### When to Use It

Packing looks like a free win at first, but since it costs us alignment, we should only use it when the memory layout of our struct has to match an exact external format byte for byte.

The most common case is **network programming**. A protocol header, like the TCP/IP header, is defined by a standard that says exactly which byte holds which field. If the compiler inserts padding inside the struct we use to parse the header, the fields will no longer line up with the bytes that came over the wire, and we will read garbage.

The same applies to **file formats**. A BMP image or a WAV audio file has a header with a strict byte layout, and we usually load it straight into a struct. Padding would shift the fields and break the parsing.

The third case is **embedded systems**. When we talk directly to hardware registers on a microcontroller, the hardware expects data at specific positions, and the struct has to mirror that layout exactly.

In all three cases the layout is dictated from outside, so matching it matters more than access speed.

### Drawbacks

The main drawback is **performance**. Packing removes the padding, so members are no longer guaranteed to start at aligned addresses. As we saw in the alignment section, reading an unaligned variable can force the CPU to do two memory cycles and merge the results, so every access to that member becomes slower. In code that touches these structs frequently, that cost adds up.

The second drawback is more serious, and it is **portability**. Some processors, such as older ARM architectures, do not tolerate unaligned reads at all. On those machines, reading an `int` that sits at an unaligned address inside a packed struct can trigger a hardware fault and crash the program. Code that runs fine on your laptop might fail on an embedded board, which makes this bug especially hard to track down.

So the rule is simple: let the compiler pad by default, order your members from largest to smallest to save space, and use packing only when an external format forces you to.

---

## Verify It Yourself

You can check all the sizes and offsets above with `sizeof` and `offsetof`:

```c
#include <stdio.h>
#include <stddef.h>

struct Bad  { char a; int b; char c; };
struct Good { int b; char a; char c; };
struct __attribute__((packed)) Packed { char a; int b; char c; };

int main(void) {
    printf("Bad:    size=%zu  a=%zu b=%zu c=%zu\n",
           sizeof(struct Bad),
           offsetof(struct Bad, a), offsetof(struct Bad, b), offsetof(struct Bad, c));

    printf("Good:   size=%zu  b=%zu a=%zu c=%zu\n",
           sizeof(struct Good),
           offsetof(struct Good, b), offsetof(struct Good, a), offsetof(struct Good, c));

    printf("Packed: size=%zu  a=%zu b=%zu c=%zu\n",
           sizeof(struct Packed),
           offsetof(struct Packed, a), offsetof(struct Packed, b), offsetof(struct Packed, c));

    return 0;
}
```

Output on a typical 64-bit GCC build:

```
Bad:    size=12  a=0 b=4 c=8
Good:   size=8  b=0 a=4 c=5
Packed: size=6  a=0 b=1 c=5
```
