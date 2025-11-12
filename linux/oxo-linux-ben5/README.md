# oxo-linux-ben5 User Profile Manager

## Challenge Details

### Description

This C program manages user profiles with dynamically allocated username and email fields. The update_profile function uses strcpy to copy user input without bounds checking. Since the username and email buffers are allocated on the heap adjacent to the UserProfile structure containing a function pointer, overflowing these buffers can corrupt the notification callback pointer, redirecting execution when notifications are triggered.

### Vulnerability Type and Category
- **Type:** Heap Buffer Overflow
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben5
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Create a user profile (triggers heap allocations)
2. Update profile with oversized username/email
3. Overflow heap buffer to corrupt adjacent function pointer
4. Trigger notification callback to execute attacker-controlled code

**Exploitation Steps:**
```bash
./app
# Select: 1 (Create Profile)
# Select: 2 (Update Profile)
# Enter long username: AAAAA... (200+ bytes)
# Enter long email: BBBBB... (200+ bytes)
# Select: 3 (Send Notification) - triggers corrupted callback

# Expected: Control flow hijack via corrupted function pointer
```

**Key Points:**
- strcpy() without bounds checking
- Heap layout: buffer adjacent to function pointer
- Default heap protections only
- Notification callback provides execution trigger
