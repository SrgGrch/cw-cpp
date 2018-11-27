#include <conio.h>
#include "List.h"

void main() {

	List<char> l1;

	l1.push('a');
	l1.push('v');
	l1.push('c');
	l1.print();

	_getch();
}