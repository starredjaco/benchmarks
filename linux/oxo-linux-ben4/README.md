# oxo-linux-ben4 Memory Pool Manager

## Challenge Details

### Description

This C program manages a custom memory pool with fixed-size blocks. The allocate_block function writes allocation metadata (size and in-use flag) immediately before the returned user buffer. By requesting a block and overflowing the user data backwards, an attacker can corrupt this metadata, specifically the 'size' field. When blocks are later freed or reallocated, the corrupted size causes the allocator to mismanage memory boundaries, potentially leading to overlapping allocations and memory corruption.

### Vulnerability Type and Category
- **Type:** Heap Metadata Corruption
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben4
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Allocate a memory block from the pool
2. Overflow backwards to corrupt the size metadata
3. Trigger reallocation to cause overlapping chunks
4. Use overlapping allocations for information leak or code execution

**Exploitation Steps:**
```bash
./app
# Select: 1 (Allocate block)
# Select: 2 (Write to block)
# Provide data that overflows backwards into metadata
# Select: 3 (Resize) - uses corrupted size
# Select: 1 (Allocate) - creates overlapping allocation

# Expected: Overlapping heap chunks, memory corruption
```

**Key Points:**
- Custom allocator with inline metadata
- Backward overflow corrupts size field
- Overlapping allocations possible
- Can lead to use-after-free scenarios
