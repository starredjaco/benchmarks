# oxo-linux-ben19 Plugin Loader

## Challenge Details

### Description

This C program loads and manages plugins with associated callback functions. The unload_plugin function frees a Plugin structure and its associated memory. However, the global plugin pointer is not set to NULL after freeing. If reload_plugin or execute_plugin is subsequently called, they dereference the freed plugin pointer, leading to a use-after-free vulnerability that can result in code execution if the freed memory is reallocated with attacker-controlled data.

### Vulnerability Type and Category
- **Type:** Use-After-Free
- **Category:** Memory Corruption

### Difficulty
Easy

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben19
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Load plugin (allocates Plugin structure)
2. Unload plugin (frees memory but pointer not NULLed)
3. Allocate new data in freed region (heap grooming)
4. Execute plugin - uses freed memory with attacker data

**Exploitation Steps:**
```bash
./app
# Select: 1 (Load Plugin)
# Select: 2 (Unload Plugin) - frees memory
# Select: 3 (Allocate Something) - reuses freed chunk
# Select: 4 (Execute Plugin) - uses freed memory

# Freed plugin structure now contains attacker-controlled data
# Function pointer in Plugin struct points to attacker code
```

**Key Points:**
- Pointer not NULLed after free
- Dangling pointer remains accessible
- Heap reallocation can control freed memory
- Function pointer in freed struct provides execution
