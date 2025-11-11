# Compiler settings
CC = gcc
CFLAGS = -m64 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -no-pie -g
LDFLAGS = -m64 -no-pie -Wl,-z,relro

# Target executable
TARGET = vulnerable

# Source files
SOURCES = vulnerable.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)
	@echo "Build complete!"
	@echo ""
	@echo "Protections status:"
	@echo "  - Stack canaries (SSP): ENABLED (strong)"
	@echo "  - NX (Non-Executable stack): ENABLED (default)"
	@echo "  - PIE: DISABLED (-no-pie) for partial ASLR"
	@echo "  - RELRO: Partial (lazy binding exploitable)"
	@echo "  - FORTIFY_SOURCE: ENABLED (runtime checks)"
	@echo "  - 64-bit mode (more complex exploitation)"
	@echo ""
	@echo "ASLR Configuration:"
	@echo "  - Current ASLR status: $$(cat /proc/sys/kernel/randomize_va_space 2>/dev/null || echo 'unknown')"
	@echo "  - For exploitation practice, ASLR should be enabled (value=2)"
	@echo "  - To enable: sudo sysctl -w kernel.randomize_va_space=2"
	@echo ""
	@echo "To verify protections:"
	@echo "  checksec $(TARGET)"
	@echo "  readelf -l $(TARGET) | grep GNU_STACK"
	@echo ""
	@echo "Usage Examples:"
	@echo "  ./$(TARGET) leak"
	@echo "  ./$(TARGET) overflow"

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(TARGET) $(OBJECTS)
	@echo "Cleaned build artifacts"

# Rebuild everything
rebuild: clean all

# Test with safe input
test:
	@echo "Testing leak command..."
	echo "test" | ./$(TARGET) leak

# Show ROP gadgets
show-gadgets:
	@echo "Searching for useful ROP gadgets..."
	@command -v ROPgadget >/dev/null 2>&1 && ROPgadget --binary $(TARGET) | grep -E "(pop|ret)" | head -20 || echo "ROPgadget not installed. Install with: pip install ROPgadget"

# Check security properties
check-security:
	@echo "=== Security Check ==="
	@command -v checksec >/dev/null 2>&1 && checksec $(TARGET) || echo "checksec not installed"
	@echo ""
	@echo "Stack executable check:"
	@readelf -l $(TARGET) | grep -A 1 GNU_STACK || echo "Could not check stack permissions"
	@echo ""
	@echo "ASLR status:"
	@cat /proc/sys/kernel/randomize_va_space

.PHONY: all clean rebuild test show-gadgets check-security
