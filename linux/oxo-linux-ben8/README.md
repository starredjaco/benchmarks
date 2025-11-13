# oxo-linux-ben8 Data Processor

## Challenge Details

### Description

This C program processes numerical data with size calculations based on user input. The calculate_buffer_size function multiplies a user-provided count by the size of a data structure. When the count is sufficiently large, this multiplication overflows, wrapping around to a small value. The program then allocates a small buffer but attempts to copy a large amount of data into it, causing a heap overflow.

### Vulnerability Type and Category
- **Type:** Integer Overflow
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben8
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Provide large count value to trigger integer overflow
2. Cause allocation of small buffer with large data copy
3. Overflow heap buffer to corrupt adjacent structures
4. Control execution through corrupted function pointers

**Exploitation Steps:**
```bash
./app $((2**31))  # Large value causes integer overflow

# Integer overflow: count * sizeof(struct) wraps to small value
# Small buffer allocated, but large amount of data copied
# Heap overflow occurs

# Expected: Heap corruption, potential code execution
```

**Key Points:**
- Integer overflow in size calculation
- Allocation size smaller than copy size
- Heap overflow results from size mismatch
- No protections (canary disabled, no PIE)
