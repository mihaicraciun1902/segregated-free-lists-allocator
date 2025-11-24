# CLI Memory Allocator (Segregated Free Lists)

A low-level simulation of a dynamic memory allocator written in **C**, implementing the **Segregated Free Lists (SFL)** strategy.  
This project mimics how real allocators handle `malloc`, `free`, fragmentation, and memory access — all inside a simulated heap.

---

## 📌 Project Overview

This application creates a virtual heap where **free memory blocks are grouped into multiple doubly linked lists** based on size.  
Unlike a flat free list, this approach drastically **reduces search times** and improves allocation efficiency.

The allocator supports:
- Block splitting
- Reinsertion of fragmented memory
- Reading/writing across non-contiguous blocks
- Segmentation fault simulation
- Full heap state visualization

---

## 🚀 Key Features

### **Dynamic Heap Initialization**
- Configurable:
  - Heap base address
  - Number of free lists
  - Bytes per list
- Full reset and destruction support

### **Custom Allocation (`MALLOC`)**
- Implements **Best Fit**
- **Fragmentation Logic:**  
  If no exact fit is found, a larger block is split:
  - Requested bytes are allocated
  - Remaining bytes form a new free block placed back into the correct list

### **Memory Deallocation (`FREE`)**
- Frees memory at the given address
- Reinserts block into the correct size bucket

### **Memory Access (`READ` / `WRITE`)**
- Read/write bytes inside allocated ranges
- Prevents access to unallocated memory (simulated **segmentation fault**)
- Supports writing data that spans multiple memory blocks

### **Diagnostics**
- `DUMP_MEMORY` displays:
  - Heap map
  - Allocation status
  - Fragmentation counters
  - Detailed list contents

---

## 🔧 Technical Highlights (For Recruiters)

This project demonstrates strong low-level system knowledge and dynamic memory reasoning:

### **Data Structures**
- Array of **Doubly Linked Lists** for block categorization
- Custom nodes storing:
  - Starting address
  - Block size
  - Links to neighbors

### **Manual Memory Management**
- Internal structures (`sfl_t`, `node_t`, and allocation list) managed with:
  - `malloc`
  - `realloc`
  - `free`
- Carefully prevents leaks or dangling pointers

### **Algorithm Design**
- **Fragmentation Handling:**  
  When allocating from a block `B` with size `SB` for request `R`:  
  `Remainder = SB - R` → new block created + placed in correct free list
- **Sorting:**  
  **Bubble Sort** used for ordered output in `DUMP_MEMORY` to improve readability

---

## 🛠️ Installation & Compilation

### **Prerequisites**
- GCC compiler  
- Make  

### **Build Instructions**
```bash
git clone https://github.com/mihaicraciun1902/segregated-free-lists-allocator.git
cd segregated-free-lists-allocator
make
```

---

## 💻 Usage

Run the main executable:

```bash
./sfl
```

---

## 📘 Command Examples

| Command | Arguments | Description |
|--------|-----------|-------------|
| `INIT_HEAP` | `<base> <lists> <bytes> <type>` | Initializes the heap environment |
| `MALLOC` | `<bytes>` | Allocates memory |
| `FREE` | `<address>` | Frees memory at given address |
| `WRITE` | `<addr> <data> <bytes>` | Writes a string into memory |
| `READ` | `<addr> <bytes>` | Reads memory |
| `DUMP_MEMORY` | – | Prints heap statistics |
| `DESTROY_HEAP` | – | Cleans up and exits |

---

## 🖥️ Sample Session

```
> INIT_HEAP 0x100 4 64 0
> MALLOC 32
> WRITE 0x100 "Hello_World" 11
> DUMP_MEMORY
+++++DUMP+++++
Total memory: 256 bytes
Total allocated memory: 32 bytes
...
> FREE 0x100
> DESTROY_HEAP
```

---

## 📂 Project Structure

```
.
├── sfl.c            # Entry point and CLI command loop
├── functii.c        # Core logic (Malloc, Free, List Ops)
├── functii.h        # Function prototypes
├── sfl.h            # Struct definitions (sfl_t, node_t)
├── Makefile         # Build rules
└── README.md        # Documentation
```

---

## 👨‍💻 Author

**Mihai-Alexandru Crăciun**  
* [LinkedIn](https://www.linkedin.com/in/craciunmihai1902/)
* [GitHub](https://github.com/mihaicraciun1902)
