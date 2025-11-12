CC = gcc
CFLAGS = -g -O0 
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Resource Manager (Easy)"
	@echo "Protections: Default heap protections"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
