# oxo-linux-ben2 Resource Manager

## Challenge Details

### Description

This C program manages resources with dynamic memory allocation. The program creates, uses, and frees Resource structures containing buffers and metadata. A critical flaw in the cleanup logic causes resources to be freed twice under certain conditions. This double-free vulnerability can corrupt heap metadata, potentially leading to arbitrary code execution when the corrupted memory is reallocated.

### Vulnerability Type and Category
- **Type:** Double Free
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben2
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Trigger the double-free condition through specific operation sequence
2. Corrupt heap metadata structures (bins, chunks)
3. Achieve arbitrary write or code execution on subsequent allocations

**Exploitation Steps:**
```bash
# Run the program and trigger double-free
./app
# Select: 1 (Create Resource)
# Select: 3 (Use Resource) 
# Select: 4 (Cleanup) - triggers double free

# Expected: Heap corruption, potential for arbitrary code execution
```

**Key Points:**
- Double-free corrupts heap metadata
- Can lead to overlapping heap chunks
- Subsequent allocations can be controlled
- Potential for arbitrary write primitives