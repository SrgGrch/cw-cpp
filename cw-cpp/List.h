#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

template <typename T>
class Node {
public:
	int nextBin = 0;
	int dataSize;
	T data;

	Node(T i);	// ����������� � ����������� ������
	Node() {};	// ������������ �� ���������
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
	dataSize = strlen(i);
}


template <typename T>
class List {
public:
	List() {	// ����������� �� ���������	
		getSizeFromFile();
		std::fstream o;
		o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
		o.seekp(0, std::ios::beg);
		o.write((char*)&(size), sizeof(int));
		o.close();
	}
	List(std::string _fileName) : fileName(_fileName) { // ����������� � ������ ����� �����
		getSizeFromFile();
		std::fstream o;
		o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
		o.seekp(0, std::ios::beg);
		o.write((char*)&(size), sizeof(int));
		o.close();
	}
	~List();	// ����������

	void insert(T x); //���������� �������� � ����� ������ 
	void insert(T x, int pos); // ���������� �������� �� ������� @pos - ������� 

	void del(int pos); // �������� �������� @pos - ������� �������� 

	void getSizeFromFile(); // ���������� ������� ������
	void clear(); // �������� ������
	T get(int pos); // ��������� �������� @pos - �������
	void print();
	void sort(); // ����������
	int getSize();

private:
	int size = 0;
	std::vector<Node<T>*> localList; // ��������� ������
	std::string fileName = "listj.bin"; // ��� ����� � ������� ����� ������� ������
	const int indent = 4; // ������ �� ������ (������ 4 ����� ������ ������ �����)

	void heal();
	void binWrite(Node<T> *n, bool flag); // ������ � �������� ����, @flag - false - ������ ������
	void binWrite(Node<T> *n, int pos); // ������ � �������� ���� @pos - ������� �� ������� ����� �������� �������
	typename Node<T>* binRead(int pos); //������ � ��������� ����� @pos - ������� ������������ ����
	void binMove(int pos, int elemSize); // ���������� ����� ������ ������ @pos - ������ ���������� ��������, @elemSize - ���-�� ����� �� ������� ���� �������� ��������
	int seek(int pos); // ���������� ��������� �� ������ ��������
	void checkPos(int pos); // ��������� ����������� ��������� ������� 
	void loadInMemory(); // ��������� ������ � ������
	int getNodeSize(Node<T> *n);
	void loadInMemory(int posStart); // ��������� ������ � ������ � ������� @posStart
	Node<T>* getNode(int pos); // �������� ������ �� ������� @pos � ������� Node<T>*
	void clearInMemoryList(); // �������� ������ ���������� � ������
	void changeSize(); // ������ ������� � �������� ����
	void sortAlg(); // ����������
};




template<typename T>
inline List<T>::~List()
{
}

template<typename T>
inline void List<T>::changeSize()
{
	std::fstream stream;
	stream.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	int *s = new int(size);
	stream.seekp(0, std::ios::beg);
	stream.write((char*)&(*s), sizeof(size)); // ������ �������
	stream.close();
}

template<typename T>
inline void List<T>::getSizeFromFile()
{
	std::fstream stream;
	stream.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	int *s = new int(0);
	stream.seekp(0, std::ios::beg);
	stream.read((char*)&(*s), sizeof(size)); // ����������� �������
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
		heal(); 
		binWrite(n, false); // ������ ���� � ������ ������� ������ 
	} else {
		binWrite(n, true); // ������ ���� ����� ������ �� ������
	}
	size++;
	changeSize();
	delete n; 
}

template<typename T>
inline void List<T>::insert(T x, int pos)
{
	Node<T> *n = new Node<T>(x);
	int elemSize = n->dataSize + sizeof(int) + sizeof(int); // ����������� ������� ����
	checkPos(pos);
	if (size == 0) {
		heal();
		binWrite(n, false); // ������ ���� � ������ ������� ������ (������� ������)
	}
	else {
		if (pos != size) {
			if (pos == 0) { // ���� ���� ��������� ������ ��������� 
				binMove(pos, elemSize); // ����� ����� ������ ��� ������������ ����� ��� ����� ����
				n->nextBin = elemSize; // ��������� ��������� �� ������ ����, ���������� ��� ���� ��������
				binWrite(n, pos); // ������ ������ ����
			} else {
				binMove(pos, elemSize); // ����� �����
				binWrite(n, pos); // ������ ������ ����
			}
		} else {
			binWrite(n, true); // ������� ������ � ����� ������
		}
	}
	size++;
	changeSize(); // ������ ������� ������ � ����
	delete n;
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
	loadInMemory();  // �������� � ������
	sortAlg(); // ����������
	size = 0; //����� �������
	for (int i = 0; i < localList.size(); i++) {
		insert(localList[i]->data); // ������
	}
	clearInMemoryList(); // �������� ����������� ������ 
}

