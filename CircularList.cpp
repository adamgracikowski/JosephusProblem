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