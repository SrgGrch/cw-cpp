#pragma once
#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
class Node {
public:
	int nextBin = 0;
	int dataSize;
	T data;

	Node(T i);
	Node(T i, int _pos) :data(i) {
		dataSize = sizeof(i);
	};
	Node() {};
};

template<typename T>
inline Node<T>::Node(T i)
{
	data = i;
	dataSize = sizeof(i);
}

template<>
inline Node<char*>::Node(char* i)
{
	data = new char[strlen(i)];
	strcpy(data, i);
	dataSize = sizeof(i);
}


template <typename T>
class List {
public:
	List() :head() {
		getSizeFromFile();
	}
	List(std::string _fileName) : head(), fileName(_fileName) {
		getSizeFromFile();
	}
	~List();

	void insert(T x); //Добавление элемента в конец списка 
	void insert(T x, int pos); // Добавление элемента на позицию @pos - позиция *

	void del(int pos); // Удаление элемента @pos - позиция элемента *

	void clear(); // Отчистка списка
	T get(int pos); // Получение элемента @pos - позиция
	void print();
	void sort(); // сортировка

	void binWrite(Node<T> *n, bool flag); // запись в бинарный файл, @flag - false - список пустой
	void binWrite(Node<T> *n, int pos); // запись в бинарный файл @pos - позиция на которую нужно записать элемент
	typename Node<T>* binRead(int pos); //чтение с бинарного файла @pos - позиция считываемого узла


	void setFileName(std::string _fileName);
	std::string getFileName();


private:
	Node<T> *firstItem;
	Node<T> *head;
	int size = 0;
	std::vector<Node<T>*> localList;

	int seek(int pos); // возвращает указатель на начало элемента
	void checkPos(int pos);
	void checkSize(); //проверка размера файла
	void binMove(int pos, int elemSize); // Перемещает часть списка вправо @pos - позция начального элемента, @elemSize - кол-во битов на которые надо сдвинуть элементы
	void loadInMemory();
	void loadInMemory(int posStart);
	Node<T>* getNode(int pos);
	void clearInMemoryList();
	void insertNode(Node<T> *n);
	std::string fileName = "list.bin"; // Имя файла в который будет записан список
	void changeSize();
	void getSizeFromFile();
	void sortAlg();
	const int indent = 4;

};




template<typename T>
inline List<T>::~List()
{
	//remove(fileName);
}

template<typename T>
inline void List<T>::changeSize()
{
	std::fstream stream;
	stream.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	int *s = new int(size);
	stream.seekp(0, std::ios::beg);
	stream.write((char*)&(*s), sizeof(size));

	stream.close();
}

template<typename T>
inline void List<T>::getSizeFromFile()
{
	std::fstream stream;
	stream.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	int *s = new int(0);
	stream.seekp(0, std::ios::beg);
	stream.read((char*)&(*s), sizeof(size));
	size = *s;

	stream.close();
}

template <typename T>
inline void List<T>::sortAlg() {
	Node<T>* tmp;
	for (int i = 1; i < localList.size(); i++)
		for (int k = i; k != 0 && localList[k]->data < localList[k - 1]->data; k--) {
			tmp = localList[k]; localList[k] = localList[k - 1]; localList[k - 1] = tmp;
		}
}

template <typename T>
void List<T>::insert(T x) {
	Node<T> *n = new Node<T>(x);
	if (size == 0) {
		binWrite(n, false);
	} else {
		head = n;
		binWrite(n, true);
	}
	size++;
	changeSize();
}

template<typename T>
inline void List<T>::insert(T x, int pos)
{
	Node<T> *n = new Node<T>(x);
	int elemSize = n->dataSize + sizeof(int) + sizeof(int);

	try {
		checkPos(pos);
	}

	catch (int e) {
		std::cout << "IndexOutOfBoundsException: pos: " << pos << " size: " << size << std::endl;
		throw 1;
	}
	if (size == 0) binWrite(n, false);
	else {
		if (pos != (size)) {
			if (pos == 0) {
				binMove(pos, elemSize);
				n->nextBin = elemSize;
				binWrite(n, pos);
			} else {
				binMove(pos, elemSize);
				binWrite(n, pos);
			}
		} else {
			binWrite(n, true);
		}
	}
	size++;
	changeSize();
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
inline void List<T>::sort()
{
	loadInMemory();
	sortAlg();
	size = 0;
	for (int i = 0; i < localList.size(); i++) {
		insert(localList[i]->data);
	}
	clearInMemoryList();
}

template<typename T>
inline void List<T>::binWrite(Node<T> *n, bool flag)
{
	std::fstream o;
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	int position = seek(size - 1);
	if (!flag) {
		o.seekp(position, std::ios::beg);
		o.write((char*)&(*n), sizeof(*n));
	} else {
		int posPrev = position;
		int sizePrev = 0;
		int *newPointer;

		o.seekp(posPrev + sizeof(int), std::ios::beg);											// переход на ячейку размера
		o.read((char*)&(sizePrev), sizeof(int));												// Чтение его размера
		o.seekp(posPrev, std::ios::beg);														// ячейка указателя

		o.seekp(posPrev, std::ios::beg);
		newPointer = new int(posPrev + sizePrev + sizeof(int) + sizeof(int));
		o.write((char*)&(*newPointer), sizeof(int));

		o.seekp(*newPointer, std::ios::beg);
		o.write((char*)&(*n), sizeof(Node<T>));


		delete newPointer;
	}
	o.close();
}

template<typename T>
inline void List<T>::binWrite(Node<T> *n, int pos)
{
	std::fstream o;
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);

	int posPrev = seek(pos - 1);
	int sizePrev = 0;
	int *newPointer;
	if (pos == 0) {
		o.seekp(0, std::ios::beg);
		o.write((char*)&(*n), sizeof(Node<T>));//sizeof(dataSize +int + int)
	} else {
		o.seekp(posPrev + sizeof(int), std::ios::beg);											// переход на ячейку размера
		o.read((char*)&(sizePrev), sizeof(int));												// Чтение его размера
		o.seekp(posPrev, std::ios::beg);														// ячейка указателя

		o.seekp(posPrev, std::ios::beg);
		newPointer = new int(posPrev + sizePrev + sizeof(int) + sizeof(int));
		o.write((char*)&(*newPointer), sizeof(int));

		o.seekp(*newPointer, std::ios::beg);
		o.write((char*)&(*n), sizeof(Node<T>)); //sizeof(dataSize +int + int)

		o.seekp(*newPointer, std::ios::beg);
		*newPointer = *newPointer + sizeof(int) + n->dataSize + sizeof(int);
		o.write((char*)&(*newPointer), sizeof(int));
		delete newPointer;
	}

	o.close();
}

