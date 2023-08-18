#include "CircularList.h"

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

