#include <iostream>
#include "Matrix.h"
using namespace std;
using namespace DataContainers;

int main() {
	srand(time(0));
	Matrix<int64_t> m(20, 20);
	m.randomFill();
	m.print();
	cout << m.determinant();

}
