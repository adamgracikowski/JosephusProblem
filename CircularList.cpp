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

void CircularList::remove(const value_type& val)
{
	Node* headCopy{ head };
	if (empty()) return;
	while (headCopy != tail) {
		if (*headCopy->data == val) {
			// private overloaded version of remove method
			headCopy = remove(headCopy);
		}
		else headCopy = headCopy->next;
	}
	if (*tail->data == val) pop_back();
}

void CircularList::remove_if(std::function<bool(const value_type& val)> pred)
{
	Node* headCopy{ head };
	if (empty()) return;
	while (headCopy != tail) {
		if (pred(*headCopy->data)) {
			// private overloaded version of remove method
			headCopy = remove(headCopy);
		}
		else headCopy = headCopy->next;
	}
	if (pred(*headCopy->data)) pop_back();
}

void CircularList::removeEveryNUntilMRemains(size_t n, size_t m)
{
	if (size() <= m) return;

	size_t counter{ 1 };
	Node* headCopy{ head };

	while (size() != m) {
		if (counter % n == 0) {
			counter = 1;
			headCopy = remove(headCopy);
		}
		else {
			counter++;
			headCopy = headCopy->next;
		}
	}
}

Node* CircularList::remove(Node* toRemove)
{
	if (empty()) return nullptr;
	if (toRemove == head) {
		pop_front();
		return head;
	}
	if (toRemove == tail) {
		pop_back();
		return head;
	}
	Node* nextNode{ toRemove->next };
	toRemove->prev->next = toRemove->next;
	toRemove->next->prev = toRemove->prev;
	decrementSize();
	delete toRemove;
	return nextNode;
}

bool operator==(const CircularList& lhs, const CircularList& rhs)
{
	if (lhs.size() != rhs.size()) return false;
	if (lhs.empty() || rhs.empty()) return true;

	Node* lhsHeadCopy{ lhs.head }, * rhsHeadCopy{ rhs.head };
	while (lhsHeadCopy != lhs.tail && rhsHeadCopy != rhs.tail) {
		if (*lhsHeadCopy->data != *rhsHeadCopy->data) return false;
		lhsHeadCopy = lhsHeadCopy->next;
		rhsHeadCopy = rhsHeadCopy->next;
	}
	if (*lhsHeadCopy->data != *rhsHeadCopy->data) return false;
	return true;
}

bool operator!=(const CircularList& lhs, const CircularList& rhs)
{
	return !(lhs == rhs);
}

CircularList operator+(const CircularList& lhs, const CircularList& rhs)
{
	CircularList sum(lhs);
	if (rhs.empty()) return sum;

	Node* rhsHeadCopy{ rhs.head };
	while (rhsHeadCopy != rhs.tail) {
		sum.push_back(*rhsHeadCopy->data);
		rhsHeadCopy = rhsHeadCopy->next;
	}
	sum.push_back(*rhsHeadCopy->data);
	return sum;
}

std::ostream& operator<<(std::ostream& os, const CircularList& cl)
{
	return cl.print(os);
}

CircularList CircularList::operator+=(const CircularList& other)
{
	return *this = *this + other;
}

void CircularList::clear()
{
	if (size() > 0)
	{
		while (head != tail) {
			head = head->next;
			delete head->prev;
		}
		delete head;
		reset();
	}
}

size_t CircularList::size() const
{
	return n;
}

void CircularList::incrementSize()
{
	n++;
}

void CircularList::decrementSize()
{
	n--;
}

bool CircularList::empty() const
{
	return size() == 0;
}

CircularList::reference CircularList::front()
{
	return *head->data;
}

CircularList::const_reference CircularList::front() const
{
	return *head->data;
}

CircularList::reference CircularList::back()
{
	return *tail->data;
}

CircularList::const_reference CircularList::back() const
{
	return *tail->data;
}

bool CircularList::pushFromFile(string filename)
{
	ifstream file(filename);
	if (!file) return false;
	CircularList toPushList{};
	value_type val{};
	while (file >> val) {
		toPushList.push_back(val);
	}
	if (!file.eof()) {
		file.close();
		return false;
	}
	*this += toPushList;
	file.close();
	return true;
}

void CircularList::generateN(size_t n, function<value_type(void)> generator)
{
	while (n--) {
		push_back(generator());
	}
}

void CircularList::reverse()
{
	Node* headCopy{ head }, * tailCopy{ tail };
	size_t middle{ size_t(size() / 2) };
	for (size_t i = 0; i < middle; ++i)
	{
		swap(headCopy->data, tailCopy->data);
		headCopy = headCopy->next;
		tailCopy = tailCopy->prev;
	}
}

void CircularList::sort(std::function<void(CircularList&)> sortingFunction)
{
	sortingFunction(*this);
}

ostream& CircularList::print(ostream& os, string delimeter) const
{
	if (empty()) {
		os << "EMPTY";
	}
	else {
		Node* tmpNode{ head };
		while (tmpNode != tail) {
			os << *tmpNode->data << delimeter;
			tmpNode = tmpNode->next;
		}
		os << *tmpNode->data;
	}
	return os;
}

void CircularList::reset()
{
	head = tail = nullptr;
	n = 0;
}

void bubbleSort(CircularList& cl)
{
	if (cl.size() < 2) return;
	for (Node* i{ cl.head }, * t{ cl.tail }; i != cl.tail; i = i->next) {
		for (Node* j{ cl.head }; j != t; j = j->next) {
			if (*j->data > *j->next->data)
				swap(j->data, j->next->data);
		}
		t->prev;
	}
}





