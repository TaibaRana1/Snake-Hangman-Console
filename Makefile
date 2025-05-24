# Define the compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall
SFML_LIBS = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

# Define the target executable
TARGET = main

# Build target
all: $(TARGET)

# Rule to build the executable
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp $(SFML_LIBS)

# Clean up build files
clean:
	rm -f $(TARGET)