template<typename T>
inline int List<T>::getSize()
{
	return size;
}

template<typename T>
inline void List<T>::heal()
{
	std::fstream o;
	std::ofstream ofs(fileName); //�������
	int *s = new int(0);
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	o.seekp(0, std::ios::beg);
	o.write((char*)&(*s), sizeof(int));
	o.close();
}

template<typename T>
inline void List<T>::binWrite(Node<T> *n, bool flag)
{
	std::fstream o;
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	int position = seek(size - 1);
	if (!flag) {
		o.seekp(position, std::ios::beg);
		int das = getNodeSize(n);
		o.write((char*)&(*n), das);
	} else {
		int posPrev = position;
		int sizePrev = 0;
		int *newPointer;
		o.seekp(posPrev + sizeof(int), std::ios::beg); // ������� �� ������ �������
		o.read((char*)&(sizePrev), sizeof(int)); // ������ ��� �������
		o.seekp(posPrev, std::ios::beg); // ������ ���������
		newPointer = new int(posPrev + sizePrev + sizeof(int) + sizeof(int)); // ��������� �� ����� ����
		o.write((char*)&(*newPointer), sizeof(int)); //������ ��������� �� ����� ����
		o.seekp(*newPointer, std::ios::beg); // ������� �� ����� ������ ����
		o.write((char*)&(*n), getNodeSize(n)); // ������ ������ ����
		delete newPointer;
	}
	o.close();
}

template<typename T>
inline void List<T>::binWrite(Node<T> *n, int pos){
	std::fstream o;
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);

	int posPrev = seek(pos - 1);
	int sizePrev = 0;
	int *newPointer;
	if (pos == 0) {
		o.seekp(0, std::ios::beg);
		o.write((char*)&(*n), getNodeSize(n));
	} else {
		o.seekp(posPrev + sizeof(int), std::ios::beg);											// ������� �� ������ �������
		o.read((char*)&(sizePrev), sizeof(int));												// ������ ��� �������
		o.seekp(posPrev, std::ios::beg);														// ������ ���������

		o.seekp(posPrev, std::ios::beg);
		newPointer = new int(posPrev + sizePrev + sizeof(int) + sizeof(int));
		o.write((char*)&(*newPointer), sizeof(int));

		o.seekp(*newPointer, std::ios::beg);
		o.write((char*)&(*n), getNodeSize(n));
		o.seekp(*newPointer, std::ios::beg);
		*newPointer = *newPointer + sizeof(int) + n->dataSize + sizeof(int);
		o.write((char*)&(*newPointer), sizeof(int));
		delete newPointer;
	}
	o.close();
}

template<>
inline void List<char*>::binWrite(Node<char*> *n, bool flag)
{
	std::fstream o;
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);
	int position = seek(size - 1);
	if (!flag) {
		o.seekp(position, std::ios::beg);
		o.write((char*)&(n->nextBin), sizeof(int));
		o.write((char*)&(n->dataSize), sizeof(int));
		o.write((char*)&(n->data[0]), n->dataSize);
	} else {
		int posPrev = position;
		int sizePrev = 0;
		int *newPointer;
		o.seekp(posPrev + sizeof(int), std::ios::beg); // ������� �� ������ �������
		o.read((char*)&(sizePrev), sizeof(int)); // ������ ��� �������
		o.seekp(posPrev, std::ios::beg); // ������ ���������
		newPointer = new int(posPrev + sizePrev + sizeof(int) + sizeof(int)); // ��������� �� ����� ����
		o.write((char*)&(*newPointer), sizeof(int)); //������ ��������� �� ����� ����
		o.seekp(*newPointer, std::ios::beg); // ������� �� ����� ������ ����
		o.write((char*)&(n->nextBin), sizeof(int));
		o.write((char*)&(n->dataSize), sizeof(int));
		o.write((char*)&(n->data[0]), n->dataSize);
		delete newPointer;
	}
	o.close();
}

