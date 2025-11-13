# oxo-linux-ben24 Extension Management Framework

## Challenge Details

### Description

This C++ program implements an extension management framework using templates and polymorphism. The ExtensionManager archives extension payloads to void* buffers and provides a template-based restore<T>() method. The vulnerability occurs when the template parameter differs from the original extension type - ConfigPayload data can be restored as ActionPayload, causing field misalignment where ConfigExtension's integer level field overlaps with ActionExtension's function pointer, enabling type confusion exploitation through template instantiation.

### Vulnerability Type and Category
- **Type:** Type Confusion
- **Category:** Memory Corruption

### Difficulty
Hard

## Build instructions
This is a C++ program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben24
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Add a ConfigExtension with controlled field values
2. Use template-based restore to load as ActionExtension
3. ConfigPayload's integer field overlaps with ActionPayload's function pointer
4. Process the action to execute corrupted function pointer

**Exploitation Steps:**
```bash
./app
# Add config extension, restore as action extension
# Template type mismatch causes payload reinterpretation
# Function pointer corruption leads to arbitrary execution

# Expected: Crash or code execution
```

**Key Points:**
- Template-based deserialization appears type-safe
- Full protections: Canary + PIE + FORTIFY
- Requires information leak for ASLR bypass
- Hard to spot: no obvious casts, modern C++ patterns
