#pragma once
#include <iostream>
#include <fstream>

const int INDENT = 2;
template <typename T>
class Node {
public:
	T data;
	//Node *next;
	int nextBin;
	Node(T i) :data(i) {};
	Node(T i, int _pos) :data(i) {};
	Node() {};
};

template <typename T>
class LocalNode {
public:
	T data;
	LocalNode<T> *next;
	int nextBin;
	LocalNode(T i) :data(i) {};
	LocalNode(T i, int _pos) :data(i) {};
	LocalNode() {};
};


template <typename T>
class List {
public:
	List() :head() {}
	List(std::string _fileName) : head(), fileName(_fileName) {}
	~List();

	void insert(T x); //Добавление элемента в конец списка 
	void insert(T x, int pos); // Добавление элемента на позицию @pos - позиция *

	void del(); // Удаление последнего элемента *
	void del(int pos); // Удаление элемента @pos - позиция элемента *

	void clear(); // Отчистка списка
	T get(int pos); // Получение элемента @pos - позиция
	void print();
	void sort(); // сортировка

	void binWrite(Node<T> *n); // запись в бинарный файл
	void binWrite(Node<T> *n, int pos); // запись в бинарный файл @pos - позиция на которую нужно записать элемент
	typename Node<T>* binRead(int pos); //чтение с бинарного файла @pos - позиция считываемого узла
	void binMove(int pos); // Перемещает часть списка вправо @pos - позция начального элемента

	void setFileName(std::string _fileName);
	std::string getFileName();




private:
	Node<T> * firstItem;
	Node<T> *head;
	int size = 0;

	void checkPos(int pos);
	void checkSize(); //проверка размера файла

	std::string fileName = "list.bin"; // Имя файла в который будет записан список
};









template<typename T>
inline List<T>::~List()
{
	//remove(fileName);
}

template <typename T>
void List<T>::insert(T x) {
	Node<T> *n = new Node<T>(x);
	if (head == nullptr) {
		firstItem = n;
		n->nextBin = (sizeof(Node<T>))*(size + 1);
		head = n;
		binWrite(head);
	} else {
		n->nextBin = (sizeof(Node<T>))*(size + 1);
		head = n;
		binWrite(head);

	}
	size++;

}

template<typename T>
inline void List<T>::insert(T x, int pos)
{
	Node<T> *n = new Node<T>(x);
	n->nextBin = sizeof(Node<T>)*pos;
	int a = 0;

	try {
		checkPos(pos);
	}

	catch (int e) {
		std::cout << "IndexOutOfBoundsException: pos: " << pos << " size: " << size << std::endl;
		throw 1;
	}
	if (pos != (size - 1)) {
		binMove(pos);
		binWrite(n, pos);
	} else {
		binWrite(n);
	}
	size++;
}


template <typename T>
void List<T>::print() {
	if (size == 0) std::cout << "List is empty";
	else {
		for (int i = 0; i < size; i++) {
			std::cout << get(i) << ' ';
		}
	}
	std::cout << std::endl;
}

template<typename T>
inline void List<T>::binWrite(Node<T> *n)
{
	std::ofstream o;
	o.open(fileName, std::ios::binary | std::ofstream::app);
	o.seekp((sizeof(Node<T>))*size, std::ios::beg);
	o.write((char*)&(*n), sizeof(Node<T>));
	o.close();
}

template<typename T>
inline void List<T>::binWrite(Node<T> *n, int pos)
{
	std::fstream o;
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	o.seekp((sizeof(Node<T>))*pos, std::ios::beg);
	o.write((char*)&(*n), sizeof(Node<T>));
	o.close();
}

template<typename T>
inline typename Node<T> * List<T>::binRead(int pos)
{
	std::ifstream i;
	Node<T> *d = new Node<T>();

	i.open(fileName, std::ios::binary | std::ofstream::app);
	i.seekg(((sizeof(Node<T>))*pos), std::ios::beg);
	i.read((char*)&(*d), sizeof(Node<T>));
	i.close();

	return d;
}

template<typename T>
inline void List<T>::binMove(int pos)
{
	std::ofstream out;
	std::ifstream in;
	std::fstream stream;
	Node<T> *tmp = new Node<T>();
	//i.open(fileName, std::ios::binary | std::ofstream::app);

	for (int i = size; i > pos; i--) {
		stream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

		stream.seekg(((sizeof(Node<T>))*(i - 1)), std::ios::beg);
		stream.read((char*)&(*tmp), sizeof(Node<T>));

		tmp->nextBin += sizeof(Node<T>);

		stream.seekp(((sizeof(Node<T>))*(i)), std::ios::beg);
		stream.write((char*)&(*tmp), sizeof(Node<T>));

		stream.close();

	}
}


template<typename T>
inline void List<T>::checkPos(int pos) {
	if (pos >= size) throw 1;
}

template <typename T>
void List<T>::clear() {
	//Преобразовать стринг в const char или убрать возможность изменения имени файла со списком.
	//const char * c = fileName.c_str();
	//remove(c);
	size = 0;
}

template <typename T>
void List<T>::del() {
	head = head->next;
	size--;
}

template <typename T>
T List<T>::get(int pos) {
	//Node *n;
	//int a = 0;
	//for (n = head; n; n = n->next) {
	//	if (a++ == x) return n->data;
	//}
	try {
		checkPos(pos);
	} catch (int e) {
		std::cout << "IndexOutOfBoundsException: pos: " << pos << " size: " << size << std::endl;
		throw 1;
	}
	Node<T> *n = binRead(pos);
	int a = 1;
	return n->data;
}