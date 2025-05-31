CXX = g++
CXXFLAGS = -Wall -Iinclude -fPIC
LDFLAGS = -L. -lblock -Wl,-rpath=.

SRC_DIR = src
INCLUDE_DIR = include

# Object files from src/*.cpp
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,%.o,$(wildcard $(SRC_DIR)/*.cpp))

# Executables from main_*.cpp and interactive_main.cpp
MAIN_SRCS = $(wildcard main_*.cpp interactive_main.cpp)
EXECUTABLES = $(patsubst %.cpp,%,$(notdir $(MAIN_SRCS)))

SO_FILE = libblock.so

.PHONY: all clean

# Main build target
all: $(SO_FILE) $(EXECUTABLES)
	@echo "Build complete."

# Shared library
$(SO_FILE): $(OBJ_FILES)
	$(CXX) -shared -o $@ $^

# Object files
%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Executables
%: %.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Clean rule (also removes block_data/blocks.txt)
clean:
	rm -f *.o *.so $(EXECUTABLES) *.out a.csv block_data/blocks.txt

