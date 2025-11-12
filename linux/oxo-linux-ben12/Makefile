CC = gcc
CFLAGS = -g -O0 -pthread -pthread
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Bank Account System (Easy)"
	@echo "Protections: Default (multi-threaded)"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
