#pragma once
#include <iostream>
template <typename T>
class List {
	struct Node {
		T data;
		Node *next;
		Node(T i) :data(i), next(nullptr) {};
		Node(T i, int _pos) :data(i), next(nullptr) {};

	};
public:
	List() :head() {}
	void insert(T x); //Добавление элемента в конец списка
	void insert(T x, int pos); // Добавление элемента на позицию @pos - позиция
	T pop(); //DEL
	void del(); // Удаление последнего элемента
	void del(int pos); // Удаление элемента @pos - позиция элемента
	void clear(); // Отчистка списка
	T get(int pos); // Получение элемента @pos - позиция
	void print();
	void binWrite(); // запись в бинарный файл
	void binRead(); //чтение с бинарного файла

	void setFileName(std::string _fileName);
	std::string getFileName();
	void sort(); // сортировка

private:
	Node * firstItem;
	Node *head;
	int size = 0;
	void checkPos(int pos);
	std::string fileName = "list.bin"; // Имя файла в который будет записан список
};


//#include "List.h"

template <typename T>
void List<T>::insert(T x) {
	Node *n = new Node(x);
	if (head == nullptr) {
		firstItem = n;
		head = n;
	}
	else {
		head->next = n;
		head = n;
	}
	size++;

}

template<typename T>
inline void List<T>::insert(T x, int pos)
{
	Node *newNode = new Node(x);
	Node *n = firstItem;
	int a = 0;

	try {
		checkPos(pos);
	}

	catch (int e) {
		std::cout << "IndexOutOfBoundsException: pos: " << pos << " size: " << size << std::endl;
		throw 1;
	}

	if (pos != 0) {
		for (; n; n = n->next) {
			if (a + 1 == pos) break;
			a++;
		}
		newNode->next = n->next;
		n->next = newNode;
	}
	else {
		newNode->next = n;
		firstItem = newNode;
	}
	size++;
}
template <typename T>
T List<T>::pop() {
	T a = head->data;
	head = head->next;
	return a;
}

template <typename T>
void List<T>::print() {
	if (firstItem == nullptr) std::cout << "List is empty";
	else {
		Node *n;
		for (n = firstItem; n; n = n->next) {
			std::cout << n->data << ' ';
		}
	}
	std::cout << std::endl;
}

template<typename T>
inline void List<T>::checkPos(int pos) {
	if (pos > size) throw 1;
}

template <typename T>
void List<T>::clear() {
	Node *n;
	for (n = firstItem; n; n = n->next) {
		firstItem = firstItem->next;
	}
	head = nullptr;
	firstItem = nullptr;
}

template <typename T>
void List<T>::del() {
	head = head->next;
	size--;
}

template <typename T>
T List<T>::get(int x) {
	Node *n;
	int a = 0;
	for (n = head; n; n = n->next) {
		if (a++ == x) return n->data;
	}
	return T();
}

