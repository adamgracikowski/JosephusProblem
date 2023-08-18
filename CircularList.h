#pragma once
#include <string>
#include <iostream>

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