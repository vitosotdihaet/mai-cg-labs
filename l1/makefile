CXX := g++
CXXFLAGS := -std=c++17 -Wall

SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -s

BUILD_DIR := ../build

main:
	$(CXX) $(CXXFLAGS) -I./lib $(wildcard ./src/*.cpp) -o $(BUILD_DIR)/l1 $(SFMLFLAGS)

clean:
	rm -rf $(BUILD_DIR)/*

