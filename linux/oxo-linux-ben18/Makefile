CC = gcc
CFLAGS = -fno-stack-protector -no-pie -g -O0 -Wno-uninitialized -no-pie
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "State Machine (Easy)"
	@echo "Protections: DISABLED (no canary, no PIE)"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
