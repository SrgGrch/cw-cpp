#include <conio.h>
#include "List.h"

int main() {

	List<int> l1;

	l1.insert(1);
	
	l1.insert(3);
	l1.insert(4);
	l1.insert(2, 3);

    //std::cout << l1.get(3);

	l1.print();

	_getch();
	return 0;
}