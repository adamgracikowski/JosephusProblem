#pragma once

template<typename T>
class Node {
public:
	Node();
	explicit Node(const T& data, Node<T>* next = nullptr, Node<T>* prev = nullptr);
	Node(const Node<T>& other);
	Node(Node<T>&& other);
	~Node();

	Node<T>& operator=(const Node<T>& other);
	Node<T>& operator=(Node<T>&& other);

	T* data;
	Node<T>* next;
	Node<T>* prev;
private:
	void reset();
};

//--------------------------------------------------------

template<typename T>
Node<T>::Node()
	: data{ nullptr }, next{ nullptr }, prev{ nullptr } {}

template<typename T>
Node<T>::Node(const T& data, Node<T>* next, Node<T>* prev)
	: data{ new T(data) }, next{ next }, prev{ prev } {}

template<typename T>
Node<T>::Node(const Node<T>& other)
	: data{ (other.data == nullptr) ? nullptr : new T(*other.data) }, next{ other.next }, prev{ other.prev } {}

template<typename T>
Node<T>::Node(Node<T>&& other)
	: data{ other.data }, next{ other.next }, prev{ other.prev }
{
	other.reset();
}

template<typename T>
Node<T>::~Node()
{
	delete data;
	reset();
}

template<typename T>
Node<T>& Node<T>::operator=(const Node<T>& other)
{
	if (this != &other) {
		delete data;
		data = (other.data == nullptr) ? nullptr : new T(*other.data);
		next = other.next;
		prev = other.prev;
	}
	return *this;
}

template<typename T>
Node<T>& Node<T>::operator=(Node<T>&& other)
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

template<typename T>
void Node<T>::reset()
{
	data = nullptr;
	next = prev = nullptr;
}

//--------------------------------------------------------
