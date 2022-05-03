SRC_FILES := $(wildcard src/*.cpp)
SRC_FILES := $(filter-out src/main.cpp, $(SRC_FILES))

testing: test/testing.cpp $(SRC_FILES)
	$(CXX) -Wall -Wextra -O2 -std=c++17 -pedantic -g $^ -I src/ -o testing

clean:
	rm -rf testing

.PHONY: clean

