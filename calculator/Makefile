CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
TARGET   = calculator
SRC      = calculator.cpp
OBJ      = $(SRC:.cpp=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./test.sh

clean:
	rm -f $(TARGET) $(OBJ)
