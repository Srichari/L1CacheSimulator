CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17

SRC = src/L1simulate.cpp src/Cache.cpp src/Bus.cpp
OBJ = $(SRC:.cpp=.o)
HDR = include/Cache.hpp include/Bus.hpp
TARGET = L1simulate

INC = -Iinclude

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
