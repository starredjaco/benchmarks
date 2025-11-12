# oxo-linux-ben14 Simple Shell

## Challenge Details

### Description

This C program implements a simple command shell that processes user commands. The process_command function uses strcpy to copy command input into a fixed-size stack buffer. With stack canaries enabled but PIE disabled, exploiting this stack overflow requires either overwriting other stack variables before the canary or finding a way to leak or bypass the canary value.

### Vulnerability Type and Category
- **Type:** Stack Buffer Overflow
- **Category:** Memory Corruption

### Difficulty
Medium

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben14
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Leak stack canary value through information disclosure
2. Craft overflow payload preserving canary
3. Overwrite return address with leaked/calculated value
4. Build ROP chain for NX bypass (if needed)

**Exploitation Steps:**
```bash
./app
# First, attempt to leak canary (if possible)
# Or brute-force canary on 32-bit (1/256 per byte)

# Craft payload: buffer + canary + saved_rbp + return_addr
python3 -c "print('A'*64 + '\x00\x01\x02\x03\x04\x05\x06\x07' + 'B'*8 + '\x41\x42\x43\x44')" | ./app

# PIE disabled: Addresses are fixed, simplifies exploitation
```

**Key Points:**
- Stack canary enabled (requires leak or brute-force)
- PIE disabled (fixed addresses)
- strcpy() allows buffer overflow
- Must preserve canary to reach return address
