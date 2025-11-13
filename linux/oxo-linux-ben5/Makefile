CC = gcc
CFLAGS = -g -O0 
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "User Profile Manager (Easy)"
	@echo "Protections: Default"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
