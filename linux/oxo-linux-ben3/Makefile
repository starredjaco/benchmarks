CC = gcc
CFLAGS = -fno-stack-protector -no-pie -g -O0 -Wno-format-security -no-pie
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Logging System (Easy)"
	@echo "Protections: DISABLED (no canary, no PIE, format security off)"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
