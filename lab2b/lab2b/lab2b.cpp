#include <iostream>
#include "struct.h"
#include "print.h"

int main() {
	int mode;
	cout << "1-INTERACT" << endl;
	cout << "2-DEMO"<< endl;
	cout << "3-BENCHMARK" << endl;
	cin >> mode;

	if (mode == 1) interactive_mode();
	else if (mode == 2) demo_mode();
	else if (mode == 3) benchmark_mode();
	else cout << "Завершення роботи" << endl;

	return 0;
}