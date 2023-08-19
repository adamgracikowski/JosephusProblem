#pragma once
#include <exception>
#include <sstream>
#include <fstream>

#include "CircularList.h"
#include "Soldier.h"

class FileException : std::exception {
public:
	explicit FileException(std::string msg) : std::exception(), msg{ msg } {}
	virtual const char* what() const noexcept { return msg.c_str(); }
private:
	std::string msg;
};