# oxo-linux-ben9 Buffer Manager

## Challenge Details

### Description

This C program manages a buffer system with allocation and validation logic. The validate_buffer function checks if a buffer pointer is valid by comparing it against allocated buffer pointers. However, it fails to verify that the pointer actually points to the start of an allocated buffer. An attacker can pass a pointer to the middle of an allocated buffer or to stack/data section memory, bypassing validation and potentially causing the program to operate on invalid memory regions.

### Vulnerability Type and Category
- **Type:** Invalid Pointer Dereference
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben9
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Pass pointer to middle of allocated buffer (bypasses validation)
2. Trigger operations on invalid memory regions
3. Corrupt adjacent data structures or cause information leak
4. Leverage invalid operations for code execution

**Exploitation Steps:**
```bash
./app
# Allocate buffer, note address
# Provide address + offset (middle of buffer) to validation
# Validation passes incorrectly
# Operations on invalid memory region

# Can point to stack, .data section, or arbitrary memory
```

**Key Points:**
- Insufficient pointer validation
- Can access unintended memory regions
- Stack/data section pointers bypass checks
- Leads to memory corruption or information disclosure
