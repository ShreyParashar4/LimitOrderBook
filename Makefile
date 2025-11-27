# Compiler variables
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Target executable name
TARGET = engine

# Build rule
all:
	$(CXX) $(CXXFLAGS) main.cpp OrderBook.cpp -o $(TARGET)

# Clean rule (delete generated files)
clean:
	rm -f $(TARGET)