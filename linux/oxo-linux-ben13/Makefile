CC = gcc
CFLAGS = -fno-stack-protector -z execstack -no-pie -g -O0 -no-pie -z execstack
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "User Management System (Easy)"
	@echo "Protections: DISABLED (no canary, no PIE, executable stack)"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
