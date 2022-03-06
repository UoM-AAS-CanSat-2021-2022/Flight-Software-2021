#include <iostream>

struct a_thing {
	long long int a_big_thing;
	bool a_small_thing;
};

a_thing make_a_thing() {
	return { 100000, false };
}

int main() {
	std::cout
		<< "Testing.\n"
		<< "Testing.\n"
		<< "1.\n"
		<< "2.\n"
		<< "3.\n";

	std::cout << "Ooh yeah lets get some structured bindings in there." << std::endl;
	auto [big, small] = make_a_thing();

	std::cout << "big: " << big << std::endl;
	std::cout << "small: " << small << std::endl;

	std::cout << "C++ 17 is amazing!!!" << std::endl;

	return 0;
}
