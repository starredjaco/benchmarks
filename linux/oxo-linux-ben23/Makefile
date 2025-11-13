CXX = g++
CXXFLAGS = -fstack-protector -no-pie -w
TARGET = app
SRC = app.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
