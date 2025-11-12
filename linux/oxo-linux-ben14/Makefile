CC = gcc
CFLAGS = -fstack-protector -no-pie -g -O0 -no-pie
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Simple Shell (Medium)"
	@echo "Protections: Stack canary ENABLED, no PIE"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
