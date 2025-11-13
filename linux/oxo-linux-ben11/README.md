# oxo-linux-ben11 Memory Writer

## Challenge Details

### Description

This C program manages a collection of items stored in a fixed-size array. The update_item function allows updating items by index but only checks if the index is less than MAX_ITEMS (100), not if it's within the actual number of items stored. This allows writing to uninitialized array positions beyond the valid items, potentially corrupting adjacent memory or causing undefined behavior.

### Vulnerability Type and Category
- **Type:** Out-of-Bounds Write
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben11
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Add small number of items to array
2. Write to indices beyond stored items but within bounds
3. Corrupt adjacent memory structures
4. Overwrite function pointers or return addresses

**Exploitation Steps:**
```bash
./app
# Add 2 items (indices 0-1)
# Update item at index 90
# Writes beyond valid items
# Corrupts adjacent memory

# Can overwrite critical data structures
```

**Key Points:**
- OOB write to uninitialized array elements
- Corrupts memory beyond valid items
- Can overwrite adjacent structures
- Potential for code execution via corrupted pointers
