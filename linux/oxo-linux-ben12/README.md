# oxo-linux-ben12 Bank Account System

## Challenge Details

### Description

This C program simulates a multi-threaded bank account system with deposit and withdrawal operations. Two threads simultaneously access and modify the same account balance without proper synchronization. The race condition between reading the balance, performing the calculation, and writing it back can lead to lost updates, where one thread's changes are overwritten by another, resulting in incorrect account balances.

### Vulnerability Type and Category
- **Type:** Race Condition
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben12
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Trigger concurrent deposit/withdrawal operations
2. Exploit TOCTOU (Time-of-Check-Time-of-Use) window
3. Cause lost updates or inconsistent balance state
4. Potentially double-spend or create money

**Exploitation Steps:**
```bash
./app
# Program spawns two threads operating on same account
# Thread 1: Deposits $1000
# Thread 2: Withdraws $500
# Race condition causes lost update

# Run multiple times to observe inconsistent results
# Expected balances vary due to race condition
```

**Key Points:**
- No mutex/lock protection on shared balance
- TOCTOU vulnerability in read-modify-write
- Lost updates cause incorrect balances
- Multi-threaded exploitation (non-deterministic)
