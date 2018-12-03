#include <conio.h>
#include "List.h"

int main() {

	List<int> l;

	l.insert(1);
	//l.insert(3);
	//l.insert(4);
	//l.clear();

	//l.binMove(1);

	//l.insert(2, 1);

	l.print();

	_getch();
	return 0;
}