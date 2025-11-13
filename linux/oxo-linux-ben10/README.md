# oxo-linux-ben10 Memory Reader

## Challenge Details

### Description

This C program manages an array of records with fixed bounds checking. The get_record function checks if an index is within the array bounds (0-99) but not if it's within the actual number of records stored. This allows reading uninitialized or stale data from the array beyond the valid records, potentially leaking sensitive information from previous allocations or uninitialized memory regions.

### Vulnerability Type and Category
- **Type:** Out-of-Bounds Read
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben10
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Add small number of records to array
2. Read indices beyond stored records but within bounds
3. Leak uninitialized memory or stale data
4. Extract sensitive information (addresses, keys, previous data)

**Exploitation Steps:**
```bash
./app
# Add 2 records (indices 0-1)
# Request record at index 50
# Reads uninitialized array memory
# Potential information leak of stack/heap data

# Extract leaked data for ASLR bypass or sensitive info
```

**Key Points:**
- OOB read of uninitialized array elements
- Leaks memory contents beyond valid records
- Can reveal addresses for ASLR bypass
- Information disclosure vulnerability
