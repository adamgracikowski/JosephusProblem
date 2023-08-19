#pragma once
#include "Node.h"

#include <initializer_list>
#include <string>
#include <iostream>
#include <functional>

//--------------------------------------------

template<typename T>
class CircularList;

template<typename T>
bool operator==(const CircularList<T>& lhs, const CircularList<T>& rhs);

template<typename T>
bool operator!=(const CircularList<T>& lhs, const CircularList<T>& rhs);

template<typename T>
CircularList<T> operator+(const CircularList<T>& lhs, const CircularList<T>& rhs);

//--------------------------------------------

template<typename T>
class CircularList
{
public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

	CircularList();
	CircularList(size_t n, const value_type& val = value_type{});
	CircularList(const CircularList<T>& other);
	CircularList(CircularList<T>&& other);
	CircularList(std::initializer_list<value_type> il);
	template <class InputIterator> CircularList(InputIterator first, InputIterator last);
	~CircularList();

	CircularList<T>& operator=(const CircularList<T>& other);
	CircularList<T>& operator=(CircularList<T>&& other);

	bool empty() const;
	size_t size() const;
	void clear();

	void push_back(const value_type& val);
	void pop_back();
	void push_front(const value_type& val);
	void pop_front();
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

	void insertAfter(size_t position, size_t howMany, const value_type& val);
	template <class InputIterator> void insertAfter(size_t position, InputIterator first, InputIterator last);
	void insertSorted(const value_type& val);

	void generateN(size_t n, std::function<value_type(void)> generator);

	void remove(const value_type& val);
	void remove_if(std::function<bool(const value_type& val)>);
	void removeEveryNUntilMRemains(size_t n, size_t m);

	void reverse();
	void bubbleSort();

	bool pushFromFile(std::string filename);

	friend bool operator== <> (const CircularList<T>& lhs, const CircularList<T>& rhs);
	friend bool operator!= <> (const CircularList<T>& lhs, const CircularList<T>& rhs);
	friend CircularList<T> operator+ <> (const CircularList<T>& lhs, const CircularList<T>& rhs);
	CircularList<T> operator+=(const CircularList<T>& other);

	std::ostream& print(std::ostream& os, std::string delimeter = " ") const;
	friend std::ostream& operator<<(std::ostream& os, const CircularList<T>& cl)
	{
		return cl.print(os);
	}

private:
	void incrementSize();
	void decrementSize();
	void reset();
	Node<T>* remove(Node<T>* toRemove);

	Node<T>* head;
	Node<T>* tail;
	size_t n;
};

//--------------------------------------------

template<typename T>
inline CircularList<T>::CircularList()
	: head{ nullptr }, tail{ nullptr }, n{ 0 } {}

template<typename T>
inline CircularList<T>::CircularList(size_t n, const value_type& val)
	: CircularList()
{
	while (n--) {
		push_back(val);
	}
}

template<typename T>
CircularList<T>::CircularList(const CircularList<T>& other)
	: CircularList()
{
	if (other.size() > 0)
	{
		Node<T>* tmpNode{ other.head };
		while (tmpNode != other.tail) {
			this->push_back(*tmpNode->data);
			tmpNode = tmpNode->next;
		}
		this->push_back(*tmpNode->data);
	}
}

template<typename T>
inline CircularList<T>::CircularList(CircularList<T>&& other)
	: head{ other.head }, tail{ other.tail }, n{ other.n }
{
	other.reset();
}

template<typename T>
inline CircularList<T>::CircularList(std::initializer_list<value_type> il)
	: CircularList()
{
	for (const auto& element : il) {
		this->push_back(element);
	}
}

template<typename T>
template<class InputIterator>
inline CircularList<T>::CircularList(InputIterator first, InputIterator last)
{
	for (InputIterator it{ first }; it != last; ++it) {
		this->push_back(*it);
	}
}

template<typename T>
inline CircularList<T>::~CircularList()
{
	this->clear();
}

template<typename T>
CircularList<T>& CircularList<T>::operator=(const CircularList<T>& other)
{
	if (this != &other) {
		this->clear();
		if (other.size() > 0)
		{
			Node<T>* tmpNode{ other.head };
			while (tmpNode != other.tail) {
				this->push_back(*tmpNode->data);
				tmpNode = tmpNode->next;
			}
			this->push_back(*tmpNode->data);
		}
	}
	return *this;
}

template<typename T>
CircularList<T>& CircularList<T>::operator=(CircularList<T>&& other)
{
	if (this != &other) {
		this->clear();
		this->head = other.head;
		this->tail = other.tail;
		this->n = other.n;
		other.reset();
	}
	return *this;
}

template<typename T>
inline bool CircularList<T>::empty() const
{
	return this->size() == 0;
}

template<typename T>
inline size_t CircularList<T>::size() const
{
	return this->n;
}

