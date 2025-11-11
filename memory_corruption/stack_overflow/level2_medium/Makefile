CC = gcc
CFLAGS = -m32 -fstack-protector -z execstack -no-pie -g -Wno-format-security
LDFLAGS = -m32 -z execstack -no-pie

TARGET = vulnerable
SOURCES = vulnerable.c

all: $(TARGET)
	@echo "Stack Overflow - Medium level built"
	@echo "Protections: Stack canary ENABLED"
	@echo "Leak canary: ./$(TARGET) leak '%p %p %p %p %p'"
	@echo "Exploit: ./$(TARGET) overflow PAYLOAD"

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
