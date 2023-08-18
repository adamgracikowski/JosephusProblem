#include "CircularList.h"
#include <fstream>

using namespace std;

Node::Node()
	: data{ nullptr }, next{ nullptr }, prev{ nullptr } {}

Node::Node(const value_type& data, Node* next, Node* prev)
	: data{ new value_type(data) }, next{ next }, prev{ prev } {}

Node::Node(const Node& other)
	: data{ (other.data == nullptr) ? nullptr : new value_type(*other.data) }, next{ other.next }, prev{ other.prev } {}

Node::Node(Node&& other)
	: data{ other.data }, next{ other.next }, prev{ other.prev }
{
	other.reset();
}

Node::~Node()
{
	delete data;
	reset();
}

Node& Node::operator=(const Node& other)
{
	if (this != &other) {
		delete data;
		data = (other.data == nullptr) ? nullptr : new value_type(*other.data);
		next = other.next;
		prev = other.prev;
	}
	return *this;
}

Node& Node::operator=(Node&& other)
{
	if (this != &other) {
		delete data;
		data = other.data;
		next = other.next;
		prev = other.prev;
		other.reset();
	}
	return *this;
}

void Node::reset()
{
	data = nullptr;
	next = prev = nullptr;
}

CircularList::CircularList()
	: head{ nullptr }, tail{ nullptr }, n{ 0 } {}

CircularList::CircularList(size_t n, const value_type& val)
	: CircularList()
{
	while (n--) {
		push_back(val);
	}
}

CircularList::CircularList(const CircularList& other)
	: CircularList()
{
	if (other.size() > 0)
	{
		Node* tmpNode{ other.head };
		while (tmpNode != other.tail) {
			push_back(*tmpNode->data);
			tmpNode = tmpNode->next;
		}
		push_back(*tmpNode->data);
	}
}

CircularList::CircularList(CircularList&& other)
	: head{ other.head }, tail{ other.tail }, n{ other.n }
{
	other.reset();
}

CircularList::CircularList(std::initializer_list<value_type> il)
	: CircularList()
{
	for (const auto& element : il) {
		push_back(element);
	}
}

CircularList::~CircularList()
{
	clear();
}

CircularList& CircularList::operator=(const CircularList& other)
{
	if (this != &other) {
		clear();
		if (other.size() > 0)
		{
			Node* tmpNode{ other.head };
			while (tmpNode != other.tail) {
				push_back(*tmpNode->data);
				tmpNode = tmpNode->next;
			}
			push_back(*tmpNode->data);
		}
	}
	return *this;
}

CircularList& CircularList::operator=(CircularList&& other)
{
	if (this != &other) {
		clear();
		head = other.head;
		tail = other.tail;
		n = other.n;
		other.reset();
	}
	return *this;
}

void CircularList::push_back(const value_type& val)
{
	Node* newNode = new Node(val);

	if (empty()) {
		head = tail = newNode;
		newNode->next = newNode->prev = newNode;
	}
	else {
		tail->next = newNode;
		newNode->prev = tail;
		head->prev = newNode;
		newNode->next = head;
		tail = tail->next;
	}
	incrementSize();
}

void CircularList::pop_back()
{
	if (empty()) return;
	if (size() == 1) {
		delete tail;
		reset();
	}
	else {
		Node* toPop = tail;
		tail = tail->prev;
		tail->next = head;
		head->prev = tail;
		delete toPop;
		decrementSize();
	}
}

void CircularList::push_front(const value_type& val)
{
	Node* newNode = new Node(val);

	if (empty()) {
		head = tail = newNode;
		newNode->next = newNode->prev = newNode;
	}
	else {
		head->prev = newNode;
		newNode->next = head;
		tail->next = newNode;
		newNode->prev = tail;
		head = head->prev;
	}
	incrementSize();
}

void CircularList::pop_front()
{
	if (empty()) return;
	if (size() == 1) {
		delete head;
		reset();
	}
	else {
		Node* toPop = head;
		head = head->next;
		head->prev = tail;
		tail->next = head;
		delete toPop;
		decrementSize();
	}
}
void CircularList::insertAfter(size_t position, size_t howMany, const value_type& val)
{
	if (position >= size()) {
		push_back(val);
		return;
	}
	Node* headCopy{ head };
	while (position--) {
		headCopy = headCopy->next;
	}
	Node* toInsert{};
	while (howMany--) {
		toInsert = new Node(val);
		headCopy->next->prev = toInsert;
		toInsert->next = headCopy->next;
		toInsert->prev = headCopy;
		headCopy->next = toInsert;
		incrementSize();
		headCopy = toInsert;
	}
}

void CircularList::insertSorted(const value_type& val)
{
	if (empty() || val < *head->data) {
		push_front(val);
	}
	else if (*tail->data < val) {
		push_back(val);
	}
	else {
		Node* toInsert{ new Node(val) };
		Node* greaterNode{ head };
		while (*greaterNode->data < val) greaterNode = greaterNode->next;
		toInsert->prev = greaterNode->prev;
		toInsert->next = greaterNode;
		toInsert->prev->next = toInsert;
		greaterNode->prev = toInsert;
		incrementSize();
	}
}