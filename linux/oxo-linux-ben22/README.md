# oxo-linux-ben22 Record Management System

## Challenge Details

### Description

This C++ program implements a record management system with polymorphic Record, CustomerRecord, and TransactionRecord classes. The handleRecord function performs C-style casting based on a user-supplied type parameter and directly calls type-specific methods. When a TransactionRecord is handled as a CustomerRecord, the transaction's amount integer field is interpreted as the CustomerRecord's notifyCallback function pointer, leading to arbitrary code execution when notify() is called.

### Vulnerability Type and Category
- **Type:** Type Confusion
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C++ program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben22
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Create a TransactionRecord object
2. Handle the transaction as a customer record
3. TransactionRecord's amount field overlaps with CustomerRecord's function pointer
4. notify() method calls corrupted function pointer leading to arbitrary execution

**Exploitation Steps:**
```bash
./app
# Create a transaction record
# Handle as customer to trigger type confusion
# Type confusion causes function pointer corruption
# notify() call executes arbitrary code

# Expected: Segmentation fault or code execution
```

**Key Points:**
- C-style casts bypass runtime type checking
- CustomerRecord has function pointer, TransactionRecord has integer at same offset
- Direct method call (notify()) triggers function pointer dereference
- No stack protections or PIE enabled
- Executable stack allows shellcode injection