template<typename T>
void CircularList<T>::clear()
{
	if (this->size() > 0)
	{
		while (this->head != this->tail) {
			this->head = this->head->next;
			delete this->head->prev;
		}
		delete this->head;
		this->reset();
	}
}

template<typename T>
void CircularList<T>::push_back(const value_type& val)
{
	Node<T>* newNode = new Node<T>(val);

	if (this->empty()) {
		this->head = this->tail = newNode;
		newNode->next = newNode->prev = newNode;
	}
	else {
		this->tail->next = newNode;
		newNode->prev = this->tail;
		this->head->prev = newNode;
		newNode->next = this->head;
		this->tail = this->tail->next;
	}
	this->incrementSize();
}

template<typename T>
void CircularList<T>::pop_back()
{
	if (this->empty()) return;
	if (this->size() == 1) {
		delete this->tail;
		this->reset();
	}
	else {
		Node<T>* toPop = this->tail;
		this->tail = this->tail->prev;
		this->tail->next = head;
		this->head->prev = this->tail;
		delete toPop;
		this->decrementSize();
	}
}

template<typename T>
void CircularList<T>::push_front(const value_type& val)
{
	Node<T>* newNode = new Node<T>(val);

	if (this->empty()) {
		this->head = this->tail = newNode;
		newNode->next = newNode->prev = newNode;
	}
	else {
		this->head->prev = newNode;
		newNode->next = this->head;
		this->tail->next = newNode;
		newNode->prev = this->tail;
		this->head = this->head->prev;
	}
	this->incrementSize();
}

template<typename T>
void CircularList<T>::pop_front()
{
	if (this->empty()) return;
	if (this->size() == 1) {
		delete this->head;
		this->reset();
	}
	else {
		Node<T>* toPop = this->head;
		this->head = this->head->next;
		this->head->prev = this->tail;
		this->tail->next = this->head;
		delete toPop;
		this->decrementSize();
	}
}

template<typename T>
typename CircularList<T>::reference CircularList<T>::front()
{
	return *this->head->data;
}

template<typename T>
typename CircularList<T>::const_reference CircularList<T>::front() const
{
	return *this->head->data;
}

template<typename T>
typename CircularList<T>::reference CircularList<T>::back()
{
	return *this->tail->data;
}

template<typename T>
typename CircularList<T>::const_reference CircularList<T>::back() const
{
	return *this->tail->data;
}

template<typename T>
inline void CircularList<T>::insertAfter(size_t position, size_t howMany, const value_type& val)
{
	if (position >= this->size()) {
		this->push_back(val);
		return;
	}
	Node<T>* headCopy{ this->head };
	while (position--) {
		headCopy = headCopy->next;
	}
	Node<T>* toInsert{};
	while (howMany--) {
		toInsert = new Node<T>(val);
		headCopy->next->prev = toInsert;
		toInsert->next = headCopy->next;
		toInsert->prev = headCopy;
		headCopy->next = toInsert;
		this->incrementSize();
		headCopy = toInsert;
	}
}

template<typename T>
template<class InputIterator>
inline void CircularList<T>::insertAfter(size_t position, InputIterator first, InputIterator last)
{
	Node<T>* headCopy{ head };

	if (position >= this->size()) {
		while (first != last) {
			this->push_back(*first);
			++first;
		}
		return;
	}
	while (position--) {
		headCopy = headCopy->next;
	}
	Node<T>* toInsert{};
	while (first != last) {
		toInsert = new Node<T>(*first);
		headCopy->next->prev = toInsert;
		toInsert->next = headCopy->next;
		toInsert->prev = headCopy;
		headCopy->next = toInsert;
		this->incrementSize();

		headCopy = toInsert;
		++first;
	}
}

template<typename T>
inline void CircularList<T>::insertSorted(const value_type& val)
{
	if (this->empty() || val < *this->head->data) {
		this->push_front(val);
	}
	else if (*this->tail->data < val) {
		this->push_back(val);
	}
	else {
		Node<T>* toInsert{ new Node<T>(val) };
		Node<T>* greaterNode{ this->head };
		while (*greaterNode->data < val) greaterNode = greaterNode->next;
		toInsert->prev = greaterNode->prev;
		toInsert->next = greaterNode;
		toInsert->prev->next = toInsert;
		greaterNode->prev = toInsert;
		this->incrementSize();
	}
}

template<typename T>
inline void CircularList<T>::generateN(size_t n, std::function<value_type(void)> generator)
{
	while (n--) {
		this->push_back(generator());
	}
}

template<typename T>
inline void CircularList<T>::remove(const value_type& val)
{
	Node<T>* headCopy{ this->head };
	if (this->empty()) return;
	while (headCopy != this->tail) {
		if (*headCopy->data == val) {
			// private overloaded version of remove method
			headCopy = this->remove(headCopy);
		}
		else headCopy = headCopy->next;
	}
	if (*this->tail->data == val) this->pop_back();
}

