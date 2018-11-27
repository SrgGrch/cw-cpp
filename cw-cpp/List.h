#pragma once
#include <iostream>
template <typename T>
class List {
	struct Node {
		T data;
		Node *next;
		int pos = 0;	
		Node(T i) :data(i), next(0), pos(0) {};
		Node(T i, int _pos) :data(i), next(0), pos(_pos) {};

	};

public:
	List() :head() {}
	void push(T x);
	T pop();
	void del(); // Удаление последнего
	void clear(); // Удаление всего
	T find(int x); // Поиск по позиции
	void print();
	//private:
	Node *head;
};


//#include "List.h"

template <typename T>
void List<T>::push(T x) {
	Node *n;
	if (head == nullptr) n = new Node(x);
	else n = new Node(x, head->pos++);
	n->next = head;
	head = n;
}
template <typename T>
T List<T>::pop() {
	T a = head->data;
	head = head->next;
	return a;
}

template <typename T>
void List<T>::print() {
	if (head == 0) std::cout << "List is empty";
	else {
		Node *n;
		for (n = head; n; n = n->next) {
			std::cout << n->data << ' ';
		}
	}
	std::cout << std::endl;
}

template <typename T>
void List<T>::clear() {
	Node *n;
	for (n = head; n; n = n->next) {
		head = head->next;
	}
}

template <typename T>
void List<T>::del() {
	head = head->next;
}

template <typename T>
T List<T>::find(int x) {
	Node *n;
	for (n = head; n; n = n->next) {
		if (n->pos == x) return n->data;
	}
	return T();
}

