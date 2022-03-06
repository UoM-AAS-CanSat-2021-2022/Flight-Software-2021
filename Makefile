SRC_FILES := $(wildcard src/*.cpp)
SRC_FILES := $(filter-out src/main.cpp, $(SRC_FILES))

testing: $(SRC_FILES) test/testing.cpp 
	$(CXX) -Wall -Wextra -O2 -std=c++17 -pedantic -g $^ -I src/ -o testing

run: testing
	./testing

clean:
	rm -rf testing

.PHONY: clean
