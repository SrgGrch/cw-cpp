#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include "List.h"
#define type char*

List<type> l;


void printMenu() {
	std::cout << "BinList 1.0" << std::endl;
	std::cout << "Developed by Sergey Grechishnikov (SrgGrch), AVT-710" << std::endl;
	std::cout << "Usage: command [Options], e.g. add 2" << std::endl << std::endl;
	std::cout << "	add <elem>			inserts element in List" << std::endl;
	std::cout << "	addI <elem, pos>		inserts element in List in a specific position" << std::endl;
	std::cout << "	del <pos>			removes element from list " << std::endl;
	std::cout << "	size				shows List size" << std::endl;
	std::cout << "	clear				clears List" << std::endl;
	std::cout << "	sort				sorts List" << std::endl;
	std::cout << "	print				prints List" << std::endl;
	std::cout << "	exit				exits from program" << std::endl;
	std::cout << "	help				prints help (this menu)" << std::endl;
	std::cout << "	cls				clear screen" << std::endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------" << std::endl;
}

void checkInp(std::string inp) {
	if (inp == "add") {
		std::string arg;
		std::cin >> arg;
		type tmp = new char[arg.length() + 1];;
		strcpy(tmp, arg.c_str());
		l.insert(tmp);
	} else if (inp == "addI") {
		std::string tmp;
		std::cin >> tmp;
		type arg1 = new char[tmp.length() + 1];;
		int arg2;
		std::cin >> arg2;
		strcpy(arg1, tmp.c_str());
		l.insert(arg1, arg2);
	} else if (inp == "del") {
		int arg;
		std::cin >> arg;
		l.del(arg);
	} else if (inp == "size") {
		std::cout << l.getSize() << std::endl;
	} else if (inp == "clear") {
		l.clear();
	} else if (inp == "sort") {
		l.sort();
	} else if (inp == "print") {
		l.print();
	} else if (inp == "help") {
		printMenu();
	} else if (inp == "cls") {
		system("cls");
	} else if (inp == "chksz") {
		l.getSizeFromFile();
	}
}

void run() {
	std::string inp;
	printMenu();
	std::cout << ">>> ";
	std::cin >> inp;
	while (inp != "exit") {
		checkInp(inp);
		std::cout << ">>> ";
		std::cin >> inp;
	}
}

int main() {
	//char* st = new char[];
	run();
	return 0;
}