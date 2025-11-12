# oxo-linux-ben3 Logging System

## Challenge Details

### Description

This C program implements a logging system that processes user input and system events. The log_message function directly passes user-controlled input as the format string to printf. This format string vulnerability allows attackers to read from or write to arbitrary memory locations using format specifiers like %x, %s, or %n, potentially leading to information disclosure or code execution.

### Vulnerability Type and Category
- **Type:** Format String
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben3
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Use format string specifiers to leak stack/memory contents
2. Use %n to write arbitrary values to memory locations
3. Overwrite GOT entries or return addresses for code execution

**Exploitation Steps:**
```bash
./app
# At log message prompt, enter format string payloads:

# Leak stack contents:
%p.%p.%p.%p.%p.%p.%p.%p

# Read from arbitrary address:
%s

# Write to memory (count of bytes printed):
%n

# Advanced: Overwrite GOT entry
# Craft payload with address + %n to overwrite function pointer
```

**Key Points:**
- Direct user input used as printf format string
- Can read arbitrary memory with %s, %x
- Can write arbitrary values with %n
- No format string protections (-Wno-format-security)
