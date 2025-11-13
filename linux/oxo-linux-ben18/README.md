# oxo-linux-ben18 State Machine

## Challenge Details

### Description

This C program implements a state machine with transition handlers. The current_state variable should be initialized before use, but in certain code paths it remains uninitialized. When process_event is called with an uninitialized state, the function pointer in the states array is read from random stack or memory contents. Calling this uninitialized function pointer can lead to arbitrary code execution or crashes.

### Vulnerability Type and Category
- **Type:** Uninitialized Memory
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben18
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Trigger code path that skips state initialization
2. Process event with uninitialized current_state value
3. Function pointer read from random memory
4. Execution redirected to arbitrary address

**Exploitation Steps:**
```bash
./app
# Follow code path that skips initialization
# Select: 2 (Process Event) before initialization
# current_state contains random stack value
# states[random_value] returns random function pointer
# Call to random address

# Expected: Crash or potential code execution if address is controlled
```

**Key Points:**
- current_state not initialized in all code paths
- Random value used as array index
- Function pointer from uninitialized memory
- Potential for controlled execution if stack can be groomed
