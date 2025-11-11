CC = gcc
CFLAGS = -m32 -fno-stack-protector -z execstack -no-pie -g
LDFLAGS = -m32 -z execstack -no-pie

TARGET = vulnerable
SOURCES = vulnerable.c

all: $(TARGET)
	@echo "Stack Overflow - Easy level built"
	@echo "Protections: ALL DISABLED"
	@echo "Run: ./$(TARGET) \$$(python3 -c 'print(\"A\"*80)')"

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
