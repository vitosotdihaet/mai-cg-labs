PROJECTS := l1

CXX := g++
CXXFLAGS := -std=c++17 -Wall -O3

SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -s

BUILD_DIR := ./build

all: $(PROJECTS)

$(PROJECTS):
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) -I$@/lib $(wildcard $@/src/*.cpp) -o $(BUILD_DIR)/$@ $(SFMLFLAGS)

clean:
	@echo "Cleaning build directories..."
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean $(PROJECTS)
