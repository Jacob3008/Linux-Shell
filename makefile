CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = shell
SRC = shell.cpp
OBJ = $(SRC:.cpp=.o)
THREAD_LIMIT = 1000

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

run: $(TARGET)
	@echo "Setting ulimit -u to $(THREAD_LIMIT) before running the program..."
	@bash -c "ulimit -u $(THREAD_LIMIT) && ./$(TARGET)"

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run