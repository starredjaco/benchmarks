# oxo-linux-ben20 Message Queue System

## Challenge Details

### Description

This C program implements a message queue with enqueue and dequeue operations. The dequeue_message function frees a message node but returns a pointer to the message data that was part of the freed structure. The caller continues to use this freed memory through process_message. With stack canaries enabled, exploitation requires carefully timing the use-after-free with heap allocations to control the freed memory contents.

### Vulnerability Type and Category
- **Type:** Use-After-Free
- **Category:** Memory Corruption

### Difficulty
Medium

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben20
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Enqueue messages to allocate queue nodes
2. Dequeue message (frees node but returns pointer to freed data)
3. Trigger allocation to reuse freed chunk
4. Access freed message pointer - now contains attacker data

**Exploitation Steps:**
```bash
./app
# Select: 1 (Enqueue message)
# Select: 2 (Dequeue message) - frees node, returns data ptr
# Select: 3 (Perform allocation) - reuses freed memory
# Program continues to use freed message pointer
# Freed memory now controlled by attacker

# Stack canary enabled: Must exploit via heap only
```

**Key Points:**
- Freed memory pointer still used
- Stack canary requires heap-only exploitation
- Heap grooming needed for reliable control
- Timing critical for successful exploitation
