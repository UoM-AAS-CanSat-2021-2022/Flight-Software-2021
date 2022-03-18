SRC_FILES := $(wildcard src/*.cpp)
IGNORED_SRC_FILES = src/main.cpp src/sout.cpp
SRC_FILES := $(filter-out $(IGNORED_SRC_FILES), $(SRC_FILES))
CXXFLAGS := -Wall -Wextra -O2 -std=c++17 -pedantic -g -D TEAM_NO=1057 -I src/ 
HEADER_FILES := $(wildcard src/*.hpp)

testing: $(SRC_FILES) $(HEADER_FILES) test/testing.cpp 
	$(CXX) $(CXXFLAGS) $(SRC_FILES) test/testing.cpp -o testing

run: testing
	./testing

clean:
	rm -rf testing

.PHONY: clean, run
