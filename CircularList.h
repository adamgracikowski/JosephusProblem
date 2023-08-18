#pragma once
#include <initializer_list>
#include <string>
#include <iostream>
#include <functional>

class Node {
public:
	using value_type = int;

	Node();
	explicit Node(const value_type& data, Node* next = nullptr, Node* prev = nullptr);
	Node(const Node& other);
	Node(Node&& other);
	~Node();

	Node& operator=(const Node& other);
	Node& operator=(Node&& other);

	value_type* data;
	Node* next;
	Node* prev;
private:
	void reset();
};

class CircularList;
void bubbleSort(CircularList& cl);

class CircularList
{
public:
	using value_type = int;
	using reference = value_type&;
	using const_reference = const reference;

	CircularList();
	CircularList(size_t n, const value_type& val = value_type{});
	CircularList(const CircularList& other);
	CircularList(CircularList&& other);
	CircularList(std::initializer_list<value_type> il);
	template <class InputIterator> CircularList(InputIterator first, InputIterator last);
	~CircularList();

	CircularList& operator=(const CircularList& other);
	CircularList& operator=(CircularList&& other);

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
	void sort(std::function<void(CircularList&)> sortingFunction = bubbleSort);

	bool pushFromFile(std::string filename);

	friend bool operator==(const CircularList& lhs, const CircularList& rhs);
	friend bool operator!=(const CircularList& lhs, const CircularList& rhs);
	friend CircularList operator+(const CircularList& lhs, const CircularList& rhs);
	CircularList operator+=(const CircularList& other);

	std::ostream& print(std::ostream& os, std::string delimeter = " ") const;
	friend std::ostream& operator<<(std::ostream& os, const CircularList& cl);
	friend void bubbleSort(CircularList& cl);

private:
	void incrementSize();
	void decrementSize();
	void reset();
	Node* remove(Node* toRemove);

	Node* head;
	Node* tail;
	size_t n;
};