CC = gcc
CFLAGS = -fstack-protector -g -O0 
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Message Queue System (Medium)"
	@echo "Protections: Stack canary ENABLED"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
