# oxo-linux-ben23 Data Management System

## Challenge Details

### Description

This C++ program implements a data management system that serializes objects to void* buffers. The DataStore's retrieve function allows callers to specify a type_hint parameter that overrides the actual stored type during deserialization. When UserAccount data is loaded with type_hint=2 (TaskItem), the account's fields are reinterpreted according to TaskItem's memory layout, causing the account data to overlap with the TaskItem's function pointer field, enabling arbitrary code execution.

### Vulnerability Type and Category
- **Type:** Type Confusion
- **Category:** Memory Corruption

### Difficulty
Medium

## Build instructions
This is a C++ program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben23
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Save UserAccount with controlled username/email fields
2. Use retrieve function with type_hint=2 to load as TaskItem
3. UserAccount data overlaps with TaskItem's callback pointer
4. Trigger run() method to execute corrupted function pointer

**Exploitation Steps:**
```bash
./app
# Save a user account, then load it as a task
# Type confusion causes memory layout mismatch
# Function pointer executes arbitrary code

# Expected: Segmentation fault or code execution
```

**Key Points:**
- void* serialization loses type information
- type_hint parameter enables type override
- Stack canary protection requires heap exploitation
- UserAccount fields overlap with TaskItem function pointer
