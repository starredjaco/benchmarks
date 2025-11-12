CC = gcc
CFLAGS = -fno-stack-protector -no-pie -g -O0 -no-pie
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Data Processor (Easy)"
	@echo "Protections: DISABLED (no canary, no PIE)"
	@echo ""
	@echo "Usage: ./app <input_value>"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
