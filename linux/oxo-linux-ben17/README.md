# oxo-linux-ben17 Ring Buffer Manager

## Challenge Details

### Description

This C program implements a ring buffer for managing a queue of items. The write_to_buffer function uses a head index to track the write position. When removing items, the head index is decremented. If items are removed when the buffer is empty (head is 0), the unsigned integer underflows to a large value (UINT_MAX), causing the next write operation to occur at an invalid memory location far beyond the buffer bounds.

### Vulnerability Type and Category
- **Type:** Integer Underflow
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben17
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Trigger underflow by removing from empty buffer
2. Head index wraps to UINT_MAX (4294967295)
3. Next write occurs at buffer[UINT_MAX]
4. Arbitrary memory write at high address

**Exploitation Steps:**
```bash
./app
# Select: 2 (Remove item) when buffer is empty
# head-- causes underflow: 0 - 1 = 0xFFFFFFFF
# Select: 1 (Add item)
# Write occurs at buffer[0xFFFFFFFF]

# Expected: Segmentation fault or memory corruption
# Can potentially write to arbitrary high memory
```

**Key Points:**
- Unsigned integer underflow
- No empty buffer check before decrement
- Write occurs at extremely high index
- Potential arbitrary memory write
