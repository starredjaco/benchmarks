# oxo-linux-ben1 Session Manager

## Challenge Details

### Description

This C program implements a session management system that uses function pointers for event handling. The program loads configuration from a file and processes user commands. Due to unsafe string copying operations in the configuration loader and command processor, buffer overflows can corrupt adjacent memory structures, including function pointers in the EventHandlers structure. This allows attackers to redirect program execution flow by overwriting callback functions.

### Vulnerability Type and Category
- **Type:** Control-Data Corruption
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben1
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Create a malicious configuration file with oversized username/password values
2. Trigger buffer overflow to overwrite EventHandlers function pointers
3. Redirect execution flow when event callbacks are invoked

**Exploitation Steps:**
```bash
# Create malicious config with large values to overflow buffers
python3 -c "print('username=' + 'A'*200 + '\npassword=' + 'B'*200)" > exploit.conf

# Run with malicious config - overflows and corrupts function pointers
./app exploit.conf

# Expected: Segmentation fault or arbitrary code execution when callbacks triggered
```

**Key Points:**
- No stack protections (canary disabled, executable stack)
- Function pointers stored adjacent to string buffers
- strcpy() used without bounds checking
- Control flow hijacked through corrupted callbacks
