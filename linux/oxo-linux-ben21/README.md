# oxo-linux-ben21 Event Dispatcher

## Challenge Details

### Description

This C program implements an event dispatching system with subscriber callbacks. The unsubscribe_event function frees an EventSubscriber structure but does not remove it from the subscribers list or NULL the pointer. When dispatch_event is called after unsubscription, it iterates through the list and dereferences freed subscriber pointers. With full protections enabled, exploitation requires information leaks and heap manipulation to control the freed memory contents.

### Vulnerability Type and Category
- **Type:** Use-After-Free
- **Category:** Memory Corruption

### Difficulty
Hard

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben21
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Subscribe multiple event handlers
2. Unsubscribe (frees subscriber but keeps in list)
3. Groom heap to control freed memory
4. Dispatch event - iterates over freed subscribers
5. Bypass ASLR/PIE through information leak

**Exploitation Steps:**
```bash
./app
# Step 1: Leak addresses for ASLR bypass
# Step 2: Subscribe handlers
# Step 3: Unsubscribe (frees but stays in list)
# Step 4: Allocate controlled data in freed region
# Step 5: Dispatch event

# Freed EventSubscriber contains attacker-controlled callback
# Full protections require advanced heap manipulation
```

**Key Points:**
- Full protections: Canary + PIE + NX + FORTIFY
- Freed subscriber remains in active list
- Requires information leak for ASLR bypass
- Heap grooming essential for control
- ROP chain needed for NX bypass
