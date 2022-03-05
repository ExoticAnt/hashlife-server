

#include <string>
#include <iostream>
#include "simple_implementation.h"
#include <vector>

using namespace std;


int main(void) {
	string msg_inicial = "Conway's game of life server\n";
	std::size_t x=10, y=10;

	cout << msg_inicial;

	auto universe = Simple_Implementation(x, y);

	cout << universe << "\n";

	universe.randomize();


	char r=0;

	while (r != 'q' && r != 'Q') {
		cout << universe << "\n";
		cin >> r;
		universe.run();
	}

	int c = 0;
	while (c<10000) {
		//cout << universe << "\n";
		//cin >> r;
		universe.run();
		c++;
		if (c % 1000 == 0) {
			cout << "Corrida " << c << "\n";
			cout << universe << "\n";
		}
	}

	return 0;
}