template<typename T>
inline typename Node<T> * List<T>::binRead(int pos)
{
	std::ifstream i;
	Node<T> *d = new Node<T>();

	i.open(fileName, std::ios::binary | std::ofstream::app);
	i.seekg(seek(pos), std::ios::beg);
	i.read((char*)&(*d), sizeof(Node<T>));

	i.close();

	return d;
}

template<typename T>
inline void List<T>::binMove(int pos, int elemSize)
{
	std::fstream stream;
	Node<T> *tmp = new Node<T>();
	stream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

	int *tmpSize = new int();
	int tmpPos = 0;
	for (int i = size; i > pos; i--) {
		tmpPos = seek(i - 1);
		stream.seekg(tmpPos + sizeof(int), std::ios::beg);							//
		stream.read((char*)&(*tmpSize), sizeof(int));								// Считываем размер элемента (int + int + dataSize)

		stream.seekg(tmpPos, std::ios::beg);										//
		stream.read((char*)&(*tmp), sizeof(int) + sizeof(int) + *tmpSize);			// Считываем элемент

		if (tmp->nextBin != 0) tmp->nextBin += elemSize;							//

		stream.seekp(tmpPos + elemSize, std::ios::beg);								// 
		stream.write((char*)&(*tmp), sizeof(int) + sizeof(int) + *tmpSize);			// Переносим элемент на elemSize бит
	}
	stream.close();
	delete tmp;
}

template<typename T>
inline void List<T>::loadInMemory()
{
	for (int i = 0; i < size; i++) {
		localList.push_back(getNode(i));
	}
}

template<typename T>
inline void List<T>::loadInMemory(int posStart)
{
	for (int i = posStart; i < size; i++) {
		localList.push_back(getNode(i));
	}
}


template<typename T>
inline Node<T>* List<T>::getNode(int pos)
{
	try {
		checkPos(pos);
	} catch (int e) {
		std::cout << "IndexOutOfBoundsException: pos: " << pos << " size: " << size << std::endl;
		throw 1;
	}
	return binRead(pos);
}

template<typename T>
inline void List<T>::clearInMemoryList()
{
	for (int i = 0; i < localList.size(); i++) {
		delete localList[i];
	}
	localList.clear();
}

template<typename T>
inline void List<T>::insertNode(Node<T>* n)
{
	int position = seek(size);
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);


	o.seekp(position, std::ios::beg);											// переход на ячейку размера
	o.write((char*)&(*n), sizeof(n->dataSize + sizeof(int) * 2));												// Чтение его размера

	o.close();
	size++;
}



template<typename T>
inline int List<T>::seek(int pos)
{
	std::fstream stream;
	stream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
	int next = indent;
	int a = 0;
	if (pos == 0) return 0 + indent;
	else {
		
		for (int i = 0; i < pos; i++) {
			if (i != 0) {
				stream.seekg(next, std::ios::beg);
				stream.read((char*)&(next), sizeof(int));
			} else {
				stream.seekg(indent, std::ios::beg);
				stream.read((char*)&(next), sizeof(int));
			}
		}
	}
	stream.close();
	return next;
}

template<typename T>
inline void List<T>::checkPos(int pos) {
	if (pos > size) throw 1;
}

template<typename T>
inline void List<T>::del(int pos)
{
	int position = seek(pos);
	int elemSize = 0;
	std::fstream stream;
	stream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

	if (pos == size - 1) size--;
	else {
		stream.seekp(position + sizeof(int), std::ios::beg);

		stream.read((char*)&(elemSize), sizeof(int));
		elemSize += sizeof(int) * 2;
		loadInMemory(pos + 1);

		int j = 0;
		Node<T> *tmp;
		for (int i = pos; i < size - 1; i++) {
			tmp = localList[j++];
			tmp->nextBin -= elemSize;
			binWrite(tmp, i);
		}
		size--;
	}
	stream.close();
	clearInMemoryList();
}

template <typename T>
void List<T>::clear() {
	const char * c = fileName.c_str();
	remove(c);
}

template <typename T>
T List<T>::get(int pos) {
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
