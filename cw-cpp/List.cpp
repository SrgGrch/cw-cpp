//#include "List.h"
//
//template <typename T>
//void List<T>::push(T x) {
//	Node *n;
//	if (head == nullptr) n = new Node(x);
//	else n = new Node(x, head->pos++);
//	n->next = head;
//	head = n;
//}
//template <typename T>
//T List<T>::pop() {
//	T a = head->data;
//	head = head->next;
//	return a;
//}
//
//template <typename T>
//void List<T>::print() {
//	if (head == 0) cout << "List is empty";
//	else {
//		Node *n;
//		for (n = head; n; n = n->next)
//			cout << n->data << ' ';
//	}
//	cout << endl;
//}
//
//template <typename T>
//void List<T>::clear() {
//	Node *n;
//	for (n = head; n; n = n->next) {
//		head = head->next;
//	}
//}
//
//template <typename T>
//void List<T>::del() {
//	head = head->next;
//}
//
//template <typename T>
//T List<T>::find(int x) {
//	Node *n;
//	for (n = head; n; n = n->next) {
//		if (n->pos == x) return n->data;
//	}
//	return T();
//}
