#pragma once
#include <string>
#include <iostream>

//--------------------------------------------

class Soldier
{
public:
	Soldier() = default;
	explicit Soldier(std::string name, size_t positionInCircle)
		: name{ name }, positionInCircle{ positionInCircle } {};
	Soldier(const Soldier&) = default;
	Soldier(Soldier&&) noexcept = default;
	~Soldier() = default;

	Soldier& operator=(const Soldier& other) = default;
	Soldier& operator=(Soldier&& other) = default;

	friend bool operator==(const Soldier& lhs, const Soldier& rhs)
	{
		return (lhs.name == rhs.name && lhs.positionInCircle == rhs.positionInCircle);
	}
	friend bool operator!=(const Soldier& lhs, const Soldier& rhs)
	{
		return !(lhs == rhs);
	}
	friend bool operator<(const Soldier& lhs, const Soldier& rhs) 
	{
		return lhs.positionInCircle < rhs.positionInCircle;
	}
	friend std::ostream& operator<<(std::ostream& os, const Soldier& soldier)
	{
		return os << soldier.positionInCircle << " " << soldier.name;
	}
	friend std::istream& operator>>(std::istream& is, Soldier& soldier)
	{
		return is >> soldier.positionInCircle >> soldier.name;
	}
private:
	std::string name;
	size_t positionInCircle;
};

