# oxo-linux-ben15 Variable Expansion Shell

## Challenge Details

### Description

This C program implements a shell with variable expansion capabilities. The expand_variables function performs string substitution by copying parts of the input into a stack buffer. When variable values are long or multiple variables are expanded, the combined output can overflow the stack buffer. With full protections enabled (canaries, PIE, NX, FORTIFY), exploitation requires sophisticated techniques like information leaks and ROP chains.

### Vulnerability Type and Category
- **Type:** Stack Buffer Overflow
- **Category:** Memory Corruption

### Difficulty
Hard

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben15
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Leak stack canary and PIE base addresses
2. Overflow stack buffer via variable expansion
3. Preserve canary in overflow payload
4. Build ROP chain for NX bypass using leaked addresses

**Exploitation Steps:**
```bash
./app
# Step 1: Leak canary and addresses
# Use format string or info leak if available

# Step 2: Craft variable expansion overflow
# Example: $VAR1$VAR2$VAR3 with long values

# Step 3: Build ROP chain
# Calculate gadget addresses using leaked PIE base
# Chain: pop_rdi; "/bin/sh"; system

# Advanced exploitation required for full protections
```

**Key Points:**
- Full protections: Canary + PIE + NX + FORTIFY
- Requires information leak (canary + addresses)
- ROP chain needed for code execution (NX)
- PIE makes addresses randomized (need leak)
