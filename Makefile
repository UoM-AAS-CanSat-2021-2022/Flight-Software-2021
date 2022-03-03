test: src/testing.cpp
	$(CXX) -Wall -Wextra -O2 -std=c++17 -pedantic -g src/testing.cpp -o test 

clean:
	rm -rf test

.PHONY: clean

