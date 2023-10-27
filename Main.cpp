#include <iostream>
#include "Matrix.h"
using namespace std;
using namespace DataContainers;

int main() {
	auto m = Matrix<double>::Parse("1 2 3\n 4 5 6\n 7 8 9");
	m.Print();
}