template<>
inline void List<char*>::binWrite(Node<char*> *n, int pos) {
	std::fstream o;
	o.open(fileName, std::ios::binary | std::ios::out | std::ios::in);

	int posPrev = seek(pos - 1);
	int sizePrev = 0;
	int *newPointer;
	if (pos == 0) {
		o.seekp(0, std::ios::beg);
		o.write((char*)&(n->nextBin), sizeof(int));
		o.write((char*)&(n->dataSize), sizeof(int));
		o.write((char*)&(n->data[0]), n->dataSize);
	} else {
		o.seekp(posPrev + sizeof(int), std::ios::beg);											// ������� �� ������ �������
		o.read((char*)&(sizePrev), sizeof(int));												// ������ ��� �������
		o.seekp(posPrev, std::ios::beg);														// ������ ���������

		o.seekp(posPrev, std::ios::beg);
		newPointer = new int(posPrev + sizePrev + sizeof(int) + sizeof(int));
		o.write((char*)&(*newPointer), sizeof(int));

		o.seekp(*newPointer, std::ios::beg);
		o.write((char*)&(n->nextBin), sizeof(int));
		o.write((char*)&(n->dataSize), sizeof(int));
		o.write((char*)&(n->data[0]), n->dataSize);
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

template<>
inline typename Node<char*> * List<char*>::binRead(int pos)
{
	std::ifstream i;
	Node<char*> *d = new Node<char*>();
	int sz = 0;
	int next = 0;
	i.open(fileName, std::ios::binary | std::ofstream::app);
	i.seekg(seek(pos) + sizeof(int), std::ios::beg);
	i.read((char*)&(sz), sizeof(int));
	i.seekg(seek(pos), std::ios::beg);
	i.read((char*)&(next), sizeof(int));

	i.seekg(seek(pos) + sizeof(int) + sizeof(int), std::ios::beg);
	char* tmp = new char[sz+1];
	for (int it = 0; it < sz; it++)
		i.read((char*)&(tmp[it]), sizeof(char));
	tmp[sz] = '\0';
	i.close();
	d->data = tmp;
	std::string asd;
	asd.append(tmp);
	d->dataSize = sz;
	d->nextBin = next;
	return d;
}

template<typename T>
inline void List<T>::binMove(int pos, int elemSize){
	std::fstream stream;
	Node<T> *tmp = new Node<T>();
	stream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
	int *tmpSize = new int();
	int tmpPos = 0;
	for (int i = size; i > pos; i--) {
		tmpPos = seek(i - 1);
		stream.seekg(tmpPos + sizeof(int), std::ios::beg);							
		stream.read((char*)&(*tmpSize), sizeof(int)); // ��������� ������ �������� (int + int + dataSize)
		stream.seekg(tmpPos, std::ios::beg);										
		stream.read((char*)&(*tmp), sizeof(int) + sizeof(int) + *tmpSize); // ��������� �������
		if (tmp->nextBin != 0) tmp->nextBin += elemSize;							
		stream.seekp(tmpPos + elemSize, std::ios::beg);								 
		stream.write((char*)&(*tmp), sizeof(int) + sizeof(int) + *tmpSize); // ��������� ������� �� elemSize ���
	}
	stream.close();
	delete tmp;
}

template<typename T>
inline void List<T>::loadInMemory(){
	for (int i = 0; i < size; i++) {
		localList.push_back(getNode(i));
	}
}

template<typename T>
inline int List<T>::getNodeSize(Node<T>* n)
{
	return n->dataSize + sizeof(int) + sizeof(int);
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
	checkPos(pos);
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
inline int List<T>::seek(int pos)
{
	if (pos != -1) checkPos(pos);
	std::fstream stream;
	stream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
	int next = indent;
	if (pos == 0) return 0 + indent;	// ������ ������� ���������� � 4 �����
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
	
	if (pos > size) {
		std::cout << "IndexOutOfBoundsException: pos: " << pos << " size: " << size << std::endl;
		throw 1;

	}
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
	size = 0;
}

template <typename T>
T List<T>::get(int pos) {
	checkPos(pos);
	Node<T> *n = binRead(pos);
	int a = 1;
	return n->data;
}