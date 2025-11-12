CC = gcc
CFLAGS = -fstack-protector -pie -fPIE -D_FORTIFY_SOURCE=2 -g -O0 -pie
TARGET = app
SRC = app.c

all: $(TARGET)
	@echo "Variable Expansion Shell (Hard)"
	@echo "Protections: FULL (canary, PIE/ASLR, NX, FORTIFY)"
	@echo ""
	@echo "Usage: ./app"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
