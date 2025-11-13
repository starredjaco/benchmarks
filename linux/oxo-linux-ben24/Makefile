CXX = g++
CXXFLAGS = -fstack-protector-strong -pie -fPIE -D_FORTIFY_SOURCE=2 -w -std=c++11
TARGET = app
SRC = app.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
