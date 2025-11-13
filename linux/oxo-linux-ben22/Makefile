CXX = g++
CXXFLAGS = -fno-stack-protector -z execstack -no-pie -w
TARGET = app
SRC = app.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
