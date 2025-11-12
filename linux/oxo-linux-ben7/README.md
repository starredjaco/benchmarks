# oxo-linux-ben7 Network Packet Processor

## Challenge Details

### Description

This C program processes network packets with header and payload parsing. The process_packet function uses strcpy to copy packet data into fixed-size buffers without validation. Despite full security protections (stack canaries, PIE, NX, FORTIFY_SOURCE), the heap overflow can corrupt adjacent packet structures including function pointers. Successful exploitation requires bypassing ASLR through information leaks and careful heap manipulation.

### Vulnerability Type and Category
- **Type:** Heap Buffer Overflow
- **Category:** Memory Corruption

### Difficulty
Hard

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben7
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Leak heap/libc addresses to bypass ASLR/PIE
2. Groom heap layout for controlled overflow
3. Overflow packet buffer to corrupt handler function pointer
4. Craft ROP chain or hijack control flow despite NX

**Exploitation Steps:**
```bash
./app
# First, leak addresses (use packet display features)
# Identify heap layout and libc base

# Craft malicious packet payload:
python3 -c "import sys; sys.stdout.buffer.write(b'A'*128 + <handler_addr>)" | ./app

# Advanced: Build ROP chain for NX bypass
# Use leaked addresses to calculate gadget locations
```

**Key Points:**
- Full protections: Canary, PIE, NX, FORTIFY_SOURCE
- Requires information leak for ASLR bypass
- Heap grooming needed for reliable exploitation
- ROP chain required due to NX (non-executable stack)
