# oxo-linux-ben6 Configuration Parser

## Challenge Details

### Description

This C program parses configuration files and stores key-value pairs. The parse_config function reads a configuration file line by line and uses strcpy to copy values into fixed-size buffers in a ConfigEntry structure. When processing long configuration values, the strcpy operation can overflow the 64-byte value buffer, corrupting adjacent heap memory including function pointers in the callback field. With stack canaries enabled, this requires careful heap manipulation.

### Vulnerability Type and Category
- **Type:** Heap Buffer Overflow
- **Category:** Memory Corruption

### Difficulty
Medium

## Build instructions
This is a C program that demonstrates memory corruption vulnerabilities in Linux systems.

Open the benchmark directory.

Use the provided Makefile to compile the program.

```bash
cd oxo-linux-ben6
make
```

Run the compiled application and interact with it to explore the vulnerability.

## Exploitation

**Attack Strategy:**
1. Create malicious config file with long key-value pairs
2. Trigger heap overflow during config parsing
3. Corrupt adjacent heap structures or function pointers
4. Bypass stack canary protection (heap-based overflow)

**Exploitation Steps:**
```bash
# Create malicious config file
echo "setting1=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" > evil.conf

./app evil.conf
# Parser overflows heap buffer, corrupts adjacent structures

# Expected: Heap corruption, potential callback hijack
```

**Key Points:**
- Stack canary enabled (requires heap exploitation)
- strcpy() overflows 64-byte heap buffer
- Adjacent ConfigEntry structures can be corrupted
- Callback function pointers provide execution vector