template<typename T>
inline void CircularList<T>::remove_if(std::function<bool(const value_type&)> pred)
{
	Node<T>* headCopy{ this->head };
	if (this->empty()) return;
	while (headCopy != this->tail) {
		if (pred(*headCopy->data)) {
			// private overloaded version of remove method
			headCopy = this->remove(headCopy);
		}
		else headCopy = headCopy->next;
	}
	if (pred(*headCopy->data)) this->pop_back();
}

template<typename T>
inline void CircularList<T>::removeEveryNUntilMRemains(size_t n, size_t m)
{
	if (this->size() <= m) return;

	size_t counter{ 1 };
	Node<T>* headCopy{ this->head };

	while (this->size() != m) {
		if (counter % n == 0) {
			counter = 1;
			headCopy = this->remove(headCopy);
		}
		else {
			counter++;
			headCopy = headCopy->next;
		}
	}
}

template<typename T>
inline void CircularList<T>::reverse()
{
	Node<T>* headCopy{ this->head }, * tailCopy{ this->tail };
	size_t middle{ size_t(this->size() / 2) };
	for (size_t i = 0; i < middle; ++i)
	{
		std::swap(headCopy->data, tailCopy->data);
		headCopy = headCopy->next;
		tailCopy = tailCopy->prev;
	}
}

template<typename T>
inline void CircularList<T>::bubbleSort()
{
	if (this->size() < 2) return;
	for (Node<T>* i{ this->head }, * t{ this->tail }; i != this->tail; i = i->next) {
		for (Node<T>* j{ this->head }; j != t; j = j->next) {
			if (*j->data > *j->next->data)
				std::swap(j->data, j->next->data);
		}
		t->prev;
	}
}

template<typename T>
inline bool CircularList<T>::pushFromFile(std::string filename)
{
	std::ifstream file(filename);
	if (!file) return false;
	CircularList<T> toPushList{};
	value_type val{};
	while (file >> val) {
		toPushList.push_back(val);
	}
	*this += toPushList;
	file.close();
	return true;
}

template<typename T>
inline CircularList<T> CircularList<T>::operator+=(const CircularList<T>& other)
{
	return *this = *this + other;
}

template<typename T>
inline std::ostream& CircularList<T>::print(std::ostream& os, std::string delimeter) const
{
	if (this->empty()) {
		os << "EMPTY";
	}
	else {
		Node<T>* tmpNode{ this->head };
		while (tmpNode != this->tail) {
			os << *tmpNode->data << delimeter;
			tmpNode = tmpNode->next;
		}
		os << *tmpNode->data;
	}
	return os;
}

template<typename T>
inline void CircularList<T>::incrementSize()
{
	this->n++;
}

template<typename T>
inline void CircularList<T>::decrementSize()
{
	this->n--;
}

template<typename T>
inline void CircularList<T>::reset()
{
	this->n = 0;
	this->head = this->tail = nullptr;
}

template<typename T>
Node<T>* CircularList<T>::remove(Node<T>* toRemove)
{
	if (this->empty()) return nullptr;
	if (toRemove == this->head) {
		this->pop_front();
		return this->head;
	}
	if (toRemove == this->tail) {
		this->pop_back();
		return this->head;
	}
	Node<T>* nextNode{ toRemove->next };
	toRemove->prev->next = toRemove->next;
	toRemove->next->prev = toRemove->prev;
	this->decrementSize();
	delete toRemove;
	return nextNode;
}

template<typename T>
bool operator==(const CircularList<T>& lhs, const CircularList<T>& rhs)
{
	if (lhs.size() != rhs.size()) return false;
	if (lhs.empty() || rhs.empty()) return true;

	Node<T>* lhsHeadCopy{ lhs.head }, * rhsHeadCopy{ rhs.head };
	while (lhsHeadCopy != lhs.tail && rhsHeadCopy != rhs.tail) {
		if (*lhsHeadCopy->data != *rhsHeadCopy->data) return false;
		lhsHeadCopy = lhsHeadCopy->next;
		rhsHeadCopy = rhsHeadCopy->next;
	}
	if (*lhsHeadCopy->data != *rhsHeadCopy->data) return false;
	return true;
}

template<typename T>
bool operator!=(const CircularList<T>& lhs, const CircularList<T>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
CircularList<T> operator+(const CircularList<T>& lhs, const CircularList<T>& rhs)
{
	CircularList<T> sum(lhs);
	if (rhs.empty()) return sum;

	Node<T>* rhsHeadCopy{ rhs.head };
	while (rhsHeadCopy != rhs.tail) {
		sum.push_back(*rhsHeadCopy->data);
		rhsHeadCopy = rhsHeadCopy->next;
	}
	sum.push_back(*rhsHeadCopy->data);
	return sum;
}

//--------------------------------------------
