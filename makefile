PROJECTS := l1

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Ilib -O3

SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

BUILD_DIR := ./build

all: $(PROJECTS)

$(PROJECTS):
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $(wildcard $@/src/*.cpp) -o $(BUILD_DIR)/$@ $(SFMLFLAGS)

clean:
	@echo "Cleaning build directories..."
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean $(PROJECTS)
