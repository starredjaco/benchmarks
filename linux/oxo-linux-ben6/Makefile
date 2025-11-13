CC = gcc
CFLAGS = -fstack-protector -g -O0 
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Configuration Parser (Medium)"
	@echo "Protections: Stack canary ENABLED"
	@echo ""
	@echo "Usage: ./app config.txt"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
